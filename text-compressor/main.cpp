#include "huffman.h"

int main()
{
    Huffman h;

    h.compressFile("input.txt","compressed.bin");
    h.decompressFile("compressed.bin","restored.txt");

    return 0;
}