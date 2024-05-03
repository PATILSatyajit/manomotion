cmake_minimum_required(VERSION 3.0)
project(safety_app)
set(EXE_TARGET_NAME safety_app)

add_definitions(-D_IS_LINUX_)
add_definitions(-DUSING_CURL)
add_definitions(-O4 -g)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

SET(OPENCV_PRROVIDED_PATH "/usr/include/opencv4")

#For OpenCV
include_directories(${OPENCV_PRROVIDED_PATH})
include_directories(${OPENCV_PRROVIDED_PATH}/opencv2)
include_directories(${OPENCV_PRROVIDED_PATH}/include)
include_directories(${OPENCV_PRROVIDED_PATH}/include/opencv)
include_directories(${OPENCV_PRROVIDED_PATH}/include/opencv2)


#For Tensor-RT
include_directories(/usr/local/cuda/include)
include_directories(${THIRDPARTY_DIR}/TensorRT8.2_common)

file(GLOB OPENCV_PRROVIDED_LIBS	/usr/lib/aarch64-linux-gnu/libopencv*)
message("OPENCV_PRROVIDED_LIBS: " ${OPENCV_PRROVIDED_LIBS})

#For Tensor-RT
link_directories(/usr/local/cuda/lib64)

link_directories(./)

#target_link_libraries(${LIB_TARGET_NAME}  ${OPENCV_PRROVIDED_LIBS} ${TF_LITE_STATIC_LIB_AND_DEPENDENCIES} curl ssl crypto "/usr/lib/aarch64-linux-gnu/libnvparsers.so" "/usr/lib/aarch64-linux-gnu/libnvonnxparser.so" cudart nvinfer nvparsers nvonnxparser) 

add_executable(${EXE_TARGET_NAME}   "Main_safety.cpp" "v4l2_helper.c" "DrawingUtils.cpp")
target_link_libraries(${EXE_TARGET_NAME} manomotion ${OPENCV_PRROVIDED_LIBS} ${TF_LITE_STATIC_LIB_AND_DEPENDENCIES}  ssl crypto "/usr/lib/aarch64-linux-gnu/libnvparsers.so" "/usr/lib/aarch64-linux-gnu/libnvonnxparser.so" cudart nvinfer nvparsers nvonnxparser)

set_property(TARGET ${EXE_TARGET_NAME} PROPERTY CXX_STANDARD 17)
