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
        int dist = 1;
        std::string cmdCpy = command->cmd;
        if (cmdCpy.size() > 1) {
            dist = std::max(dist, stoi(cmdCpy.substr(0, cmdCpy.size()-1)));
        }
        for (int i = 0; i < dist; ++i) {
            switch(cmdCpy[cmdCpy.size()-1]) {
                case 'h':
                    editor->CharLeft();
                    break;
                case 'j':
                    editor->LineDown();
                    break;
                case 'k':
                    editor->LineUp();
                    break;
                case 'l':
                    editor->CharRight();
                    break;
            }
        }
    }
    command->clear();
    commandBar->Clear();
}

// TODO:
// create function that accepts (int, *func) and does that func that many times
// useful for any command that can be prepended by a number
