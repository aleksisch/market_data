cmake_minimum_required(VERSION 3.27)
project(coinbase_parser)

set(CMAKE_CXX_STANDARD 20)
set(WARNINGS
        -Wpedantic -Wall -Wextra -Wwrite-strings -Wno-unused-function
        -Wno-unused-parameter -D_FORTIFY_SOURCE=2 -fsanitize=address
#        -fsanitize=undefined
# # Looks like this sanitizer fails with LLVM `error: Cannot represent a difference across sections` due to coroutines. todo: investigate
# # Similar closed issue: https://github.com/llvm/llvm-project/issues/49689
        -fsanitize=leak -fsanitize=pointer-subtract
        -fsanitize=pointer-compare -fno-omit-frame-pointer -fstack-protector-all
        -fstack-clash-protection -fcf-protection
)

add_compile_options(${WARNINGS})
add_link_options(-fsanitize=address)

find_package( Boost REQUIRED)
find_package(OpenSSL REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include( FetchContent )
FetchContent_Declare(
        daw_json_link
        GIT_REPOSITORY https://github.com/beached/daw_json_link
        GIT_TAG release
)
FetchContent_MakeAvailable(daw_json_link)

add_executable(coinbase_parser main.cpp
        market_data/coinbase.cpp
)
target_link_libraries( coinbase_parser daw::daw-json-link OpenSSL::SSL OpenSSL::Crypto)

enable_testing()

add_executable(ema_tests tests/ema_stats.cpp)

add_subdirectory(fuzztest SYSTEM)

include(GoogleTest)

link_fuzztest(ema_tests)
gtest_discover_tests(ema_tests)

add_executable(serde_tests tests/serde_tests.cpp)
target_link_libraries(
        serde_tests
        GTest::gtest_main
)
target_link_libraries( serde_tests daw::daw-json-link)
target_include_directories(serde_tests PUBLIC ${PROJECT_SOURCE_DIR})
gtest_discover_tests(serde_tests)