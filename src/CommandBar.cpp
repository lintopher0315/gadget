#include "CommandBar.h"

CommandBar::CommandBar(wxWindow *parent) : wxRichTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(wxGetDisplaySize().GetWidth()/2, 30), wxRE_MULTILINE, wxDefaultValidator) {
    SetEditable(false);
    EnableVerticalScrollbar(false);
    wxFont *font = new wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString);
    SetFont(*font);

	SetBackgroundColour(wxColour(37, 37, 38));
	wxRichTextAttr attr = GetBasicStyle();
	attr.SetTextColour(wxColour(224, 227, 23));
	SetBasicStyle(attr);

	Bind(wxEVT_LEFT_DOWN, &CommandBar::onClick, this);
	Bind(wxEVT_RIGHT_DOWN, &CommandBar::onClick, this);
}

void CommandBar::onClick(wxMouseEvent& event) {
	event.Skip(false);
}
