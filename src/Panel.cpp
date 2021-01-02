#include "Panel.h"

Panel::Panel(wxWindow *parent) : wxAuiNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE) {
    AddPage(new Editor(this), "[NO FILE]");
    AddPage(new Editor(this), "[NO FILE]");
    AddPage(new Editor(this), "[NO FILE]");
    // indicate it's a new file

    Bind(wxEVT_AUINOTEBOOK_PAGE_CHANGED, &Panel::onTabChange, this);
    Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSED, &Panel::onTabRemove, this);
}

Window *Panel::getWindow() {
    return (Window *)GetParent();
}

void Panel::onTabChange(wxAuiNotebookEvent& event) {
    Window *w = getWindow();
    w->currEditor = event.GetSelection();

    w->command->clear();
    w->commandBar->Clear();
}

void Panel::onTabRemove(wxAuiNotebookEvent& event) {
    Window *w = getWindow();

    w->command->clear();
    w->commandBar->Clear();

    if (GetPageCount() == 0) {
        w->getFrame()->Destroy();
    }
}

void Panel::deleteCurr() {
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
