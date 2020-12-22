#pragma once

#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>

class StatusBar : public wxRichTextCtrl {
    public:
        StatusBar(wxWindow *parent);
};
