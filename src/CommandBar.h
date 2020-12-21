#pragma once

#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>

class CommandBar : public wxRichTextCtrl {
    public:
        CommandBar(wxWindow *parent);
};
