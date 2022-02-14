# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/external/bluepad32_template/components/bluepad32 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/components)
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/external/bluepad32_template/components/bluepad32_arduino DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/components)
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/external/bluepad32_template/components/btstack DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/components)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/external/bluepad32/src/components/bluepad32 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/components)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/external/bluepad32/src/components/bluepad32_arduino DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/components)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/external/bluepad32/external/btstack DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/components)

# BTStack
set(REAL_IDF_PATH $ENV{IDF_PATH})
set(ENV{IDF_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

execute_process(COMMAND ${PYTHON_EXECUTABLE} external/bluepad32/external/btstack/port/esp32/integrate_btstack.py
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(ENV{IDF_PATH} ${REAL_IDF_PATH})

file(COPY_FILE ${CMAKE_CURRENT_SOURCE_DIR}/external/zwenergy/bitstream/default_720p.bit ${CMAKE_CURRENT_SOURCE_DIR}/data/720p.bit ONLY_IF_DIFFERENT)
file(COPY_FILE ${CMAKE_CURRENT_SOURCE_DIR}/external/zwenergy/bitstream/default_1080p.bit ${CMAKE_CURRENT_SOURCE_DIR}/data/1080p.bit ONLY_IF_DIFFERENT)


# See: https://gitlab.com/ricardoquesada/bluepad32/-/issues/9
set(ENV{BLUEPAD32_ARDUINO} TRUE)


include($ENV{IDF_PATH}/tools/cmake/project.cmake)

add_compile_definitions(I2C_BUFFER_LENGTH=132)

project(gbaHD)

#spiffs_create_partition_image(spiffs ${CMAKE_CURRENT_SOURCE_DIR}/data FLASH_IN_PROJECT)
littlefs_create_partition_image(spiffs ${CMAKE_CURRENT_SOURCE_DIR}/data FLASH_IN_PROJECT)
