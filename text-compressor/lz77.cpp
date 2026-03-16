#include "lz77.h"
#include <vector>

const int WINDOW = 4096;
const int LOOK = 18;
const int HASH = 65536;

inline int hash3(const std::string& s,int i)
{
    return ((s[i]<<8) ^ (s[i+1]<<4) ^ s[i+2]) & (HASH-1);
}

std::vector<Token> lz77Compress(const std::string& input)
{
    std::vector<Token> tokens;

    if(input.size() < 3)
    {
        for(char c : input)
            tokens.push_back({0,0,c});
        return tokens;
    }

    std::vector<int> head(HASH,-1);
    std::vector<int> prev(input.size(),-1);

    int pos = 0;

    while(pos < input.size())
    {
        int bestLen = 0;
        int bestOffset = 0;

        if(pos+2 < input.size())
        {
            int h = hash3(input,pos);

            int candidate = head[h];
            int depth = 0;

            while(candidate != -1 &&
                  pos - candidate <= WINDOW &&
                  depth < 64)
            {
                int length = 0;

                while(length < LOOK &&
                      pos+length < input.size() &&
                      input[candidate+length] == input[pos+length])
                {
                    length++;
                }

                if(length > bestLen)
                {
                    bestLen = length;
                    bestOffset = pos - candidate;
                }

                candidate = prev[candidate];
                depth++;
            }

            prev[pos] = head[h];
            head[h] = pos;
        }

        Token t;

        t.offset = bestOffset;
        t.length = bestLen;

        if(pos + bestLen < input.size())
            t.next = input[pos + bestLen];
        else
            t.next = '\0';

        tokens.push_back(t);

        pos += bestLen + 1;
    }

    return tokens;
}

std::string lz77Decompress(const std::vector<Token>& tokens)
{
    std::string output;

    for(const Token& t : tokens)
    {
        if(t.offset > 0)
        {
            int start = output.size() - t.offset;

            for(int i=0;i<t.length;i++)
                output += output[start+i];
        }

        if(t.next!='\0')
            output += t.next;
    }

    return output;
}