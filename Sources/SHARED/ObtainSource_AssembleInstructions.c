Copyright 2016 Gregory BryantLicensed under the Apache License, Version 2.0 (the "License");you may not use this file except in compliance with the License.You may obtain a copy of the License at    http://www.apache.org/licenses/LICENSE-2.0Unless required by applicable law or agreed to in writing, softwaredistributed under the License is distributed on an "AS IS" BASIS,WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.See the License for the specific language governing permissions andlimitations under the License./***********************************************************************/#include "ProjectHeaders.h"#define INCREMENT(X)								\X++;												\gSrcPosAccum++;										\if(X==bSize)										\{													\fRead=true;											\asmPB.sCount = bSize;								\FSRead(asmPB.fRefNumber,&asmPB.sCount,srcBuffer);	\bSize = asmPB.sCount;								\X=0;												\if(!asmPB.sCount){break;}							\}#define WHITE_SPACE(X,Y)	\(							\(X[Y]=='\t')				\||							\(X[Y]=='\r')				\||							\(X[Y]=='\n')				\||							\(X[Y]==' ')					\||							\(X[Y]=='#')					\||							\(X[Y]==',')					\||							\(X[Y]=='|')					\||							\(X[Y]==']')					\||							\(X[Y]==':')					\)void ObtainSource_AssembleInstructions(AssemblerRecord	*asmRecord){Boolean						fRead=false,ran;long						dataSize,totalIncludedDataSize,bSize;long						n;long						prevVal;long						i,bMarker,bmHolder,tempDataSize,j,k;Boolean						match,error;LabelEntry					*firstLabelEntry;BranchEntry					*firstBranchEntry;void 						*err;AssemblyPBlock				asmPB;LabelEntry					**currLabel;BranchEntry					**currBranch,*temp,*cb;Str255						dataHolder,txt,errorMsg,tempTxt,tempTxt2,tempTxt3;short						tempREF;long						tempDataSize2;Boolean						boolVal;KeyMap						keyMap;LabelList					*topMacroCalls,**currMacroCalls;DataVariable				*dataV,*tempGarbage,*currDataV;long						templong;unsigned long				count,tempcount;long						fPos;unsigned char				*srcBuffer;long 						branchPCount;long						totalLabelByteSize,lbPos,labelCount;char						*labelBlock=0;long						expSectionLength;long						hashTablePower;PEFExportedSymbolHashSlot	*exportHashTable=0;PEFExportedSymbolKey		*exportKeyTable=0;PEFExportedSymbol			*exportSymbolTable=0;long						eKeyTableIndex,eSymTableIndex;unsigned long				fullHashWord,hashTableIndex,prevfullHashWord;HashRecord					*hashTableRecord=0;long						*nameOffset;long						nOffPos;long						currBlockAssigner;PreProcessSourcePBlock		ppsPB;IncludeList					*tempd,*currInc;AsmMacro					*currAsmMacros,*tempAM;WriteStatus("Preprocessing...");asmPB.currImpRec = nil;asmRecord->import.prev=nil;asmRecord->import.next=nil;asmRecord->import.lib=nil;asmPB.currExpRec = nil;asmRecord->exports.prev=nil;asmRecord->exports.next=nil;asmRecord->exports.label=nil;gTotalBranchCount=0;ppsPB.mainsource 		= asmRecord->key->asm_;ppsPB.asmMacros			= 0;ppsPB.currAsmMacros 	= &ppsPB.asmMacros;ppsPB.asmMacros 		= nil;ppsPB.currincludedFiles	= &ppsPB.includedFiles;ppsPB.includedFiles		= nil;ppsPB.dataSection		= nil;ppsPB.dataV = nil;ppsPB.currDataV = &ppsPB.dataV;ppsPB.currDataOff = &gDataOffsets;ppsPB.key = asmRecord->key;*ppsPB.currincludedFiles = (IncludeList*)NewPtr(sizeof(IncludeList));FileRef_GetFileName(ppsPB.mainsource,(*ppsPB.currincludedFiles)->name);(*ppsPB.currincludedFiles)->next=nil;ppsPB.currincludedFiles = &(*ppsPB.currincludedFiles)->next;ppsPB.totalDataSize=0;ppsPB.includeCount=0;DT_SetProgressState(gAppControls.progressBar,kControlProgressBarIndeterminateTag,true);PreProcessSource(&ppsPB);currInc = ppsPB.includedFiles;while(currInc){tempd = currInc->next;DisposePtr((char*)currInc);currInc = tempd;}dataV = ppsPB.dataV;asmRecord->dataSection = ppsPB.dataSection; asmPB.isRecord = (IncludeSaveRecord*)NewPtr(sizeof(IncludeSaveRecord)*ppsPB.includeCount);for(i=0;i<ppsPB.includeCount;i++){asmPB.isRecord[i].file = FileRef_Allocate();}asmPB.currSrc = FileRef_Allocate();FileRef_To_FileRef_Copy(asmRecord->key->asm_,asmPB.currSrc);err = FSpOpenDF(asmRecord->key->asm_,fsRdPerm,&asmPB.fRefNumber);if(CheckError_FSpOpenDF(err)){DebugFunction("\pFile System Error");}GetEOF(asmPB.fRefNumber,&dataSize);/*if(dataSize < BUFFER_SIZE){asmPB.sCount = dataSize;}else{asmPB.sCount = BUFFER_SIZE;}еееееееееееее*/asmPB.sCount = BUFFER_SIZE;srcBuffer = (unsigned char*)NewPtr(asmPB.sCount);if(!srcBuffer){DebugFunction("\pMemory Allocation Error");}bMarker = 0;FSRead(asmPB.fRefNumber,&asmPB.sCount,srcBuffer);bSize=asmPB.sCount;asmRecord->insts = (long*)NewPtr(DATA_BUFFER_SIZE*sizeof(long));asmPB.codeOutSize = DATA_BUFFER_SIZE;asmPB.codeOutFSS = FileRef_Allocate();FileRef_To_FileRef_NewName(asmRecord->key->asm_,"\pTEMP_CODE_SECTION",asmPB.codeOutFSS);FSpCreate(asmPB.codeOutFSS,'Doug','RSRC',smSystemScript);//if(err){DebugFunction("\pError Creating Temp Data File");}FSpOpenDF(asmPB.codeOutFSS,fsRdWrPerm,&asmPB.codeOutRef);asmRecord->instcount	= 0;asmRecord->instaccum	= 0;bMarker 				= 0;error					= false;asmPB.bMarker			= (long*)&bMarker;asmPB.asmBuffer			= (char*)srcBuffer;asmPB.asmRecord 		= asmRecord;asmPB.labels			= nil;bMarker 				= 0;currLabel 				= &asmPB.labels;asmPB.branches			= nil;asmPB.currBranch 		= &asmPB.branches;asmPB.macro 			= ppsPB.asmMacros;;asmPB.dataV				= dataV;asmPB.asmBSize			= &bSize;DT_SetProgressState(gAppControls.progressBar,kControlProgressBarIndeterminateTag,false);SetControl32BitMaximum(gAppControls.progressBar,(ppsPB.totalDataSize/10000));//SendMessage(gAppControls.progressBar,PBM_SETSTEP,1,0);WriteStatus("Assembling...");totalLabelByteSize=0;labelCount=0;prevVal=0;asmPB.lineNum = 1;gSrcPosAccum=0;bMarker=0;asmPB.depth=0;ran=false;while(asmPB.depth>=0){	if(asmPB.depth || ran)	{	asmPB.lineNum = asmPB.isRecord[asmPB.depth].lineNum;				FileRef_To_FileRef_Copy(asmPB.isRecord[asmPB.depth].file,asmPB.currSrc);	err = FSpOpenDF(asmPB.currSrc,fsRdPerm,&asmPB.fRefNumber);	if(CheckError_FSpOpenDF(err)){DebugFunction("\pFile System Error");}			SetFPos(asmPB.fRefNumber,fsFromStart,asmPB.isRecord[asmPB.depth].pos);	//asmPB.sCount = (*asmPB.asmBSize);	asmPB.sCount=BUFFER_SIZE;	FSRead(asmPB.fRefNumber,&asmPB.sCount,asmPB.asmBuffer);	(*asmPB.asmBSize) = asmPB.sCount;	(*asmPB.bMarker)=0;	}	while(asmPB.sCount)	{		#ifdef __POWERPC__		GetKeys(keyMap);		if(keyMap[1] == 0x00808000){gAborted=true;goto abort;}		SetControl32BitValue(gAppControls.progressBar,(gSrcPosAccum/10000));		if(prevVal!=gSrcPosAccum/10000)		{		ReceiveNextEvent(0,NULL,kEventWaitTime,false,NULL);		prevVal=gSrcPosAccum/10000;		}		#endif				#ifdef __INTEL__		//SendMessage(gAppControls.progressBar,PBM_SETPOS,(gSrcPosAccum/10000),0);		#endif				switch(srcBuffer[bMarker])		{		case ' ' :		case '\t':		case '\n':		case '\r':asmPB.lineNum++;INCREMENT(bMarker);break;		case '#':while(srcBuffer[bMarker]!=0x0D){INCREMENT(bMarker)}break;		case ':':				{					i=1;					INCREMENT(bMarker)					while(!WHITE_SPACE(srcBuffer,bMarker))					{					dataHolder[i] = srcBuffer[bMarker];					i++;					INCREMENT(bMarker)					}					dataHolder[0] = i-1;					(*currLabel)=(LabelEntry*)NewPtr(sizeof(LabelEntry));					totalLabelByteSize+=(dataHolder[0]+1);					(*currLabel)->name = NewPtr(dataHolder[0]+1);					MyPStrAdd(dataHolder,"\p",(unsigned char*)(*currLabel)->name);					(*currLabel)->location = asmPB.asmRecord->instcount+asmPB.asmRecord->instaccum;					(*currLabel)->next = nil;					currLabel = &((*currLabel)->next);					labelCount++;				break;				}		case '@':Byte_ATSYMBOL(&asmPB);break;		case '_':Byte_MACRO(&asmPB);break;		default: Byte_DEFAULT(&asmPB);break;		}	}	asmPB.depth--;	ran=true;}			tempcount=(asmPB.asmRecord->instcount*sizeof(long));#ifdef __INTEL__for(i=0;i<asmPB.asmRecord->instcount;i++){Swap_LONG(&asmPB.asmRecord->insts[i]);}#endifFSWrite(asmPB.codeOutRef,&tempcount,asmPB.asmRecord->insts);GetFPos(asmPB.codeOutRef,&fPos);SetEOF(asmPB.codeOutRef,fPos);FSClose(asmPB.codeOutRef);#ifdef __POWERPC__//FlushVol(0,((FSSpec*)asmRecord->codeSection)->vRefNum);#endifDisposePtr((char*)asmPB.isRecord);currDataV = dataV;while(currDataV){tempGarbage = currDataV;currDataV = currDataV->next;DisposePtr(tempGarbage->name);DisposePtr((char*)tempGarbage);}SetControl32BitValue(gAppControls.progressBar,0);SetControl32BitMaximum(gAppControls.progressBar,gTotalBranchCount/100);WriteStatus("Writing Branches...");#ifdef __POWERPC__DrawControls(gAppWindow);ReceiveNextEvent(0,NULL,kEventWaitTime,false,NULL);#endiflabelBlock = NewPtr(totalLabelByteSize);lbPos = 0;nameOffset = (long*)NewPtr(sizeof(long)*(labelCount+2));nOffPos=0;nameOffset[nOffPos]=0;nOffPos++;currLabel = &asmPB.labels;while(*currLabel){nameOffset[nOffPos]=nameOffset[nOffPos-1]+(*currLabel)->name[0]+1;nOffPos++;MyPStrAdd( (unsigned char*)(*currLabel)->name,"\p",(unsigned char*)(labelBlock+lbPos) );lbPos+=(*currLabel)->name[0]+1;currLabel = &((*currLabel)->next);}if(labelCount>=kPEFHashSlotMaxKeyIndex){DebugFunction("\pError: Limit: JKDY Reached, Please Report");}nOffPos=0;hashTablePower=PEFComputeHashTableExponent(labelCount);exportKeyTable = (PEFExportedSymbolKey*)NewPtr(sizeof(PEFExportedSymbolKey)*labelCount);exportSymbolTable = (PEFExportedSymbol*)NewPtr(sizeof(PEFExportedSymbol)*labelCount);exportHashTable = (PEFExportedSymbolHashSlot*)NewPtr(sizeof(PEFExportedSymbolHashSlot)*_pow(2,hashTablePower));for(i=0;i<_pow(2,hashTablePower);i++){exportHashTable[i].countAndStart=0;}eKeyTableIndex=0;hashTableRecord = (HashRecord*)NewPtr(sizeof(HashRecord)*_pow(2,hashTablePower));;for(i=0;i<_pow(2,hashTablePower);i++){hashTableRecord[i].assigned=0;hashTableRecord[i].marker=0;hashTableRecord[i].count=0;}currLabel = &asmPB.labels;while(*currLabel){fullHashWord	= PEFComputeHashWord((unsigned char*)((*currLabel)->name+1),(unsigned long)(*currLabel)->name[0]);hashTableIndex	= PEFHashTableIndex(fullHashWord,hashTablePower);hashTableRecord[hashTableIndex].count++;if(hashTableRecord[hashTableIndex].count>=kPEFHashSlotMaxSymbolCount){DebugFunction("\pError: Limit: DHUE Reached, Please Report");}currLabel = &((*currLabel)->next);}currBlockAssigner=0;currLabel = &asmPB.labels;while(*currLabel){fullHashWord	= PEFComputeHashWord((unsigned char*)((*currLabel)->name+1),(unsigned long)(*currLabel)->name[0]);hashTableIndex	= PEFHashTableIndex(fullHashWord,hashTablePower);if(!hashTableRecord[hashTableIndex].assigned){hashTableRecord[hashTableIndex].marker=currBlockAssigner;exportHashTable[hashTableIndex].countAndStart = PEFComposeExportedSymbolHashSlot(1,currBlockAssigner);currBlockAssigner+=hashTableRecord[hashTableIndex].count;hashTableRecord[hashTableIndex].assigned=true;}else{exportHashTable[hashTableIndex].countAndStart += 1<<kPEFHashSlotSymCountShift;}exportKeyTable[hashTableRecord[hashTableIndex].marker].u.fullHashWord = fullHashWord;exportSymbolTable[hashTableRecord[hashTableIndex].marker].classAndName	= nameOffset[nOffPos];exportSymbolTable[hashTableRecord[hashTableIndex].marker].symbolValue	= (*currLabel)->location;hashTableRecord[hashTableIndex].marker++;nOffPos++;currLabel = &((*currLabel)->next);}DisposePtr((char*)nameOffset);prevVal=0;branchPCount=0;gLabelOffsets = asmPB.labels;if(asmPB.branches){	FSpOpenDF(asmPB.codeOutFSS,fsRdWrPerm,&asmPB.codeOutRef);	currBranch = &asmPB.branches;	while(*currBranch)	{		branchPCount++;				#ifdef __POWERPC__		GetKeys(keyMap);		if(keyMap[1] == 0x00808000){gAborted=true;goto abort;}				SetControl32BitValue(gAppControls.progressBar,branchPCount/100);		if(prevVal!=branchPCount/100)		{		ReceiveNextEvent(0,NULL,kEventWaitTime,false,NULL);		prevVal=branchPCount/100;		}		#endif				#ifdef __INTEL__		//SendMessage(gAppControls.progressBar,PBM_SETPOS,branchPCount/100,0);		#endif				fullHashWord	= PEFComputeHashWord((unsigned char*)((*currBranch)->name+1),(unsigned long)(*currBranch)->name[0]);		hashTableIndex	= PEFHashTableIndex(fullHashWord,hashTablePower);				match=false;		j=PEFHashSlotFirstKey(exportHashTable[hashTableIndex].countAndStart);		for(i=0;i<PEFHashSlotSymbolCount(exportHashTable[hashTableIndex].countAndStart);i++)		{			if(exportKeyTable[j].u.fullHashWord==fullHashWord)			{match=true;break;}			j++;		}		if(match)		{			match=false;			for(;i<PEFHashSlotSymbolCount(exportHashTable[hashTableIndex].countAndStart);i++)			{				if(match=MyPStrCmp( (unsigned char*)(*currBranch)->name,(unsigned char*)labelBlock+exportSymbolTable[j].classAndName))				{					SetFPos(asmPB.codeOutRef,fsFromStart,(*currBranch)->location*4);					count=sizeof(long);					FSRead(asmPB.codeOutRef,&count,&templong);					#ifdef __INTEL__					Swap_LONG(&templong);					#endif					templong+=((exportSymbolTable[j].symbolValue-(*currBranch)->location)&(*currBranch)->mask)<<2;					SetFPos(asmPB.codeOutRef,fsFromStart,(*currBranch)->location*4);					count=sizeof(long);					#ifdef __INTEL__					Swap_LONG(&templong);					#endif					FSWrite(asmPB.codeOutRef,&count,&templong);					break;				}				j++;			}						if(!match)			{									MyPStrAdd((unsigned char*)"\pBranch To Missing Label: ",(unsigned char*)(*currBranch)->name,(unsigned char*)errorMsg);				DebugFunction(errorMsg);							/*				MyPStrAdd((unsigned char*)"\pBranch To Missing Label: ",(unsigned char*)(*currBranch)->name,(unsigned char*)errorMsg);												MyPStrAdd("\pBranch To Missing Label: ",asmPB->currSrc.name,tempTxt);				MyPStrAdd(tempTxt,"\p\rSource: ",tempTxt2);								MyPStrAdd("\pBranch To Missing Label:\rSource: ",asmPB->currSrc.name,tempTxt2);				MyPStrAdd(tempTxt2,"\p\rLine: ",tempTxt);				NumberToString(asmPB->lineNum,(char*)tempTxt3);				MyPStrAdd(tempTxt,tempTxt3,tempTxt2);				DebugFunction(tempTxt2);				*/			}		}		else		{			MyPStrAdd((unsigned char*)"\pBranch To Missing Label: ",(unsigned char*)(*currBranch)->name,(unsigned char*)errorMsg);			DebugFunction(errorMsg);		}			currBranch = &((*currBranch)->next);	}FSClose(asmPB.codeOutRef);}asmRecord->codeSection = asmPB.codeOutFSS;abort:;if(hashTableRecord){DisposePtr((char*)hashTableRecord);}if(exportHashTable){DisposePtr((char*)exportHashTable);}if(exportSymbolTable){DisposePtr((char*)exportSymbolTable);}if(exportKeyTable){DisposePtr((char*)exportKeyTable);}if(labelBlock){DisposePtr(labelBlock);}if(asmPB.branches){	cb = asmPB.branches;	while(cb)	{	temp = cb->next;	DisposePtr(cb->name);	DisposePtr((char*)cb);	cb = temp;	}}DisposePtr((char*)srcBuffer);if(asmPB.macro){	currAsmMacros=asmPB.macro;	while(currAsmMacros)	{	tempAM = currAsmMacros->next;	DisposePtr(currAsmMacros->name);	DisposePtr((char*)currAsmMacros->data);	DisposePtr((char*)currAsmMacros);	currAsmMacros = tempAM;	}}asmRecord->instaccum+=asmRecord->instcount;DT_SetProgressState(gAppControls.progressBar,kControlProgressBarIndeterminateTag,true);#ifdef __INTEL__//ShowWindow(gAppControls.progressBar,SW_HIDE);ShowWindow(gAppTopText,SW_HIDE);#endif//DisposePtr(asmBuffer);}void ObtainSource_Byte_ILLEGAL(AssemblyPBlock* asmPB){FileRef_GetFileName(asmPB->currSrc,gTempTxt3);																			MyPStrAdd("\pAn illegal byte was found in your source\rSource: ",gTempTxt3,gTempTxt2);	MyPStrAdd(gTempTxt2,"\p\rLine: ",gTempTxt);																NumberToString(asmPB->lineNum,(char*)gTempTxt3);														MyPStrAdd(gTempTxt,gTempTxt3,gTempTxt2);																DebugFunction(gTempTxt2);}