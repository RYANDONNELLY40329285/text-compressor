#include "huffman.h"

#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>

struct Compare
{
    bool operator()(Node* a, Node* b)
    {
        if (a->freq == b->freq)
            return a->ch > b->ch;
        return a->freq > b->freq;
    }
};

void generateCodes(Node* root,
                   std::string code,
                   std::unordered_map<char,std::string>& codes)
{
    if(!root) return;

    if(!root->left && !root->right)
        codes[root->ch] = code;

    generateCodes(root->left, code + "0", codes);
    generateCodes(root->right, code + "1", codes);
}

Node* buildTree(std::unordered_map<char,int>& freq)
{
    std::priority_queue<Node*,std::vector<Node*>,Compare> pq;

    for(auto& p : freq)
        pq.push(new Node(p.first,p.second));

    while(pq.size() > 1)
    {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        Node* parent = new Node('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }

    return pq.top();
}

std::vector<unsigned char> packBits(const std::string& bits)
{
    std::vector<unsigned char> bytes;

    for(size_t i=0;i<bits.size();i+=8)
    {
        std::string byteStr = bits.substr(i,8);

        while(byteStr.size()<8)
            byteStr += "0";

        unsigned char byte = 0;

        for(char c : byteStr)
        {
            byte <<= 1;
            if(c=='1') byte |= 1;
        }

        bytes.push_back(byte);
    }

    return bytes;
}

std::string unpackBits(std::vector<unsigned char>& bytes)
{
    std::string bits;

    for(unsigned char b : bytes)
    {
        for(int i=7;i>=0;i--)
            bits += ((b>>i)&1)?'1':'0';
    }

    return bits;
}

void Huffman::compressFile(const std::string& inputFile,
                           const std::string& outputFile)
{
    std::ifstream in(inputFile);

    std::string text((std::istreambuf_iterator<char>(in)),
                      std::istreambuf_iterator<char>());

    std::unordered_map<char,int> freq;

    for(char c : text)
        freq[c]++;

    Node* root = buildTree(freq);

    std::unordered_map<char,std::string> codes;

    generateCodes(root,"",codes);

    std::string encoded;

    for(char c : text)
        encoded += codes[c];

    std::vector<unsigned char> packed = packBits(encoded);

    std::ofstream out(outputFile,std::ios::binary);

    int tableSize = freq.size();
    out.write((char*)&tableSize,sizeof(tableSize));

    for(auto& p : freq)
    {
        out.write(&p.first,sizeof(char));
        out.write((char*)&p.second,sizeof(int));
    }

    int bitLength = encoded.size();
    out.write((char*)&bitLength,sizeof(bitLength));

    int dataSize = packed.size();
    out.write((char*)&dataSize,sizeof(dataSize));

    for(auto b : packed)
        out.put(b);

    std::cout<<"Compressed to "<<outputFile<<"\n";
}

void Huffman::decompressFile(const std::string& inputFile,
                             const std::string& outputFile)
{
    std::ifstream in(inputFile,std::ios::binary);

    int tableSize;
    in.read((char*)&tableSize,sizeof(tableSize));

    std::unordered_map<char,int> freq;

    for(int i=0;i<tableSize;i++)
    {
        char c;
        int f;

        in.read(&c,sizeof(char));
        in.read((char*)&f,sizeof(int));

        freq[c]=f;
    }

    Node* root = buildTree(freq);

    int bitLength;
    in.read((char*)&bitLength,sizeof(bitLength));

    int dataSize;
    in.read((char*)&dataSize,sizeof(dataSize));

    std::vector<unsigned char> bytes(dataSize);

    for(int i=0;i<dataSize;i++)
        bytes[i]=in.get();

    std::string bits = unpackBits(bytes);

    bits = bits.substr(0,bitLength);

    std::ofstream out(outputFile);

    Node* current = root;

    for(char bit : bits)
    {
        if(bit=='0')
            current = current->left;
        else
            current = current->right;

        if(!current->left && !current->right)
        {
            out<<current->ch;
            current = root;
        }
    }

    std::cout<<"Decompressed to "<<outputFile<<"\n";
}