cmake_minimum_required(VERSION 3.22.1)
project(vulkan_geometry)

add_library(vulkan_geometry SHARED main.cpp)

find_library(log-lib log)
find_library(android-lib android)
find_library(vulkan-lib vulkan)

target_link_libraries(vulkan_geometry ${log-lib} ${android-lib} ${vulkan-lib})
