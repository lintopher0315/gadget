#include "Command.h"

Command::Command() {
    const std::string CMD_LIST[]={"(wq|w|q|e)", "tabedit [^ ]+"};
    const std::string ACT_LIST[]={"[iIaA]", "[0-9]+[hjkl]"};
}

void Command::clear() {
    prefix = NORMAL_PREFIX;
    cmd = "";
}

bool Command::checkValid() {
    if (prefix == NORMAL_PREFIX) {
        for (int i = 0; i < sizeof(ACT_LIST) / sizeof(ACT_LIST[0]); ++i) {
            if (std::regex_match(cmd, std::regex(CMD_LIST[i]))) {
                return true;
            }
        }
    }
    else if (prefix == COMMAND_PREFIX) {
        for (int i = 0; i < sizeof(CMD_LIST) / sizeof(CMD_LIST[0]); ++i) {
            if (std::regex_match(cmd, std::regex(CMD_LIST[i]))) {
                return true;
            }
        }
    }
}
