#include "huffman.h"
#include <iostream>
#include <fstream>

long getFileSize(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if(!file)
        return -1;

    return file.tellg();
}

int main()
{
    Huffman h;

    std::string input = "input.txt";
    std::string compressed = "input.huff";
    std::string restored = "input_restored.txt";

    h.compressFile(input, compressed);
    h.decompressFile(compressed, restored);

    long originalSize = getFileSize(input);
    long compressedSize = getFileSize(compressed);
    long restoredSize = getFileSize(restored);

    std::cout << "\n===== Compression Results =====\n";

    std::cout << "Original size:   " << originalSize << " bytes\n";
    std::cout << "Compressed size: " << compressedSize << " bytes\n";
    std::cout << "Restored size:   " << restoredSize << " bytes\n";

    if(originalSize > 0)
    {
        double ratio = (double)compressedSize / originalSize * 100;
        double saved = 100 - ratio;

        std::cout << "Compression ratio: " << ratio << "%\n";
        std::cout << "Space saved:       " << saved << "%\n";
    }

    std::cout << "===============================\n";

    return 0;
}