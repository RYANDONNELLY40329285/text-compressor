# 📦 C++ Text Compression Tool (LZ77)

A simple **lossless text compression tool written in C++** implementing the **LZ77 compression algorithm** with a hash-based match finder.

The program compresses text files into a custom binary format and can restore them back to their original contents.

This project demonstrates fundamental compression techniques used in real algorithms such as **DEFLATE (used in gzip / zip)**.

---

# 🚀 Features

- LZ77 sliding window compression
- Hash-based match search for fast compression
- Custom binary compression format
- Full decompression support
- Compression statistics output
- Works with large files
- Simple command-line executable


# 🧠 Algorithm Overview

The compressor uses the **LZ77 algorithm**, which replaces repeated sequences with references to previous occurrences.

Instead of storing repeated text like:


hello hello hello hello


The compressor stores tokens in the form:


(offset, length, next_char)


Example token:


(5,5,' ')


Meaning:


Copy 5 characters from 5 positions back, then append a space


This allows repeated patterns to be stored very efficiently.

---

# 🏗 Project Structure


text-compressor
│
├── main.cpp
├── lz77.h
├── lz77.cpp
├── huffman.h
├── huffman.cpp
│
├── input.txt
├── input.huff
├── input_restored.txt
│
└── README.md


---

# ⚙️ Build Instructions

Requires a C++ compiler such as **g++ / MinGW / MSYS2 / Clang**.

Compile the project with:

```bash
g++ main.cpp lz77.cpp huffman.cpp -O2 -o compressor
▶️ Running the Program

Run the executable:

./compressor

The program will:

Compress input.txt

Create input.huff

Decompress the file

Produce input_restored.txt

Display compression statistics

Example output:

===== Compression Results =====

Original size:   2450000 bytes
Compressed size: 515800 bytes
Restored size:   2450000 bytes

Compression ratio: 21.05%
Space saved:       78.94%


🔍 How It Works
1️⃣ Read Input File

The input file is loaded into memory.

2️⃣ LZ77 Compression

The algorithm searches for repeated sequences within a sliding window of previous data.

3️⃣ Token Encoding

Matches are stored using the format:

offset
length
next character
4️⃣ Binary Output

Tokens are written to a compressed binary file.

5️⃣ Decompression

The original file is reconstructed by replaying the token sequence.

📚 Concepts Demonstrated

This project demonstrates:

Lossless data compression

Sliding window algorithms

Hash-based search structures

Binary file formats

C++ file I/O

Algorithm implementation

Performance measurement