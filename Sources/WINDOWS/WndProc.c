Copyright 2016 Gregory BryantLicensed under the Apache License, Version 2.0 (the "License");you may not use this file except in compliance with the License.You may obtain a copy of the License at    http://www.apache.org/licenses/LICENSE-2.0Unless required by applicable law or agreed to in writing, softwaredistributed under the License is distributed on an "AS IS" BASIS,WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.See the License for the specific language governing permissions andlimitations under the License./***********************************************************************/#include "ProjectHeaders.h"LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){switch(msg){	case WM_COMMAND:	{		switch(LOWORD(wParam))		{		case ID_FILE_GLOBAL:	DoSettings();break;		case ID_FILE_EXIT:		PostMessage(hwnd,WM_CLOSE,0,0);break;		case ID_ASSM_SELECT:	SelectSourceAndBuild();break;		case ID_DASSM_SELECT:	Disassemble();break;		case ID_EXP_SELECT:		Explore();break;		case ID_HELP_ABOUT:		AboutApplication();break;		}	break;	}	case WM_CLOSE:	DestroyWindow(hwnd);	break;	case WM_DESTROY:	PostQuitMessage(0);	break;	default:	return DefWindowProc(hwnd, msg, wParam, lParam);}return 0;}