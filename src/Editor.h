#pragma once

#include <wx/wx.h>
#include <wx/stc/stc.h>

class Editor : public wxStyledTextCtrl {
    public:
        Editor(wxWindow *parent);
};
