set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(LIB_NAME ui_lib)

project(${LIB_NAME})

include_directories(.)
file(GLOB_RECURSE SOURCES ./*.cpp)
file(GLOB_RECURSE HEADERS ./*.h)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Gui)
qt_add_library(${LIB_NAME} STATIC ${SOURCES} ${HEADERS} resources.qrc)

target_include_directories(${LIB_NAME}
        PUBLIC
        .)

target_link_libraries(${LIB_NAME}
        PUBLIC
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
        engine_lib
        config_lib)