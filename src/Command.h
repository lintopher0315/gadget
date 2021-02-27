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
        const std::string CMD_LIST[6]={"q", "(w|wq)( [^ ]+)?", "e [^ ]+", "tabedit( [^ ]+)*", "v?split", "help"};
        const std::string ACT_LIST[13]={"[iIaA]", "([1-9][0-9]*)?[hjkl]", "([1-9][0-9]*)?[oO]", "[_$0]", "(gg|G|[1-9][0-9]*G)", "[1-9][0-9]*gt", "([1-9][0-9]*)?[wb]", "[fFtT].", "v", "V", "[pP]", "x", "(([1-9][0-9]*)?dd|D)"};
		const std::string VIS_LIST[8]={"([1-9][0-9]*)?[hjkl]", "[xd]", "[uU]", "(gg|G|[1-9][0-9]*G)", "y", "([1-9][0-9]*)?[wb]", "[_$0]", "[fFtT]."};
		const std::string LIN_LIST[6]={"([1-9][0-9]*)?[jk]", "[xd]", "[uU]", "([1-9][0-9]*)?[<>]", "y", "(gg|G|[1-9][0-9]*G)"};
};
