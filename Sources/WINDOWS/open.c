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


#include "ProjectHeaders.h"








#define swap(A,X)					\
for(i=0;i<(X/2);i+=2)				\
{									\
a=((char*)A)[i];					\
((char*)A)[i] = ((char*)A)[i+1];	\
((char*)A)[i+1] = a;				\
}










void open(void)
{
OPENFILENAME 			ofn;
char 					szFileName[MAX_PATH] = "";
HANDLE 					hFile;
long 					fSize,i;
unsigned char			a;
unsigned char*			fileData;
unsigned long			bytesRead;
long					error;
char					testByte;
long					testLong;

IMAGE_DOS_HEADER *header;
IMAGE_NT_HEADERS *peHeader;




ZeroMemory(&ofn, sizeof(ofn));

ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
// ofn.hwndOwner = hwnd;
//ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
ofn.lpstrFile = szFileName;
ofn.nMaxFile = MAX_PATH;
ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
ofn.lpstrDefExt = "txt";

if(GetOpenFileName(&ofn))
{
	hFile = CreateFile(ofn.lpstrFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		fSize = GetFileSize(hFile,NULL);
		fileData = (unsigned char*)HeapAlloc(heap,0,fSize);
		if(heap)
		{
			ReadFile(hFile,fileData,fSize,&bytesRead,NULL);
			(unsigned char*)header = fileData;
			//swap(header,sizeof(IMAGE_DOS_HEADER))
			if(header->e_magic == IMAGE_DOS_SIGNATURE)
			{
				(unsigned char*)peHeader = fileData+header->e_lfanew;
//				//swap(peHeader,sizeof(IMAGE_NT_HEADERS))
				testByte = ((char*)peHeader)[1];
				testLong = ((long*)peHeader)[0];
				if(peHeader->Signature == IMAGE_NT_SIGNATURE)
				{
				
				}
			}
		CloseHandle(hFile);
		HeapFree(heap,0,fileData);
		}
	}
}










}
