cmake_minimum_required(VERSION 3.10)

# 项目名称
project(PosixDemo C)

# 指定生成目录
set(BUILD_DIR "${CMAKE_BINARY_DIR}/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})

# FreeRTOS 相关目录
set(FREERTOS_DIR "../../../FreeRTOS")
set(FREERTOS_PLUS_DIR "../../../FreeRTOS-Plus")

set(KERNEL_DIR "${FREERTOS_DIR}/Source")

# 包含目录
include_directories(
    .
    ${KERNEL_DIR}/include
    ${KERNEL_DIR}/portable/ThirdParty/GCC/Posix
    ${KERNEL_DIR}/portable/ThirdParty/GCC/Posix/utils
    ${FREERTOS_DIR}/Demo/Common/include
    ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-Trace/Include
    ${FREERTOS_DIR}/Demo/Posix_GCC/data
    ${FREERTOS_DIR}/Demo/Posix_GCC/tool
)

# 基础源文件
file(GLOB SOURCE_FILES
    "*.c"
    "${FREERTOS_DIR}/Source/*.c"
    "${FREERTOS_DIR}/Demo/Posix_GCC/tool/*.c"
    "${FREERTOS_DIR}/Demo/Posix_GCC/data/*.c"
    "${KERNEL_DIR}/portable/MemMang/heap_3.c"
    "${KERNEL_DIR}/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c"
    "${KERNEL_DIR}/portable/ThirdParty/GCC/Posix/port.c"
)

# 添加 FreeRTOS 的 Minimal 示例
file(GLOB MINIMAL_FILES
    "${FREERTOS_DIR}/Demo/Common/Minimal/*.c"
)
list(APPEND SOURCE_FILES ${MINIMAL_FILES})

# 条件处理
if(DEFINED TRACE_ON_ENTER AND TRACE_ON_ENTER)
    add_compile_definitions(TRACE_ON_ENTER=1)
else()
    add_compile_definitions(TRACE_ON_ENTER=0)
endif()

if(DEFINED COVERAGE_TEST AND COVERAGE_TEST)
    add_compile_definitions(projCOVERAGE_TEST=1)
    list(APPEND SOURCE_FILES
        "${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-Trace/trcKernelPort.c"
        "${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-Trace/trcSnapshotRecorder.c"
        "${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-Trace/trcStreamingRecorder.c"
        "${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-Trace/streamports/File/trcStreamingPort.c"
    )
else()
    add_compile_definitions(projCOVERAGE_TEST=0)
endif()

# 添加宏定义
add_compile_definitions(
    -DBUILD_DIR=\"${BUILD_DIR}\"
    -D_WINDOWS_
)

# 编译器和链接选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -ggdb3")
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_DEBUG "-O0 -pg")

# 添加目标
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# 链接库
target_link_libraries(${PROJECT_NAME} pthread)

# 自定义清理规则
# add_custom_target(clean
#     COMMAND ${CMAKE_COMMAND} -E rm -rf ${BUILD_DIR}
#     COMMENT "Cleaning up build files"
# )
