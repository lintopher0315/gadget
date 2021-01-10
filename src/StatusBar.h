#pragma once

#include <wx/wx.h>

#include "StatusSection.h"

class StatusBar : public wxWindow {
    public:
        StatusBar(wxWindow *parent);

		StatusSection *modeDisplay;
		StatusSection *pathDisplay;
		StatusSection *positionDisplay;
		StatusSection *sizeDisplay;

	private:
		wxBoxSizer *sizer;
};
