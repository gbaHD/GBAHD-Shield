
set(srcs
        "main.c"
        "arduino_main.cpp"
        "web_handler.cpp"
        "bitstream_handler.cpp"
        "wifi_handler.cpp"
        "mega_handler.cpp"
        "preferences_handler.cpp"
        "ota_handler.cpp"
        "log_handler.cpp")
     
set(requires "bluepad32"
             "bluepad32_arduino"
             "arduino"
             "json"
             "app_update"
             "AsyncTCP"
             "ESPAsyncWebServer")

idf_component_register(SRCS "preferences_handler.cpp" "${srcs}"
                    INCLUDE_DIRS "."
                    REQUIRES "${requires}")
