cmake_minimum_required(VERSION 3.10)
project(n-body-simulator VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Setup output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add compiler warnings and optimizations
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -O3 -march=native)
elseif(MSVC)
    add_compile_options(/W4 /O2)
endif()

# Check for OpenMP support
find_package(OpenMP REQUIRED)


# Find all source files
file(GLOB SOURCES "src/*.cpp")

# Define include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Create executable
add_executable(n-body-simulator ${SOURCES})

# Link OpenMP if found
if(OpenMP_CXX_FOUND)
    target_link_libraries(n-body-simulator PUBLIC OpenMP::OpenMP_CXX)
endif()

# Install target
#install(TARGETS n-body-simulator
#        RUNTIME DESTINATION bin)

# Copy font file to build directory
set(FONT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/JetBrainsMonoNLNerdFontMono-Regular.ttf")

# Simple target to ensure the build directory exists with proper permissions
add_custom_target(create_results_dir
    COMMAND ${CMAKE_COMMAND} -E touch_nocreate ${CMAKE_BINARY_DIR}/.cmake_directory_marker
)

# Add option to include debug symbols
option(DEBUG_SYMBOLS "Include debug symbols" OFF)
if(DEBUG_SYMBOLS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-g)
    elseif(MSVC)
        add_compile_options(/Z7)
    endif()
endif()

# Check for required libraries
#find_package(Threads REQUIRED)
#target_link_libraries(n-body-simulator PRIVATE Threads::Threads)

# Find pkg-config for SDL2_ttf
find_package(PkgConfig REQUIRED)

# Setup module path for custom find modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# Find SDL2 packages
find_package(SDL2 REQUIRED)
# Use pkg-config for SDL2_ttf
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

# If FindSDL2.cmake is not available by default, try with custom modules
if(NOT SDL2_FOUND)
    # Ensure cmake directory exists
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    message(STATUS "SDL2 not found with default modules, trying custom modules...")
    find_package(SDL2 REQUIRED)
endif()

# Include SDL2 directories
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

# Link SDL2 libraries
target_link_libraries(n-body-simulator PRIVATE 
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)


# Copy font file after build is complete
add_custom_command(
    TARGET n-body-simulator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${FONT_FILE}
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/JetBrainsMonoNLNerdFontMono-Regular.ttf
)

add_dependencies(n-body-simulator create_results_dir)

# Optional: Add test directory if tests are available
# Commenting out test directory section since test directory doesn't contain CMakeLists.txt
# if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test")
#     enable_testing()
#     add_subdirectory(test)
# endif()

# Print configuration summary
message(STATUS "")
message(STATUS "Project: ${PROJECT_NAME} ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Binary output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "SDL2 Found: ${SDL2_FOUND}")
message(STATUS "SDL2_ttf Found: ${SDL2_TTF_FOUND}")
message(STATUS "")

