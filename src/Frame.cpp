#include "Frame.h"

Frame::Frame(const wxSize& size) : wxFrame(NULL, wxID_ANY, "Gadget", wxDefaultPosition, size) {
    editor = new wxTextCtrl(this, wxID_ANY, "Hello", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH);
}
