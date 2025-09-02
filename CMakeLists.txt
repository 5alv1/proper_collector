cmake_minimum_required(VERSION 3.22)
project(proper_collector C)

set(CMAKE_C_STANDARD 23)

add_library(proper_collector STATIC collector.c
        storage.h
        storage.c
        utils.h)
