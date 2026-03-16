#include <iostream>
#include <string>
#include "file_utils.h"
#include "huffman.h"

int main()
{
    std::string text = readFile("input.txt");

    Huffman h;

    std::string encoded = h.compress(text);

    std::cout << "\nEncoded bits:\n";
    std::cout << encoded << "\n";

    std::string decoded = h.decompress(encoded);

    std::cout << "\nDecoded text:\n";
    std::cout << decoded << "\n";

    std::cout << "\nOriginal size: " << text.size() << " bytes\n";
    std::cout << "Encoded bits: " << encoded.size() << "\n";

    return 0;
}