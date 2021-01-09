#include "StatusBar.h"

StatusBar::StatusBar(wxWindow *parent) : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(wxGetDisplaySize().GetWidth()/2, 30)) {
    wxFont *font = new wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString);
    SetFont(*font);

	sizer = new wxBoxSizer(wxHORIZONTAL);

	modeDisplay = new StatusSection(this, wxSize(90, 30), wxSize(90, 30));
	modeDisplay->setText("~ NORMAL ~");

	pathDisplay = new StatusSection(this, wxSize(90, 30), parent->GetMaxClientSize());
	pathDisplay->setText("[NO FILE]");

	positionDisplay = new StatusSection(this, wxSize(90, 30), wxSize(150, 30));
	positionDisplay->setText("1,1 | 100%");

	sizeDisplay = new StatusSection(this, wxSize(150, 30), wxSize(210, 30));
	sizeDisplay->setText("0 bytes | 1 lines");

	sizer->Add(modeDisplay, 1, wxEXPAND | wxALL, 0);
	sizer->Add(pathDisplay, 2, wxEXPAND | wxALL, 0);
	sizer->Add(positionDisplay, 1, wxEXPAND | wxALL, 0);
	sizer->Add(sizeDisplay, 2, wxEXPAND | wxALL, 0);

	SetSizer(sizer);
}
