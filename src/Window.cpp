#include "Window.h"

Window::Window(wxWindow *parent) : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize) {
    editor = new Editor(this);
    statusBar = new StatusBar(this);
}
