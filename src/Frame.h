#pragma once

#include <wx/wx.h>
#include <wx/stc/stc.h>

#include "Editor.h"

class Frame : public wxFrame {
    public:
        Frame(const wxSize& size);
        Editor *editor;
};
