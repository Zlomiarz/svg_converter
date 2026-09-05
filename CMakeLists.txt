cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0135 NEW)

enable_language(CXX ASM)

project(svg_converter)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set ( TARGET_SRCS
    src/main.cpp
    src/png_wrapper.cpp
    src/skeletonize.cpp
)

set ( COMMON_SRCS
)

set ( TESTS_SRCS
    tests/main.cpp
    tests/image.cpp
    tests/image_utils.cpp
)

find_package(PNG REQUIRED)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
enable_testing()

FetchContent_MakeAvailable(googletest)

add_executable(svg_converter ${TARGET_SRCS} ${COMMON_SRCS} )
target_link_libraries(svg_converter PNG::PNG)   

set(GCC_COMPILE_OPTIONS "-m64;-std=c++23;-Wall;-Wextra;-Wredundant-decls;-Wmain;-Wundef;-Winline;-Wswitch-enum;-Wmissing-declarations;-Wnon-virtual-dtor;-Wzero-as-null-pointer-constant;-Winit-self;-Wshadow;-Wswitch-default;-Wfloat-equal;-Wunreachable-code;-ftemplate-depth=128;")
set(GCC_COMPILE_DEBUG_OPTIONS "${GCC_COMPILE_OPTIONS};-ggdb;-O0")
set(GCC_COMPILE_RELEASE_OPTIONS "${GCC_COMPILE_OPTIONS};-O3")
target_compile_options(svg_converter PUBLIC "$<$<CONFIG:Debug>:${GCC_COMPILE_DEBUG_OPTIONS}>")
target_compile_options(svg_converter PUBLIC "$<$<CONFIG:Release>:${GCC_COMPILE_RELEASE_OPTIONS}>")

add_executable(tests ${COMMON_SRCS} ${TESTS_SRCS} )
target_include_directories(tests PRIVATE . src/)
target_link_libraries(tests GTest::gtest_main GTest::gmock_main) 

set(GCC_TESTS_COMPILE_OPTIONS "-m64;-std=c++23")
set(GCC_TESTS_COMPILE_DEBUG_OPTIONS "${GCC_TESTS_COMPILE_OPTIONS};-ggdb;-O0")
set(GCC_TESTS_COMPILE_RELEASE_OPTIONS "${GCC_TESTS_COMPILE_OPTIONS};-O3")
target_compile_options(tests PUBLIC "$<$<CONFIG:Debug>:${GCC_TESTS_COMPILE_DEBUG_OPTIONS}>")
target_compile_options(tests PUBLIC "$<$<CONFIG:Release>:${GCC_TESTS_COMPILE_RELEASE_OPTIONS}>")

include(GoogleTest)
gtest_discover_tests(tests)