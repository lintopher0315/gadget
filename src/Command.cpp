#include "Command.h"

Command::Command() {
    prefix = NORMAL_PREFIX;
}

void Command::clear() {
    prefix = NORMAL_PREFIX;
    cmd = "";
}

int Command::isValid() {
    // instead of looping; concatenate with proper regex and check for match
    if (prefix == NORMAL_PREFIX) {
        for (int i = 0; i < sizeof(ACT_LIST) / sizeof(ACT_LIST[0]); ++i) {
            if (std::regex_match(cmd, std::regex(ACT_LIST[i]))) {
                return i;
            }
        }
    }
    else if (prefix == COMMAND_PREFIX) {
        for (int i = 0; i < sizeof(CMD_LIST) / sizeof(CMD_LIST[0]); ++i) {
            if (std::regex_match(cmd, std::regex(CMD_LIST[i]))) {
                return i;
            }
        }
    }
    return -1;
}

// TODO:
// create function that automatically parses command into (num, action)
