#include <Arduino.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <RTClib.h>
#include <FS.h>
#include <SPIFFS.h>

// Adresse I2C de l'afficheur LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Définir la taille du clavier
const byte ROWS = 4;
const byte COLS = 3;

// Définir les symboles sur les touches du clavier
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

// Connecter les lignes du clavier aux broches du ESP32
byte rowPins[ROWS] = {5, 18, 23, 19};
// Connecter les colonnes du clavier aux broches du ESP32
byte colPins[COLS] = {4, 9, 10};

// Initialiser la bibliothèque Keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Initialiser le module RTC
RTC_DS3231 rtc;

// Définir les différentes pages de menu
const char* menuPages[][4] = {
  {"1-No animal", "2-Choix du test", "3-RUN", "#suivant"},
  {"4-Ajust.parametre", "5-Ajust.date/heure", "6-VIDE", "*precedent  #suivant"},
  {"7-Verifier log", "8-Test boite", "9-VIDE2", "*precedent"}
};

int currentPage = 0;

void setup() {
  Wire.begin();
  lcd.begin();
  lcd.backlight();
  
  if (!rtc.begin()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RTC non detecté");
    while (1);
  }

  if (rtc.lostPower()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RTC a perdu l'heure");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (!SPIFFS.begin(true)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erreur SPIFFS");
    while (1);
  }

  displayMenu();
}

void loop() {
  char key = keypad.getKey();
  
  if (key) {
    handleKeyPress(key);
  }
}

void displayMenu() {
  lcd.clear();
  for (int i = 0; i < 4; i++) {
    lcd.setCursor(0, i);
    lcd.print(menuPages[currentPage][i]);
  }
}

void handleKeyPress(char key) {
  if (currentPage == 0) {
    handleMainMenu(key);
  } else if (currentPage == 1) {
    handleParamMenu(key);
  } else if (currentPage == 2) {
    handleLogMenu(key);
  }
}

void handleMainMenu(char key) {
  switch (key) {
    case '1':
      noAnimalMenu();
      break;
    case '2':
      choixDuTest();
      break;
    case '3':
      run();
      break;
    case '#':
      if (currentPage < 2) {
        currentPage++;
        displayMenu();
      }
      break;
    case '*':
      if (currentPage > 0) {
        currentPage--;
        displayMenu();
      }
      break;
    default:
      break;
  }
}

void handleParamMenu(char key) {
  switch (key) {
    case '4':
      ajustParametre();
      break;
    case '5':
      ajustDateHeure();
      break;
    case '#':
      if (currentPage < 2) {
        currentPage++;
        displayMenu();
      }
      break;
    case '*':
      if (currentPage > 0) {
        currentPage--;
        displayMenu();
      }
      break;
    default:
      break;
  }
}

void handleLogMenu(char key) {
  switch (key) {
    case '7':
      verifierLog();
      break;
    case '8':
      testBoite();
      break;
    case '9':
      // Gérer l'option 9 si nécessaire
      break;
    case '*':
      if (currentPage > 0) {
        currentPage--;
        displayMenu();
      }
      break;
    case '#':
      if (currentPage < 2) {
        currentPage++;
        displayMenu();
      }
      break;
    default:
      break;
  }
}

void noAnimalMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1-Entrer no animal");
  lcd.setCursor(0, 1);
  lcd.print("2-Auto RFID on/off");
  lcd.setCursor(0, 3);
  lcd.print("*RETOUR #ENREGISTRER");

  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key == '*') {
        displayMenu();
        return;
      } else if (key == '#') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enregistre...");
        delay(2000);
        displayMenu();
        return;
      } else if (key == '1') {
        enterAnimalNumber();
        return;
      } else if (key == '2') {
        toggleRFID();
        return;
      }
    }
  }
}

void enterAnimalNumber() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Entrer no animal:");
  String animalNumber = "";
  
  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key == '*') {
        noAnimalMenu();
        return;
      } else if (key == '#') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("No animal: ");
        lcd.print(animalNumber);
        lcd.setCursor(0, 1);
        lcd.print("Enregistre...");
        saveToFile("/animalNumber.txt", animalNumber);
        delay(2000);
        noAnimalMenu();
        return;
      } else if (key >= '0' && key <= '9') {
        if (animalNumber.length() < 10) {
          animalNumber += key;
          lcd.setCursor(0, 1);
          lcd.print(animalNumber);
        }
      }
    }
  }
}

