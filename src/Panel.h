#pragma once

#include <wx/wx.h>
#include <wx/aui/auibook.h>

class Panel;

#include "Editor.h"

class Panel : public wxAuiNotebook {
    public:
        Panel(wxWindow *parent);

        Window *getWindow();
        void onTabChange(wxAuiNotebookEvent& event);
        void onTabRemove(wxAuiNotebookEvent& event);
};
