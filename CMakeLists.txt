cmake_minimum_required(VERSION 3.21)
project(pointerList C)

set(CMAKE_C_STANDARD 99)

set(PROJECT_HEADERS
        #days/day1_1.h
        days/day1_2.h
        library/pointerList.h library/intList.h)

set(PROJECT_SOURCES
        #days/day1_1.c
        days/day1_2.c
        library/pointerList.c library/intList.c)

add_executable(main ${PROJECT_SOURCES} ${PROJECT_HEADERS})