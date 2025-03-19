cmake_minimum_required(VERSION 3.14...3.22)
project(deskgui_example LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0135 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)  # 强制子项目继承
set(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})

include(FetchContent)

FetchContent_Declare(
        deskgui
        GIT_REPOSITORY https://github.com/wenbingzhang/deskgui
        GIT_TAG main
)
set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
set(BUILD_TESTS_AND_BENCHMARKS OFF CACHE BOOL "Build tests and benchmarks" FORCE)
FetchContent_MakeAvailable(deskgui)

find_package(cpr REQUIRED)

#message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
set(source main.cpp)

if(APPLE)
    enable_language(OBJCXX)
    set(source ${source} gui/gui.h gui/gui_macos.mm)
endif()

if (WIN32)
    if(MSVC)
        add_compile_options(/utf-8)
    else()
        add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
    endif()
    add_definitions(-DUNICODE -D_UNICODE)  # 强制使用宽字符API
    set(source ${source} gui2/platform.h gui2/windows.cpp
            gui/gui.h
            gui/gui_windows.cpp)
endif ()

add_executable(${PROJECT_NAME} ${source})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME} PRIVATE deskgui cpr::cpr)

list(APPEND CMAKE_MODULE_PATH ${deskgui_SOURCE_DIR}/cmake)

# embed resources
include(resource_compiler/resource_compiler)

# Specify the assets
file(GLOB_RECURSE ASSETS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/frontend/dist/*)

resource_compiler(
        TARGET_NAME  ${PROJECT_NAME}
        RESOURCE_FILES "${ASSETS_FILES}"
        PACK_NAME "assets"
        ROOT_FOLDER "/frontend/dist"
)


if(APPLE)
    # Set macOS bundle properties
    set_target_properties(${PROJECT_NAME} PROPERTIES
            MACOSX_BUNDLE ON
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.zhangwenbing.${PROJECT_NAME}"
            MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
            MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
            MACOSX_BUNDLE_ICON_FILE "icon.icns"  # 如果有图标
    )

    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
            COMMAND rm -rf $<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources
            COMMAND cp -r ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources
    )
endif ()