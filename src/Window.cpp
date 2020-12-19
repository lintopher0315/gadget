#include "Window.h"

Window::Window(wxWindow *parent) : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize) {
    sizer = new wxBoxSizer(wxVERTICAL);

    editor = new Editor(this);

    statusBar = new StatusBar(this);

    sizer->Add(editor, 1, wxEXPAND | wxALL, 0);
    sizer->Add(statusBar, 0, wxEXPAND | wxALL, 0);

    SetSizer(sizer);

    command = new Command();

    mode = NORMAL_MODE;
}
