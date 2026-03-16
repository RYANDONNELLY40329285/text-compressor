#include "lz77.h"
#include <fstream>
#include <iostream>

void compressFile(const std::string& inputFile,
                  const std::string& outputFile)
{
    std::ifstream in(inputFile);

    std::string text((std::istreambuf_iterator<char>(in)),
                     std::istreambuf_iterator<char>());

    auto tokens = lz77Compress(text);

    std::ofstream out(outputFile, std::ios::binary);

    int tokenCount = tokens.size();
    out.write((char*)&tokenCount, sizeof(tokenCount));

    for (auto& t : tokens)
    {
        out.write((char*)&t.offset, sizeof(t.offset));
        out.write((char*)&t.length, sizeof(t.length));
        out.write(&t.next, 1);
    }

    std::cout << "Compressed to " << outputFile << "\n";
}

void decompressFile(const std::string& inputFile,
                    const std::string& outputFile)
{
    std::ifstream in(inputFile, std::ios::binary);

    int tokenCount;
    in.read((char*)&tokenCount, sizeof(tokenCount));

    std::vector<Token> tokens(tokenCount);

    for (int i = 0; i < tokenCount; i++)
    {
        in.read((char*)&tokens[i].offset, sizeof(uint16_t));
        in.read((char*)&tokens[i].length, sizeof(uint8_t));
        in.read(&tokens[i].next, 1);
    }

    std::string text = lz77Decompress(tokens);

    std::ofstream out(outputFile);
    out << text;

    std::cout << "Decompressed to " << outputFile << "\n";
}