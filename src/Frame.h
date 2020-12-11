#pragma once

#include <wx/wx.h>
#include <wx/stc/stc.h>

class Frame : public wxFrame {
    public:
        Frame(const wxSize& size);
        wxStyledTextCtrl *editor;
};
