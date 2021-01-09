#pragma once

#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>

class StatusSection : public wxRichTextCtrl {
	public:
		StatusSection(wxWindow *parent);

		void onClick(wxMouseEvent& event);

		void setCenteredText(const std::string& text);
};
