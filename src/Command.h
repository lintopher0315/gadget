#pragma once

#include <string>
#include <ctype.h>
#include <regex>
#include <utility>

#define NORMAL_PREFIX 0
#define COMMAND_PREFIX 1

class Command {
    public:
        Command(void);

        void clear(void);
		bool isClear(void) const;
        int isValidNormal(void) const;
		int isValidCommand(void) const;
		int isValidVisual(void) const;
		int isValidLine(void) const;
        std::pair<int, std::string> parseNormal(void) const;
        std::vector<std::string> parseCommand(void) const;

        unsigned short prefix;
        std::string cmd;

    private:
        const std::string CMD_LIST[5]={"q", "(w|wq)( [^ ]+)?", "e [^ ]+", "tabedit( [^ ]+)*", "v?split"};
        const std::string ACT_LIST[10]={"[iIaA]", "([1-9][0-9]*)?[hjkl]", "([1-9][0-9]*)?[oO]", "[_$0]", "(gg|G|[1-9][0-9]*G)", "[1-9][0-9]*gt", "([1-9][0-9]*)?[wb]", "[fFtT].", "v", "V"};
		const std::string VIS_LIST[1]={"([1-9][0-9]*)?[hjkl]"};
		const std::string LIN_LIST[1]={"([1-9][0-9]*)?[jk]"};
};
