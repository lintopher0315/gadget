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

Frame *Window::getFrame() const {
    return (Frame *)GetParent();
}

Editor *Window::getCurrentEditor() const {
    return (Editor *)panel->GetPage(currEditor);
}

void Window::executeNormal(const int& cmdInd) {
    std::pair<int, std::string> parsedCmd;
    Editor *e = getCurrentEditor();
    switch(cmdInd) {
        case 0:
            mode = EDIT_MODE;

            if (command->cmd == "a") {
                e->append();
            }
            else if (command->cmd == "A") {
                e->LineEnd();
            }
            else if (command->cmd == "I") {
                e->VCHome();
            }

            // change status bar; prob update this later
            statusBar->Clear();
            statusBar->AppendText("~ EDIT ~");
            break;
        case 1:
            parsedCmd = command->parseNormal();
            if (parsedCmd.second == "h") {
                e->caretLeft(parsedCmd.first);
            }
            else if (parsedCmd.second == "j") {
                e->caretDown(parsedCmd.first);
            }
            else if (parsedCmd.second == "k") {
                e->caretUp(parsedCmd.first);
            }
            else if (parsedCmd.second == "l") {
                e->caretRight(parsedCmd.first);
            }
            break;
        case 2:
            parsedCmd = command->parseNormal();
            if (parsedCmd.second == "o") {
                e->insertLineBelow(parsedCmd.first);
            }
            else if (parsedCmd.second == "O") {
                e->insertLineAbove(parsedCmd.first);
            }
            break;
        case 3:
            if (command->cmd == "_") {
                e->VCHome();
            }
            else if (command->cmd == "$") {
                e->LineEnd();
                e->caretLeft(1);
            }
            break;
    }
    command->clear();
    commandBar->Clear();
}

void Window::executeCommand(const int& cmdInd) {
    std::vector<std::string> parsedCmd;
    Editor *e = getCurrentEditor();
    switch(cmdInd) {
        case 0:
            panel->deleteCurr();
            break;
        case 1:
            parsedCmd = command->parseCommand();
            if (parsedCmd.size() == 1) {
                if (e->relPath == "") {
                    // later this should display an error: editor has no file
                    break;
                }
            }
            else {
                if (isValidPath(parsedCmd[1])) {
                    e->relPath = parsedCmd[1];
                    panel->SetPageText(currEditor, parsedCmd[1]);
                }
                else {
                    // also display error
                    break;
                }
            }
            e->SaveFile(cwd + e->relPath);
            if (parsedCmd[0] == "wq") {
                panel->deleteCurr();
            }
            break;
        case 2:
            // later: check if curr editor saved before replacing
            parsedCmd = command->parseCommand();
            if (isExistingPath(parsedCmd[1])) {
                e->relPath = parsedCmd[1];
                panel->SetPageText(currEditor, parsedCmd[1]);
                e->LoadFile(cwd + e->relPath);
            }
            break;
        case 3:
            parsedCmd = command->parseCommand();
            break;
    }
    command->clear();
    commandBar->Clear();
}

bool Window::isExistingPath(const std::string& relPath) const {
    if (relPath.empty()) {
        return false;
    }
    std::string absPath = cwd + relPath;
    if (std::filesystem::exists(absPath) && std::filesystem::is_regular_file(absPath)) {
        return true;
    }
    return false;
}

bool Window::isValidPath(const std::string& relPath) const {
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
