# Huffman Coding

### Introduction
This is a Huffman Compressor and Decompressor created by Xuan(James) Zhai. 

Its idea is to read a list of chars as data and save each char as an 8 bits binary number. The program will build a Huffman tree based on those numbers. After the compression, the program will create a compressed.txt and save the compressed code. It will also created a key.txt to store the tree.

For the decompression, it will rebuild the tree based on the key.txt and find the decompressed code with compressed.txt. After that, it will store the decompressed data into Decompressed.txt, and it will compare that with the one before compression.

I did not use design patterns when I initially constructed this program, but I can see the advantages in it. Therefore, in the later versions, I may add design patterns to improve the code structure.


### How to run the code
You will need to pass in a parameter which is the filename of your file that you want to compress.

It will create compressed data and decompressed data as txt files.

It will also print the comparison accuracy to the prompt.


Update:

    v1.1: The input file now accepts space and and newline. Please check the sample input file.

    v1.2: Use array on the heap instead of vector to avoid touching the memory limit.

    v1.3: Make a release for the code.