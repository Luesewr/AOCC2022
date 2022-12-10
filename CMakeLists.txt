#cmake_minimum_required(VERSION 3.21)
project(pointerList C)

set(CMAKE_C_STANDARD 99)

set(PROJECT_HEADERS
#        days/day1/day1_1.h
#        days/day1/day1_2.h
#        days/day2/day2_1.h
#        days/day2/day2_2.h
#        days/day3/day3_1.h
#        days/day3/day3_2.h
#        days/day4/day4_1.h
#        days/day4/day4_2.h
#        days/day5/day5_1.h
#        days/day5/day5_2.h
#        days/day6/day6_1.h
#        days/day6/day6_2.h
#        days/day7/day7_functions.h
#        days/day7/day7_1.h
#        days/day7/day7_2.h
#        days/day8/day8_1.h
#        days/day8/day8_2.h
        days/day9/day9_functions.h
#        days/day9/day9_1.h
        days/day9/day9_2.h
#        days/day10/day10_1.h
#        days/day10/day10_2.h
        library/pointerList.h library/intList.h)

set(PROJECT_SOURCES
#        days/day1/day1_1.c
#        days/day1/day1_2.c
#        days/day2/day2_1.c
#        days/day2/day2_2.c
#        days/day3/day3_1.c
#        days/day3/day3_2.c
#        days/day4/day4_1.c
#        days/day4/day4_2.c
#        days/day5/day5_1.c
#        days/day5/day5_2.c
#        days/day6/day6_1.c
#        days/day6/day6_2.c
#        days/day7/day7_functions.c
#        days/day7/day7_1.c
#        days/day7/day7_2.c
#        days/day8/day8_1.c
#        days/day8/day8_2.c
        days/day9/day9_functions.c
#        days/day9/day9_1.c
        days/day9/day9_2.c
#        days/day10/day10_1.c
#        days/day10/day10_2.c
        library/pointerList.c library/intList.c)

add_executable(main ${PROJECT_SOURCES} ${PROJECT_HEADERS})