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
    Window *w = getWindow();
    if (c != WXK_NONE) {
        if (c >= 32) {
            if (w->mode == NORMAL_MODE) {
                w->statusBar->AppendText(c);
                if (c == ':' && w->command->cmd.empty()) {
                    w->command->prefix = COMMAND_PREFIX;
                    return;
                }
                w->command->cmd += c;
                // TODO:
                // if NORMAL_PREFIX
                // check if command is valid
                // execute and clear
                return;
            }
        }
    }
    event.Skip();
}

void Editor::onKey(wxKeyEvent& event) {
    Window *w = getWindow();
    switch (event.GetKeyCode()) {
        case WXK_ESCAPE:
            w->mode = NORMAL_MODE;
            w->command->clear();
            w->statusBar->Clear();
            break;
        case WXK_RETURN:
            // TODO:
            // check mode
            // if normal, check if command is valid
            // execute and clear
            return;
        case WXK_BACK:
            if (w->mode == NORMAL_MODE) {
                if (!w->command->cmd.empty()) {
                    w->command->cmd.pop_back();
                    if (w->command->cmd.empty()) {
                        w->command->prefix = NORMAL_PREFIX;
                    }
                }
                int l = w->statusBar->GetLineLength(0);
                w->statusBar->Remove(l-1, l);
            }
            return;
    }
    event.Skip();
}