void toggleRFID() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Auto RFID on/off:");
  // Ajoutez ici le code pour activer/désactiver RFID
  delay(2000);
  noAnimalMenu();
}

void choixDuTest() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1-8 Afficher tests");
  lcd.setCursor(0, 1);
  lcd.print("0-Attribuer no test");
  lcd.setCursor(0, 3);
  lcd.print("*RETOUR  #ENREGISTER");

  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key == '*') {
        displayMenu();
        return;
      } else if (key == '#') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enregistre...");
        saveToFile("/testOptions.txt", "Enregistre option");
        delay(2000);
        displayMenu();
        return;
      } else if (key >= '1' && key <= '8') {
        afficherTest(key - '1');
        return;
      } else if (key == '0') {
        attribuerNoTest();
        return;
      }
    }
  }
}

void afficherTest(int testIndex) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test ");
  lcd.print(testIndex + 1);
  lcd.print(": Page");
  delay(2000);
  choixDuTest();
}

void attribuerNoTest() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Attribuer no test:");
  String testNumber = "";
  
  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key == '*') {
        choixDuTest();
        return;
      } else if (key == '#') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Test no: ");
        lcd.print(testNumber);
        lcd.setCursor(0, 1);
        lcd.print("Enregistre...");
        saveToFile("/testNumber.txt", testNumber);
        delay(2000);
        choixDuTest();
        return;
      } else if (key >= '0' && key <= '9') {
        if (testNumber.length() < 10) {
          testNumber += key;
          lcd.setCursor(0, 1);
          lcd.print(testNumber);
        }
      }
    }
  }
}

void ajustParametre() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1-Voir date/heure");
  lcd.setCursor(0, 1);
  lcd.print("2-Delai de commencer");
  lcd.setCursor(0, 2);
  lcd.print("3-ITI");
  lcd.setCursor(0, 3);
  lcd.print("*RETOUR #ENREGISTRER");

  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key == '*') {
        displayMenu();
        return;
      } else if (key == '1') {
        viewRTCDateTime();
        return;
      } else if (key == '2') {
        // Autre paramètre
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Delai");
        saveToFile("/DELAI.txt", "Delai");
        delay(2000);
        ajustParametre();
        return;
      } else if (key == '3') {
        // Autre paramètre
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ITI");
        saveToFile("/ITI.txt", "ITI");
        delay(2000);
        ajustParametre();
        return;
      }
    }
  }
}

void viewRTCDateTime() {
  DateTime now = rtc.now();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Date/Heure RTC:");
  lcd.setCursor(0, 1);
  lcd.print(now.day(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.year(), DEC);
  lcd.setCursor(0, 2);
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.setCursor(5,2);
  lcd.print(':');
  lcd.print(now.second(), DEC);
  delay(2000);
  ajustParametre();
}

void ajustDateHeure() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1-Date");
  lcd.setCursor(0, 1);
  lcd.print("2-Heure");
  lcd.setCursor(0, 2);
  lcd.print("3-Boite no");
  lcd.setCursor(0, 3);
  lcd.print("*RETOUR #ENREGISTRER");

  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key == '*') {
        displayMenu();
        return;
      } else if (key == '#') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enregistre...");
        saveToFile("/dateHeure.txt", "Enregistre D/H");
        delay(2000);
        displayMenu();
        return;
      } else if (key == '1') {
        adjustDate();
        return;
      } else if (key == '2') {
        adjustTime();
        return;
      } else if (key == '3') {
        adjustBoxNumber();
        return;
      }
    }
  }
}

