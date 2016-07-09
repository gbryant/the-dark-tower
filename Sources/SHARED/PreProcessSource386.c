Copyright 2016 Gregory BryantLicensed under the Apache License, Version 2.0 (the "License");you may not use this file except in compliance with the License.You may obtain a copy of the License at    http://www.apache.org/licenses/LICENSE-2.0Unless required by applicable law or agreed to in writing, softwaredistributed under the License is distributed on an "AS IS" BASIS,WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.See the License for the specific language governing permissions andlimitations under the License./***********************************************************************/#include "ProjectHeaders.h"#define INCREMENT(X)					\X++;									\if(X==bSize)							\{										\fRead=true;								\count = bSize;							\FSRead(fRefNumber,&count,srcBuffer);	\bSize = count;							\X=0;									\if(!count){break;}						\}#define INCREMENT_DATA(X)						\X++;											\if(X==dbSize)									\{												\wcount = (dbSize*sizeof(long));					\FSWrite(dataFRef,&wcount,data);					\X=0;											\}#define WHITE_SPACE(X,Y)	\(							\(X[Y]=='\t')				\||							\(X[Y]=='\r')				\||							\(X[Y]=='\n')				\||							\(X[Y]==' ')					\||							\(X[Y]=='#')					\||							\(X[Y]==',')					\||							\(X[Y]=='|')					\||							\(X[Y]==']')					\||							\(X[Y]==':')					\)void PreProcessSource386(PreProcessSourcePBlock	*ppsPB){unsigned char				*srcBuffer,numHold[36];long						numCount;unsigned long				bMarker;Str255						dataHolder;void						*fRefNumber,*prevfRefNumber;long						i,j,k,bmHolder,foundSpot,mark,dataSize,bSize,dbSize;unsigned long				count,wcount;FileReference				incSrc;FileReference				prevSrc;char						*tempBuff;Boolean						process=true;void						*outFRef;long						check=0;Boolean						hex;unsigned char				charLong[4];IncludeList					**currInc;long						tempDataSize;Boolean						match;long						fakeMarker;AssemblerRecord				fakeAsmRecord;AssemblyPBlock				macroAsmPB;long						macroSize;Boolean						fRead=false;long						fPos;void						*err;unsigned long				arg,offset;long						*data;long						dMark;void						*dataFRef;long 						zzz=0;Str255						tempTxt,tempTxt2,tempTxt3;long						lineNum;incSrc = FileRef_Allocate();err = FSpOpenDF(ppsPB->mainsource,fsRdPerm,&fRefNumber);if(CheckError_FSpOpenDF(err)){DebugFunction("\pFile System Error");}GetEOF(fRefNumber,&dataSize);ppsPB->totalDataSize+=dataSize;if(dataSize < BUFFER_SIZE){count = dataSize;}else{count = BUFFER_SIZE;}srcBuffer = (unsigned char*)NewPtr(count);if(!srcBuffer){DebugFunction("\pMemory Allocation Error");}bMarker = 0;FSRead(fRefNumber,&count,srcBuffer);bSize=count;lineNum=1;mark=0;bMarker=0;while(count){	#ifdef __POWERPC__	zzz++;if(!(zzz%1000)){IdleControls(GetFrontWindowOfClass(kAllWindowClasses,0));ReceiveNextEvent(0,NULL,kEventWaitTime,false,NULL);}	#endif	switch(srcBuffer[bMarker])	{		case '@':		{			i=1;			INCREMENT(bMarker)			while(!WHITE_SPACE(srcBuffer,bMarker))			{			dataHolder[i] = srcBuffer[bMarker];			i++;			INCREMENT(bMarker)			}			dataHolder[0] = i-1;			if(MyPStrCmp(dataHolder,"\pDefine"))			{										i=1;			INCREMENT(bMarker)			while(!WHITE_SPACE(srcBuffer,bMarker))			{			dataHolder[i] = srcBuffer[bMarker];			i++;			INCREMENT(bMarker)			}			dataHolder[0] = i-1;													/*				i=1;				while(!(srcBuffer[bMarker]=='"'))				{				INCREMENT(bMarker)				}				INCREMENT(bMarker)				while(!(srcBuffer[bMarker]=='"'))				{				dataHolder[i] = srcBuffer[bMarker];				i++;				INCREMENT(bMarker)				}				dataHolder[0] = i-1;				*/								(*ppsPB->currAsmMacros) = (AsmMacro*)NewPtr(sizeof(AsmMacro));				(*ppsPB->currAsmMacros)->next = nil;				(*ppsPB->currAsmMacros)->name = NewPtr(dataHolder[0]);				MyPStrAdd(dataHolder,"\p",(unsigned char*)(*ppsPB->currAsmMacros)->name);								if(srcBuffer[bMarker]=='\r'){lineNum++;}				while(!(srcBuffer[bMarker]=='{')){INCREMENT(bMarker)if(srcBuffer[bMarker]=='\r'){lineNum++;}}				INCREMENT(bMarker)if(srcBuffer[bMarker]=='\r'){lineNum++;}				INCREMENT(bMarker)if(srcBuffer[bMarker]=='\r'){lineNum++;}								fRead=false;				macroSize=0;				GetFPos(fRefNumber,&fPos);				fPos-=bSize;				fPos+=bMarker;				bmHolder = bMarker;								while(!(srcBuffer[bMarker]=='}')){INCREMENT(bMarker)if(srcBuffer[bMarker]=='\r'){macroSize++;/*lineNum++;*/}}								if(fRead)				{				SetFPos(fRefNumber,fsFromStart,fPos);				count = bSize;				FSRead(fRefNumber,&count,srcBuffer);				bSize = count;				bMarker=0;				}				else				{bMarker = bmHolder;}								fakeMarker = bMarker;				macroAsmPB.asmBuffer	= (char*)srcBuffer;				//(*macroAsmPB.asmBSize)	= bSize;								macroAsmPB.lineNum = lineNum;								macroAsmPB.currSrc = FileRef_Allocate();								FileRef_To_FileRef_Copy(ppsPB->mainsource,macroAsmPB.currSrc);								macroAsmPB.asmBSize	= &bSize;				macroAsmPB.bMarker		= (long*)&bMarker;				macroAsmPB.fRefNumber	= fRefNumber;				macroAsmPB.asmRecord	= &fakeAsmRecord;				fakeAsmRecord.instcount = 0;				fakeAsmRecord.insts = (long*)NewPtr(macroSize*sizeof(long)); //look here!												for(k=0;k<macroSize;k++)				{				Byte_DEFAULT(&macroAsmPB);				INCREMENT(bMarker)				lineNum++;				macroAsmPB.lineNum = lineNum;				}				//bSize = (*macroAsmPB.asmBSize);				(*ppsPB->currAsmMacros)->data = fakeAsmRecord.insts;				(*ppsPB->currAsmMacros)->count = fakeAsmRecord.instcount;				ppsPB->currAsmMacros = &((*ppsPB->currAsmMacros)->next);												while(!(srcBuffer[bMarker]=='}')){INCREMENT(bMarker)};INCREMENT(bMarker)											}			else if(MyPStrCmp(dataHolder,"\pSectionD"))			{			data = (long*)NewPtr(DATA_BUFFER_SIZE*sizeof(long));			dMark = 0;			dbSize = DATA_BUFFER_SIZE;			offset=0;															ppsPB->dataSection = FileRef_Allocate();						FileRef_To_FileRef_NewName(ppsPB->mainsource,"\pTEMP_DATA_SECTION",ppsPB->dataSection);									FSpCreate(ppsPB->dataSection,'Doug','RSRC',smSystemScript);			//if(err){DebugFunction("\pError Creating Temp Data File");}			FSpOpenDF(ppsPB->dataSection,fsRdWrPerm,&dataFRef);			while(srcBuffer[bMarker]!='{'){if(srcBuffer[bMarker]=='\r'){lineNum++;}INCREMENT(bMarker)}INCREMENT(bMarker)			//#include "HandleSectionD.c"			while(srcBuffer[bMarker]!='}' && count)			{				switch(srcBuffer[bMarker])				{					case '$':					{						arg=0;						INCREMENT(bMarker)						for(j=7,arg=0;j>=0;j--)						{							if(srcBuffer[bMarker]>=0x30&&srcBuffer[bMarker]<=0x39)							{							arg += ((srcBuffer[bMarker]-0x30)*(_pow(0x10,j)));							}							else							{							arg += ((srcBuffer[bMarker]-0x37)*(_pow(0x10,j)));							}							INCREMENT(bMarker)						}						data[dMark]=arg;						INCREMENT_DATA(dMark)						if(srcBuffer[bMarker]==':') //check for a data variable						{							INCREMENT(bMarker)							i=1;							while(!WHITE_SPACE(srcBuffer,bMarker))							{							dataHolder[i] = srcBuffer[bMarker];							i++;							INCREMENT(bMarker)							}							dataHolder[0] = i-1;							(*ppsPB->currDataV) = (DataVariable*)NewPtr(sizeof(DataVariable));							(*ppsPB->currDataV)->off = offset;							(*ppsPB->currDataV)->next = nil;							(*ppsPB->currDataV)->name = NewPtr(dataHolder[0]+1);							for(k=0;k<dataHolder[0];k++){(*ppsPB->currDataV)->name[k]=dataHolder[k+1];}(*ppsPB->currDataV)->name[k]=0;							ppsPB->currDataV = &(*ppsPB->currDataV)->next;						}						offset+=4;						break;					}					case '\'':					{						arg=0;						INCREMENT(bMarker)						arg+=srcBuffer[bMarker]<<24;INCREMENT(bMarker)						arg+=srcBuffer[bMarker]<<16;INCREMENT(bMarker)						arg+=srcBuffer[bMarker]<<8;INCREMENT(bMarker)						arg+=srcBuffer[bMarker];INCREMENT(bMarker)INCREMENT(bMarker) //to get past the other single quote						data[dMark]=arg;						INCREMENT_DATA(dMark)						if(srcBuffer[bMarker]==':') //check for a data variable						{							INCREMENT(bMarker)							i=1;							while(!WHITE_SPACE(srcBuffer,bMarker))							{							dataHolder[i] = srcBuffer[bMarker];							i++;							INCREMENT(bMarker)							}							dataHolder[0] = i-1;							(*ppsPB->currDataV) = (DataVariable*)NewPtr(sizeof(DataVariable));							(*ppsPB->currDataV)->off = offset;							(*ppsPB->currDataV)->next = nil;							(*ppsPB->currDataV)->name = NewPtr(dataHolder[0]+1);							for(k=0;k<dataHolder[0];k++){(*ppsPB->currDataV)->name[k]=dataHolder[k+1];}(*ppsPB->currDataV)->name[k]=0;							ppsPB->currDataV = &(*ppsPB->currDataV)->next;						}						offset+=4;						break;					}					case '+':					{						data[dMark]=0;						INCREMENT_DATA(dMark)						INCREMENT(bMarker)						i=1;						while(!WHITE_SPACE(srcBuffer,bMarker))						{						dataHolder[i] = srcBuffer[bMarker];						i++;						INCREMENT(bMarker)						}						dataHolder[0] = i-1;						(*ppsPB->currDataOff) = (DataVariable*)NewPtr(sizeof(DataVariable));						(*ppsPB->currDataOff)->off = offset;						(*ppsPB->currDataOff)->next = nil;						(*ppsPB->currDataOff)->name = NewPtr(dataHolder[0]+1);						for(k=0;k<=dataHolder[0];k++){(*ppsPB->currDataOff)->name[k]=dataHolder[k];}						ppsPB->currDataOff = &(*ppsPB->currDataOff)->next;						if(srcBuffer[bMarker]==':') //check for a data variable						{							INCREMENT(bMarker)							i=1;							while(!WHITE_SPACE(srcBuffer,bMarker))							{							dataHolder[i] = srcBuffer[bMarker];							i++;							INCREMENT(bMarker)							}							dataHolder[0] = i-1;							(*ppsPB->currDataV) = (DataVariable*)NewPtr(sizeof(DataVariable));							(*ppsPB->currDataV)->off = offset;							(*ppsPB->currDataV)->next = nil;							(*ppsPB->currDataV)->name = NewPtr(dataHolder[0]+1);							for(k=0;k<dataHolder[0];k++){(*ppsPB->currDataV)->name[k]=dataHolder[k+1];}(*ppsPB->currDataV)->name[k]=0;							ppsPB->currDataV = &(*ppsPB->currDataV)->next;						}						offset+=4;						break;					}					case '@':					{						hex = false;						INCREMENT(bMarker)						i=1;						while(!WHITE_SPACE(srcBuffer,bMarker))						{						dataHolder[i] = srcBuffer[bMarker];						i++;						INCREMENT(bMarker)						}						dataHolder[0] = i-1;						if(MyPStrCmp(dataHolder,"\pZeroByte"))						{							while(WHITE_SPACE(srcBuffer,bMarker)){INCREMENT(bMarker)}							if((srcBuffer[bMarker]-0x30)>9||(srcBuffer[bMarker]-0x30)<0)							{								if(srcBuffer[bMarker]!='$')								{								FileRef_GetFileName(ppsPB->mainsource,tempTxt3);								MyPStrAdd("\pBad argument to SectionD:ZeroByte\rSource: ",tempTxt3,tempTxt2);								MyPStrAdd(tempTxt2,"\p\rLine: ",tempTxt);								NumberToString(lineNum,(char*)tempTxt3);								MyPStrAdd(tempTxt,tempTxt3,tempTxt2);								DebugFunction(tempTxt2);								}								else{hex=true;INCREMENT(bMarker)}							}														numCount=0;							while(!WHITE_SPACE(srcBuffer,bMarker))							{							numHold[numCount] = srcBuffer[bMarker];							numCount++;							INCREMENT(bMarker)							}														arg=0;							if(hex)							{									numCount--;								for(i=0;numCount>=0;i++,numCount--)								{								if(numHold[i]>=0x30&&numHold[i]<=0x39)								{								arg += ((numHold[i]-0x30)*(_pow(0x10,numCount)));								}								else								{								arg += ((numHold[i]-0x37)*(_pow(0x10,numCount)));								}								}							}							else							{									numCount--;								for(i=0;numCount>=0;i++,numCount--)								{								arg += ((numHold[i]-0x30)*(_pow(10,numCount)));								}							}														for(j=0;j<(arg/4);j++){data[dMark]=0x00;INCREMENT_DATA(dMark)}							if(srcBuffer[bMarker]==':') //check for a data variable							{								INCREMENT(bMarker)								i=1;								while(!WHITE_SPACE(srcBuffer,bMarker))								{								dataHolder[i] = srcBuffer[bMarker];								i++;								INCREMENT(bMarker)								}								dataHolder[0] = i-1;								(*ppsPB->currDataV) = (DataVariable*)NewPtr(sizeof(DataVariable));								(*ppsPB->currDataV)->off = offset;								(*ppsPB->currDataV)->next = nil;								(*ppsPB->currDataV)->name = NewPtr(dataHolder[0]+1);								for(k=0;k<dataHolder[0];k++){(*ppsPB->currDataV)->name[k]=dataHolder[k+1];}(*ppsPB->currDataV)->name[k]=0;								ppsPB->currDataV = &(*ppsPB->currDataV)->next;							}							offset+=arg;						}						else						{						FileRef_GetFileName(ppsPB->mainsource,tempTxt3);						MyPStrAdd("\pIllegal @SectionD directive\rSource: ",tempTxt3,tempTxt2);						MyPStrAdd(tempTxt2,"\p\rLine: ",tempTxt);						NumberToString(lineNum,(char*)tempTxt3);						MyPStrAdd(tempTxt,tempTxt3,tempTxt2);						DebugFunction(tempTxt2);						}						break;					}					case '#':					{					while(srcBuffer[bMarker]!=0x0D){INCREMENT(bMarker)}					break;					}					case '\r':					{					lineNum++;					INCREMENT(bMarker)					break;					}					default:					{					FileRef_GetFileName(ppsPB->mainsource,tempTxt3);					MyPStrAdd("\pIllegal @SectionD element\rSource: ",tempTxt3,tempTxt2);					MyPStrAdd(tempTxt2,"\p\rLine: ",tempTxt);					NumberToString(lineNum,(char*)tempTxt3);					MyPStrAdd(tempTxt,tempTxt3,tempTxt2);					DebugFunction(tempTxt2);					break;					}				}			}			if(!count){DebugFunction("\pMissing A Closing Brace, @SectionD");}			wcount = (dMark*sizeof(long));			FSWrite(dataFRef,&wcount,data);			#ifdef __POWERPC__			//FlushVol(0,((FSSpec*)ppsPB->mainsource)->vRefNum);			#endif			GetFPos(dataFRef,&fPos);			SetEOF(dataFRef,fPos);			FSClose(dataFRef);			DisposePtr((char*)data);			INCREMENT(bMarker)			}			else if(MyPStrCmp(dataHolder,"\pInclude"))			{				ppsPB->includeCount++;				i=1;				while(!(srcBuffer[bMarker]=='"'))				{				INCREMENT(bMarker)				}				INCREMENT(bMarker)				while(!(srcBuffer[bMarker]=='"'))				{				dataHolder[i] = srcBuffer[bMarker];				i++;				INCREMENT(bMarker)				}				dataHolder[0] = i-1;												prevSrc = ppsPB->mainsource;								FileRef_To_FileRef_NewName(ppsPB->mainsource,dataHolder,incSrc);										match = false;				currInc = &ppsPB->includedFiles;				while(*currInc)				{				FileRef_GetFileName(incSrc,gTempTxt);				match = MyPStrCmp(gTempTxt,(*currInc)->name);				if(match){break;}				currInc = &((*currInc)->next);				}								if(!match)				{				*ppsPB->currincludedFiles = (IncludeList*)NewPtr(sizeof(IncludeList));				FileRef_GetFileName(incSrc,gTempTxt);				MyPStrAdd(gTempTxt,"\p",(*ppsPB->currincludedFiles)->name);				(*ppsPB->currincludedFiles)->next=nil;				ppsPB->currincludedFiles = &((*ppsPB->currincludedFiles)->next);				ppsPB->mainsource = incSrc;				PreProcessSource(ppsPB);				ppsPB->mainsource = prevSrc;				INCREMENT(bMarker)				}				else				{				FileRef_GetFileName(&incSrc,tempTxt);				MyPStrAdd("\pDuplicate #Include file: ",tempTxt,dataHolder);				DebugFunction(dataHolder);				}			}			else if(MyPStrCmp(dataHolder,"\pImport")||MyPStrCmp(dataHolder,"\p_Import"))			{			while(!(srcBuffer[bMarker]=='}'))			{			if(srcBuffer[bMarker]=='\r'){lineNum++;}			INCREMENT(bMarker)			}			INCREMENT(bMarker)			}			else if(MyPStrCmp(dataHolder,"\pdata"))			{			/*			while(srcBuffer[bMarker]!='"')			{INCREMENT(bMarker)}			INCREMENT(bMarker)			while(srcBuffer[bMarker]!='"')			{INCREMENT(bMarker)}			INCREMENT(bMarker)			*/			while(srcBuffer[bMarker]!='#' && srcBuffer[bMarker]!='\r')			{if(srcBuffer[bMarker]=='\r'){lineNum++;};INCREMENT(bMarker)}						}			else if(MyPStrCmp(dataHolder,"\pExport"))			{			while(srcBuffer[bMarker]!='#' && srcBuffer[bMarker]!='\r')			{if(srcBuffer[bMarker]=='\r'){lineNum++;};INCREMENT(bMarker)}			}			else if(MyPStrCmp(dataHolder,"\pmain"))			{			while(srcBuffer[bMarker]!='#' && srcBuffer[bMarker]!='\r')			{if(srcBuffer[bMarker]=='\r'){lineNum++;}INCREMENT(bMarker)}			}			else if(MyPStrCmp(dataHolder,"\pinit"))			{			while(srcBuffer[bMarker]!='#' && srcBuffer[bMarker]!='\r')			{if(srcBuffer[bMarker]=='\r'){lineNum++;}INCREMENT(bMarker)}			}			else if(MyPStrCmp(dataHolder,"\pterm"))			{			while(srcBuffer[bMarker]!='#' && srcBuffer[bMarker]!='\r')			{if(srcBuffer[bMarker]=='\r'){lineNum++;}INCREMENT(bMarker)}			}			else			{			FileRef_GetFileName(ppsPB->mainsource,tempTxt3);			MyPStrAdd("\pAn illegal directive was found in your source\rSource: ",tempTxt3,tempTxt2);			MyPStrAdd(tempTxt2,"\p\rLine: ",tempTxt);			NumberToString(lineNum,(char*)tempTxt3);			MyPStrAdd(tempTxt,tempTxt3,tempTxt2);			DebugFunction(tempTxt2);			}			break;		}		case '_':		case '#':		{		while(srcBuffer[bMarker]!=0x0D){INCREMENT(bMarker)}		break;		}		case ':':		{		while(srcBuffer[bMarker]!='#' && srcBuffer[bMarker]!='\r')		{INCREMENT(bMarker)}		break;		}		case ' ' :		case '\t':{INCREMENT(bMarker);break;}		case '\n':		case '\r':{lineNum++;INCREMENT(bMarker);break;}		default:		{		while(srcBuffer[bMarker]!='#' && srcBuffer[bMarker]!='\r')		{INCREMENT(bMarker)}		break;		}	}}FSClose(fRefNumber);DisposePtr((char*)srcBuffer);}