#pragma once

#include <string>
#include <ctype.h>
#include <regex>
#include <utility>

#define NORMAL_PREFIX 0
#define COMMAND_PREFIX 1

class Command {
    public:
        Command();

        void clear(void);
        int isValid(void) const;
        std::pair<int, std::string> parseNormal(void) const;
        std::vector<std::string> parseCommand(void) const;

        unsigned short prefix;
        std::string cmd;

    private:
        const std::string CMD_LIST[4]={"q", "(w|wq)( [^ ]+)?", "e [^ ]+", "tabedit( [^ ]+)*"};
        const std::string ACT_LIST[5]={"[iIaA]", "[1-9]*[hjkl]", "[1-9]*[oO]", "[_$0]", "[1-9][0-9]*gt"};
};
