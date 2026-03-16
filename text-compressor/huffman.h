#pragma once
#include <string>
#include <unordered_map>

struct Node
{
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char c, int f)
    {
        ch = c;
        freq = f;
        left = nullptr;
        right = nullptr;
    }
};

class Huffman
{
public:
    void compressFile(const std::string& inputFile,
                      const std::string& outputFile);

    void decompressFile(const std::string& inputFile,
                        const std::string& outputFile);
};