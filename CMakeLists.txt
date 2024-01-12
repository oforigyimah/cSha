cmake_minimum_required(VERSION 3.27)
project(cSha C)

set(CMAKE_C_STANDARD 23)

add_executable(cSha main.c
        cSha.h
        constants.c
        constants.h
        sortFile.c
        set.c
        get_mt.c
        get_mt.h
        sha.c
)
