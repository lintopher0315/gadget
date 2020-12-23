#pragma once

#include <string>
#include <ctype.h>
#include <regex>

#define NORMAL_PREFIX 0
#define COMMAND_PREFIX 1

class Command {
    public:
        Command();

        void clear(void);
        int isValid(void);

        unsigned short prefix;
        std::string cmd;

    private:
        const std::string CMD_LIST[2]={"(wq|w|q|e)", "tabedit [^ ]+"};
        const std::string ACT_LIST[2]={"[iIaA]", "[0-9]*[hjkl]"};
};
