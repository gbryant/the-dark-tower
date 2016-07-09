Copyright 2016 Gregory BryantLicensed under the Apache License, Version 2.0 (the "License");you may not use this file except in compliance with the License.You may obtain a copy of the License at    http://www.apache.org/licenses/LICENSE-2.0Unless required by applicable law or agreed to in writing, softwaredistributed under the License is distributed on an "AS IS" BASIS,WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.See the License for the specific language governing permissions andlimitations under the License./***********************************************************************/#include "ProjectHeaders.h"#define	LOADER_SECTION_OFFSET_M 	(												\									sizeof(PEFContainerHeader)					+	\									(sizeof(PEFSectionHeader)*3)				+	\									sectionNameTableLength							\									)									#define RELOC_INST_OFFSET_M 	(												\								sizeof(PEFLoaderInfoHeader)					+	\								(sizeof(PEFImportedLibrary)*libCount)		+	\								(sizeof(PEFImportedSymbol)*symCount)		+	\								sizeof(PEFLoaderRelocationHeader)			 	\								)								#define LOADER_STRINGS_OFFSET_PRE_M (												\									sizeof(PEFLoaderInfoHeader)					+	\									(sizeof(PEFImportedLibrary)*libCount)		+	\									(sizeof(PEFImportedSymbol)*symCount)		+	\									sizeof(PEFLoaderRelocationHeader)			+	\									relocSize/*reloc insts*/					 	\									)#define LOADER_STRINGS_OFFSET_M 	(											\									LOADER_STRINGS_OFFSET_PRE_M			+		\									(4 - (LOADER_STRINGS_OFFSET_PRE_M % 4))		\									)																		#define EXPORT_HASH_OFFSET_M 	(												\								sizeof(PEFLoaderInfoHeader)					+	\								(sizeof(PEFImportedLibrary)*libCount)		+	\								(sizeof(PEFImportedSymbol)*symCount)		+	\								sizeof(PEFLoaderRelocationHeader)			+	\								relocSize/*reloc insts*/					+	\								(4 - (LOADER_STRINGS_OFFSET_PRE_M % 4))		+	\								loaderNameTableLength						 	\								)																									#define	LOADER_SECTION_LENGTH_M 	(												\									sizeof(PEFLoaderInfoHeader)					+	\									(sizeof(PEFImportedLibrary)*libCount)		+	\									(sizeof(PEFImportedSymbol)*symCount)		+	\									sizeof(PEFLoaderRelocationHeader)			+	\									relocSize/*reloc insts*/					+	\									(4 - (LOADER_STRINGS_OFFSET_PRE_M % 4))		+	\									loaderNameTableLength						+	\									expSectionLength								\									)								#define CODE_SECTION_OFFSET_PRE_M 	(											\								sizeof(PEFContainerHeader)					+	\								(sizeof(PEFSectionHeader)*3)				+	\								4/*section name table*/						+	\								sizeof(PEFLoaderInfoHeader)					+	\								(sizeof(PEFImportedLibrary)*libCount)		+	\								(sizeof(PEFImportedSymbol)*symCount)		+	\								sizeof(PEFLoaderRelocationHeader)			+	\								relocSize/*reloc insts*/					+	\								(4 - (LOADER_STRINGS_OFFSET_PRE_M % 4))		+	\								loaderNameTableLength						+	\								expSectionLength								\								)								#define CODE_SECTION_OFFSET_M 	(											\								CODE_SECTION_OFFSET_PRE_M				+	\								(4 - (CODE_SECTION_OFFSET_PRE_M % 4))		\								)void MakeApp(AssemblerRecord	*asmRecord){long						*tempLPtr;FileReference				newAppFSS;void						*tempRef;PEFContainerHeader			containerHeader;PEFSectionHeader			codeSectionHeader;PEFSectionHeader			dataSectionHeader;PEFSectionHeader			loaderSectionHeader;PEFLoaderInfoHeader			loaderInfoHeader;PEFImportedLibrary			*importedLibrary;PEFImportedSymbol			*importedSymbol;PEFLoaderRelocationHeader	*loaderRelocationHeader;char*						sectionNameTable;char*						loaderNameTable;long						sectionNameTableLength;long						loaderNameTableLength;long						i,j,k,data;long						relocSize;short						*relocSection;long 						libCount,symCount,expCount;long						mainSection,mainOffset;ImportRecord				*currImpRec,*tempImpRec;ExportRecord				*currExpRec;long						*nameOffset;long						nOffPos,symIndex,totalWritten;unsigned long				count;void						*fRefNumber;short						bitLength;long						dataContainerLength;long						CODE_SECTION_OFFSET;long						LOADER_SECTION_LENGTH;long						RELOC_INST_OFFSET;long						LOADER_STRINGS_OFFSET;long						EXPORT_HASH_OFFSET;long						LOADER_SECTION_OFFSET;char						alignCount;long						expSectionLength;long						hashTablePower;PEFExportedSymbolHashSlot	*exportHashTable;PEFExportedSymbolKey		*exportKeyTable;PEFExportedSymbol			*exportSymbolTable=0;long						eKeyTableIndex,eSymTableIndex;unsigned long				fullHashWord,hashTableIndex;char						byte;char						*dataArgumentArray=nil;DataExport					*currDEB;DataVariable				*currDataOffset;LabelEntry					*currLabelOffset;Boolean						found;void						*err;long						tempLong;DataVariable				*tempGarbage;LabelEntry					*tempDispose;HashRecord					*hashTableRecord;long						currBlockAssigner;MacBinaryIIIHeader 			mbin3Header,mbin3ResourceHeader;void						*mbinResRef;unsigned long				dForkLength;char						*resData;Str255						tnme;newAppFSS = FileRef_Allocate();if(asmRecord->key->rsrcmb){MyPStrAdd(asmRecord->key->target,"\p.bin",tnme);FileRef_To_FileRef_InDirectory(asmRecord->parSpec,tnme,newAppFSS);FSpCreate(newAppFSS,'SITx','BINA',smSystemScript);}else{FileRef_To_FileRef_InDirectory(asmRecord->parSpec,asmRecord->key->target,newAppFSS);FSpCreate(newAppFSS,asmRecord->key->creator,asmRecord->key->type,smSystemScript);}if(asmRecord->key->rsrc){#ifdef __POWERPC__FSpCreateResFile((FSSpec*)newAppFSS,asmRecord->key->creator,asmRecord->key->type,smSystemScript);CopyResources(asmRecord->key->rsrc,newAppFSS);#endif}if(asmRecord->parSpec){DisposePtr((char*)asmRecord->parSpec);}currImpRec=&asmRecord->import;loaderNameTableLength=0;libCount=0;symCount=0;if(currImpRec->lib){while(currImpRec){	libCount++;	symCount += currImpRec->symcount;	for(i=0;currImpRec->lib[i]!=0;i++)	{loaderNameTableLength++;}	loaderNameTableLength++;	for(i=0,j=0;i<currImpRec->symcount;i++)	{		for(;currImpRec->sym[j]!=0;j++)		{loaderNameTableLength++;}		loaderNameTableLength++;		j++;	}	currImpRec = currImpRec->next;}}currExpRec=&asmRecord->exports;expCount=0;if(currExpRec->label){	while(currExpRec)	{		expCount++;		for(i=0;currExpRec->label[i]!=0;i++)		{loaderNameTableLength++;}		loaderNameTableLength++;		currExpRec = currExpRec->next;	}}// do data export counting here!if(asmRecord->key->deb){	currDEB = &asmRecord->key->de;	while(currDEB)	{	expCount++;	//i=0;	//while(currDEB->label[i]!=0){i++;}	//loaderNameTableLength+=(i+1);	loaderNameTableLength+=currDEB->label[0]+1;	currDEB = currDEB->next;	}}if(!loaderNameTableLength){loaderNameTableLength=4;loaderNameTable = NewPtr(loaderNameTableLength);loaderNameTable[0]=0;loaderNameTable[1]=0;loaderNameTable[2]=0;loaderNameTable[3]=0;}else{loaderNameTable = NewPtr(loaderNameTableLength);}k=0;currImpRec=&asmRecord->import;if(currImpRec->lib){while(currImpRec){	for(i=0;currImpRec->lib[i]!=0;i++,k++)	{	loaderNameTable[k] = currImpRec->lib[i];	}loaderNameTable[k] = currImpRec->lib[i];k++;		for(i=0,j=0;i<currImpRec->symcount;i++)	{		for(;currImpRec->sym[j]!=0;j++,k++)		{		loaderNameTable[k] = currImpRec->sym[j];		}loaderNameTable[k] = currImpRec->sym[j];k++;		j++;	}	currImpRec = currImpRec->next;}}currExpRec=&asmRecord->exports;if(currExpRec->label){while(currExpRec){	for(i=0;currExpRec->label[i]!=0;i++,k++)	{	loaderNameTable[k] = currExpRec->label[i];	}loaderNameTable[k] = currExpRec->label[i];k++;	currExpRec = currExpRec->next;}}if(asmRecord->key->deb){	currDEB = &asmRecord->key->de;	while(currDEB)	{		for(i=0;i<currDEB->label[0];i++)		{		loaderNameTable[k] = currDEB->label[i+1];k++;		}loaderNameTable[k]=0;k++;		/*		for(i=0;currDEB->label[i]!=0;i++,k++)		{		loaderNameTable[k] = currDEB->label[i];		}loaderNameTable[k] = currDEB->label[i];k++;		*/		currDEB = currDEB->next;	}}k=(libCount+symCount+expCount+1);nameOffset = (long*)NewPtr(sizeof(long)*(libCount+symCount+expCount+2));if(nameOffset){nameOffset[0]=0;for(i=0,nOffPos=1;i<loaderNameTableLength;){j=i;while(loaderNameTable[i]!=0){i++;}nameOffset[nOffPos]=(i-j+1)+nameOffset[nOffPos-1];nOffPos++;i++;}}	loaderInfoHeader.mainOffset = 0;	loaderInfoHeader.initOffset = 0;	loaderInfoHeader.termOffset = 0;if(asmRecord->key->secoffb || asmRecord->key->secoffob){	if(asmRecord->key->secoffb)	{	loaderInfoHeader.mainOffset = asmRecord->key->mainOffset;	loaderInfoHeader.initOffset = asmRecord->key->initOffset;	loaderInfoHeader.termOffset = asmRecord->key->termOffset;	}	else	{	FSpOpenDF(asmRecord->key->secoff,fsRdPerm,&tempRef);	tempLPtr=(long*)NewPtr(12);	count=12;	FSRead(tempRef,&count,tempLPtr);	FSClose(tempRef);	loaderInfoHeader.mainOffset = tempLPtr[0];	loaderInfoHeader.initOffset = tempLPtr[1];	loaderInfoHeader.termOffset = tempLPtr[2];	DisposePtr((char*)tempLPtr);	}}else{	if(asmRecord->main>=0)	{		loaderInfoHeader.mainOffset = (4*symCount);		if(asmRecord->init>=0)		{			loaderInfoHeader.initOffset	= (4*symCount)+8;			if(asmRecord->term>=0)			{				loaderInfoHeader.termOffset	= (4*symCount)+16;			}		}		else if(asmRecord->term>=0)		{			loaderInfoHeader.termOffset	= (4*symCount)+8;		}	}	else if(asmRecord->init>=0)	{		loaderInfoHeader.initOffset	= (4*symCount);		if(asmRecord->term>=0)		{			loaderInfoHeader.termOffset	= (4*symCount)+8;		}	}	else if(asmRecord->term>=0)	{		loaderInfoHeader.termOffset	= (4*symCount);	}}if(asmRecord->key->datab){FSpOpenDF(asmRecord->key->data,fsRdPerm,&tempRef);GetEOF(tempRef,&dataContainerLength);FSClose(tempRef);}else if(asmRecord->dataSection){	err = FSpOpenDF(asmRecord->dataSection,fsRdWrPerm,&tempRef);	if(CheckError_FSpOpenDF(err)){DebugFunction("\pFile System Error");}		GetEOF(tempRef,&dataContainerLength);	if(gDataOffsets)	{		currDataOffset = gDataOffsets;		while(currDataOffset)		{			currLabelOffset = gLabelOffsets;			while(currLabelOffset)			{			if(found=MyPStrCmp((unsigned char*)currDataOffset->name,(unsigned char*)currLabelOffset->name)){break;}			currLabelOffset = currLabelOffset->next;			}			if(found)			{			SetFPos(tempRef,fsFromStart,currDataOffset->off);			count=4;			data=currLabelOffset->location*4;			#ifdef __INTEL__			Swap_LONG(&data);			#endif			FSWrite(tempRef,&count,&data);			//((long*)dataArgumentArray)[currDataOffset->off/4] = currLabelOffset->location*4;			}			else			{				if(MyPStrCmp((unsigned char*)currDataOffset->name,"\pmain"))				{				SetFPos(tempRef,fsFromStart,currDataOffset->off);				count=4;				#ifdef __INTEL__				Swap_LONG(&asmRecord->main);				#endif				FSWrite(tempRef,&count,&asmRecord->main);				//((long*)dataArgumentArray)[currDataOffset->off/4] = asmRecord->main;				}				else if(MyPStrCmp((unsigned char*)currDataOffset->name,"\pinit"))				{				SetFPos(tempRef,fsFromStart,currDataOffset->off);				count=4;				#ifdef __INTEL__				Swap_LONG(&asmRecord->init);				#endif				FSWrite(tempRef,&count,&asmRecord->init);				//((long*)dataArgumentArray)[currDataOffset->off/4] = asmRecord->init;				}				else if(MyPStrCmp((unsigned char*)currDataOffset->name,"\pterm"))				{				SetFPos(tempRef,fsFromStart,currDataOffset->off);				count=4;				#ifdef __INTEL__				Swap_LONG(&asmRecord->term);				#endif				FSWrite(tempRef,&count,&asmRecord->term);				//((long*)dataArgumentArray)[currDataOffset->off/4] = asmRecord->term;				}			}			tempGarbage = currDataOffset;			currDataOffset = currDataOffset->next;			DisposePtr(tempGarbage->name);			DisposePtr((char*)tempGarbage);		}	}	FSClose(tempRef);}else{k=1;if(asmRecord->init>=0){k++;}if(asmRecord->term>=0){k++;}dataContainerLength = (4*symCount)+(k*8);dataArgumentArray = NewPtr(dataContainerLength);for(i=0;i<dataContainerLength;i++){dataArgumentArray[i]=0;}if(asmRecord->main>=0){((long*)dataArgumentArray)[loaderInfoHeader.mainOffset/4] = asmRecord->main;}if(asmRecord->init>=0){((long*)dataArgumentArray)[loaderInfoHeader.initOffset/4] = asmRecord->init;}if(asmRecord->term>=0){((long*)dataArgumentArray)[loaderInfoHeader.termOffset/4] = asmRecord->term;}}currLabelOffset = gLabelOffsets;while(currLabelOffset){tempDispose = currLabelOffset;currLabelOffset = currLabelOffset->next;DisposePtr(tempDispose->name);DisposePtr((char*)tempDispose);}if(asmRecord->key->relocb){relocSize = asmRecord->reloccount*2;}else{if(libCount){relocSize=4;}else{relocSize=2;}}sectionNameTableLength=4;sectionNameTable = NewPtr(4);for(i=0;i<4;i++){sectionNameTable[i]=0;}hashTablePower=PEFComputeHashTableExponent(expCount);if(expCount){nOffPos = (libCount+symCount);	exportKeyTable = (PEFExportedSymbolKey*)NewPtr(sizeof(PEFExportedSymbolKey)*expCount);	exportSymbolTable = (PEFExportedSymbol*)NewPtr(sizeof(PEFExportedSymbol)*expCount);	exportHashTable = (PEFExportedSymbolHashSlot*)NewPtr(sizeof(PEFExportedSymbolHashSlot)*_pow(2,hashTablePower));	for(i=0;i<_pow(2,hashTablePower);i++){exportHashTable[i].countAndStart=0;}	expSectionLength = (sizeof(PEFExportedSymbolKey)*expCount)+(sizeof(PEFExportedSymbol)*expCount)+(sizeof(PEFExportedSymbolHashSlot)*_pow(2,hashTablePower));	eKeyTableIndex=0;	hashTableRecord = (HashRecord*)NewPtr(sizeof(HashRecord)*_pow(2,hashTablePower));;	for(i=0;i<_pow(2,hashTablePower);i++){hashTableRecord[i].assigned=0;hashTableRecord[i].marker=0;hashTableRecord[i].count=0;}			currExpRec = &asmRecord->exports;	if(currExpRec->label)	{		while(currExpRec)		{		fullHashWord	= PEFComputeHashWord((unsigned char*)currExpRec->label,(unsigned long)StringLength(currExpRec->label));		hashTableIndex	= PEFHashTableIndex(fullHashWord,hashTablePower);		hashTableRecord[hashTableIndex].count++;		if(hashTableRecord[hashTableIndex].count>=kPEFHashSlotMaxSymbolCount){DebugFunction("\pError: Limit: JIKL Reached, Please Report");}		currExpRec = currExpRec->next;		}	}		if(asmRecord->key->deb)	{		currDEB = &asmRecord->key->de;		while(currDEB)		{		fullHashWord	= PEFComputeHashWord((unsigned char*)(currDEB->label+1),(unsigned long)currDEB->label[0]);		hashTableIndex	= PEFHashTableIndex(fullHashWord,hashTablePower);		hashTableRecord[hashTableIndex].count++;		if(hashTableRecord[hashTableIndex].count>=kPEFHashSlotMaxSymbolCount){DebugFunction("\pError: Limit: JDYT Reached, Please Report");}		currDEB = currDEB->next;		}	}			currBlockAssigner=0;	currExpRec = &asmRecord->exports;	if(currExpRec->label)	{		while(currExpRec)		{		fullHashWord	= PEFComputeHashWord((unsigned char*)currExpRec->label,(unsigned long)StringLength(currExpRec->label));		hashTableIndex	= PEFHashTableIndex(fullHashWord,hashTablePower);				if(!hashTableRecord[hashTableIndex].assigned)		{		hashTableRecord[hashTableIndex].marker=currBlockAssigner;		exportHashTable[hashTableIndex].countAndStart = PEFComposeExportedSymbolHashSlot(1,currBlockAssigner);		currBlockAssigner+=hashTableRecord[hashTableIndex].count;		hashTableRecord[hashTableIndex].assigned=true;		}		else		{		exportHashTable[hashTableIndex].countAndStart += 1<<kPEFHashSlotSymCountShift;		}		exportKeyTable[hashTableRecord[hashTableIndex].marker].u.fullHashWord = fullHashWord;		exportSymbolTable[hashTableRecord[hashTableIndex].marker].classAndName	= PEFComposeExportedSymbol(kPEFCodeSymbol,nameOffset[nOffPos]);		exportSymbolTable[hashTableRecord[hashTableIndex].marker].symbolValue	= currExpRec->offset;		exportSymbolTable[hashTableRecord[hashTableIndex].marker].sectionIndex 	= 0;				hashTableRecord[hashTableIndex].marker++;		nOffPos++;		currExpRec = currExpRec->next;		}	}		if(asmRecord->key->deb)	{		currDEB = &asmRecord->key->de;		while(currDEB)		{		fullHashWord	= PEFComputeHashWord((unsigned char*)(currDEB->label+1),(unsigned long)currDEB->label[0]);		hashTableIndex	= PEFHashTableIndex(fullHashWord,hashTablePower);				if(!hashTableRecord[hashTableIndex].assigned)		{		hashTableRecord[hashTableIndex].marker=currBlockAssigner;		exportHashTable[hashTableIndex].countAndStart = PEFComposeExportedSymbolHashSlot(1,currBlockAssigner);		currBlockAssigner+=hashTableRecord[hashTableIndex].count;		hashTableRecord[hashTableIndex].assigned=true;		}		else		{		exportHashTable[hashTableIndex].countAndStart += 1<<kPEFHashSlotSymCountShift;		}		exportKeyTable[hashTableRecord[hashTableIndex].marker].u.fullHashWord = fullHashWord;		exportSymbolTable[hashTableRecord[hashTableIndex].marker].classAndName	= PEFComposeExportedSymbol(kPEFDataSymbol,nameOffset[nOffPos]);		exportSymbolTable[hashTableRecord[hashTableIndex].marker].symbolValue	= currDEB->offset;		exportSymbolTable[hashTableRecord[hashTableIndex].marker].sectionIndex 	= 1;				hashTableRecord[hashTableIndex].marker++;		nOffPos++;		currDEB = currDEB->next;		}	}						}else{hashTablePower=1;/*exportSymbolTable = (PEFExportedSymbol*)NewPtr(sizeof(PEFExportedSymbol)*1);exportSymbolTable->classAndName=0;exportSymbolTable->symbolValue=0;exportSymbolTable->sectionIndex=0;*/expSectionLength = (sizeof(long)*2);//+sizeof(PEFExportedSymbol);}if(libCount){CODE_SECTION_OFFSET = CODE_SECTION_OFFSET_M;LOADER_SECTION_OFFSET = LOADER_SECTION_OFFSET_M;LOADER_SECTION_LENGTH = LOADER_SECTION_LENGTH_M;RELOC_INST_OFFSET = RELOC_INST_OFFSET_M;LOADER_STRINGS_OFFSET = LOADER_STRINGS_OFFSET_M;EXPORT_HASH_OFFSET = EXPORT_HASH_OFFSET_M;}else{libCount=1;symCount=1;CODE_SECTION_OFFSET = CODE_SECTION_OFFSET_M;LOADER_SECTION_OFFSET = LOADER_SECTION_OFFSET_M;LOADER_SECTION_LENGTH = LOADER_SECTION_LENGTH_M;RELOC_INST_OFFSET = RELOC_INST_OFFSET_M;LOADER_STRINGS_OFFSET = LOADER_STRINGS_OFFSET_M;EXPORT_HASH_OFFSET = EXPORT_HASH_OFFSET_M;libCount=0;symCount=0;}containerHeader.tag1	 						= 'Joy!';containerHeader.tag2							= 'peff';containerHeader.architecture					= 'pwpc';containerHeader.formatVersion					= 1;#ifdef __POWERPC__GetDateTime(&containerHeader.dateTimeStamp);#endif#ifdef __INTEL__containerHeader.dateTimeStamp=0;#endifcontainerHeader.oldDefVersion					= 0;containerHeader.oldImpVersion					= 0;containerHeader.currentVersion					= 0;containerHeader.sectionCount					= 3;containerHeader.instSectionCount				= 2;containerHeader.reservedA						= 0;codeSectionHeader.nameOffset				= -1;codeSectionHeader.defaultAddress			= 0;codeSectionHeader.totalLength				= sizeof(long)*asmRecord->instcount;codeSectionHeader.unpackedLength			= sizeof(long)*asmRecord->instcount;codeSectionHeader.containerLength			= sizeof(long)*asmRecord->instcount;codeSectionHeader.containerOffset			= CODE_SECTION_OFFSET;codeSectionHeader.sectionKind				= kPEFCodeSection;codeSectionHeader.shareKind					= kPEFGlobalShare;codeSectionHeader.alignment					= 4;codeSectionHeader.reservedA					= 0;dataSectionHeader.nameOffset				= -1;dataSectionHeader.defaultAddress			= 0;//dataSectionHeader.totalLength				= dataContainerLength+4;dataSectionHeader.totalLength				= dataContainerLength;dataSectionHeader.unpackedLength			= dataContainerLength;dataSectionHeader.containerLength			= dataContainerLength;dataSectionHeader.containerOffset			= CODE_SECTION_OFFSET+(sizeof(long)*asmRecord->instcount);dataSectionHeader.sectionKind				= kPEFUnpackedDataSection;dataSectionHeader.shareKind					= kPEFProcessShare;dataSectionHeader.alignment					= 4;dataSectionHeader.reservedA					= 0;loaderSectionHeader.nameOffset				= -1;loaderSectionHeader.defaultAddress			= 0;loaderSectionHeader.totalLength				= 0;loaderSectionHeader.unpackedLength			= 0;loaderSectionHeader.containerLength			= LOADER_SECTION_LENGTH;loaderSectionHeader.containerOffset			= LOADER_SECTION_OFFSET;loaderSectionHeader.sectionKind				= kPEFLoaderSection;loaderSectionHeader.shareKind				= kPEFGlobalShare;loaderSectionHeader.alignment				= 4;loaderSectionHeader.reservedA				= 0;				/*				if(asmRecord->key->secoffb)				{				FSpOpenDF(asmRecord->key->reloc,fsRdPerm,&tempRef);				tempLPtr=(long*)NewPtr(12);				count=12;				FSRead(tempRef,&count,tempLPtr);				FSClose(tempRef);				loaderInfoHeader.mainOffset = tempLPtr[0];				loaderInfoHeader.initOffset = tempLPtr[1];				loaderInfoHeader.termOffset = tempLPtr[2];				}				else				{				loaderInfoHeader.mainOffset		=	 (4*symCount);				loaderInfoHeader.initOffset		=	 (4*symCount)+8;				loaderInfoHeader.termOffset		=	 (4*symCount)+16;				}				*//*-----loaderInfoHeader Stuff :0-----*/if(asmRecord->main < 0){loaderInfoHeader.mainSection					= -1;}else{loaderInfoHeader.mainSection					= 1;}if(asmRecord->init < 0){loaderInfoHeader.initSection					= -1;}else{loaderInfoHeader.initSection					= 1;}if(asmRecord->term < 0){loaderInfoHeader.termSection					= -1;}else{loaderInfoHeader.termSection					= 1;}loaderInfoHeader.importedLibraryCount			= libCount;loaderInfoHeader.totalImportedSymbolCount		= symCount;loaderInfoHeader.relocSectionCount				= 1;loaderInfoHeader.relocInstrOffset				= RELOC_INST_OFFSET;loaderInfoHeader.loaderStringsOffset			= LOADER_STRINGS_OFFSET;loaderInfoHeader.exportHashOffset				= EXPORT_HASH_OFFSET;loaderInfoHeader.exportHashTablePower			= hashTablePower;loaderInfoHeader.exportedSymbolCount			= expCount;/*-----loaderInfoHeader Stuff :0-----*/if(asmRecord->key->relocb){relocSection = asmRecord->relocs;}else{	if(libCount)	{		(long*)relocSection=(long*)NewPtr(4);		*(long*)relocSection = 0x4A004600;		for(i=1;i<symCount;i++)		{		*(long*)relocSection += 0x00010000;		}		if(loaderInfoHeader.initSection >= 0)		{		*(long*)relocSection += 0x00000001;		}		if(loaderInfoHeader.termSection >= 0)		{		*(long*)relocSection += 0x00000001;		}	}	else	{		(short*)relocSection=(short*)NewPtr(2);		*(short*)relocSection = 0x4600;		if(loaderInfoHeader.initSection >= 0)		{		*(short*)relocSection += 0x00000001;		}		if(loaderInfoHeader.termSection >= 0)		{		*(short*)relocSection += 0x00000001;		}	}}if(libCount){	importedLibrary = (PEFImportedLibrary*)NewPtr(sizeof(PEFImportedLibrary)*libCount);	importedSymbol = (PEFImportedSymbol*)NewPtr(sizeof(PEFImportedSymbol)*symCount);	currImpRec=&asmRecord->import;	symIndex=0;	nOffPos=0;	for(i=0,k=0;i<libCount;i++)	{		importedLibrary[i].nameOffset = nameOffset[nOffPos];nOffPos++;		importedLibrary[i].oldImpVersion = 0;		importedLibrary[i].currentVersion = 0;		importedLibrary[i].importedSymbolCount = currImpRec->symcount;		importedLibrary[i].firstImportedSymbol = symIndex;symIndex+=currImpRec->symcount;		importedLibrary[i].options = 0;		if(currImpRec->weak){importedLibrary[i].options ^= 0x40;}		importedLibrary[i].reservedA = 0;		importedLibrary[i].reservedB = 0;		for(j=0;j<currImpRec->symcount;j++,k++)		{		importedSymbol[k].classAndName = PEFComposeImportedSymbol(2,nameOffset[nOffPos]);nOffPos++;		}				currImpRec = currImpRec->next;	}}else{libCount=1;symCount=1;importedLibrary = (PEFImportedLibrary*)NewPtr(sizeof(PEFImportedLibrary)*libCount);importedSymbol = (PEFImportedSymbol*)NewPtr(sizeof(PEFImportedSymbol)*symCount);}DisposePtr((char*)nameOffset);currImpRec=&asmRecord->import;if(currImpRec->lib){DisposePtr(currImpRec->lib);}if(currImpRec->sym){DisposePtr(currImpRec->sym);}currImpRec = currImpRec->next;while(currImpRec){DisposePtr(currImpRec->lib);DisposePtr(currImpRec->sym);tempImpRec = currImpRec;currImpRec = currImpRec->next;DisposePtr((char*)tempImpRec);}loaderRelocationHeader = (PEFLoaderRelocationHeader*)NewPtr(sizeof(PEFLoaderRelocationHeader));loaderRelocationHeader->sectionIndex = 1;loaderRelocationHeader->reservedA = 0;loaderRelocationHeader->relocCount = relocSize/2;loaderRelocationHeader->firstRelocOffset = 0;FSpOpenDF(newAppFSS,fsWrPerm,&fRefNumber);totalWritten = 0;if(asmRecord->key->rsrcmb){FSpOpenDF(asmRecord->key->rsrcmb,fsRdPerm,&mbinResRef);count = sizeof(MacBinaryIIIHeader);FSRead (mbinResRef, &count, &mbin3ResourceHeader);for(i=0;i<sizeof(MacBinaryIIIHeader);i++){((char*)&mbin3Header)[i]=0;}mbin3Header.fileNameLength = asmRecord->key->target[0];for(i=0;i<asmRecord->key->target[0];i++){mbin3Header.fileName[i]=asmRecord->key->target[i+1];}mbin3Header.fileType=asmRecord->key->type;mbin3Header.fileCreator=asmRecord->key->creator;mbin3Header.resourceForkLength=mbin3ResourceHeader.resourceForkLength;mbin3Header.signature='mBIN';mbin3Header.macBinaryVersion=130;mbin3Header.minimumVersion=130;#ifdef __INTEL__Swap_LONG(&mbin3Header.fileCreator);Swap_LONG(&mbin3Header.fileType);Swap_LONG(&mbin3Header.resourceForkLength);Swap_MACBINARYIII_HEADER(&mbin3Header);#endifcount=sizeof(MacBinaryIIIHeader);FSWrite (fRefNumber, &count, &mbin3Header);totalWritten+=count;}count = sizeof(PEFContainerHeader);#ifdef __INTEL__Swap_PEF_CONTAINER_HEADER(&containerHeader);#endifFSWrite(fRefNumber,&count,&containerHeader);totalWritten+=count;count = sizeof(PEFSectionHeader);#ifdef __INTEL__Swap_PEF_SECTION_HEADER(&codeSectionHeader);#endifFSWrite(fRefNumber,&count,&codeSectionHeader);totalWritten+=count;count = sizeof(PEFSectionHeader);#ifdef __INTEL__Swap_PEF_SECTION_HEADER(&dataSectionHeader);#endifFSWrite(fRefNumber,&count,&dataSectionHeader);totalWritten+=count;count = sizeof(PEFSectionHeader);#ifdef __INTEL__Swap_PEF_SECTION_HEADER(&loaderSectionHeader);#endifFSWrite(fRefNumber,&count,&loaderSectionHeader);totalWritten+=count;count = sectionNameTableLength;FSWrite(fRefNumber,&count,sectionNameTable);totalWritten+=count;DisposePtr(sectionNameTable);count = sizeof(PEFLoaderInfoHeader);#ifdef __INTEL__Swap_PEF_LOADER_INFO_HEADER(&loaderInfoHeader);#endifFSWrite(fRefNumber,&count,&loaderInfoHeader);totalWritten+=count;count = sizeof(PEFImportedLibrary)*libCount;#ifdef __INTEL__for(i=0;i<libCount;i++){Swap_PEF_IMPORTED_LIBRARY(&importedLibrary[i]);}#endifFSWrite(fRefNumber,&count,importedLibrary);totalWritten+=count;DisposePtr((char*)importedLibrary);count = sizeof(PEFImportedSymbol)*symCount;#ifdef __INTEL__for(i=0;i<symCount;i++){Swap_LONG(&importedSymbol[i]);}#endifFSWrite(fRefNumber,&count,importedSymbol);totalWritten+=count;DisposePtr((char*)importedSymbol);count = sizeof(PEFLoaderRelocationHeader);#ifdef __INTEL__Swap_PEFLoaderRelocationHeader(loaderRelocationHeader);#endifFSWrite(fRefNumber,&count,loaderRelocationHeader);totalWritten+=count;DisposePtr((char*)loaderRelocationHeader);count = relocSize;#ifdef __INTEL__for(i=0;i<relocSize;i++){Swap_SHORT((unsigned short*)&relocSection[i]);}#endifFSWrite(fRefNumber,&count,relocSection);totalWritten+=count;/*alignment*/alignCount=(4 - (LOADER_STRINGS_OFFSET_PRE_M % 4));for(i=0;i<alignCount;i++){byte = 0;count = sizeof(char);FSWrite(fRefNumber,&count,&byte);totalWritten+=count;}/*alignment*/count = loaderNameTableLength;FSWrite(fRefNumber,&count,loaderNameTable);totalWritten+=count;DisposePtr(loaderNameTable);if(expCount){count = (sizeof(PEFExportedSymbolHashSlot)*_pow(2,hashTablePower));#ifdef __INTEL__for(i=0;i<_pow(2,hashTablePower);i++){Swap_LONG(&exportHashTable[i]);}#endifFSWrite(fRefNumber,&count,exportHashTable);totalWritten+=count;count = (sizeof(PEFExportedSymbolKey)*expCount);#ifdef __INTEL__for(i=0;i<expCount;i++){Swap_LONG(&exportKeyTable[i].u.fullHashWord);}#endifFSWrite(fRefNumber,&count,exportKeyTable);totalWritten+=count;count = (sizeof(PEFExportedSymbol)*expCount);#ifdef __INTEL__for(i=0;i<expCount;i++){Swap_PEF_EXPORTED_SYMBOL(&exportSymbolTable[i]);}#endifFSWrite(fRefNumber,&count,exportSymbolTable);totalWritten+=count;}else{//hash slot 0data = 0;count = sizeof(long);FSWrite(fRefNumber,&count,&data);totalWritten+=count;//symbol key 0data = 0;count = sizeof(long);FSWrite(fRefNumber,&count,&data);totalWritten+=count;/*//symbol 0count = sizeof(PEFExportedSymbol);FSWrite(fRefNumber,&count,exportSymbolTable);totalWritten+=count;*/if(exportSymbolTable){DisposePtr((char*)exportSymbolTable);}}/*alignment*/alignCount=(4 - (CODE_SECTION_OFFSET_PRE_M % 4));for(i=0;i<alignCount;i++){byte = 0;count = sizeof(char);FSWrite(fRefNumber,&count,&byte);totalWritten+=count;}/*alignment*/#ifdef __POWERPC__//FlushVol(0,((FSSpec*)asmRecord->key->asm_)->vRefNum);#endifFSpOpenDF(asmRecord->codeSection,fsRdPerm,&tempRef);GetEOF(tempRef,&tempLong);FileToFileCopy(tempRef,fRefNumber);totalWritten+=tempLong;FSClose(tempRef);#ifdef __POWERPC__if(FSpDelete((FSSpec*)asmRecord->codeSection)){DebugFunction("\pError Erasing TEMP_CODE_SECTION");}#endifif(dataArgumentArray){count = dataContainerLength;FSWrite(fRefNumber,&count,dataArgumentArray);totalWritten+=count;}else if(asmRecord->dataSection){FSpOpenDF(asmRecord->dataSection,fsRdPerm,&tempRef);GetEOF(tempRef,&tempLong);FileToFileCopy(tempRef,fRefNumber);totalWritten+=tempLong;FSClose(tempRef);#ifdef __POWERPC__//if(FSpDelete((FSSpec*)asmRecord->dataSection)){DebugFunction("\pError Erasing TEMP_DATA_SECTION");}#endifDisposePtr((char*)asmRecord->dataSection);}else{FSpOpenDF(asmRecord->key->data,fsRdPerm,&tempRef);GetEOF(tempRef,&tempLong);FileToFileCopy(tempRef,fRefNumber);totalWritten+=tempLong;FSClose(tempRef);}if(asmRecord->key->rsrcmb){dForkLength = totalWritten - sizeof(MacBinaryIIIHeader);alignCount=(128 - (dForkLength % 128));for(i=0,byte=0,count=1;i<alignCount;i++){FSWrite(fRefNumber, &count, &byte);totalWritten++;}#ifdef __INTEL__Swap_LONG(&mbin3ResourceHeader.resourceForkLength);#endifresData = NewPtr(mbin3ResourceHeader.resourceForkLength);count = mbin3ResourceHeader.resourceForkLength;FSRead(mbinResRef,&count,resData);FSWrite(fRefNumber,&count,resData);totalWritten+=count;SetFPos(fRefNumber,fsFromStart,83);#ifdef __INTEL__Swap_LONG(&dForkLength);#endifcount=4;FSWrite(fRefNumber,&count,&dForkLength);FSClose(mbinResRef);DisposePtr(resData);}SetEOF(fRefNumber,totalWritten);FSClose(fRefNumber);}