cmake_minimum_required(VERSION 3.15)
project(posix_demo C)

# —— 用户可配置路径 —— 
# 在 CMake GUI / 命令行里也可通过 -DFREERTOS_DIR=/path/to/FreeRTOS 来覆盖
set(FREERTOS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeRTOS" CACHE PATH "Path to FreeRTOS root")
set(FREERTOS_PLUS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeRTOS-Plus" CACHE PATH "Path to FreeRTOS-Plus root")
set(KERNEL_DIR "${FREERTOS_DIR}/Source")

# —— 可选项 —— 
option(TRACE_ON_ENTER   "Enable TRACE_ON_ENTER=1 (else 0)" OFF)
option(COVERAGE_TEST    "Enable projCOVERAGE_TEST=1 (else 0)" OFF)
option(PROFILE          "Enable profiling (-pg -O0)" OFF)
option(SANITIZE_ADDRESS "Enable AddressSanitizer" OFF)
option(SANITIZE_LEAK    "Enable LeakSanitizer" OFF)
option(BLINKY_DEMO      "Build blinky demo (USER_DEMO=0)" OFF)
option(FULL_DEMO        "Build full demo   (USER_DEMO=1)" OFF)

# —— 全局编译选项 —— 
# 和 Makefile 保持一致的 -ggdb3
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb3")

# PROFILE vs 默认优化
if(PROFILE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg -O0")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg -O0")
else()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O3")
endif()

# Address & Leak Sanitizers
if(SANITIZE_ADDRESS)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fsanitize=alignment")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -fsanitize=alignment")
endif()
if(SANITIZE_LEAK)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=leak")
endif()

# —— 收集源文件 —— 
file(GLOB APP_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)
file(GLOB KERNEL_SRCS
    "${KERNEL_DIR}/*.c"
)
file(GLOB MOTOR_DRIVERS_SRCS
    "${FREERTOS_DIR}/Demo/motor_os/drivers/*.c"
)
file(GLOB MOTOR_HAL_SRCS
    "${FREERTOS_DIR}/Demo/motor_os/hal/*.c"
)
file(GLOB_RECURSE MOTOR_MIDDLEWARE_SRCS
    "${FREERTOS_DIR}/Demo/motor_os/middleware/*.c"
)
file(GLOB MOTOR_UI_SRCS
    "${FREERTOS_DIR}/Demo/motor_os/ui/*.c"
)
# Memory manager + Posix port
set(MEMMANG_SRCS
  "${KERNEL_DIR}/portable/MemMang/heap_3.c"
)
set(POSIX_SRCS
  "${KERNEL_DIR}/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c"
  "${KERNEL_DIR}/portable/ThirdParty/GCC/Posix/port.c"
)

# 汇总所有源
set(ALL_SRCS
  ${APP_SRCS}
  ${KERNEL_SRCS}
  ${MOTOR_DRIVERS_SRCS}
  ${MOTOR_HAL_SRCS}
  ${MOTOR_MIDDLEWARE_SRCS}
  ${MOTOR_UI_SRCS}
  ${MEMMANG_SRCS}
  ${POSIX_SRCS}
)

# —— 生成可执行文件 —— 
add_executable(${PROJECT_NAME} ${ALL_SRCS})

# —— include 路径 —— 
target_include_directories(${PROJECT_NAME} PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${KERNEL_DIR}/include"
  "${KERNEL_DIR}/portable/ThirdParty/GCC/Posix"
  "${KERNEL_DIR}/portable/ThirdParty/GCC/Posix/utils"
  "${FREERTOS_DIR}/Demo/Common/include"
  "${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-Trace/Include"
  "${FREERTOS_DIR}/Demo/motor_os/drivers"
  "${FREERTOS_DIR}/Demo/motor_os/hal"
  "${FREERTOS_DIR}/Demo/motor_os/middleware"
  "${FREERTOS_DIR}/Demo/motor_os/ui"
)

# —— 宏定义 —— 
# BUILD_DIR
target_compile_definitions(${PROJECT_NAME} PRIVATE
  BUILD_DIR=\"${CMAKE_BINARY_DIR}\"
  _WINDOWS_
)
# Trace & Coverage
if(TRACE_ON_ENTER)
  target_compile_definitions(${PROJECT_NAME} PRIVATE TRACE_ON_ENTER=1)
else()
  target_compile_definitions(${PROJECT_NAME} PRIVATE TRACE_ON_ENTER=0)
endif()
if(COVERAGE_TEST)
  target_compile_definitions(${PROJECT_NAME} PRIVATE projCOVERAGE_TEST=1)
else()
  target_compile_definitions(${PROJECT_NAME} PRIVATE projCOVERAGE_TEST=0)
endif()
# USER_DEMO
if(BLINKY_DEMO)
  target_compile_definitions(${PROJECT_NAME} PRIVATE USER_DEMO=0)
elseif(FULL_DEMO)
  target_compile_definitions(${PROJECT_NAME} PRIVATE USER_DEMO=1)
endif()

# —— 链接选项 —— 
# pthread 对应 -pthread
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

