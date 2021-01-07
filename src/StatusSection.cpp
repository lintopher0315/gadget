#include "StatusSection.h"

StatusSection::StatusSection(wxWindow *parent) : wxRichTextCtrl(parent) {
    SetBackgroundColour(wxColour(0xB5, 0xB5, 0xB5));
	SetEditable(false);
	EnableVerticalScrollbar(false);
}

void StatusSection::setCenteredText(const std::string& text) {
	Clear();
	AppendText(text);
}
