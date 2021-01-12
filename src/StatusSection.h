#pragma once

#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>

class StatusSection : public wxRichTextCtrl {
	public:
		StatusSection(wxWindow *parent, wxSize minSize, wxSize maxSize, wxColour background, wxColour foreground);

		void setText(const std::string& text);
		void setForeground(const wxColour& colour);
		void setBackground(const wxColour& colour);
		wxColour getForeground(void) const;
		wxColour getBackground(void) const;

	private:
		void onClick(wxMouseEvent& event);
};
