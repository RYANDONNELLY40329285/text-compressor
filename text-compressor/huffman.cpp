#include "huffman.h"

#include <unordered_map>
#include <queue>
#include <vector>
#include <fstream>
#include <iostream>

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

void generateCodes(Node* root, std::string code,
                   std::unordered_map<char, std::string>& codes)
{
    if (!root)
        return;

    if (!root->left && !root->right)
        codes[root->ch] = code;

    generateCodes(root->left, code + "0", codes);
    generateCodes(root->right, code + "1", codes);
}

std::vector<unsigned char> packBits(const std::string& bits)
{
    std::vector<unsigned char> bytes;

    for (size_t i = 0; i < bits.size(); i += 8)
    {
        std::string byteStr = bits.substr(i, 8);

        while (byteStr.size() < 8)
            byteStr += "0";

        unsigned char byte = 0;

        for (char c : byteStr)
        {
            byte <<= 1;
            if (c == '1')
                byte |= 1;
        }

        bytes.push_back(byte);
    }

    return bytes;
}

std::string Huffman::compress(const std::string& text)
{

    std::unordered_map<char, int> freq;

    for (char c : text)
        freq[c]++;

    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;

    for (auto& p : freq)
        pq.push(new Node(p.first, p.second));

    while (pq.size() > 1)
    {
        Node* left = pq.top();
        pq.pop();

        Node* right = pq.top();
        pq.pop();

        Node* parent = new Node('\0', left->freq + right->freq);

        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }

    Node* root = pq.top();

    std::unordered_map<char, std::string> codes;

    generateCodes(root, "", codes);

    std::cout << "\nHuffman Codes\n";

    for (auto& p : codes)
    {
        if (p.first == ' ')
            std::cout << "[space]";
        else
            std::cout << p.first;

        std::cout << " -> " << p.second << "\n";
    }

    std::string encoded;

    for (char c : text)
        encoded += codes[c];

    std::vector<unsigned char> packed = packBits(encoded);

    std::ofstream file("compressed.bin", std::ios::binary);

    for (auto b : packed)
        file.put(b);

    file.close();

    std::cout << "\nEncoded bits: " << encoded.size() << "\n";
    std::cout << "Compressed file written: compressed.bin\n";

    return encoded;
}

std::string Huffman::decompress(const std::string& encoded)
{
    std::string result;

    Node* current = root;

    for(char bit : encoded)
    {
        if(bit == '0')
            current = current->left;
        else
            current = current->right;

        if(!current->left && !current->right)
        {
            result += current->ch;
            current = root;
        }
    }

    return result;
}