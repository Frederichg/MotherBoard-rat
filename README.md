# MotherBoard-rat
Mother board with lcd and keypad for rat skinner boxes. Based on esp32 DEVKIT-M-1

There are multiple tests in the scr folder. To upload the main project, simply upload as you would normaly. To upload a test, edit the platformio.ini file and uncomment the right build_src_filter

##### Default config :
```ini
build_src_filter = +<*> -<.git/> -<.svn/> -<*.cpp> +<main.cpp>
; build_src_filter = +<*> -<.git/> -<.svn/> -<*.cpp> +<I2C_scan.cpp>
; build_src_filter = +<*> -<.git/> -<.svn/> -<*.cpp> +<Keypad.cpp>
; build_src_filter = +<*> -<.git/> -<.svn/> -<*.cpp> +<LCD.cpp>
; build_src_filter = +<*> -<.git/> -<.svn/> -<*.cpp> +<MCP23X17.cpp>
; build_src_filter = +<*> -<.git/> -<.svn/> -<*.cpp> +<SD.cpp>
```

>**_Note :_** Only one of these lines can be uncommented at a time