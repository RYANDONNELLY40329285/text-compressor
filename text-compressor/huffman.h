#pragma once
#include <string>

class Huffman
{
public:

    void compressFile(const std::string& inputFile,
                      const std::string& outputFile);

    void decompressFile(const std::string& inputFile,
                        const std::string& outputFile);
};