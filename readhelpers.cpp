#include "readhelpers.h"

#include <iostream>
#include <cctype>

bool canReadMore()
{
    return std::cin.peek() == ' ' || islower(std::cin.peek());
}

void readWordsFromCin(char* buffer, size_t size)
{
    buffer[size - 1] = '\0';
    std::cin.read(buffer, size - 1);

    if (islower(std::cin.peek()))
    {
        char* ch = buffer + size - 2;

        do
        {
            std::cin.putback(*ch);
            --ch;
        } while (*ch != ' ');
        *ch = '\0';
    }
}