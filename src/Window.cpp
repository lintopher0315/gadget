#include "Window.h"

Window::Window(wxWindow *parent) : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize) {
    sizer = new wxBoxSizer(wxVERTICAL);

    editor = new Editor(this);
    statusBar = new StatusBar(this);
    commandBar = new CommandBar(this);

    sizer->Add(editor, 1, wxEXPAND | wxALL, 0);
    sizer->Add(statusBar, 0, wxEXPAND | wxALL, -3);
    sizer->Add(commandBar, 0, wxEXPAND | wxALL, 0);

    SetSizer(sizer);

    command = new Command();

    mode = NORMAL_MODE;

    cwd = std::filesystem::current_path().string();
}

void Window::executeCommand(int cmdInd) {
    std::pair<int, std::string> parsedCmd;
    switch(cmdInd) {
        case 0:
            mode = EDIT_MODE;

            if (command->cmd == "a") {
                editor->append();
            }
            else if (command->cmd == "A") {
                editor->LineEnd();
            }
            else if (command->cmd == "I") {
                editor->VCHome();
            }

            // change status bar; prob update this later
            statusBar->Clear();
            statusBar->AppendText("~ EDIT ~");
            break;
        case 1:
            parsedCmd = command->parse();
            if (parsedCmd.second == "h") {
                editor->caretLeft(parsedCmd.first);
            }
            else if (parsedCmd.second == "j") {
                editor->caretDown(parsedCmd.first);
            }
            else if (parsedCmd.second == "k") {
                editor->caretUp(parsedCmd.first);
            }
            else if (parsedCmd.second == "l") {
                editor->caretRight(parsedCmd.first);
            }
            break;
        case 2:
            parsedCmd = command->parse();
            if (parsedCmd.second == "o") {
                editor->insertLineBelow(parsedCmd.first);
            }
            else if (parsedCmd.second == "O") {
                editor->insertLineAbove(parsedCmd.first);
            }
            break;
        case 3:
            if (command->cmd == "_") {
                editor->VCHome();
            }
            else if (command->cmd == "$") {
                editor->LineEnd();
                editor->caretLeft(1);
            }
            break;
    }
    command->clear();
    commandBar->Clear();
}
