#include "huffman.h"
#include "lz77.h"

#include <fstream>
#include <iostream>   
#include <queue>
#include <vector>
#include <unordered_map>

struct Node
{
    char ch;
    int freq;

    Node* left;
    Node* right;

    Node(char c,int f)
    {
        ch=c;
        freq=f;
        left=nullptr;
        right=nullptr;
    }
};

struct Compare
{
    bool operator()(Node* a,Node* b)
    {
        return a->freq > b->freq;
    }
};

Node* buildTree(std::unordered_map<char,int>& freq)
{
    std::priority_queue<Node*,std::vector<Node*>,Compare> pq;

    for(auto&p:freq)
        pq.push(new Node(p.first,p.second));

    while(pq.size()>1)
    {
        Node* a=pq.top(); pq.pop();
        Node* b=pq.top(); pq.pop();

        Node* parent=new Node('\0',a->freq+b->freq);
        parent->left=a;
        parent->right=b;

        pq.push(parent);
    }

    return pq.top();
}

void buildCodes(Node* root,std::string code,
                std::unordered_map<char,std::string>& codes)
{
    if(!root) return;

    if(!root->left && !root->right)
        codes[root->ch]=code;

    buildCodes(root->left,code+"0",codes);
    buildCodes(root->right,code+"1",codes);
}

void Huffman::compressFile(const std::string& inputFile,
                           const std::string& outputFile)
{
    std::ifstream in(inputFile,std::ios::binary);

    std::string text((std::istreambuf_iterator<char>(in)),
                      std::istreambuf_iterator<char>());

    auto tokens = lz77Compress(text);

    std::string tokenData;

    for(auto&t:tokens)
    {
        tokenData+=(char)(t.offset>>8);
        tokenData+=(char)(t.offset&255);
        tokenData+=(char)t.length;
        tokenData+=t.next;
    }

    std::unordered_map<char,int> freq;

    for(char c:tokenData)
        freq[c]++;

    Node* root=buildTree(freq);

    std::unordered_map<char,std::string> codes;
    buildCodes(root,"",codes);

    std::string encoded;

    for(char c:tokenData)
        encoded+=codes[c];

    std::ofstream out(outputFile,std::ios::binary);

    int size = tokenData.size();
    out.write((char*)&size,4);

    int table = freq.size();
    out.write((char*)&table,4);

    for(auto&p:freq)
    {
        out.write(&p.first,1);
        out.write((char*)&p.second,4);
    }

    int bits = encoded.size();
    out.write((char*)&bits,4);

    for(size_t i=0;i<bits;i+=8)
    {
        unsigned char byte=0;

        for(int j=0;j<8;j++)
        {
            byte<<=1;

            if(i+j<bits && encoded[i+j]=='1')
                byte|=1;
        }

        out.put(byte);
    }
}

void Huffman::decompressFile(const std::string& inputFile,
                             const std::string& outputFile)
{
    std::ifstream in(inputFile, std::ios::binary);

    if(!in)
    {
        std::cerr<<"Error opening compressed file\n";
        return;
    }

    int tokenSize;
    in.read((char*)&tokenSize,4);

    int tableSize;
    in.read((char*)&tableSize,4);

    std::unordered_map<char,int> freq;

    for(int i=0;i<tableSize;i++)
    {
        char c;
        int f;

        in.read(&c,1);
        in.read((char*)&f,4);

        freq[c] = f;
    }

    Node* root = buildTree(freq);

    int bitLength;
    in.read((char*)&bitLength,4);

    std::vector<unsigned char> bytes;

    while(in.peek()!=EOF)
        bytes.push_back(in.get());

    std::string bits;

    for(unsigned char b : bytes)
    {
        for(int i=7;i>=0;i--)
            bits += ((b>>i)&1)?'1':'0';
    }

    bits = bits.substr(0,bitLength);

    std::string decoded;

    Node* current = root;

    for(char bit : bits)
    {
        if(bit=='0')
            current=current->left;
        else
            current=current->right;

        if(!current->left && !current->right)
        {
            decoded+=current->ch;
            current=root;
        }
    }

    decoded = decoded.substr(0,tokenSize);

    std::vector<Token> tokens;

    for(size_t i=0;i+3<decoded.size();i+=4)
    {
        Token t;

        t.offset = ((unsigned char)decoded[i]<<8) |
                   (unsigned char)decoded[i+1];

        t.length = (unsigned char)decoded[i+2];
        t.next   = decoded[i+3];

        tokens.push_back(t);
    }

    std::string restored = lz77Decompress(tokens);

    std::ofstream out(outputFile,std::ios::binary);
    out<<restored;
}