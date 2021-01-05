#include "Frame.h"

Frame::Frame(const wxSize& size) : wxFrame(NULL, wxID_ANY, "Gadget", wxDefaultPosition, size) {
	sizer = new wxBoxSizer(wxHORIZONTAL);

    window = new Window(this);
	tree = new FileTree(this);

	sizer->Add(tree, 1, wxEXPAND | wxALL, 10);
	sizer->Add(window, 3, wxEXPAND | wxALL, 0);

	SetSizer(sizer);
}
