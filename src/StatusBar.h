#pragma once

#include <wx/wx.h>

#include "StatusSection.h"

class StatusBar : public wxWindow {
    public:
        StatusBar(wxWindow *parent);

		wxBoxSizer *sizer;
		StatusSection *modeDisplay;
		StatusSection *fileTypeDisplay;
		StatusSection *emptySpace;
		StatusSection *fileEncodingDisplay;
		StatusSection *positionDisplay;
};
