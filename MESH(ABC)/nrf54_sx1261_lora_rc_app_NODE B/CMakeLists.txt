cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(nrf-projects_lora_sx1261_rx)

target_sources(app PRIVATE src/main.c)
