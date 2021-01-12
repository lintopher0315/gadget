#include "StatusSection.h"

StatusSection::StatusSection(wxWindow *parent, wxSize minSize, wxSize maxSize, wxColour background, wxColour foreground) : wxRichTextCtrl(parent) {
	SetEditable(false);
	EnableVerticalScrollbar(false);

	SetMinClientSize(minSize);
	SetMaxClientSize(maxSize);

	wxRichTextAttr attr = GetBasicStyle();
	attr.SetTextColour(foreground);
	SetBasicStyle(attr);
	SetBackgroundColour(wxColour(background));

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
