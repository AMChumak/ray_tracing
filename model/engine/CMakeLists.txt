set(LIB_NAME engine_lib)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
project(${LIB_NAME})

include_directories(include)
file(GLOB_RECURSE SOURCES *.cpp)
file(GLOB_RECURSE HEADERS *.h)

find_package(Eigen3 REQUIRED)


find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Gui)
qt_add_library(${LIB_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${LIB_NAME}
        PUBLIC
        .)

target_link_libraries(${LIB_NAME}
        PUBLIC
        config_lib
        Eigen3::Eigen
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
)