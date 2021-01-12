#include "StatusSection.h"

StatusSection::StatusSection(wxWindow *parent, wxSize minSize, wxSize maxSize, wxColour background, wxColour foreground) : wxRichTextCtrl(parent) {
	SetEditable(false);
	EnableVerticalScrollbar(false);

	SetMinClientSize(minSize);
	SetMaxClientSize(maxSize);

	setForeground(foreground);
	setBackground(background);

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

void StatusSection::setForeground(const wxColour& colour) {
	wxRichTextAttr attr = GetBasicStyle();
	attr.SetTextColour(colour);
	SetBasicStyle(attr);
}

void StatusSection::setBackground(const wxColour& colour) {
	SetBackgroundColour(colour);
}

wxColour StatusSection::getForeground(void) const {
	return GetBasicStyle().GetTextColour();
}

wxColour StatusSection::getBackground(void) const {
	return GetBackgroundColour();
}
