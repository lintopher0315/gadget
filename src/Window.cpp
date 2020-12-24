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
        for (int i = 0; i < parsedCmd.first; ++i) {
            if (parsedCmd.second == "h") {
                editor->CharLeft();
            }
            else if (parsedCmd.second == "j") {
                editor->LineDown();
            }
            else if (parsedCmd.second == "k") {
                editor->LineUp();
            }
            else if (parsedCmd.second == "l") {
                editor->CharRight();
            }
        }
    }
    command->clear();
    commandBar->Clear();
}

// TODO:
// create function that accepts (int, *func) and does that func that many times
// useful for any command that can be prepended by a number
