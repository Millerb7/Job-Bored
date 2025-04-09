cmake_minimum_required(VERSION 3.10)
project(JobBored)

set(CMAKE_CXX_STANDARD 17)

# Path to your specific CEF binary folder
set(CEF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../external/cef_binary_135.0.17")

# Add CEF's parent folder to include path so "include/cef_app.h" works
include_directories(${CEF_ROOT})

# Link the Release directory for DLLs/libs
link_directories(${CEF_ROOT}/Release)

# Link directory where libcef_dll_wrapper.lib is built
link_directories(${CEF_ROOT}/build_wrapper/libcef_dll_wrapper)

# Source files
file(GLOB SRC_FILES src/*.cpp)

# Build target
add_executable(JobBored ${SRC_FILES})

add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)

set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

# Link to CEF libs
target_link_libraries(JobBored
    libcef
    libcef_dll_wrapper
)

# Post-build: copy DLLs and Resources
add_custom_command(TARGET JobBored POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CEF_ROOT}/Resources"
        "$<TARGET_FILE_DIR:JobBored>/resources"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CEF_ROOT}/Resources/locales"
        "$<TARGET_FILE_DIR:JobBored>/locales"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CEF_ROOT}/Resources/icudtl.dat"
        "$<TARGET_FILE_DIR:JobBored>/icudtl.dat"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CEF_ROOT}/Resources/chrome_100_percent.pak"
        "$<TARGET_FILE_DIR:JobBored>/chrome_100_percent.pak"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CEF_ROOT}/Resources/chrome_200_percent.pak"
        "$<TARGET_FILE_DIR:JobBored>/chrome_200_percent.pak"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CEF_ROOT}/Resources/resources.pak"
        "$<TARGET_FILE_DIR:JobBored>/resources.pak"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CEF_ROOT}/Release"
        "$<TARGET_FILE_DIR:JobBored>"
)

