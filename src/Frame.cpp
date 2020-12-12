#include "Frame.h"

Frame::Frame(const wxSize& size) : wxFrame(NULL, wxID_ANY, "Gadget", wxDefaultPosition, size) {
    window = new Window(this);
}
