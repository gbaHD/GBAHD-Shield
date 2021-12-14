# Define which platform
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUNI_PLATFORM_ARDUINO")
set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -DUNI_PLATFORM_ARDUINO")

set(COMPONENT_ADD_INCLUDEDIRS ".")

set(COMPONENT_SRCS "arduino_main.cpp"
                   "main.c"
                   "web_handler.cpp"
                   "bitstream_handler.cpp"
                   )

set(COMPONENT_REQUIRES "bluepad32"
                       "bluepad32_arduino"
                       "arduino"
                       )

register_component()
