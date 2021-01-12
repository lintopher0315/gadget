#include "Frame.h"

Frame::Frame(const wxSize& size) : wxFrame(NULL, wxID_ANY, "Gadget", wxDefaultPosition, size) {
	SetBackgroundColour(wxColour(54, 54, 54));
	sizer = new wxBoxSizer(wxHORIZONTAL);

    cwd = std::filesystem::current_path().string() + "/";

    window = new Window(this);
	tree = new FileTree(this);

	sizer->Add(tree, 1, wxEXPAND | wxALL, 0);
	sizer->Add(window, 3, wxEXPAND | wxALL, 0);

	SetSizerAndFit(sizer);
	SetSize(size);
}
