cmake_minimum_required(VERSION 3.16)
project(JobBored)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
add_compile_definitions(
    $<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=0>
    $<$<CONFIG:Debug>:_HAS_ITERATOR_DEBUGGING=0>
)

# Qt configuration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Toolchain and Qt paths
set(CMAKE_TOOLCHAIN_FILE "C:/Users/wnd/Documents/github/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(CMAKE_PREFIX_PATH "C:/Users/wnd/Documents/github/vcpkg/installed/x64-windows/share" CACHE STRING "")
set(QT_DLL_PATH "C:/Users/wnd/Documents/github/vcpkg/installed/x64-windows/bin" CACHE STRING "")
set(QT_PLUGIN_PATH "${QT_DLL_PATH}/../Qt6/plugins")

# CEF
set(CEF_ROOT "${CMAKE_SOURCE_DIR}/../external/cef_binary_135.0.17+gcbc1c5b+chromium-135.0.7049.52_windows64")
include_directories(${CEF_ROOT} ${CEF_ROOT}/include)

# Qt packages
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Resources
qt_add_resources(RESOURCES resources/resources.qrc)

# Source files (refactored layout)
set(SOURCES
    src/main.cpp
    src/core/JobManager.cpp
    src/core/BrowserCore.cpp

    src/ui/templates/MainWindow.cpp
    src/ui/pages/BrowserPage.cpp

    src/ui/atoms/IconButton.cpp
    src/ui/atoms/NavIcon.cpp
    src/ui/atoms/SearchIcon.cpp

    src/ui/molecules/ControlButtons.cpp
    src/ui/molecules/UrlBar.cpp

    src/ui/organisms/Sidebar.cpp
    src/ui/organisms/BottomBar.cpp

    src/handlers/SimpleApp.cpp
    src/handlers/SimpleHandler.cpp
)

set(HEADERS
    src/core/JobManager.h
    src/core/BrowserCore.h

    src/ui/templates/MainWindow.h
    src/ui/pages/BrowserPage.h

    src/ui/atoms/IconButton.h
    src/ui/atoms/NavIcon.h
    src/ui/atoms/SearchIcon.h

    src/ui/molecules/ControlButtons.h
    src/ui/molecules/UrlBar.h

    src/ui/organisms/Sidebar.h
    src/ui/organisms/BottomBar.h

    src/handlers/SimpleApp.h
    src/handlers/SimpleHandler.h
)

add_executable(JobBored ${SOURCES} ${HEADERS} ${RESOURCES})

target_compile_definitions(JobBored PRIVATE CEF_USE_SANDBOX=0 NOMINMAX WIN32_LEAN_AND_MEAN)

target_link_directories(JobBored
    PRIVATE
        "${CEF_ROOT}/$<CONFIG>"
        "${CEF_ROOT}/build_release/libcef_dll_wrapper"
)

target_link_libraries(JobBored
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    libcef
    libcef_dll_wrapper
)

# Post-build steps
add_custom_command(TARGET JobBored POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CEF_ROOT}/Release/libcef.dll"
        "${CEF_ROOT}/Resources/icudtl.dat"
        ${QT_DLL_PATH}/Qt6Core.dll
        ${QT_DLL_PATH}/Qt6Widgets.dll
        ${QT_DLL_PATH}/Qt6Gui.dll
        ${QT_DLL_PATH}/icuin74.dll
        ${QT_DLL_PATH}/icuuc74.dll
        ${QT_DLL_PATH}/icudt74.dll
        ${QT_DLL_PATH}/zlib1.dll
        ${QT_DLL_PATH}/zstd.dll
        ${QT_DLL_PATH}/libpng16.dll
        ${QT_DLL_PATH}/harfbuzz.dll
        ${QT_DLL_PATH}/pcre2-16.dll
        ${QT_DLL_PATH}/freetype.dll
        ${QT_DLL_PATH}/brotlidec.dll
        ${QT_DLL_PATH}/brotlicommon.dll
        ${QT_DLL_PATH}/bz2.dll
        $<TARGET_FILE_DIR:JobBored>

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CEF_ROOT}/Resources/locales"
        "$<TARGET_FILE_DIR:JobBored>/locales"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${QT_PLUGIN_PATH}/platforms"
        "$<TARGET_FILE_DIR:JobBored>/platforms"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CEF_ROOT}/Resources/chrome_100_percent.pak"
        "${CEF_ROOT}/Resources/resources.pak"
        "${CEF_ROOT}/Release/v8_context_snapshot.bin"
        $<TARGET_FILE_DIR:JobBored>
)
