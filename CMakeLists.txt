project(killproc)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -U__STRICT_ANSI__ -Wall -Wextra -Wpedantic -Woverloaded-virtual -fpermissive")

add_executable(killproc killproc.cpp getpid.cpp)
