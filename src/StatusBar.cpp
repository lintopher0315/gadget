#include "StatusBar.h"

StatusBar::StatusBar(wxWindow *parent) : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(wxGetDisplaySize().GetWidth()/2, 30)) {
    wxFont *font = new wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString);
    SetFont(*font);

	sizer = new wxBoxSizer(wxHORIZONTAL);

	modeDisplay = new StatusSection(this);
	modeDisplay->setCenteredText("~ NORMAL ~");

	pathDisplay = new StatusSection(this);
	pathDisplay->setCenteredText("[NO FILE]");

	positionDisplay = new StatusSection(this);
	positionDisplay->setCenteredText("1,1 | 100%");

	sizeDisplay = new StatusSection(this);
	sizeDisplay->setCenteredText("0 bytes | 1 lines");

	sizer->Add(modeDisplay, 2, wxEXPAND | wxALL, 0);
	sizer->Add(pathDisplay, 7, wxEXPAND | wxALL, 0);
	sizer->Add(positionDisplay, 2, wxEXPAND | wxALL, 0);
	sizer->Add(sizeDisplay, 3, wxEXPAND | wxALL, 0);

	SetSizer(sizer);
}
