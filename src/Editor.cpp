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
                w->commandBar->AppendText(c);
                if (c == ':' && w->command->cmd.empty()) {
                    w->command->prefix = COMMAND_PREFIX;
                    return;
                }
                w->command->cmd += c;
                // TODO:
                // if NORMAL_PREFIX
                // check if command is valid
                // execute and clear
                int ind=-1;
                if (w->command->prefix == NORMAL_PREFIX && (ind = w->command->isValid()) >= 0) {
                    w->executeCommand(ind);
                }
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
            if (w->mode == EDIT_MODE) {
                caretLeft(1);
            }
            w->mode = NORMAL_MODE;
            w->command->clear();
            w->commandBar->Clear();

            // change status bar; prob update this later
            w->statusBar->Clear();
            w->statusBar->AppendText("~ NORMAL ~");
            return;
        case WXK_RETURN:
            if (w->mode == NORMAL_MODE) {
                return;
            }
            break;
            // TODO:
            // check mode
            // if normal, check if command is valid
            // execute and clear
        case WXK_BACK:
            if (w->mode == NORMAL_MODE) {
                if (!w->command->cmd.empty()) {
                    w->command->cmd.pop_back();
                    if (w->command->cmd.empty()) {
                        w->command->prefix = NORMAL_PREFIX;
                    }
                }
                int l = w->commandBar->GetLineLength(0);
                w->commandBar->Remove(l-1, l);
                return;
            }
            break;
    }
    event.Skip();
}

int Editor::linePos() {
    return GetCurrentPos() - lineStartPos();
}

int Editor::lineStartPos() {
    return PositionFromLine(LineFromPosition(GetCurrentPos()));
}

int Editor::lineEndPos() {
    return GetLineEndPosition(LineFromPosition(GetCurrentPos()));
}

void Editor::caretLeft(int num) {
    for (int i = 0; i < num; ++i) {
        if (GetCurrentPos() == lineStartPos()) {
            return;
        }
        CharLeft();
    }
}

void Editor::caretRight(int num) {
    for (int i = 0; i < num; ++i) {
        if (GetCurrentPos() == lineEndPos() - 1) {
            return;
        }
        CharRight();
    }
}

void Editor::caretUp(int num) {
    for (int i = 0; i < num; ++i) {
        if (GetCurrentLine() == 0) {
            return;
        }
        LineUp();
        if (GetCurrentPos() == lineEndPos()) {
            caretLeft(1);
        }
    }
}

void Editor::caretDown(int num) {
    for (int i = 0; i < num; ++i) {
        if (GetCurrentLine() == GetLineCount() - 1) {
            return;
        }
        LineDown();
        if (GetCurrentPos() == lineEndPos()) {
            caretLeft(1);
        }
    }
}

void Editor::append() {
    if (lineEndPos() - 1 == GetCurrentPos()) {
        LineEnd();
    }
    else {
        caretRight(1);
    }
}
