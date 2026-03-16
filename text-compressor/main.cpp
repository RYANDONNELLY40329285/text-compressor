#include <iostream>
#include <string>
#include <filesystem>
#include "huffman.h"

int main()
{
    Huffman h;

    while(true)
    {
        std::cout << "\n==== Huffman Compressor ====\n";
        std::cout << "1. Compress file\n";
        std::cout << "2. Decompress file\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter choice: ";

        int choice;
        std::cin >> choice;

        if(choice == 1)
        {
            std::string input;

            std::cout << "Enter input file: ";
            std::cin >> input;

            std::string output =
                std::filesystem::path(input).stem().string() + ".huff";

            h.compressFile(input, output);

            std::cout << "Compressed file created: " << output << "\n";
        }

        else if(choice == 2)
        {
            std::string input;

            std::cout << "Enter compressed file: ";
            std::cin >> input;

            std::string output =
                std::filesystem::path(input).stem().string() + "_restored.txt";

            h.decompressFile(input, output);

            std::cout << "Restored file created: " << output << "\n";
        }

        else if(choice == 3)
        {
            std::cout << "Exiting program.\n";
            break;
        }

        else
        {
            std::cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}