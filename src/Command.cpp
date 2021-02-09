#include "Command.h"

Command::Command(void) {
    prefix = NORMAL_PREFIX;
}

void Command::clear(void) {
    prefix = NORMAL_PREFIX;
    cmd = "";
}

bool Command::isClear(void) const {
	return prefix == NORMAL_PREFIX && cmd.empty();
}

int Command::isValidNormal(void) const {
    if (prefix == NORMAL_PREFIX) {
        for (int i = 0; i < sizeof(ACT_LIST) / sizeof(ACT_LIST[0]); ++i) {
            if (std::regex_match(cmd, std::regex(ACT_LIST[i]))) {
                return i;
            }
        }
    }
    return -1;
}

int Command::isValidCommand(void) const {
    if (prefix == COMMAND_PREFIX) {
        for (int i = 0; i < sizeof(CMD_LIST) / sizeof(CMD_LIST[0]); ++i) {
            if (std::regex_match(cmd, std::regex(CMD_LIST[i]))) {
                return i;
            }
        }
    }
	return -1;
}

int Command::isValidVisual(void) const {
    if (prefix == NORMAL_PREFIX) {
        for (int i = 0; i < sizeof(VIS_LIST) / sizeof(VIS_LIST[0]); ++i) {
            if (std::regex_match(cmd, std::regex(VIS_LIST[i]))) {
                return i;
            }
        }
    }
    return -1;
}

int Command::isValidLine(void) const {
    if (prefix == NORMAL_PREFIX) {
        for (int i = 0; i < sizeof(LIN_LIST) / sizeof(LIN_LIST[0]); ++i) {
            if (std::regex_match(cmd, std::regex(LIN_LIST[i]))) {
                return i;
            }
        }
    }
    return -1;
}

std::pair<int, std::string> Command::parseNormal() const {
    int num = 0;
    int i = 0;
    while (i < cmd.size() && isdigit(cmd[i])) {
        num *= 10;
        num += cmd[i] - '0';
        ++i;
    }
    return {std::max(1, num), cmd.substr(i, cmd.size()-i)};
}

std::vector<std::string> Command::parseCommand() const {
    std::vector<std::string> result;
    std::string token;
    std::istringstream tokenStream(cmd);
    while (std::getline(tokenStream, token, ' ')) {
        result.push_back(token);
    }
    return result;
}
