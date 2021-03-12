#include "Window.h"
#include "Editor.h"
#include "FileHelper.h"

Editor::Editor(wxWindow *parent, int lexer) : wxStyledTextCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, "editor") {
    SetCaretStyle(2);
    SetCaretPeriod(0);
    SetTabWidth(4);
    SetUseHorizontalScrollBar(false);
    SetWrapMode(1);
    SetMarginType(1, wxSTC_MARGIN_NUMBER);
	SetMarginWidth(1, 30);
	SetUseTabs(false);

	std::string whitespaceChars = std::string(GetWhitespaceChars().mb_str()) + "\n";
	SetWhitespaceChars(whitespaceChars);

	StyleSetBackground(wxSTC_STYLE_DEFAULT, wxColour(30, 30, 30));
	StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColour(255, 255, 255));
	SetCaretForeground(wxColour(219, 131, 0));

	StyleClearAll();

	applyLexer(lexer);

	StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(37, 37, 38));
	StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(240, 240, 41));

    Bind(wxEVT_CHAR, &Editor::onChar, this);
    Bind(wxEVT_KEY_DOWN, &Editor::onKey, this);
	Bind(wxEVT_LEFT_UP, &Editor::onClickUp, this);
	Bind(wxEVT_LEFT_DOWN, &Editor::onClickDown, this);
	Bind(wxEVT_STC_CHANGE, &Editor::onChange, this);

	saved = true;
	readOnly = false;
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
	if (key >= 32 && key <= 127) {
		if (!event.ControlDown() || key == 67 || key == 86) {
			event.Skip();
		}
	}
	else if (key == WXK_ESCAPE) {
		if (w->mode == EDIT_MODE) {
			caretLeft(1);
		}
		else if (w->mode == VISUAL_MODE || w->mode == LINE_MODE) {
			removeSelection();
			if (GetCurrentPos() == lineEndPos()) {
				caretLeft(1);
			}
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
			std::string indentSpace = getIndentSpace(currLine());
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
			std::string upToCaret = std::string(GetTextRange(lineStartPos(), GetCurrentPos()));
			if (upToCaret.empty() || upToCaret.find_first_not_of(' ') != std::string::npos) {
				DeleteBack();
			}
			else {
				DeleteRange(std::max(lineStartPos(), GetCurrentPos() - 4), std::min(linePos(), 4));
			}
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

void Editor::onChange(wxStyledTextEvent& event) {
	saved = false;
	event.Skip();
}

void Editor::applyLexer(const int& lexer) {
	if (lexer == CPP_LEX) {
		SetLexer(wxSTC_LEX_CPP);

		StyleSetForeground(wxSTC_C_COMMENTLINE, wxColor(60, 162, 2));
		StyleSetForeground(wxSTC_C_COMMENT, wxColor(60, 162, 2));
		StyleSetForeground(wxSTC_C_PREPROCESSOR, wxColor(250,128,114));
		StyleSetForeground(wxSTC_C_STRING, wxColor(222, 203, 62));
		StyleSetForeground(wxSTC_C_NUMBER, wxColor(83, 195, 230));
		StyleSetForeground(wxSTC_C_CHARACTER, wxColor(123, 224, 112));
		StyleSetForeground(wxSTC_C_WORD, wxColor(245, 113, 208));
		StyleSetForeground(wxSTC_C_WORD2, wxColor(230, 78, 86));
		SetKeyWords(0, wxT(
					"alignas alignof and and_eq asm atomic_cancel atomic_commit atomic_noexcept auto"
					" bitand bitor bool char char8_t char16_t char32_t compl string"
					" concept consteval constexpr constinit const_cast co_await co_return"
					" co_yield decltype default delete do double dynamic_cast enum explicit export"
					" extern float friend inline int long mutable namespace new noexcept"
					" not not_eq nullptr operator or or_eq reflexpr register"
					" reinterpret_cast requires short signed sizeof static static_assert static_cast"
					" struct synchronized template this thread_local typedef typeid"
					" typename union unsigned using virtual void volatile wchar_t xor xor_eq"
					));
		SetKeyWords(1, wxT(
					"const class private protected public false true if for goto break case catch"
					" continue else return switch throw try while"
					));
		SetProperty("styling.within.preprocessor", "1");
	}
	else {
		SetLexer(wxSTC_LEX_NULL);
	}
	wxFont *font = new wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString);
	for (int i = 0; i < 32; ++i) {
		StyleSetFont(i, *font);
	}
}

void Editor::loadFormatted(const std::string& file) {
	LoadFile(file);
	convertTabs();
}

void Editor::convertTabs(void) {
	std::string oldText = std::string(GetText().mb_str());
	std::string newText;
	for (char c : oldText) {
		if (c == '\t') {
			newText.append("    ");
		}
		else {
			newText.push_back(c);
		}
	}
	wxString docText(newText);
	SetText(docText);
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

std::string Editor::getIndentSpace(const int& line) const {
	std::string lineIndent = std::string(GetLine(line).mb_str());
	int ind = 0;
	while (ind < lineIndent.size() && lineIndent[ind] == ' ') {
		++ind;
	}
	return lineIndent.substr(0, ind);
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
	std::string indentSpace = getIndentSpace(currLine());
    for (int i = 0; i < num; ++i) {
        NewLine();
		AddText(indentSpace);
    }
    Window *w = getWindow();
    w->mode = EDIT_MODE;    
}

void Editor::insertLineAbove(const int& num) {
	std::string indentSpace = getIndentSpace(currLine());
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

void Editor::cutToLineEnd(void) {
	SetAnchor(GetCurrentPos());
	SetCurrentPos(lineEndPos() - 1);
	cutSelection();
	caretLeft(1);
}

void Editor::cutLines(const int& num) {
	GotoPos(lineStartPos());
	DeleteBack();
	int endLine = std::min(GetLineCount() - 1, currLine() + num - 1);
	SetAnchor(GetLineEndPosition(endLine));
	cutSelection();
	caretDown(1);
	VCHome();
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

void Editor::copySelection(void) {
	if (GetAnchor() <= GetCurrentPos()) {
		CharRightExtend();
	}
	Copy();	
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
	if (GetCurrentPos() != pos && GetCurrentPos() == GetAnchor()) {
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

void Editor::jumpStartVis(void) {
	if (GetCurrentPos() >= GetAnchor()) {
		SetAnchor(GetAnchor() + 1);
	}
	SetCurrentPos(0);
}

void Editor::jumpEndVis(void) {
	if (GetCurrentPos() < GetAnchor()) {
		SetAnchor(GetAnchor() - 1);	
	}
	SetCurrentPos(GetTextLength() - 1);
}

void Editor::jumpLineVis(const int& line) {
	int endPos = PositionFromLine(line);
	if (endPos >= GetAnchor() - 1 && GetAnchor() > GetCurrentPos()) {
		SetAnchor(GetAnchor() - 1);
	}
	else if (endPos < GetAnchor() && GetAnchor() <= GetCurrentPos()) {
		SetAnchor(GetAnchor() + 1);
	}
	SetCurrentPos(endPos);
}

void Editor::wordLeftVis(const int& num) {
	for (int i = 0; i < num; ++i) {
		int startPos = GetCurrentPos();
		WordLeftExtend();
		if (GetCurrentPos() == startPos) {
			return;
		}
		if (GetCurrentPos() < GetAnchor() && GetAnchor() <= startPos) {
			SetAnchor(GetAnchor() + 1);
		}
	}
}

void Editor::wordRightVis(const int& num) {
	for (int i = 0; i < num; ++i) {
		int startPos = GetCurrentPos();
		WordRightExtend();
		if (GetCurrentPos() == startPos) {
			return;
		}
		if (GetCurrentPos() >= GetAnchor() - 1 && GetAnchor() - 1 >= startPos) {
			SetAnchor(GetAnchor() - 1);
		}
	}
}

void Editor::lineEndVis(void) {
	int startPos = GetCurrentPos();
	LineEndExtend();
	if (GetCurrentPos() >= GetAnchor() - 1 && GetAnchor() - 1 >= startPos) {
		SetAnchor(GetAnchor() - 1);
	}
	caretLeftVis(1);
}

void Editor::lineStartVis(void) {
	int startPos = GetCurrentPos();
	HomeExtend();
	if (GetCurrentPos() != startPos && GetCurrentPos() < GetAnchor() && GetAnchor() <= startPos) {
		SetAnchor(GetAnchor() + 1);
	}
}

void Editor::lineHomeVis(void) {
	int startPos = GetCurrentPos();
	VCHomeExtend();
	if (GetCurrentPos() != startPos && GetCurrentPos() < GetAnchor() && GetAnchor() <= startPos) {
		SetAnchor(GetAnchor() + 1);
	}
}

void Editor::charSearchAheadVis(const char& c, const bool& inc) {
	int endPos = GetCurrentPos() + 1;
	while (endPos < lineEndPos() && GetCharAt(endPos) != c) {
		++endPos;
	}
	if (endPos >= lineEndPos()) {
		return;
	}
	if (!inc) {
		--endPos;
	}
	if (endPos >= GetAnchor() - 1 && GetAnchor() - 1 >= GetCurrentPos()) {
		SetAnchor(GetAnchor() - 1);
	}
	SetCurrentPos(endPos);
}

void Editor::charSearchBehindVis(const char& c, const bool& inc) {
	int endPos = GetCurrentPos() - 1;
	while (endPos >= lineStartPos() && GetCharAt(endPos) != c) {
		--endPos;
	}
	if (endPos < lineStartPos()) {
		return;
	}
	if (!inc) {
		++endPos;
	}
	if (endPos < GetAnchor() && GetAnchor() <= GetCurrentPos()) {
		SetAnchor(GetAnchor() + 1);
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
	int endLine = std::min(currLine() + num, GetLineCount() - 1);
	int startLine = LineFromPosition(GetAnchor());
	if (endLine >= startLine) {
		SetAnchor(PositionFromLine(startLine));
		SetCurrentPos(GetLineEndPosition(endLine));
	}
	else {
		SetCurrentPos(PositionFromLine(endLine));
	}
}

void Editor::shiftLine(const int& num, const bool& dir) {
	int startLine = LineFromPosition(GetSelectionStart());
	int endLine = LineFromPosition(GetSelectionEnd());
	if (dir == 1) {
		std::string indent(num * 4, ' ');
		for (int i = startLine; i <= endLine; ++i) {
			InsertText(PositionFromLine(i), indent);
		}
	}
	else {
		for (int i = startLine; i <= endLine; ++i) {
			int rem = std::min((int)getIndentSpace(i).size(), num * 4);
			DeleteRange(PositionFromLine(i), rem);
		}
	}
}

void Editor::jumpStartLine(void) {
	int startLine = LineFromPosition(GetAnchor());
	SetAnchor(GetLineEndPosition(startLine));
	SetCurrentPos(0);
}

void Editor::jumpEndLine(void) {
	int startLine = LineFromPosition(GetAnchor());
	SetAnchor(PositionFromLine(startLine));
	SetCurrentPos(GetTextLength() - 1);
}

void Editor::jumpLineLine(const int& line) {
	int startLine = LineFromPosition(GetAnchor());
	if (line >= startLine) {
		SetAnchor(PositionFromLine(startLine));
		SetCurrentPos(GetLineEndPosition(line));
	}
	else {
		SetAnchor(GetLineEndPosition(startLine));
		SetCurrentPos(PositionFromLine(line));
	}
}
