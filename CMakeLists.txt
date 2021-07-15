cmake_minimum_required(VERSION 3.16)

project(game-of-life)

set(CMAKE_CXX_STANDARD 20)

include(cmake/ucm.cmake)

ucm_add_flags("-fprofile-instr-generate" "-fcoverage-mapping")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message(" ")
    message("CMAKE IN DEBUG MODE")
    message(" ")
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    message(" ")
    message("CMAKE IN RELEASE MODE")
    message(" ")
endif ()

if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else ()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif ()

ucm_add_dirs(src TO SOURCES)

ucm_add_dirs(tests TO TESTS)

ucm_add_dirs(app TO APP)

include_directories(src)

include_directories(tests)

add_executable(game-of-life-tests ${TESTS} ${SOURCES})

if (APPLE)
    set(MACOSX_BUNDLE_ICON_FILE icon.icns)

    # And this part tells CMake where to find and install the file itself
    set(myApp_ICON ${CMAKE_SOURCE_DIR}/cmake/icon.icns)
    set_source_files_properties(${myApp_ICON} PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources")
    add_executable(game-of-life MACOSX_BUNDLE ${myApp_ICON} ${APP} ${SOURCES})

    set_target_properties(game-of-life PROPERTIES
            BUNDLE True
            MACOSX_BUNDLE_GUI_IDENTIFIER com.vanyabeat.gameoflife
            MACOSX_BUNDLE_BUNDLE_NAME GameOfLife
            MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
            )
endif ()

target_link_libraries(game-of-life-tests ${CONAN_LIBS})

target_link_libraries(game-of-life ${CONAN_LIBS})