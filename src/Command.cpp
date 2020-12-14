#include "Command.h"

void Command::add(char c) {
    if (isdigit(c)) {
        if (action == "") {
            if (!c) {
                return;
            }
            else {
                number *= 10;
                number += c - '0';
            }
        }
        else {
            action += c;
        }
    }
    else {
        action += c;
    }
}

void Command::clear() {
    prefix = NORMAL_PREFIX;
    number = 0;
    action = "";
}

bool Command::checkValid() {

}
