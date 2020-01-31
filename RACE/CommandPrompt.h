#include "stdafx.h"



typedef struct COMMANDPROMPT_{
	
	HWND hEdit;
	HWND hOwner;
	HWND hCombo;
	HWND hCommand;
	HWND hSend;
	HWND hConnect;


}COMMANDPROMPT;

LRESULT CALLBACK CommandPromptProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);