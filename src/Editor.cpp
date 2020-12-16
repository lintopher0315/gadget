#include "Editor.h"

Editor::Editor(wxWindow *parent) : wxStyledTextCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, "editor") {
    SetCaretStyle(2);
    SetCaretPeriod(0);
    SetTabWidth(10);
    SetUseHorizontalScrollBar(false);
    SetWrapMode(1);
    SetMarginType(1, wxSTC_MARGIN_NUMBER);
    wxFont *font = new wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString);
    StyleSetFont(0, *font);

    Bind(wxEVT_CHAR, &Editor::onChar, this);
    Bind(wxEVT_KEY_DOWN, &Editor::onKey, this);
}

Window *Editor::getWindow() {
    return (Window *)GetParent();
}

void Editor::onChar(wxKeyEvent& event) {
    wxChar c = event.GetUnicodeKey();
    if (c != WXK_NONE) {
        if (c >= 32) {
            if (getWindow()->mode == NORMAL_MODE) {
                getWindow()->statusBar->AppendText(c);
                return;
            }
            // also add to parent command object
        }
    }
    event.Skip();
}

void Editor::onKey(wxKeyEvent& event) {
    switch (event.GetKeyCode()) {
        case WXK_ESCAPE:
            getWindow()->mode = NORMAL_MODE;
            break;
    }
    event.Skip();
}
