#include "Editor.h"

Editor::Editor(wxWindow *parent) : wxStyledTextCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, "editor") {
    SetCaretStyle(2);
    SetCaretPeriod(0);
    SetTabWidth(4);
    SetUseHorizontalScrollBar(false);
    SetWrapMode(1);
    SetMarginType(1, wxSTC_MARGIN_NUMBER);
	SetMarginWidth(1, 30);
	SetUseTabs(false);

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
	Bind(wxEVT_LEFT_UP, &Editor::onClickUp, this);
	Bind(wxEVT_LEFT_DOWN, &Editor::onClickDown, this);
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
				if ((ind = w->command->isValidNormal()) >= 0) {
					w->executeNormal(ind);
				}
            }
			else if (w->mode == EDIT_MODE) {
				AddText(c);
				w->updateStatus();
			}
			else if (w->mode == VISUAL_MODE) {
				w->commandBar->AppendText(c);
				w->command->cmd += c;

				int ind = -1;
				if ((ind = w->command->isValidVisual()) >= 0) {
					w->executeVisual(ind);
				}
			}
			else if (w->mode == LINE_MODE) {
				w->commandBar->AppendText(c);
				w->command->cmd += c;

				int ind = -1;
				if ((ind = w->command->isValidLine()) >= 0) {
					w->executeLine(ind);
				}
			}
        }
    }
}

void Editor::onKey(wxKeyEvent& event) {
    Window *w = getWindow();
	int key = event.GetKeyCode();
	if (key >= 32 && key <= 127 && !event.ControlDown()) {
		event.Skip();
	}
	else if (key == WXK_ESCAPE) {
		if (w->mode == EDIT_MODE) {
			caretLeft(1);
		}
		else if (w->mode == VISUAL_MODE || w->mode == LINE_MODE) {
			removeSelection();
		}
		w->mode = NORMAL_MODE;
		w->command->clear();
		w->commandBar->Clear();
	}
	else if (key == WXK_RETURN) {
		if (w->mode == NORMAL_MODE) {
			int ind = -1;
			if ((ind = w->command->isValidCommand()) >= 0) {
				w->executeCommand(ind);
			}
		}
		else if (w->mode == EDIT_MODE) {
			std::string indentSpace = getIndentSpace();
			NewLine();
			AddText(indentSpace);
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
		else if (w->mode == EDIT_MODE) {
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

void Editor::onClickUp(wxMouseEvent& event) {
	Window *w = getWindow();

	if (GetSelectionStart() != GetSelectionEnd()) {
		w->mode = VISUAL_MODE;
	}
	else {
		if (w->mode == VISUAL_MODE || w->mode == LINE_MODE) {
			w->mode = NORMAL_MODE;
		}
	}
	getWindow()->updateStatus();
	event.Skip();
}

void Editor::onClickDown(wxMouseEvent& event) {
	Window *w = getWindow();

	removeSelection();
	w->command->clear();
	w->commandBar->Clear();
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

std::string Editor::getIndentSpace(void) {
	std::string line = std::string(GetCurLine().mb_str());
	int ind = 0;
	while (ind < line.size() && line[ind] == ' ') {
		++ind;
	}
	return line.substr(0, ind);
}

void Editor::caretLeft(const int& num) {
	SetSelectionEnd(std::max(lineStartPos(), GetCurrentPos() - num));
}

void Editor::caretRight(const int& num) {
	if (lineStartPos() < lineEndPos()) {
		SetSelectionStart(std::min(lineEndPos() - 1, GetCurrentPos() + num));
	}
}

void Editor::caretUp(const int& num) {
	int pos = linePos();
	GotoLine(std::max(0, currLine() - num));
	caretRight(pos);
}

void Editor::caretDown(const int& num) {
	int pos = linePos();
	GotoLine(std::min(GetLineCount() - 1, currLine() + num));
	caretRight(pos);
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
	std::string indentSpace = getIndentSpace();
    for (int i = 0; i < num; ++i) {
        NewLine();
		AddText(indentSpace);
    }
    Window *w = getWindow();
    w->mode = EDIT_MODE;    
}

void Editor::insertLineAbove(const int& num) {
	std::string indentSpace = getIndentSpace();
    for (int i = 0; i < num; ++i) {
		Home();
        NewLine();
    	caretUp(1);
		AddText(indentSpace);
    }

    Window *w = getWindow();
    w->mode = EDIT_MODE;    
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

void Editor::removeSelection(void) {
	SetEmptySelection(GetCurrentPos());
}

void Editor::cutSelection(void) {
	if (GetAnchor() <= GetCurrentPos()) {
		CharRightExtend();
	}
	Cut();
}

void Editor::caseChangeSelection(const bool& upper) {
	if (GetAnchor() <= GetCurrentPos()) {
		CharRightExtend();
	}
	if (upper) {
		UpperCase();
	}
	else {
		LowerCase();
	}
	if (GetAnchor() <= GetCurrentPos()) {
		CharLeftExtend();
	}
}

void Editor::caretLeftVis(const int& num) {
	int pos = std::max(lineStartPos(), GetCurrentPos() - num);
	if (GetCurrentPos() == GetAnchor()) {
		SetAnchor(GetAnchor() + 1);
	}
	SetCurrentPos(pos);
}

void Editor::caretRightVis(const int& num) {
	int pos = std::min(lineEndPos() - 1, GetCurrentPos() + num);
	if (pos == GetAnchor() - 1) {
		SetAnchor(GetAnchor() - 1);
	}
	SetCurrentPos(pos);
}

void Editor::caretUpVis(const int& num) {
	int pos = linePos();
	int endLine = std::max(0, currLine() - num);
	int endPos = std::min(GetLineEndPosition(endLine) - 1, PositionFromLine(endLine) + pos);
	if (PositionFromLine(endLine) == GetLineEndPosition(endLine)) {
		++endPos;
	}
	if (endPos < GetAnchor() && GetAnchor() <= GetCurrentPos()) {
		SetAnchor(GetAnchor() + 1);
	}
	SetCurrentPos(endPos);
}

void Editor::caretDownVis(const int& num) {
	int pos = linePos();
	int endLine = std::min(GetLineCount() - 1, currLine() + num);
	int endPos = std::min(GetLineEndPosition(endLine) - 1, PositionFromLine(endLine) + pos);
	if (PositionFromLine(endLine) == GetLineEndPosition(endLine)) {
		++endPos;
	}
	if (endPos >= GetAnchor() - 1 && GetAnchor() > GetCurrentPos()) {
		SetAnchor(GetAnchor() - 1);
	}
	SetCurrentPos(endPos);
}

void Editor::caretUpLine(const int& num) {
	int endLine = std::max(currLine() - num, 0);
	int startLine = LineFromPosition(GetAnchor());
	if (endLine <= startLine) {
		SetAnchor(GetLineEndPosition(startLine));
		SetCurrentPos(PositionFromLine(endLine));
	}
	else {
		SetCurrentPos(GetLineEndPosition(endLine));
	}
}

void Editor::caretDownLine(const int& num) {
	int endLine = std::min(currLine() + num, GetLineCount());
	int startLine = LineFromPosition(GetAnchor());
	if (endLine >= startLine) {
		SetAnchor(PositionFromLine(startLine));
		SetCurrentPos(GetLineEndPosition(endLine));
	}
	else {
		SetCurrentPos(PositionFromLine(endLine));
	}
}
