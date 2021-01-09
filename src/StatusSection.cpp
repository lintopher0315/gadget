#include "StatusSection.h"

StatusSection::StatusSection(wxWindow *parent) : wxRichTextCtrl(parent) {
    SetBackgroundColour(wxColour(0xB5, 0xB5, 0xB5));
	SetEditable(false);
	EnableVerticalScrollbar(false);

	Bind(wxEVT_LEFT_DOWN, &StatusSection::onClick, this);
	Bind(wxEVT_RIGHT_DOWN, &StatusSection::onClick, this);
}

void StatusSection::onClick(wxMouseEvent& event) {
	event.Skip(false);
}

void StatusSection::setCenteredText(const std::string& text) {
	Clear();
	AppendText(text);
}
