# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/external/bluepad32_template/components/bluepad32 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/components)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/external/bluepad32_template/components/bluepad32_arduino DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/components)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/external/bluepad32_template/components/btstack DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/components)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(gbaHD)

spiffs_create_partition_image(spiffs ${CMAKE_CURRENT_SOURCE_DIR}/data )
