cmake_minimum_required(VERSION 3.16)
project(JobBored)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Match MSVC runtime and iterator level to CEF prebuilt binaries
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_compile_definitions(
    $<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=0>
    $<$<CONFIG:Debug>:_HAS_ITERATOR_DEBUGGING=0>
)

# Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Use vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/Users/wnd/Documents/github/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(CMAKE_PREFIX_PATH "C:/Users/wnd/Documents/github/vcpkg/installed/x64-windows/share" CACHE STRING "")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# CEF setup
set(CEF_ROOT "${CMAKE_SOURCE_DIR}/../external/cef_binary_135.0.17+gcbc1c5b+chromium-135.0.7049.52_windows64")
include_directories(${CEF_ROOT} ${CEF_ROOT}/include)
set(_VCPKG_INSTALLED_DIR "C:/Users/wnd/Documents/github/vcpkg/installed")

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/simple_app.cpp
    src/simple_handler.cpp
)

set(HEADERS
    src/MainWindow.h
    src/simple_app.h
    src/simple_handler.h
)

# Executable
add_executable(JobBored ${SOURCES} ${HEADERS})
# Match CEF's build settings
target_compile_definitions(JobBored PRIVATE CEF_USE_SANDBOX=0 NOMINMAX WIN32_LEAN_AND_MEAN)

# Link directories
target_link_directories(JobBored
    PRIVATE
        "${CEF_ROOT}/$<CONFIG>"
        "${CEF_ROOT}/build_release/libcef_dll_wrapper"
)

# Link libraries
target_link_libraries(JobBored
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    libcef
    libcef_dll_wrapper
)

# Post-build: copy CEF resources
add_custom_command(TARGET JobBored POST_BUILD
    # Ensure output directories exist
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:JobBored>/locales"

    # Copy CEF resources
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CEF_ROOT}/Resources/icudtl.dat" "$<TARGET_FILE_DIR:JobBored>"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CEF_ROOT}/Resources/locales" "$<TARGET_FILE_DIR:JobBored>/locales"

    # Copy DLLs needed to run
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CEF_ROOT}/Release/libcef.dll" "$<TARGET_FILE_DIR:JobBored>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CEF_ROOT}/Release/chrome_elf.dll" "$<TARGET_FILE_DIR:JobBored>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CEF_ROOT}/Release/d3dcompiler_47.dll" "$<TARGET_FILE_DIR:JobBored>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CEF_ROOT}/Release/libEGL.dll" "$<TARGET_FILE_DIR:JobBored>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CEF_ROOT}/Release/libGLESv2.dll" "$<TARGET_FILE_DIR:JobBored>"

    # Copy Qt runtime DLLs (just in case)
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "C:/Users/wnd/Documents/github/vcpkg/installed/x64-windows/bin/Qt6Core.dll" "$<TARGET_FILE_DIR:JobBored>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "C:/Users/wnd/Documents/github/vcpkg/installed/x64-windows/bin/Qt6Gui.dll" "$<TARGET_FILE_DIR:JobBored>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "C:/Users/wnd/Documents/github/vcpkg/installed/x64-windows/bin/Qt6Widgets.dll" "$<TARGET_FILE_DIR:JobBored>"
)

