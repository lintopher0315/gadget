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

    cwd = std::filesystem::current_path().string();
}

Editor *Window::getCurrentEditor() {
    return (Editor *)panel->GetPage(currEditor);
}

void Window::executeCommand(int cmdInd) {
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
            parsedCmd = command->parse();
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
            parsedCmd = command->parse();
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
