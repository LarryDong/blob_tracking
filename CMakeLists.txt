
project(blob_tracking)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)

include_directories(
    src/include
    /usr/local/include
)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/blob.cpp
)

target_link_libraries(
    ${PROJECT_NAME} opencv_core opencv_highgui opencv_imgproc
)

