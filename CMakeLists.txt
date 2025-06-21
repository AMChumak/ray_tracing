cmake_minimum_required(VERSION 3.30)
set(PROJECT_NAME ray_tracing)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "/home/anton/Qt/6.8.1/gcc_64/lib/cmake")

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Gui)

find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_subdirectory(model/config)
add_subdirectory(model/engine)
add_subdirectory(view)


qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
        WIN32 MACOSX_BUNDLE
        main.cpp
)


target_link_libraries(${PROJECT_NAME}
        PUBLIC
        config_lib
        engine_lib
        ui_lib
        PUBLIC
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
        TARGET ${PROJECT_NAME}
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})