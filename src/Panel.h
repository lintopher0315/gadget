#pragma once

#include <wx/wx.h>
#include <wx/aui/auibook.h>

class Frame;
class Window;

#include "Editor.h"

class Panel : public wxAuiNotebook {
    public:
        Panel(wxWindow *parent);

        Window *getWindow(void);

        void deleteCurr(void);
		void setTab(const int& ind);

	private:
        void onTabChange(wxAuiNotebookEvent& event);
        void onTabRemove(wxAuiNotebookEvent& event);

		wxAuiSimpleTabArt *tabArt;
};
