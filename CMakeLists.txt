cmake_minimum_required(VERSION 3.30.5)

set(SOURCES
        src/Master.cpp
        src/Device.cpp
)

set(COMPONENTS
        driver
)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})