void adjustDate() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Entrer date:");
  lcd.setCursor(0, 1);
  lcd.print("DD/MM/YY");

  String date = "DD/MM/YY";
  int dateIndex = 0;
  int datePositions[6] = {0, 1, 3, 4, 6, 7}; // Positions of the digits in the date string

  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key == '*') {
        ajustDateHeure();
        return;
      } else if (key == '#') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Date: ");
        lcd.print(date);
        lcd.setCursor(0, 1);
        lcd.print("Enregistre...");
        DateTime dt = DateTime(
          date.substring(6, 8).toInt() + 2000, // Année
          date.substring(3, 5).toInt(),         // Mois
          date.substring(0, 2).toInt()          // Jour
        );
        rtc.adjust(dt);
        saveToFile("/date.txt", date);
        delay(2000);
        ajustDateHeure();
        return;
      } else if (key >= '0' && key <= '9') {
        if (dateIndex < 6) {
          date[datePositions[dateIndex]] = key;
          dateIndex++;
          lcd.setCursor(0, 1);
          lcd.print(date);
        }
      }
    }
  }
}

void adjustTime() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Entrer heure:");
  lcd.setCursor(0, 1);
  lcd.print("HH:MM");

  String time = "HH:MM";
  int timeIndex = 0;
  int timePositions[4] = {0, 1, 3, 4}; // Positions of the digits in the time string

  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key == '*') {
        ajustDateHeure();
        return;
      } else if (key == '#') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Heure: ");
        lcd.print(time);
        lcd.setCursor(0, 1);
        lcd.print("Enregistre...");
        DateTime dt = rtc.now();
        rtc.adjust(DateTime(dt.year(), dt.month(), dt.day(),
                            time.substring(0, 2).toInt(),
                            time.substring(3, 5).toInt()));
        saveToFile("/time.txt", time);
        delay(2000);
        ajustDateHeure();
        return;
      } else if (key >= '0' && key <= '9') {
        if (timeIndex < 4) {
          time[timePositions[timeIndex]] = key;
          timeIndex++;
          lcd.setCursor(0, 1);
          lcd.print(time);
        }
      }
    }
  }
}

void adjustBoxNumber() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Entrer boite no:");
  String boxNumber = "";

  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key == '*') {
        ajustDateHeure();
        return;
      } else if (key == '#') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Boite no: ");
        lcd.print(boxNumber);
        lcd.setCursor(0, 1);
        lcd.print("Enregistre...");
        saveToFile("/boxNumber.txt", boxNumber);
        delay(2000);
        ajustDateHeure();
        return;
      } else if (key >= '0' && key <= '9') {
        if (boxNumber.length() < 10) {
          boxNumber += key;
          lcd.setCursor(0, 1);
          lcd.print(boxNumber);
        }
      }
    }
  }
}

void verifierLog() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("data");
  printFiles();
  delay(2000);
  displayMenu();
}

void testBoite() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("8-Test boite");
  // Ajoutez ici le code pour gérer cette option
  delay(2000);
  displayMenu();
}

void run() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RUN");
  // Ajoutez ici le code pour gérer cette option
  delay(2000);
  displayMenu();
}

void executeOption(int option) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Option ");
  lcd.print(option);
  // Ajoutez ici le code pour gérer les options restantes
  delay(2000);
  displayMenu();
}

void saveToFile(const char* path, const String& data) {
  File file = SPIFFS.open(path, FILE_WRITE);
  if (!file) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erreur ouverture fichier");
    return;
  }
  file.print(data);
  file.close();
}

String readFromFile(const char* path) {
  File file = SPIFFS.open(path, FILE_READ);
  if (!file) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erreur lecture fichier");
    return "";
  }
  String data = file.readString();
  file.close();
  return data;
}

void printFiles() {
  File root = SPIFFS.open("/");
  if (!root) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erreur d'ouverture");
    return;
  }

  File file = root.openNextFile();
  int line = 1;
  while (file) {
    lcd.clear();
    lcd.setCursor(0, line);
    lcd.print("Fichier: ");
    lcd.print(file.name());
    line++;
    
    String content = "";
    while (file.available()) {
      content += (char)file.read();
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Contenu:");
    lcd.setCursor(0, 1);
    lcd.print(content.substring(0, 16));  // les 1er 16 caractères du contenu
    delay(2000);

    if (line > 3) {  // Passe à la ligne suivante
      line = 0;
      lcd.clear();
    }
    
    file = root.openNextFile();
  }

  root.close();
}

