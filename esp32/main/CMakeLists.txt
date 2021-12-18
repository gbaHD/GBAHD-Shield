
set(srcs
        "main.c"
        "arduino_main.cpp"
        "web_handler.cpp"
        "bitstream_handler.cpp")
     
set(requires "bluepad32"
             "bluepad32_arduino"
             "arduino")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "."
                    REQUIRES "${requires}")
