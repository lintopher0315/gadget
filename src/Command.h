#pragma once

#include <string>
#include <ctype.h>

#define NORMAL_PREFIX 0
#define COMMAND_PREFIX 1

class Command {
    public:
        void add(char c);
        void clear(void);
        bool checkValid(void); // checks using regex if current command is executable

        unsigned short prefix;
        unsigned int number;
        std::string action;
};
