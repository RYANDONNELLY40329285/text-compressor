#pragma once
#include <string>

struct Node {
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

class Huffman {
public:
    Node* root = nullptr;

    std::string compress(const std::string& text);
    std::string decompress(const std::string& encoded);
};