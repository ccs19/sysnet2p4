cmake_minimum_required(VERSION 3.1)
project(sysnet2p4)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(SOURCE_FILES rdtSender.c common.c)
add_executable(rdtSender ${SOURCE_FILES} )

set(SOURCE_FILES rdtReceiver.c common.c)
add_executable(rdtReceiver ${SOURCE_FILES})

set(SOURCE_FILES proxy.c common.c)
add_executable(proxy ${SOURCE_FILES})