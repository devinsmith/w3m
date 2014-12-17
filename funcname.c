FuncList w3mFuncList[] = {
	 /* 0 */ {"@@@", nulcmd},
	 /* 1 */ {"ABORT", quitfm},
	 /* 2 */ {"ACCESSKEY", accessKey},
	 /* 3 */ {"ADD_BOOKMARK", adBmark},
	 /* 4 */ {"ALARM", setAlarm},
	 /* 5 */ {"BACK", backBf},
	 /* 6 */ {"BEGIN", goLineF},
	 /* 7 */ {"BOOKMARK", ldBmark},
	 /* 8 */ {"CENTER_H", ctrCsrH},
	 /* 9 */ {"CENTER_V", ctrCsrV},
	 /* 10 */ {"CHARSET", docCSet},
	 /* 11 */ {"CLOSE_TAB", closeT},
	 /* 12 */ {"CLOSE_TAB_MOUSE", closeTMs},
	 /* 13 */ {"COMMAND", execCmd},
	 /* 14 */ {"COOKIE", cooLst},
	 /* 15 */ {"DEFAULT_CHARSET", defCSet},
	 /* 16 */ {"DEFINE_KEY", defKey},
	 /* 17 */ {"DELETE_PREVBUF", deletePrevBuf},
	 /* 18 */ {"DICT_WORD", dictword},
	 /* 19 */ {"DICT_WORD_AT", dictwordat},
	 /* 20 */ {"DISPLAY_IMAGE", dispI},
	 /* 21 */ {"DOWN", ldown1},
	 /* 22 */ {"DOWNLOAD", svSrc},
	 /* 23 */ {"DOWNLOAD_LIST", ldDL},
	 /* 24 */ {"EDIT", editBf},
	 /* 25 */ {"EDIT_SCREEN", editScr},
	 /* 26 */ {"END", goLineL},
	 /* 27 */ {"ESCBMAP", escbmap},
	 /* 28 */ {"ESCMAP", escmap},
	 /* 29 */ {"EXEC_SHELL", execsh},
	 /* 30 */ {"EXIT", quitfm},
	 /* 31 */ {"EXTERN", extbrz},
	 /* 32 */ {"EXTERN_LINK", linkbrz},
	 /* 33 */ {"FRAME", rFrame},
	 /* 34 */ {"GOTO", goURL},
	 /* 35 */ {"GOTO_LINE", goLine},
	 /* 36 */ {"GOTO_LINK", followA},
	 /* 37 */ {"GOTO_RELATIVE", gorURL},
	 /* 38 */ {"HELP", ldhelp},
	 /* 39 */ {"HISTORY", ldHist},
	 /* 40 */ {"INFO", pginfo},
	 /* 41 */ {"INTERRUPT", susp},
	 /* 42 */ {"ISEARCH", isrchfor},
	 /* 43 */ {"ISEARCH_BACK", isrchbak},
	 /* 44 */ {"LEFT", col1L},
	 /* 45 */ {"LINE_BEGIN", linbeg},
	 /* 46 */ {"LINE_END", linend},
	 /* 47 */ {"LINE_INFO", curlno},
	 /* 48 */ {"LINK_BEGIN", topA},
	 /* 49 */ {"LINK_END", lastA},
	 /* 50 */ {"LINK_MENU", linkMn},
	 /* 51 */ {"LIST", linkLst},
	 /* 52 */ {"LIST_MENU", listMn},
	 /* 53 */ {"LOAD", ldfile},
	 /* 54 */ {"MAIN_MENU", mainMn},
	 /* 55 */ {"MARK", _mark},
	 /* 56 */ {"MARK_MID", chkNMID},
	 /* 57 */ {"MARK_URL", chkURL},
	 /* 58 */ {"MARK_WORD", chkWORD},
	 /* 59 */ {"MENU", mainMn},
	 /* 60 */ {"MENU_MOUSE", menuMs},
	 /* 61 */ {"MOUSE", mouse},
	 /* 62 */ {"MOUSE_TOGGLE", msToggle},
	 /* 63 */ {"MOVE_DOWN", movD},
	 /* 64 */ {"MOVE_DOWN1", movD1},
	 /* 65 */ {"MOVE_LEFT", movL},
	 /* 66 */ {"MOVE_LEFT1", movL1},
	 /* 67 */ {"MOVE_LIST_MENU", movlistMn},
	 /* 68 */ {"MOVE_MOUSE", movMs},
	 /* 69 */ {"MOVE_RIGHT", movR},
	 /* 70 */ {"MOVE_RIGHT1", movR1},
	 /* 71 */ {"MOVE_UP", movU},
	 /* 72 */ {"MOVE_UP1", movU1},
	 /* 73 */ {"MSGS", msgs},
	 /* 74 */ {"MULTIMAP", multimap},
	 /* 75 */ {"NEW_TAB", newT},
	 /* 76 */ {"NEXT", nextBf},
	 /* 77 */ {"NEXT_DOWN", nextD},
	 /* 78 */ {"NEXT_LEFT", nextL},
	 /* 79 */ {"NEXT_LEFT_UP", nextLU},
	 /* 80 */ {"NEXT_LINK", nextA},
	 /* 81 */ {"NEXT_MARK", nextMk},
	 /* 82 */ {"NEXT_PAGE", pgFore},
	 /* 83 */ {"NEXT_RIGHT", nextR},
	 /* 84 */ {"NEXT_RIGHT_DOWN", nextRD},
	 /* 85 */ {"NEXT_TAB", nextT},
	 /* 86 */ {"NEXT_UP", nextU},
	 /* 87 */ {"NEXT_VISITED", nextVA},
	 /* 88 */ {"NEXT_WORD", movRW},
	 /* 89 */ {"NOTHING", nulcmd},
	 /* 90 */ {"NULL", nulcmd},
	 /* 91 */ {"OPTIONS", ldOpt},
	 /* 92 */ {"PCMAP", pcmap},
	 /* 93 */ {"PEEK", curURL},
	 /* 94 */ {"PEEK_IMG", peekIMG},
	 /* 95 */ {"PEEK_LINK", peekURL},
	 /* 96 */ {"PIPE_BUF", pipeBuf},
	 /* 97 */ {"PIPE_SHELL", pipesh},
	 /* 98 */ {"PREV", prevBf},
	 /* 99 */ {"PREV_LINK", prevA},
	 /* 100 */ {"PREV_MARK", prevMk},
	 /* 101 */ {"PREV_PAGE", pgBack},
	 /* 102 */ {"PREV_TAB", prevT},
	 /* 103 */ {"PREV_VISITED", prevVA},
	 /* 104 */ {"PREV_WORD", movLW},
	 /* 105 */ {"PRINT", svBuf},
	 /* 106 */ {"QUIT", qquitfm},
	 /* 107 */ {"READ_SHELL", readsh},
	 /* 108 */ {"REDO", redoPos},
	 /* 109 */ {"REDRAW", rdrwSc},
	 /* 110 */ {"REG_MARK", reMark},
	 /* 111 */ {"REINIT", reinit},
	 /* 112 */ {"RELOAD", reload},
	 /* 113 */ {"RESHAPE", reshape},
	 /* 114 */ {"RIGHT", col1R},
	 /* 115 */ {"SAVE", svSrc},
	 /* 116 */ {"SAVE_IMAGE", svI},
	 /* 117 */ {"SAVE_LINK", svA},
	 /* 118 */ {"SAVE_SCREEN", svBuf},
	 /* 119 */ {"SEARCH", srchfor},
	 /* 120 */ {"SEARCH_BACK", srchbak},
	 /* 121 */ {"SEARCH_FORE", srchfor},
	 /* 122 */ {"SEARCH_NEXT", srchnxt},
	 /* 123 */ {"SEARCH_PREV", srchprv},
	 /* 124 */ {"SELECT", selBuf},
	 /* 125 */ {"SELECT_MENU", selMn},
	 /* 126 */ {"SETENV", setEnv},
	 /* 127 */ {"SET_OPTION", setOpt},
	 /* 128 */ {"SHELL", execsh},
	 /* 129 */ {"SHIFT_LEFT", shiftl},
	 /* 130 */ {"SHIFT_RIGHT", shiftr},
	 /* 131 */ {"SOURCE", vwSrc},
	 /* 132 */ {"STOP_IMAGE", stopI},
	 /* 133 */ {"SUBMIT", submitForm},
	 /* 134 */ {"SUSPEND", susp},
	 /* 135 */ {"TAB_GOTO", tabURL},
	 /* 136 */ {"TAB_GOTO_RELATIVE", tabrURL},
	 /* 137 */ {"TAB_LEFT", tabL},
	 /* 138 */ {"TAB_LINK", tabA},
	 /* 139 */ {"TAB_MENU", tabMn},
	 /* 140 */ {"TAB_MOUSE", tabMs},
	 /* 141 */ {"TAB_RIGHT", tabR},
	 /* 142 */ {"UNDO", undoPos},
	 /* 143 */ {"UP", lup1},
	 /* 144 */ {"VERSION", dispVer},
	 /* 145 */ {"VIEW", vwSrc},
	 /* 146 */ {"VIEW_BOOKMARK", ldBmark},
	 /* 147 */ {"VIEW_IMAGE", followI},
	 /* 148 */ {"WHEREIS", srchfor},
	 /* 149 */ {"WRAP_TOGGLE", wrapToggle},
	{NULL, NULL}
};
