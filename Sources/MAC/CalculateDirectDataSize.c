Copyright 2016 Gregory BryantLicensed under the Apache License, Version 2.0 (the "License");you may not use this file except in compliance with the License.You may obtain a copy of the License at    http://www.apache.org/licenses/LICENSE-2.0Unless required by applicable law or agreed to in writing, softwaredistributed under the License is distributed on an "AS IS" BASIS,WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.See the License for the specific language governing permissions andlimitations under the License./***********************************************************************/#include "ProjectHeaders.h"long CalculateDirectDataSize(char* asmBuffer, long dataSize, FSSpecPtr parSpec){long			size,count,i,j,k,tempDataSize,tempDataSize2;Str255			dataHolder;FSSpec			tempFSS;void			*tempREF;size = 0;for(i=0,count=0;i<dataSize;i++){	if(asmBuffer[i] == '#')	{while(asmBuffer[i]!=0x0D){i++;}}	if(asmBuffer[i] == 0xD)	{		count++;	}	if(asmBuffer[i] == '@')	{count++;		i++;		j=i;		_FindWhiteSpace(asmBuffer,i)		tempDataSize=i-j;		i=j;		dataHolder[0]=tempDataSize;		for(k=0;k<tempDataSize;k++)		{		dataHolder[k+1] = asmBuffer[i];		i++;		}		if(MyPStrCmp(dataHolder,"\pdata"))		{			_FindAlpha(asmBuffer,i)			j=i;			_FindQuote(asmBuffer,i)			tempDataSize = i-j;			i=j;			dataHolder[0]=tempDataSize;			for(k=0;k<tempDataSize;k++)			{			dataHolder[k+1]=asmBuffer[i];			i++;			}			FSMakeFSSpec(parSpec->vRefNum,parSpec->parID,dataHolder,&tempFSS);			FSpOpenDF(&tempFSS,fsRdPerm,&tempREF);			GetEOF(tempREF,&tempDataSize2);			FSClose(tempREF);			tempDataSize2 +=  4 - (tempDataSize2 % 4);			size+=tempDataSize2;		}	}}return size;}