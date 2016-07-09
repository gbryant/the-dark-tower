Copyright 2016 Gregory BryantLicensed under the Apache License, Version 2.0 (the "License");you may not use this file except in compliance with the License.You may obtain a copy of the License at    http://www.apache.org/licenses/LICENSE-2.0Unless required by applicable law or agreed to in writing, softwaredistributed under the License is distributed on an "AS IS" BASIS,WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.See the License for the specific language governing permissions andlimitations under the License./***********************************************************************/#include "ProjectHeaders.h"#define NameToPath(X)\for(i=0;((char*)keyFile)[i]!=0;i++){}\for(;((char*)keyFile)[i]!='\\';i--){}i++;\for(j=0;j<i;j++){X[j]=((char*)keyFile)[j];}\for(j=0;j<dataHolder[0];j++){X[i+j]=dataHolder[j+1];}X[i+j]=0;#define GET_32																									\																												\neg=false;hex=false;																							\_FindNumeric(dataBuffer,(bMarker))																				\if(!(dataBuffer[(bMarker)]>=0x30 && dataBuffer[(bMarker)]<=0x39))												\{																												\if(dataBuffer[(bMarker)]=='-'){neg=true;(bMarker)++;}															\if(dataBuffer[(bMarker)]=='$'){hex=true;(bMarker)++;}															\}bmHolder = (bMarker);																							\if(!hex)																										\{																												\while(!(dataBuffer[(bMarker)]>=0x30 && dataBuffer[(bMarker)]<=0x39)){(bMarker)++;}								\bmHolder = (bMarker);																							\while(dataBuffer[(bMarker)]>=0x30 && dataBuffer[(bMarker)]<=0x39){(bMarker)++;}									\																												\arg = 0;																										\j = (bMarker) - bmHolder;																						\j--;																											\(bMarker) = bmHolder;																							\for(;j>=0;(bMarker)++,j--)																						\{																												\arg += ((dataBuffer[(bMarker)]-0x30)*(_pow(10,j)));																\}																												\}																												\else																											\{																												\while(!((dataBuffer[(bMarker)]>=0x30&&dataBuffer[(bMarker)]<=0x39)||(dataBuffer[(bMarker)]>='A'&&dataBuffer[(bMarker)]<='F'))){(bMarker)++;}	\bmHolder = (bMarker);																															\while((dataBuffer[(bMarker)]>=0x30&&dataBuffer[(bMarker)]<=0x39)||(dataBuffer[(bMarker)]>='A'&&dataBuffer[(bMarker)]<='F')){(bMarker)++;}		\arg = 0;																										\j = (bMarker) - bmHolder;																						\j--;																											\(bMarker) = bmHolder;																							\for(;j>=0;(bMarker)++,j--)																						\{																												\if(dataBuffer[(bMarker)]>=0x30&&dataBuffer[(bMarker)]<=0x39)													\{																												\arg += ((dataBuffer[(bMarker)]-0x30)*(_pow(0x10,j)));															\}																												\else																											\{																												\arg += ((dataBuffer[(bMarker)]-0x37)*(_pow(0x10,j)));															\}																												\}																												\}																												\#define		_GetName(X)						\											\_FindQuote(dataBuffer,bMarker)				\bMarker++;									\bmHolder=bMarker;							\_FindQuote(dataBuffer,bMarker)				\tempDataSize = bMarker-bmHolder;			\bMarker=bmHolder;							\X[0]=tempDataSize;							\for(i=0;i<tempDataSize;i++)					\{											\X[i+1]=dataBuffer[bMarker];					\bMarker++;									\}void ReadKeyFile(Key *keyStructure,void *keyFile){void			*fRefNumber;char*			dataBuffer;long			dataSize;long			bMarker,arg,j;unsigned long	count;Str255			dataHolder;long			i,bmHolder,tempDataSize;Boolean neg,hex;DataExport *currDE;Str255 tempTxt,tempTxt2,tempTxt3;long	lineNum;void 	*err;keyStructure->asmb=false;keyStructure->rsrcb=false;keyStructure->rsrcmbb=false;keyStructure->datab=false;keyStructure->secoffb=false;keyStructure->secoffob=false;keyStructure->relocb=false;keyStructure->deb=false;keyStructure->machb=false;keyStructure->cfrgb=false;keyStructure->intelb=false;keyStructure->asm_=0;keyStructure->rsrc=0;keyStructure->rsrcmb=0;keyStructure->data=0;keyStructure->secoff=0;keyStructure->reloc=0;keyStructure->machinfo=0;err = FSpOpenDF(keyFile,fsRdPerm,&fRefNumber);if(CheckError_FSpOpenDF(err)){DebugFunction("\pFile System Error");}SetFPos(fRefNumber,fsFromStart,0);GetEOF(fRefNumber,&dataSize);if(!dataSize){DebugFunction("\pZero length key file!");}dataBuffer = NewPtr(dataSize);count = dataSize;FSRead(fRefNumber,&count,dataBuffer);FSClose(fRefNumber);lineNum=1;bMarker=0;while(bMarker<dataSize){	switch(dataBuffer[bMarker])	{		case(' '):		case('\t'):		case('"'):		{		bMarker++;		break;		}		case('\r'):		case('\n'):		{		lineNum++;		bMarker++;		break;		}		case('@'):		{		bMarker++;		bmHolder=bMarker;		_FindWhiteSpace2(dataBuffer,bMarker)		tempDataSize=bMarker-bmHolder;		bMarker=bmHolder;		dataHolder[0]=tempDataSize;		for(i=0;i<tempDataSize;i++)		{		dataHolder[i+1]=dataBuffer[bMarker];		bMarker++;		}		if(MyPStrCmp(dataHolder,"\pTarget"))		{		_GetName(dataHolder);bMarker++;		if(MyPStrCmp(dataHolder,"\pcfrg"))		{keyStructure->cfrgb=true;}		else if(MyPStrCmp(dataHolder,"\pmach"))		{keyStructure->machb=true;}		else if(MyPStrCmp(dataHolder,"\pintel"))		{keyStructure->intelb=true;}		}		else if(MyPStrCmp(dataHolder,"\pAssembly"))		{		_GetName(dataHolder);bMarker++;		keyStructure->asm_ = FileRef_Allocate();		FileRef_To_FileRef_NewName(keyFile,dataHolder,keyStructure->asm_);		keyStructure->asmb=true;		}		else if(MyPStrCmp(dataHolder,"\pData"))		{		_GetName(dataHolder);bMarker++;		keyStructure->data = FileRef_Allocate();		FileRef_To_FileRef_NewName(keyFile,dataHolder,keyStructure->data);		keyStructure->datab=true;		}		else if(MyPStrCmp(dataHolder,"\pData:"))		{		_GetName(dataHolder);bMarker++;		keyStructure->data = FileRef_Allocate();		FileRef_To_FileRef_NewName(keyFile,dataHolder,keyStructure->data);		keyStructure->datab=true;		keyStructure->deb=true;		currDE=&keyStructure->de;		currDE->next = nil;		_Find('{',dataBuffer,bMarker)		_Find('$',dataBuffer,bMarker)		while(dataBuffer[bMarker]!='}')		{			GET_32			currDE->offset = arg;			_GetName(dataHolder)			currDE->label = NewPtr(dataHolder[0]+1);			MyPStrAdd(dataHolder,"\p",(unsigned char*)currDE->label);			while(dataBuffer[bMarker]!='$' && dataBuffer[bMarker]!='}'){bMarker++;}			if(dataBuffer[bMarker]!='}')			{			currDE->next = (DataExport*)NewPtr(sizeof(DataExport));			currDE = currDE->next;			currDE->next = nil;			}		}		bMarker++;		}		else if(MyPStrCmp(dataHolder,"\pSecOff"))		{			bmHolder = bMarker;			while( !( (dataBuffer[bMarker]=='"') || (dataBuffer[bMarker]=='{')) ){bMarker++;}			if(dataBuffer[bMarker]=='"')			{				bMarker = bmHolder;				_GetName(dataHolder);bMarker++;				keyStructure->secoff = FileRef_Allocate();				FileRef_To_FileRef_NewName(keyFile,dataHolder,keyStructure->secoff);				keyStructure->secoffb=false;				keyStructure->secoffob=true;			}			else			{			_Find('$',dataBuffer,bMarker)			GET_32			if(dataBuffer[bMarker+1]=='m')keyStructure->mainOffset=arg;			else if(dataBuffer[bMarker+1]=='i')keyStructure->initOffset=arg;			else if(dataBuffer[bMarker+1]=='t')keyStructure->termOffset=arg;						_Find('$',dataBuffer,bMarker)			GET_32			if(dataBuffer[bMarker+1]=='m')keyStructure->mainOffset=arg;			else if(dataBuffer[bMarker+1]=='i')keyStructure->initOffset=arg;			else if(dataBuffer[bMarker+1]=='t')keyStructure->termOffset=arg;						_Find('$',dataBuffer,bMarker)			GET_32			if(dataBuffer[bMarker+1]=='m')keyStructure->mainOffset=arg;			else if(dataBuffer[bMarker+1]=='i')keyStructure->initOffset=arg;			else if(dataBuffer[bMarker+1]=='t')keyStructure->termOffset=arg;						_Find('}',dataBuffer,bMarker)			bMarker++;			keyStructure->secoffb=true;			keyStructure->secoffob=false;			}		}		else if(MyPStrCmp(dataHolder,"\pRelocation"))		{		_GetName(dataHolder);bMarker++;		keyStructure->reloc = FileRef_Allocate();		FileRef_To_FileRef_NewName(keyFile,dataHolder,keyStructure->reloc);		keyStructure->relocb=true;		}		else if(MyPStrCmp(dataHolder,"\pResource"))		{		_GetName(dataHolder);bMarker++;		keyStructure->rsrc = FileRef_Allocate();		FileRef_To_FileRef_NewName(keyFile,dataHolder,keyStructure->rsrc);		keyStructure->rsrcb=true;		}		else if(MyPStrCmp(dataHolder,"\pResourceMacBinary"))		{		_GetName(dataHolder);bMarker++;		keyStructure->rsrcmb = FileRef_Allocate();		FileRef_To_FileRef_NewName(keyFile,dataHolder,keyStructure->rsrcmb);		keyStructure->rsrcmbb=true;		}		else if(MyPStrCmp(dataHolder,"\pProduct"))		{		_GetName(dataHolder);bMarker++;		if(dataHolder[0]>30){DebugFunction("\pKeyfile: Product Name Too Long");}		MyPStrAdd("\p",dataHolder,keyStructure->target);		}		else if(MyPStrCmp(dataHolder,"\pType"))		{		_FindQuote(dataBuffer,bMarker)		bMarker++;		keyStructure->type = *((long*)(dataBuffer+bMarker));		bMarker+=5;		}		else if(MyPStrCmp(dataHolder,"\pCreator"))		{		_FindQuote(dataBuffer,bMarker)		bMarker++;		keyStructure->creator = *((long*)(dataBuffer+bMarker));		bMarker+=5;		}		else if(MyPStrCmp(dataHolder,"\pMachInfo"))		{		_GetName(dataHolder);bMarker++;		keyStructure->machinfo = FileRef_Allocate();		FileRef_To_FileRef_NewName(keyFile,dataHolder,keyStructure->machinfo);		}		break;		}		case('#'):		{		while(dataBuffer[bMarker]!=0x0D){bMarker++;}		break;		}		default:		{		NumberToString(lineNum,(char*)tempTxt);		MyPStrAdd("\pAn illegal byte was found in your key file\rLine: ",tempTxt,tempTxt2);		DebugFunction(tempTxt2);		}	}}DisposePtr(dataBuffer);}