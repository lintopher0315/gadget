#include "StatusBar.h"

StatusBar::StatusBar(wxWindow *parent) : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(wxGetDisplaySize().GetWidth()/2, 30)) {
    wxFont *font = new wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString);
    SetFont(*font);

	sizer = new wxBoxSizer(wxHORIZONTAL);

	modeDisplay = new StatusSection(this);
	modeDisplay->setCenteredText("~ NORMAL ~");
	emptySpace = new StatusSection(this);
	fileTypeDisplay = new StatusSection(this);
	fileEncodingDisplay = new StatusSection(this);
	positionDisplay = new StatusSection(this);

	sizer->Add(modeDisplay, 1, wxEXPAND | wxALL, 0);
	sizer->Add(emptySpace, 8, wxEXPAND | wxALL, 0);
	sizer->Add(fileTypeDisplay, 1, wxEXPAND | wxALL, 0);
	sizer->Add(fileEncodingDisplay, 1, wxEXPAND | wxALL, 0);
	sizer->Add(positionDisplay, 1, wxEXPAND | wxALL, 0);

	SetSizer(sizer);
}
