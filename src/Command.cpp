#include "Command.h"

void Command::add(char c) {
    if (c == ':') {
        if (prefix == "") {
            prefix += c;
        }
        else {
            action += c;
        }
    }
    else if (isalpha(c)) {
        action += c;
    }
    else if (isdigit(c)) {
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
}

void Command::clear() {
    prefix = "";
    number = 0;
    action = "";
}

bool Command::checkValid() {

}
