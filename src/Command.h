#pragma once

#include <string>
#include <ctype.h>

class Command {
    public:
        void add(char c);
        void clear(void);
        bool checkValid(void); // checks using regex if current command is executable

        std::string prefix;
        unsigned int number;
        std::string action;
};
