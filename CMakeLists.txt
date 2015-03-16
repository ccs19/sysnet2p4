cmake_minimum_required(VERSION 3.1)
project(sysnet2p4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES sender.c)
add_executable(sysnet2p4 ${SOURCE_FILES} commondefinitions.h rdtSender.h rdtReceiver.h)