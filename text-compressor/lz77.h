#ifndef LZ77_H
#define LZ77_H

#include <string>
#include <vector>
#include <cstdint>

struct Token
{
    uint16_t offset;
    uint8_t length;
    char next;
};

std::vector<Token> lz77Compress(const std::string& input);
std::string lz77Decompress(const std::vector<Token>& tokens);

#endif