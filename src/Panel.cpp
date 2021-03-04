#include "Frame.h"
#include "Window.h"
#include "Panel.h"

Panel::Panel(wxWindow *parent) : wxAuiNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE) {
	SetBackgroundColour(wxColour(54, 54, 54));
	tabArt = new wxAuiSimpleTabArt();
	tabArt->SetColour(wxColour(77, 77, 77));
	tabArt->SetActiveColour(wxColour(227, 73, 73));

	SetArtProvider(tabArt);

    AddPage(new Editor(this), "[NO FILE]");
    AddPage(new Editor(this), "[NO FILE]");
    AddPage(new Editor(this), "[NO FILE]");
    // indicate it's a new file

    Bind(wxEVT_AUINOTEBOOK_PAGE_CHANGED, &Panel::onTabChange, this);
    Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSED, &Panel::onTabRemove, this);
}

Window *Panel::getWindow(void) {
    return (Window *)GetParent();
}

void Panel::onTabChange(wxAuiNotebookEvent& event) {
    Window *w = getWindow();
    w->currEditor = event.GetSelection();

    w->command->clear();
    w->commandBar->Clear();

	// call window update status bar
	w->updateStatus();
}

void Panel::onTabRemove(wxAuiNotebookEvent& event) {
    Window *w = getWindow();

    w->command->clear();
    w->commandBar->Clear();

    if (GetPageCount() == 0) {
        w->getFrame()->Destroy();
    }
}

void Panel::deleteCurr(void) {
    DeletePage(GetSelection());
    if (GetPageCount() == 0) {
        getWindow()->getFrame()->Destroy();
    }
}

void Panel::setTab(const int& ind) {
	if (ind <= 0 || ind > GetPageCount()) {
		return;
	}
	SetSelection(ind - 1);
}
