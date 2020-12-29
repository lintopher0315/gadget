#include "Window.h"

Window::Window(wxWindow *parent) : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize) {
    sizer = new wxBoxSizer(wxVERTICAL);

    panel = new Panel(this);
    statusBar = new StatusBar(this);
    commandBar = new CommandBar(this);

    sizer->Add(panel, 1, wxEXPAND | wxALL, 0);
    sizer->Add(statusBar, 0, wxEXPAND | wxALL, -3);
    sizer->Add(commandBar, 0, wxEXPAND | wxALL, 0);

    SetSizer(sizer);

    command = new Command();

    mode = NORMAL_MODE;
    currEditor = 0;

    cwd = std::filesystem::current_path().string() + "/";
}

Frame *Window::getFrame() {
    return (Frame *)GetParent();
}

Editor *Window::getCurrentEditor() {
    return (Editor *)panel->GetPage(currEditor);
}

void Window::executeNormal(int cmdInd) {
    std::pair<int, std::string> parsedCmd;
    switch(cmdInd) {
        case 0:
            mode = EDIT_MODE;

            if (command->cmd == "a") {
                getCurrentEditor()->append();
            }
            else if (command->cmd == "A") {
                getCurrentEditor()->LineEnd();
            }
            else if (command->cmd == "I") {
                getCurrentEditor()->VCHome();
            }

            // change status bar; prob update this later
            statusBar->Clear();
            statusBar->AppendText("~ EDIT ~");
            break;
        case 1:
            parsedCmd = command->parseNormal();
            if (parsedCmd.second == "h") {
                getCurrentEditor()->caretLeft(parsedCmd.first);
            }
            else if (parsedCmd.second == "j") {
                getCurrentEditor()->caretDown(parsedCmd.first);
            }
            else if (parsedCmd.second == "k") {
                getCurrentEditor()->caretUp(parsedCmd.first);
            }
            else if (parsedCmd.second == "l") {
                getCurrentEditor()->caretRight(parsedCmd.first);
            }
            break;
        case 2:
            parsedCmd = command->parseNormal();
            if (parsedCmd.second == "o") {
                getCurrentEditor()->insertLineBelow(parsedCmd.first);
            }
            else if (parsedCmd.second == "O") {
                getCurrentEditor()->insertLineAbove(parsedCmd.first);
            }
            break;
        case 3:
            if (command->cmd == "_") {
                getCurrentEditor()->VCHome();
            }
            else if (command->cmd == "$") {
                getCurrentEditor()->LineEnd();
                getCurrentEditor()->caretLeft(1);
            }
            break;
    }
    command->clear();
    commandBar->Clear();
}

void Window::executeCommand(int cmdInd) {
    std::vector<std::string> parsedCmd;
    switch(cmdInd) {
        case 0:
            panel->DeletePage(currEditor);
            if (panel->GetPageCount() == 0) {
                getFrame()->Destroy();
            }
            break;
        case 1:
            parsedCmd = command->parseCommand();
            if (parsedCmd[0] == "w") {
                if (parsedCmd.size() == 1) {
                    if (getCurrentEditor()->relPath == "") {
                        // later this should display an error: editor has no file
                        break;
                    }
                }
                else {
                    if (isValidPath(parsedCmd[1])) {
                        getCurrentEditor()->relPath = parsedCmd[1];
                    }
                }
                getCurrentEditor()->SaveFile(cwd + getCurrentEditor()->relPath);
            }
            break;
        case 2:
            parsedCmd = command->parseCommand();
            break;
    }
    command->clear();
    commandBar->Clear();
}

bool Window::isValidPath(std::string relPath) {
    // currently only allows relative paths from cwd
    if (relPath.empty()) {
        return false;
    }
    std::string absPath = cwd + relPath;
    if (std::filesystem::exists(absPath)) {
        if (std::filesystem::is_regular_file(absPath)) {
            return true;
        }
        return false;
    }
    std::string absDir = absPath;
    // maybe check for backwards slash on Windows
    while (absDir.back() != '/') {
        absDir.pop_back();
    }
    if (std::filesystem::exists(absDir) && std::filesystem::is_directory(absDir)) {
        std::ofstream file{absPath};
        return true;
    }
    return false;
}
