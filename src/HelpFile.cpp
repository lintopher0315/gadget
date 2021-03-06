#include "Window.h"
#include "FileHelper.h"

HelpFile::HelpFile(wxWindow *parent) : Editor(parent, NULL_LEX) {
	SetLexer(wxSTC_LEX_LUA);

	StyleSetForeground(wxSTC_LUA_WORD4, wxColor(209, 73, 94));
	StyleSetForeground(wxSTC_LUA_WORD5, wxColor(219, 131, 0));
	StyleSetForeground(wxSTC_LUA_WORD6, wxColor(120, 161, 109));
	StyleSetForeground(wxSTC_LUA_WORD7, wxColor(147, 196, 82));
	StyleSetForeground(wxSTC_LUA_WORD8, wxColor(222, 149, 222));

	SetKeyWords(3, wxT("Gadget"));
	SetKeyWords(4, wxT("NORMAL"));
	SetKeyWords(5, wxT("EDIT"));
	SetKeyWords(6, wxT("VISUAL"));
	SetKeyWords(7, wxT("LINE"));

	AddText(
		"Welcome to the Gadget help file!\n"
		"This file is read-only\n\n"

		"Gadget is a GUI text editor inspired by GVim, the interface variant of the popular text editor, Vim.\n\n"

		"Gadget features four modes: NORMAL EDIT VISUAL and LINE\n\n"

		"~~~~~~~~~~~~\nNORMAL MODE:\n~~~~~~~~~~~~\n\n"

		"hjkl\n\t\t\t\th - left, j - down, k - up, l - right; can be prepended with [num]\n"
		"w\n\t\t\t\tjump forward to the start of the next word; can be prepended with [num]\n"
		"b\n\t\t\t\tjump backward to the start of the previous word; can be prepended with [num]\n"
		"gg\n\t\t\t\tjump to the beginning of the file\n"
		"G\n\t\t\t\tjump to the end of the file\n"
		"[num]G\n\t\t\t\tjump to the beginning of the line indicated by [num]\n"
		"_\n\t\t\t\tjump to the first non-whitespace character of the current line\n"
		"0\n\t\t\t\tjump to the beginning of the current line\n"
		"$\n\t\t\t\tjump to the end of the current line\n"
		"x\n\t\t\t\tcuts the character the caret is currently on\n"
		"o\n\t\t\t\tappend an empty line below the current line; can be prepended with [num]\n"
		"O\n\t\t\t\tappend an empty line above the current line; can be prepended with [num]\n"
		"dd\n\t\t\t\tcut a line; can be prepended with [num]\n"
		"D\n\t\t\t\tcut to the end of the current line\n"
		"f [char]\n\t\t\t\tjump to the next instance of [char] on the current line\n"
		"F [char]\n\t\t\t\tjump to the previous instance of [char] on the current line\n"
		"t [char]\n\t\t\t\tjump just before the next instance of [char] on the current line\n"
		"T [char]\n\t\t\t\tjump just after the previous instance of [char] on the current line\n"
		"p\n\t\t\t\tpaste the contents of the clipboard after the caret\n"
		"P\n\t\t\t\tpaste the contents of the clipboard before the caret\n"
		"[num]gt\n\t\t\t\tswitch to the tab indicated by [num]\n"
		"i\n\t\t\t\tswitch to EDIT mode\n"
		"I\n\t\t\t\tswitch to EDIT mode and jump to the first non-whitespace character of the current line\n"
		"a\n\t\t\t\tswitch to EDIT mode and move caret right once\n"
		"A\n\t\t\t\tswitch to EDIT mode and jump to the end of the current line\n"
		"v\n\t\t\t\tswitch to VISUAL mode\n"
		"V\n\t\t\t\tswitch to LINE mode\n"
		":q\n\t\t\t\tquit and close the buffer\n"
		":w\n\t\t\t\tsave the buffer to the file it points to; can be postpended by [string] to specify a file\n"
		":wq\n\t\t\t\tsave and close the buffer\n"
		":e [string]\n\t\t\t\tedit a new file indicated by [string] in the current buffer\n"
		":tabedit\n\t\t\t\topen a new buffer; can be postpended by [string] to specify a file\n"
		":split\n\t\t\t\tsplit the window horizontally\n"
		":vsplit\n\t\t\t\tsplit the window vertically\n"
		":help\n\t\t\t\topen the help file\n\n"

		"~~~~~~~~~~~~\nVISUAL MODE:\n~~~~~~~~~~~~\n\n"

		"hjkl\n\t\t\t\textend the selection left, down, up, or right; can be prepended with [num]\n"
		"w\n\t\t\t\textend the selection to the start of the next word; can be prepended with [num]\n"
		"b\n\t\t\t\textend the selection to the start of the previous word; can be prepended with [num]\n"
		"gg\n\t\t\t\textend the selection to the beginning of the file\n"
		"G\n\t\t\t\textend the selection to the end of the file\n"
		"[num]G\n\t\t\t\textend the selection to the beginning of the line indicated by [num]\n"
		"_\n\t\t\t\textend the selection to the first non-whitespace character of the current line\n"
		"0\n\t\t\t\textend the selection to the beginning of the current line\n"
		"$\n\t\t\t\textend the selection to the end of the current line\n"
		"x\n\t\t\t\tcuts the selection\n"
		"d\n\t\t\t\tsame as x\n"
		"f [char]\n\t\t\t\textend the selection to the next instance of [char] on the current line\n"
		"F [char]\n\t\t\t\textend the selection to the previous instance of [char] on the current line\n"
		"t [char]\n\t\t\t\textend the selection just before the next instance of [char] on the current line\n"
		"T [char]\n\t\t\t\textend the selection just after the previous instance of [char] on the current line\n"
		"u\n\t\t\t\tswitch selection to lowercase\n"
		"U\n\t\t\t\tswitch selection to uppercase\n"
		"y\n\t\t\t\tcopy the selection to the clipboard\n\n"

		"~~~~~~~~~~\nLINE MODE:\n~~~~~~~~~~\n\n"

		"jk\n\t\t\t\textend the line selection down or up; can be prepended with [num]\n"
		"gg\n\t\t\t\textend the line selection to the beginning of the file\n"
		"G\n\t\t\t\textend the line selection to the end of the file\n"
		"[num]G\n\t\t\t\textend the line selection to the beginning of the line indicated by [num]\n"
		"x\n\t\t\t\tcuts the line selection\n"
		"d\n\t\t\t\tsame as x\n"
		"u\n\t\t\t\tswitch line selection to lowercase\n"
		"U\n\t\t\t\tswitch line selection to uppercase\n"
		"y\n\t\t\t\tcopy the line selection to the clipboard\n"
		"<\n\t\t\t\tshift the lines selected to the left; can be prepended with [num]\n"
		">\n\t\t\t\tshift the lines selected to the right; can be prepended with [num]\n\n"
	);

	SetEditable(false);
	SetReadOnly(true);
}
