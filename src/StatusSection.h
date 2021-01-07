#pragma once

#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>

class StatusSection : public wxRichTextCtrl {
	public:
		StatusSection(wxWindow *parent);

		void setCenteredText(const std::string& text);
};
