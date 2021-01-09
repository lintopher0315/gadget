#include "StatusSection.h"

StatusSection::StatusSection(wxWindow *parent, wxSize minSize, wxSize maxSize) : wxRichTextCtrl(parent) {
    SetBackgroundColour(wxColour(0xB5, 0xB5, 0xB5));
	SetEditable(false);
	EnableVerticalScrollbar(false);

	SetMinClientSize(minSize);
	SetMaxClientSize(maxSize);

	Bind(wxEVT_LEFT_DOWN, &StatusSection::onClick, this);
	Bind(wxEVT_RIGHT_DOWN, &StatusSection::onClick, this);
}

void StatusSection::onClick(wxMouseEvent& event) {
	event.Skip(false);
}

void StatusSection::setText(const std::string& text) {
	Clear();
	AppendText(text);
}
