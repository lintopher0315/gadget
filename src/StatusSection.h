#pragma once

#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>

class StatusSection : public wxRichTextCtrl {
	public:
		StatusSection(wxWindow *parent, wxSize minSize, wxSize maxSize, wxColour background, wxColour foreground);

		void setText(const std::string& text);

	private:
		void onClick(wxMouseEvent& event);
};
