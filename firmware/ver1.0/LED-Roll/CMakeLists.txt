# !!! WARNING !!! AUTO-GENERATED FILE, PLEASE DO NOT MODIFY IT AND USE
# https://docs.platformio.org/page/projectconf/section_env_build.html#build-flags
#
# If you need to override existing CMake configuration or add extra,
# please create `CMakeListsUser.txt` in the root of project.
# The `CMakeListsUser.txt` will not be overwritten by PlatformIO.

cmake_minimum_required(VERSION 3.13)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

project("LED-Roll" C CXX)

include(CMakeListsPrivate.txt)

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeListsUser.txt)
    include(CMakeListsUser.txt)
endif ()

include_directories(lib)
include_directories(lib/Adafruit-GFX-Library_gb2312)
include_directories(lib/ESP32_HUB75_LED_MATRIX_PANEL_DMA_Display)
include_directories(lib/ArduinoJson/src)


add_custom_target(
        Production ALL
        COMMAND platformio -c clion run "$<$<NOT:$<CONFIG:All>>:-e${CMAKE_BUILD_TYPE}>"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
        Debug ALL
        COMMAND platformio -c clion debug "$<$<NOT:$<CONFIG:All>>:-e${CMAKE_BUILD_TYPE}>"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(Z_DUMMY_TARGET ${SRC_LIST} src/resources/bmp/weather.h src/config/config.h src/app/app_typedef.h src/app/app_dispFx.h src/app/app_dispFx.cpp src/app/app_getData.cpp src/app/app_getData.h src/app/pages/page_startup.cpp src/app/pages/page_startup.h src/app/pages/page_clock.cpp src/app/pages/page_clock.h src/app/pages/pageManager.cpp src/app/pages/pageManager.h src/app/app_extern.h src/hal/hal_display.cpp src/hal/hal.h src/hal/hal_wifi.cpp src/hal/hal_timer.cpp src/app/app_tasks.cpp src/app/app_tasks.h src/hal/hal_homeButton.cpp)

