#pragma once

#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>

class StatusSection : public wxRichTextCtrl {
	public:
		StatusSection(wxWindow *parent, wxSize minSize, wxSize maxSize);

		void onClick(wxMouseEvent& event);

		void setText(const std::string& text);
};
