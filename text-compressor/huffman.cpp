#include "huffman.h"
#include "lz77.h"

#include <fstream>
#include <vector>
#include <iostream>

void Huffman::compressFile(const std::string& inputFile,
                           const std::string& outputFile)
{
    std::ifstream in(inputFile,std::ios::binary);

    std::string text((std::istreambuf_iterator<char>(in)),
                      std::istreambuf_iterator<char>());

    auto tokens = lz77Compress(text);

    std::ofstream out(outputFile,std::ios::binary);

    int count = tokens.size();
    out.write((char*)&count,4);

    for(auto& t : tokens)
    {
        out.write((char*)&t.offset,2);
        out.write((char*)&t.length,1);
        out.write(&t.next,1);
    }

    std::cout<<"Compressed to "<<outputFile<<"\n";
}

void Huffman::decompressFile(const std::string& inputFile,
                             const std::string& outputFile)
{
    std::ifstream in(inputFile,std::ios::binary);

    int count;

    in.read((char*)&count,4);

    std::vector<Token> tokens(count);

    for(int i=0;i<count;i++)
    {
        in.read((char*)&tokens[i].offset,2);
        in.read((char*)&tokens[i].length,1);
        in.read(&tokens[i].next,1);
    }

    std::string restored = lz77Decompress(tokens);

    std::ofstream out(outputFile,std::ios::binary);
    out<<restored;

    std::cout<<"Decompressed to "<<outputFile<<"\n";
}