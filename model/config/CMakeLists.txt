set(LIB_NAME config_lib)
project(${LIB_NAME})

include_directories(include)
file(GLOB_RECURSE SOURCES *.cpp)
file(GLOB_RECURSE HEADERS *.h)

find_package(Eigen3 REQUIRED)

add_library(${LIB_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${LIB_NAME}
        PUBLIC
        .)

target_link_libraries(${LIB_NAME}
        PUBLIC
        engine_lib
        Eigen3::Eigen
)