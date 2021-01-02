#pragma once

#include <wx/wx.h>
#include <wx/aui/auibook.h>

class Panel;

#include "Editor.h"

class Panel : public wxAuiNotebook {
    public:
        Panel(wxWindow *parent);

        Window *getWindow(void);
        void onTabChange(wxAuiNotebookEvent& event);
        void onTabRemove(wxAuiNotebookEvent& event);
        void deleteCurr(void);
		void setTab(const int& ind);
};
