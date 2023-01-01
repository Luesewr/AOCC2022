#cmake_minimum_required(VERSION 3.21)
project(AOCC C)

set(CMAKE_C_STANDARD 99)

set(PROJECT_HEADERS
#        days/day07/day7_functions.h
#        days/day09/day9_functions.h
#        days/day11/day11_functions.h
#        days/day13/day13_functions.h
#        days/day14/day14_functions.h
#        days/day15/day15_functions.h
#        days/day16/day16_functions.h
        library/pointerList.h library/intList.h)

set(PROJECT_SOURCES
#        days/day01/day1_1.c
#        days/day01/day1_2.c
#        days/day02/day2_1.c
#        days/day02/day2_2.c
#        days/day03/day3_1.c
#        days/day03/day3_2.c
#        days/day04/day4_1.c
#        days/day04/day4_2.c
#        days/day05/day5_1.c
#        days/day05/day5_2.c
#        days/day06/day6_1.c
#        days/day06/day6_2.c
#        days/day07/day7_functions.c
#        days/day07/day7_1.c
#        days/day07/day7_2.c
#        days/day08/day8_1.c
#        days/day08/day8_2.c
#        days/day09/day9_functions.c
#        days/day09/day9_1.c
#        days/day09/day9_2.c
#        days/day10/day10_1.c
#        days/day10/day10_2.c
#        days/day11/day11_functions.c
#        days/day11/day11_1.c
#        days/day11/day11_2.c
#        days/day12/day12_1.c
#        days/day12/day12_2.c
#        days/day13/day13_functions.c
#        days/day13/day13_1.c
#        days/day13/day13_2.c
#        days/day14/day14_functions.c
#        days/day14/day14_1.c
#        days/day14/day14_2.c
#        days/day15/day15_functions.c
#        days/day15/day15_1.c
#        days/day15/day15_2.c
#        days/day16/day16_functions.c
#        days/day16/day16_1.c
#        days/day16/day16_2.c
#        days/day17/day17_1.c
#        days/day17/day17_2.c
#        days/day18/day18_1.c
#        days/day18/day18_2.c
#        days/day19/day19_1.c
#        days/day19/day19_2.c
#        days/day20/day20_1.c
#        days/day20/day20_2.c
#        days/day21/day21_1.c
#        days/day21/day21_2.c
#        days/day22/day22_1.c
#        days/day22/day22_2.c
#        days/day23/day23_1.c
#        days/day23/day23_2.c
#        days/day24/day24_1.c
#        days/day24/day24_2.c
        days/day25/day25_1.c
#        days/day25/day25_2.c
        library/pointerList.c library/intList.c)

add_executable(main ${PROJECT_SOURCES} ${PROJECT_HEADERS})