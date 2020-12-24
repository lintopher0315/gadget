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
}

void Window::executeCommand(int cmdInd) {
    if (cmdInd == 0) {
        mode = EDIT_MODE;

        // change status bar; prob update this later
        statusBar->Clear();
        statusBar->AppendText("~ EDIT ~");
    }
    else if (cmdInd == 1) {
        std::pair<int, std::string> parsedCmd = command->parse();
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
    }
    command->clear();
    commandBar->Clear();
}
