Copyright 2016 Gregory Bryant

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
/***********************************************************************/


#define MAINSOURCEFILE 1
#include "ProjectHeaders.h"
#include "globals.h"





int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

MSG 			Msg;

unsigned char			*cmdLine,tbuffer[MAX_PATH];
long			a,i,j,k;

SHFILEINFO shFInfo;

mach_header				machHeader;
PEFContainer			appPEF;
IMAGE_DOS_HEADER 		header;
IMAGE_NT_HEADERS 		peHeader;
FileReference			destFolder;

unsigned long			count;

LPITEMIDLIST	ppidl;
unsigned long			attrib,strLen,clPos,strPos;

LPWSTR		wideStr,*theStrings;

int			numArgs;
Boolean		end;
FSSpec				*parSpec;
void					*appRefNumber;
FSSpec					*dissFolder;


//#define _WIN32_DCOM
//CoInitializeEx
//shbrowseforfolder


Initialize(hInstance,hPrevInstance,lpCmdLine,nCmdShow);

//lpCmdLine = GetCommandLine();



if(lpCmdLine[0])
{
	end=false;
	strPos=0;
	clPos=0;
	do
	{
		while(lpCmdLine[clPos]!=0 && lpCmdLine[clPos]!=' '){clPos++;}
		if(lpCmdLine[clPos]==0){end=true;}
		lpCmdLine[clPos]=0;clPos++;
		
		if(GetLongPathName(lpCmdLine+strPos,(char*)tbuffer,MAX_PATH))
		
		i=0;
		while(tbuffer[i]!=0){i++;}
		cmdLine = (unsigned char*)NewPtr(i+1);
		for(j=0;j<i;j++){cmdLine[j+1] = tbuffer[j];}
		cmdLine[j+1]=0;
		cmdLine[0]=i;
		DebugFunction(cmdLine);
		
		SHGetFileInfo(lpCmdLine+strPos,0,&shFInfo,sizeof(SHFILEINFO),SHGFI_ATTRIBUTES);
		if(shFInfo.dwAttributes & SFGAO_FOLDER )
		{
		DebugFunction("\pFolder");
		
		destFolder = MyGetFolder("\pSelect Location For The Assembled File");
		
		if(destFolder)
		{
		//SelectAssembleMethod((FileReference)cmdLine,destFolder);
		}
		
		
		}
		else
		{
			parSpec = (FSSpec*)MyGetFolder("\pSelect Folder To Save Package To");
			if(parSpec)
			{
			SelectDisassembleMethod((FileReference)cmdLine,parSpec);
			}
		}
		
		strPos=clPos;
		}
		while(!end);
}





while(GetMessage(&Msg, NULL, 0, 0) > 0)
{
TranslateMessage(&Msg);
DispatchMessage(&Msg);
}




return Msg.wParam;
}






void Initialize(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
WNDCLASSEX wc;
HMENU menuHandle;
MENUITEMINFO fileMenu;
HMENU		fileMenuHandle;
HMENU		helpMenuHandle;
HMENU		assembleMenuHandle;
HMENU		disassembleMenuHandle;
HMENU		exploreMenuHandle;

long							cyVScroll;
RECT							winRect;



gGlobalSettings.labels = true;
gGlobalSettings.procs = true;
gGlobalSettings.bfspeed = true;

gAborted = false;



wc.cbSize        = sizeof(WNDCLASSEX);
wc.style         = 0;
wc.lpfnWndProc   = WndProc;
wc.cbClsExtra    = 0;
wc.cbWndExtra    = 0;
wc.hInstance     = hInstance;
wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
wc.lpszMenuName  = NULL;
wc.lpszClassName = g_szClassName;
wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

RegisterClassEx(&wc);


gHwnd = CreateWindowEx
(
WS_EX_CLIENTEDGE,
g_szClassName,
"The Dark Tower",
WS_OVERLAPPEDWINDOW,
CW_USEDEFAULT, CW_USEDEFAULT, 340, 220,
NULL, NULL, hInstance, NULL
);




menuHandle = CreateMenu();
fileMenuHandle = CreatePopupMenu();
helpMenuHandle = CreatePopupMenu();
assembleMenuHandle = CreatePopupMenu();
disassembleMenuHandle = CreatePopupMenu();
exploreMenuHandle = CreatePopupMenu();


AppendMenu(menuHandle, MF_STRING | MF_POPUP, (UINT)fileMenuHandle, "&File");
	AppendMenu(fileMenuHandle, MF_STRING, ID_FILE_GLOBAL, "&Global Settings...");
	AppendMenu(fileMenuHandle, MF_STRING, ID_FILE_EXIT, "E&xit");



AppendMenu(menuHandle, MF_STRING | MF_POPUP, (UINT)assembleMenuHandle, "&Assemble");
	AppendMenu(assembleMenuHandle, MF_STRING, ID_ASSM_SELECT, "&Select Target...");


AppendMenu(menuHandle, MF_STRING | MF_POPUP, (UINT)disassembleMenuHandle, "&Disassemble");
	AppendMenu(disassembleMenuHandle, MF_STRING, ID_DASSM_SELECT, "&Select Target...");
	

AppendMenu(menuHandle, MF_STRING | MF_POPUP, (UINT)exploreMenuHandle, "&Explore");
	AppendMenu(exploreMenuHandle, MF_STRING, ID_EXP_SELECT, "&Select Target...");



AppendMenu(menuHandle, MF_STRING | MF_POPUP, (UINT)helpMenuHandle, "&Help");
	AppendMenu(helpMenuHandle, MF_STRING, ID_HELP_ABOUT, "About TheDarkTower...");



SetMenu(gHwnd,menuHandle);

DrawMenuBar(gHwnd);

ShowWindow(gHwnd, nCmdShow);
UpdateWindow(gHwnd);


heap = HeapCreate(0,5120,0);




cyVScroll = GetSystemMetrics(SM_CYHSCROLL);
InitCommonControls();

GetClientRect(gHwnd,&winRect);

gAppProgressBar = CreateWindowEx
(
0,
PROGRESS_CLASS,
0,
WS_CHILD,
winRect.left,
winRect.bottom-cyVScroll,
winRect.right,
cyVScroll,
gHwnd,
(HMENU)0,
(HINSTANCE)0,
0
);

gAppTopText = CreateWindowEx
(
0,
"Static",
0,
//WS_CHILD|WS_VISIBLE|SS_CENTER,
WS_CHILD|SS_CENTER,
winRect.left,
winRect.bottom-winRect.bottom/2,
winRect.right,
50,
gHwnd,
(HMENU)0,
(HINSTANCE)0,
0
);



}








