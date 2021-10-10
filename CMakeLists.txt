cmake_minimum_required(VERSION 3.15)
project(Huffman_Coding)

set(CMAKE_CXX_STANDARD 14)



set (input01 "sampleinput.txt")

#
##########################################

# Copy data files to build dir
foreach(file IN LISTS input01)
    configure_file(${file} ${file} COPYONLY)
endforeach()

add_executable(Huffman_Coding main.cpp Compressor.h Compressor.cpp Node.h Node.cpp Decompressor.h Decompressor.cpp)
