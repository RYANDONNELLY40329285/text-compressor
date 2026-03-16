#include <iostream>
#include "huffman.h"

int main(int argc, char* argv[])
{
    if(argc < 4)
    {
        std::cout<<"Usage:\n";
        std::cout<<"compressor compress input.txt output.huff\n";
        std::cout<<"compressor decompress input.huff output.txt\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string input = argv[2];
    std::string output = argv[3];

    Huffman h;

    if(mode == "compress")
        h.compressFile(input,output);

    else if(mode == "decompress")
        h.decompressFile(input,output);

    else
        std::cout<<"Unknown command\n";
}