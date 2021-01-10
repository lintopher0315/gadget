#pragma once

#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>

class StatusSection : public wxRichTextCtrl {
	public:
		StatusSection(wxWindow *parent, wxSize minSize, wxSize maxSize);

		void setText(const std::string& text);

	private:
		void onClick(wxMouseEvent& event);
};
