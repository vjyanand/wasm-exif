cmake_minimum_required(VERSION 3.27)
project(exif-wasm)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_BUILD_TYPE)

set(JSON_DIRECTORY "json")
set(EXIV2_DIRECTORY "exiv2")
set(BROTLI_DIRECTORY "brotli")

add_subdirectory(${JSON_DIRECTORY})
add_subdirectory(${EXIV2_DIRECTORY})


get_directory_property(
        EXIV2_BUILD_DIR
        DIRECTORY
        ${EXIV2_DIRECTORY}
        DEFINITION
        CMAKE_CURRENT_BINARY_DIR
)

message(PROJECT_SOURCE_DIR="${EXIV2_BUILD_DIR}")

if (IS_WASM)
    add_executable(
        ${PROJECT_NAME}
        src/wasm/main.cpp
        src/lib/parser.cpp
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "--bind")
else ()
    add_executable(
            ${PROJECT_NAME}
            src/exe/main.cpp
            src/lib/parser.cpp
    )
endif (IS_WASM)

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        ${JSON_DIRECTORY}/include
        ${EXIV2_DIRECTORY}/include
        ${EXIV2_DIRECTORY}/contrib/vs2019/solution
)

target_link_libraries(
        ${PROJECT_NAME}
        exiv2lib
)

