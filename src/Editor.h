#pragma once

#include <wx/wx.h>
#include <wx/stc/stc.h>

class Editor;

#include "Window.h"

class Editor : public wxStyledTextCtrl {
    public:
        Editor(wxWindow *parent);

        Window *getWindow(void);

        int linePos(void) const;
		int currLine(void) const;
        int lineStartPos(void) const;
        int lineEndPos(void) const;
		std::string getIndentSpace(const int& line) const;

        void caretLeft(const int& num);
        void caretRight(const int& num);
        void caretUp(const int& num);
        void caretDown(const int& num);
        void append(void);
        void insertLineBelow(const int& num);
        void insertLineAbove(const int& num);
		void wordLeft(const int& num);
		void wordRight(const int& num);
		void charSearchAhead(const char& c, const bool& inc);
		void charSearchBehind(const char& c, const bool& inc);

		void removeSelection(void);
		void cutSelection(void);
		void copySelection(void);
		void caseChangeSelection(const bool& upper);

		void caretLeftVis(const int& num);
		void caretRightVis(const int& num);
		void caretUpVis(const int& num);
		void caretDownVis(const int& num);
		void jumpStartVis(void);
		void jumpEndVis(void);
		void jumpLineVis(const int& line);
		void wordLeftVis(const int& num);
		void wordRightVis(const int& num);
		void lineEndVis(void);
		void lineStartVis(void);
		void lineHomeVis(void);

		void caretUpLine(const int& num);
		void caretDownLine(const int& num);
		void shiftLine(const int& num, const bool& dir);
		void jumpStartLine(void);
		void jumpEndLine(void);
		void jumpLineLine(const int& line);

        std::string relPath;
		bool saved;

	private:
        void onChar(wxKeyEvent& event);
        void onKey(wxKeyEvent& event);
		void onClickUp(wxMouseEvent& event);
		void onClickDown(wxMouseEvent& event);
		void onModified(wxStyledTextEvent& event);
};
