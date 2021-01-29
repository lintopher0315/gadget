#include "Editor.h"

Editor::Editor(wxWindow *parent) : wxStyledTextCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, "editor") {
    SetCaretStyle(2);
    SetCaretPeriod(0);
    SetTabWidth(10);
    SetUseHorizontalScrollBar(false);
    SetWrapMode(1);
    SetMarginType(1, wxSTC_MARGIN_NUMBER);
	SetMarginWidth(1, 30);

    wxFont *font = new wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString);
	StyleSetBackground(wxSTC_STYLE_DEFAULT, wxColour(30, 30, 30));
	StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColour(255, 255, 255));
	SetCaretForeground(wxColour(255, 255, 255));

	StyleClearAll();
    StyleSetFont(0, *font);
	StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(37, 37, 38));
	StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(240, 240, 41));

    Bind(wxEVT_CHAR, &Editor::onChar, this);
    Bind(wxEVT_KEY_DOWN, &Editor::onKey, this);
	Bind(wxEVT_LEFT_UP, &Editor::onClick, this);
	Bind(wxEVT_STC_MODIFIED, &Editor::onModified, this);

	saved = true;
}

Window *Editor::getWindow(void) {
    return (Window *)GetGrandParent();
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

                int ind = -1;
                if (w->command->prefix == NORMAL_PREFIX && (ind = w->command->isValid()) >= 0) {
                    w->executeNormal(ind);
                }
                return;
            }
			AddText(c);
			w->updateStatus();
        }
    }
}

void Editor::onKey(wxKeyEvent& event) {
    Window *w = getWindow();
	int key = event.GetKeyCode();
	if (key >= 32 && key <= 127) {
		event.Skip();
	}
	else if (key == WXK_ESCAPE) {
		if (w->mode == EDIT_MODE) {
			caretLeft(1);
		}
		w->mode = NORMAL_MODE;
		w->command->clear();
		w->commandBar->Clear();

		w->statusBar->modeDisplay->setText(" ~NORMAL~");
		w->statusBar->modeDisplay->setBackground(wxColour(219, 131, 0));
	}
	else if (key == WXK_RETURN) {
		if (w->mode == NORMAL_MODE) {
			int ind = -1;
			if (w->command->prefix == COMMAND_PREFIX && (ind = w->command->isValid()) >= 0) {
				w->executeCommand(ind); 
			}
		}
		else {
			NewLine();
		}
	}
	else if (key == WXK_BACK) {
		if (w->mode == NORMAL_MODE) {
			if (!w->command->cmd.empty()) {
				w->command->cmd.pop_back();
			}
			else {
				w->command->prefix = NORMAL_PREFIX;
			}
			int l = w->commandBar->GetLineLength(0);
			w->commandBar->Remove(l-1, l);
		}
		else {
			DeleteBack();
		}
	}
	else if (key == WXK_TAB) {
		if (w->mode == EDIT_MODE) {
			Tab();
		}
	}
	else if (key == WXK_LEFT) {
		if (w->command->isClear()) {
			caretLeft(1);
		}
	}
	else if (key == WXK_RIGHT) {
		if (w->command->isClear()) {
			caretRight(1);
		}
	}
	else if (key == WXK_UP) {
		if (w->command->isClear()) {
			caretUp(1);
		}
	}
	else if (key == WXK_DOWN) {
		if (w->command->isClear()) {
			caretDown(1);
		}
	}
	else if (key == WXK_END) {
		if (w->command->isClear()) {
			LineEnd();
			caretLeft(1);
		}
	}
	else if (key == WXK_HOME) {
		if (w->command->isClear()) {
			VCHome();
		}
	}
	else if (key == WXK_PAGEUP) {
		if (w->command->isClear()) {
			PageUp();
		}
	}
	else if (key == WXK_PAGEDOWN) {
		if (w->command->isClear()) {
			PageDown();
		}
	}
	w->updateStatus();
}

void Editor::onClick(wxMouseEvent& event) {
	getWindow()->updateStatus();
	event.Skip();
}

void Editor::onModified(wxStyledTextEvent& event) {
	saved = false;
	event.Skip();
}

int Editor::linePos(void) const {
    return GetCurrentPos() - lineStartPos();
}

int Editor::currLine(void) const {
	return LineFromPosition(GetCurrentPos());
}

int Editor::lineStartPos(void) const {
    return PositionFromLine(currLine());
}

int Editor::lineEndPos(void) const {
    return GetLineEndPosition(currLine());
}

void Editor::caretLeft(const int& num) {
    for (int i = 0; i < num; ++i) {
        if (GetCurrentPos() == lineStartPos()) {
            return;
        }
        CharLeft();
    }
}

void Editor::caretRight(const int& num) {
    for (int i = 0; i < num; ++i) {
        if (GetCurrentPos() == lineEndPos() - 1) {
            return;
        }
        CharRight();
    }
}

void Editor::caretUp(const int& num) {
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

void Editor::caretDown(const int& num) {
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

void Editor::append(void) {
    if (lineEndPos() - 1 == GetCurrentPos()) {
        LineEnd();
    }
    else {
        caretRight(1);
    }
}

void Editor::insertLineBelow(const int& num) {
    LineEnd();
    for (int i = 0; i < num; ++i) {
        NewLine();
    }
    Window *w = getWindow();
    w->mode = EDIT_MODE;    

	w->statusBar->modeDisplay->setText("  ~EDIT~");
	w->statusBar->modeDisplay->setBackground(wxColour(118, 158, 108));
}

void Editor::insertLineAbove(const int& num) {
    Home();
    for (int i = 0; i < num; ++i) {
        NewLine();
    }
    caretUp(1);
    VCHome();

    Window *w = getWindow();
    w->mode = EDIT_MODE;    

	w->statusBar->modeDisplay->setText("  ~EDIT~");
	w->statusBar->modeDisplay->setBackground(wxColour(118, 158, 108));
}

void Editor::wordLeft(const int& num) {
	for (int i = 0; i < num; ++i) {
		WordLeft();
	}
}

void Editor::wordRight(const int& num) {
	for (int i = 0; i < num; ++i) {
		WordRight();
	}
}

void Editor::charSearchAhead(const char& c, const bool& inc) {	
	for (int i = GetCurrentPos() + 1; i < lineEndPos(); ++i) {
		if (GetCharAt(i) == c) {
			if (inc) {
				GotoPos(i);
			}
			else {
				GotoPos(i-1);
			}
			return;
		}
	}
}

void Editor::charSearchBehind(const char& c, const bool& inc) {
	for (int i = GetCurrentPos() - 1; i >= lineStartPos(); --i) {
		if (GetCharAt(i) == c) {
			if (inc) {
				GotoPos(i);
			}
			else {
				GotoPos(i+1);
			}
			return;
		}
	}
}
