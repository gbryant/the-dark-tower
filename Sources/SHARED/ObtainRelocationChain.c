Copyright 2016 Gregory BryantLicensed under the Apache License, Version 2.0 (the "License");you may not use this file except in compliance with the License.You may obtain a copy of the License at    http://www.apache.org/licenses/LICENSE-2.0Unless required by applicable law or agreed to in writing, softwaredistributed under the License is distributed on an "AS IS" BASIS,WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.See the License for the specific language governing permissions andlimitations under the License./***********************************************************************/#include "ProjectHeaders.h"long 						theLineNum;unsigned char				*theFileName;LongChain					*tempLCToReturn;LongChain					*tempLC;#define GET_8BIT																											\																															\while(!(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39)){(*asmPB->bMarker)++;}		\bmHolder = (*asmPB->bMarker);																								\while(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39){(*asmPB->bMarker)++;}			\arg = 0;																													\j = (*asmPB->bMarker) - bmHolder;																							\j--;																														\(*asmPB->bMarker) = bmHolder;																								\for(;j>=0;(*asmPB->bMarker)++,j--)																							\{																															\arg += ((asmPB->asmBuffer[(*asmPB->bMarker)]-0x30)*(_pow(10,j)));															\}																															\if(arg>0xFF)																												\{																															\MyPStrAdd("\pAn argument to an instruction was too large\rSource: ",theFileName,gTempTxt2);									\MyPStrAdd(gTempTxt2,"\p\rLine: ",gTempTxt);																					\NumberToString(theLineNum,(char*)gTempTxt3);																					\MyPStrAdd(gTempTxt,gTempTxt3,gTempTxt2);																					\DebugFunction(gTempTxt2);																									\}#define GET_8BIT_M1																											\																															\while(!(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39)){(*asmPB->bMarker)++;}		\bmHolder = (*asmPB->bMarker);																								\while(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39){(*asmPB->bMarker)++;}			\arg = 0;																													\j = (*asmPB->bMarker) - bmHolder;																							\j--;																														\(*asmPB->bMarker) = bmHolder;																								\for(;j>=0;(*asmPB->bMarker)++,j--)																							\{																															\arg += ((asmPB->asmBuffer[(*asmPB->bMarker)]-0x30)*(_pow(10,j)));															\}																															\if(arg>0x100)																												\{																															\MyPStrAdd("\pAn argument to an instruction was too large\rSource: ",theFileName,gTempTxt2);									\MyPStrAdd(gTempTxt2,"\p\rLine: ",gTempTxt);																					\NumberToString(theLineNum,(char*)gTempTxt3);																					\MyPStrAdd(gTempTxt,gTempTxt3,gTempTxt2);																					\DebugFunction(gTempTxt2);																									\}#define GET_22BIT																											\																															\while(!(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39)){(*asmPB->bMarker)++;}		\bmHolder = (*asmPB->bMarker);																								\while(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39){(*asmPB->bMarker)++;}			\arg = 0;																													\j = (*asmPB->bMarker) - bmHolder;																							\j--;																														\(*asmPB->bMarker) = bmHolder;																								\for(;j>=0;(*asmPB->bMarker)++,j--)																							\{																															\arg += ((asmPB->asmBuffer[(*asmPB->bMarker)]-0x30)*(_pow(10,j)));															\}																															\if(arg>0x003FFFFF)																											\{																															\MyPStrAdd("\pAn argument to an instruction was too large\rSource: ",theFileName,gTempTxt2);									\MyPStrAdd(gTempTxt2,"\p\rLine: ",gTempTxt);																					\NumberToString(theLineNum,(char*)gTempTxt3);																					\MyPStrAdd(gTempTxt,gTempTxt3,gTempTxt2);																					\DebugFunction(gTempTxt2);																									\}#define GET_26BIT																											\																															\while(!(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39)){(*asmPB->bMarker)++;}		\bmHolder = (*asmPB->bMarker);																								\while(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39){(*asmPB->bMarker)++;}			\arg = 0;																													\j = (*asmPB->bMarker) - bmHolder;																							\j--;																														\(*asmPB->bMarker) = bmHolder;																								\for(;j>=0;(*asmPB->bMarker)++,j--)																							\{																															\arg += ((asmPB->asmBuffer[(*asmPB->bMarker)]-0x30)*(_pow(10,j)));															\}																															\if(arg>0x03FFFFFF)																											\{																															\MyPStrAdd("\pAn argument to an instruction was too large\rSource: ",theFileName,gTempTxt2);									\MyPStrAdd(gTempTxt2,"\p\rLine: ",gTempTxt);																					\NumberToString(theLineNum,(char*)gTempTxt3);																					\MyPStrAdd(gTempTxt,gTempTxt3,gTempTxt2);																					\DebugFunction(gTempTxt2);																									\}#define GET_6BIT																											\																															\while(!(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39)){(*asmPB->bMarker)++;}		\bmHolder = (*asmPB->bMarker);																								\while(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39){(*asmPB->bMarker)++;}			\arg = 0;																													\j = (*asmPB->bMarker) - bmHolder;																							\j--;																														\(*asmPB->bMarker) = bmHolder;																								\for(;j>=0;(*asmPB->bMarker)++,j--)																							\{																															\arg += ((asmPB->asmBuffer[(*asmPB->bMarker)]-0x30)*(_pow(10,j)));															\}																															\if(arg>0x3F)																												\{																															\MyPStrAdd("\pAn argument to an instruction was too large\rSource: ",theFileName,gTempTxt2);									\MyPStrAdd(gTempTxt2,"\p\rLine: ",gTempTxt);																					\NumberToString(theLineNum,(char*)gTempTxt3);																					\MyPStrAdd(gTempTxt,gTempTxt3,gTempTxt2);																					\DebugFunction(gTempTxt2);																									\}#define GET_4BIT																											\																															\while(!(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39)){(*asmPB->bMarker)++;}		\bmHolder = (*asmPB->bMarker);																								\while(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39){(*asmPB->bMarker)++;}			\arg = 0;																													\j = (*asmPB->bMarker) - bmHolder;																							\j--;																														\(*asmPB->bMarker) = bmHolder;																								\for(;j>=0;(*asmPB->bMarker)++,j--)																							\{																															\arg += ((asmPB->asmBuffer[(*asmPB->bMarker)]-0x30)*(_pow(10,j)));															\}																															\if(arg>0xF)																													\{																															\MyPStrAdd("\pAn argument to an instruction was too large\rSource: ",theFileName,gTempTxt2);									\MyPStrAdd(gTempTxt2,"\p\rLine: ",gTempTxt);																					\NumberToString(theLineNum,(char*)gTempTxt3);																					\MyPStrAdd(gTempTxt,gTempTxt3,gTempTxt2);																					\DebugFunction(gTempTxt2);																									\}#define GET_4BIT_M1																											\																															\while(!(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39)){(*asmPB->bMarker)++;}		\bmHolder = (*asmPB->bMarker);																								\while(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39){(*asmPB->bMarker)++;}			\arg = 0;																													\j = (*asmPB->bMarker) - bmHolder;																							\j--;																														\(*asmPB->bMarker) = bmHolder;																								\for(;j>=0;(*asmPB->bMarker)++,j--)																							\{																															\arg += ((asmPB->asmBuffer[(*asmPB->bMarker)]-0x30)*(_pow(10,j)));															\}																															\if(arg>0x10)																												\{																															\MyPStrAdd("\pAn argument to an instruction was too large\rSource: ",theFileName,gTempTxt2);									\MyPStrAdd(gTempTxt2,"\p\rLine: ",gTempTxt);																					\NumberToString(theLineNum,(char*)gTempTxt3);																					\MyPStrAdd(gTempTxt,gTempTxt3,gTempTxt2);																					\DebugFunction(gTempTxt2);																									\}#define GET_9BIT																											\																															\while(!(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39)){(*asmPB->bMarker)++;}		\bmHolder = (*asmPB->bMarker);																								\while(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39){(*asmPB->bMarker)++;}			\arg = 0;																													\j = (*asmPB->bMarker) - bmHolder;																							\j--;																														\(*asmPB->bMarker) = bmHolder;																								\for(;j>=0;(*asmPB->bMarker)++,j--)																							\{																															\arg += ((asmPB->asmBuffer[(*asmPB->bMarker)]-0x30)*(_pow(10,j)));															\}																															\if(arg>0x1FF)																												\{																															\MyPStrAdd("\pAn argument to an instruction was too large\rSource: ",theFileName,gTempTxt2);									\MyPStrAdd(gTempTxt2,"\p\rLine: ",gTempTxt);																					\NumberToString(theLineNum,(char*)gTempTxt3);																					\MyPStrAdd(gTempTxt,gTempTxt3,gTempTxt2);																					\DebugFunction(gTempTxt2);																									\}#define GET_9BIT_M1																											\																															\while(!(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39)){(*asmPB->bMarker)++;}		\bmHolder = (*asmPB->bMarker);																								\while(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39){(*asmPB->bMarker)++;}			\arg = 0;																													\j = (*asmPB->bMarker) - bmHolder;																							\j--;																														\(*asmPB->bMarker) = bmHolder;																								\for(;j>=0;(*asmPB->bMarker)++,j--)																							\{																															\arg += ((asmPB->asmBuffer[(*asmPB->bMarker)]-0x30)*(_pow(10,j)));															\}																															\if(arg>0x200)																												\{																															\MyPStrAdd("\pAn argument to an instruction was too large\rSource: ",theFileName,gTempTxt2);									\MyPStrAdd(gTempTxt2,"\p\rLine: ",gTempTxt);																					\NumberToString(theLineNum,(char*)gTempTxt3);																					\MyPStrAdd(gTempTxt,gTempTxt3,gTempTxt2);																					\DebugFunction(gTempTxt2);																									\}#define GET_12BIT_M1																										\																															\while(!(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39)){(*asmPB->bMarker)++;}		\bmHolder = (*asmPB->bMarker);																								\while(asmPB->asmBuffer[(*asmPB->bMarker)]>=0x30 && asmPB->asmBuffer[(*asmPB->bMarker)]<=0x39){(*asmPB->bMarker)++;}			\arg = 0;																													\j = (*asmPB->bMarker) - bmHolder;																							\j--;																														\(*asmPB->bMarker) = bmHolder;																								\for(;j>=0;(*asmPB->bMarker)++,j--)																							\{																															\arg += ((asmPB->asmBuffer[(*asmPB->bMarker)]-0x30)*(_pow(10,j)));															\}																															\if(arg>0x1000)																												\{																															\MyPStrAdd("\pAn argument to an instruction was too large\rSource: ",theFileName,gTempTxt2);									\MyPStrAdd(gTempTxt2,"\p\rLine: ",gTempTxt);																					\NumberToString(theLineNum,(char*)gTempTxt3);																					\MyPStrAdd(gTempTxt,gTempTxt3,gTempTxt2);																					\DebugFunction(gTempTxt2);																									\}LongChain* ObtainRelocationChain(Key *key){void				*fRefNumber;long				dataSize;unsigned long		count;char				tempBuffer[255];long				i,j,k;AssemblyPBlock		asmPB;long				compatL=0;AssemblerRecord		*asmRecord;tempLCToReturn=0;asmRecord = (AssemblerRecord*)NewPtr(sizeof(AssemblerRecord));tempLCToReturn=0;theFileName = (unsigned char*)NewPtr(255);FileRef_GetFileName(key->reloc,theFileName);FSpOpenDF(key->reloc,fsRdPerm,&fRefNumber);GetEOF(fRefNumber,&dataSize);asmPB.asmBuffer = NewPtr(dataSize+8);count = dataSize;FSRead(fRefNumber,&count,asmPB.asmBuffer);FSClose(fRefNumber);for(i=0,j=0;i<dataSize;i++){if(asmPB.asmBuffer[i]=='\r'){j++;}}asmPB.relocs = (short*)NewPtr((j*8));//hopefully that is enough padding...asmPB.rMarker=0;for(i=0;i<(j*8);i++){((char*)asmPB.relocs)[i]=0;}asmPB.bMarker = &compatL;asmPB.asmRecord = asmRecord;(*asmPB.bMarker)=0;theLineNum=1;while(*asmPB.bMarker<dataSize){	switch(asmPB.asmBuffer[*asmPB.bMarker])	{	case ' ' :	case '\t':	case '\n':	case '\r':theLineNum++;(*asmPB.bMarker)++;break;	case '#':while(asmPB.asmBuffer[*asmPB.bMarker]!=0x0D){(*asmPB.bMarker)++;}break;	default:{			i=1;			while((asmPB.asmBuffer[*asmPB.bMarker]!=0x09) && (asmPB.asmBuffer[*asmPB.bMarker]!=0x20) && (asmPB.asmBuffer[*asmPB.bMarker]!=0x0D))			{			tempBuffer[i] = asmPB.asmBuffer[*asmPB.bMarker];			(*asmPB.bMarker)++;			i++;			}			tempBuffer[0] = (i-1);			i=0;						switch(tempBuffer[i])			{				case 3:				{					i++;					switch(tempBuffer[i])					{						case 'R':						{							i++;							switch(tempBuffer[i])							{								case 'P':								{									i++;									switch(tempBuffer[i])									{										case 'T':										{											GUI_RPT(&asmPB);											break;										}										default: Byte_ILLEGAL(&asmPB);									}									break;								}								default: Byte_ILLEGAL(&asmPB);							}							break;						}					}					break;				}				case 4:				{					i++;					switch(tempBuffer[i])					{						case 'C':						{							i++;							switch(tempBuffer[i])							{								case 'D':								{									i++;									switch(tempBuffer[i])									{										case 'I':										{											i++;											switch(tempBuffer[i])											{												case 'S':												{													GUI_CDIS(&asmPB);													break;												}												default: Byte_ILLEGAL(&asmPB);											}											break;										}										default: Byte_ILLEGAL(&asmPB);									}									break;								}								case 'O':								{									i++;									switch(tempBuffer[i])									{										case 'D':										{											i++;											switch(tempBuffer[i])											{												case 'E':												{													GUI_CODE(&asmPB);													break;												}												default: Byte_ILLEGAL(&asmPB);											}											break;										}										default: Byte_ILLEGAL(&asmPB);									}									break;								}								default: Byte_ILLEGAL(&asmPB);							}							break;						}						case 'D':						{							i++;							switch(tempBuffer[i])							{								case 'A':								{									i++;									switch(tempBuffer[i])									{										case 'T':										{											i++;											switch(tempBuffer[i])											{												case 'A':												{													GUI_DATA(&asmPB);													break;												}												default: Byte_ILLEGAL(&asmPB);											}											break;										}										default: Byte_ILLEGAL(&asmPB);									}									break;								}								case 'D':								{									i++;									switch(tempBuffer[i])									{										case 'A':										{											i++;											switch(tempBuffer[i])											{												case 'T':												{													GUI_DDAT(&asmPB);													break;												}												default: Byte_ILLEGAL(&asmPB);											}											break;										}										default: Byte_ILLEGAL(&asmPB);									}									break;								}								case 'E':								{									i++;									switch(tempBuffer[i])									{										case 'L':										{											i++;											switch(tempBuffer[i])											{												case 'T':												{													GUI_DELT(&asmPB);													break;												}												default: Byte_ILLEGAL(&asmPB);											}											break;										}										case 'S':										{											i++;											switch(tempBuffer[i])											{												case 'C':												{													GUI_DESC(&asmPB);													break;												}												default: Byte_ILLEGAL(&asmPB);											}											break;										}										default: Byte_ILLEGAL(&asmPB);									}									break;								}								case 'S':								{									i++;									switch(tempBuffer[i])									{										case 'C':										{											i++;											switch(tempBuffer[i])											{												case '2':												{													GUI_DSC2(&asmPB);													break;												}												default: Byte_ILLEGAL(&asmPB);											}											break;										}										default: Byte_ILLEGAL(&asmPB);									}									break;								}								case 'T':								{									i++;									switch(tempBuffer[i])									{										case 'I':										{											i++;											switch(tempBuffer[i])											{												case 'S':												{													GUI_DTIS(&asmPB);													break;												}												default: Byte_ILLEGAL(&asmPB);											}											break;										}										default: Byte_ILLEGAL(&asmPB);									}									break;								}								default: Byte_ILLEGAL(&asmPB);							}							break;						}						case 'L':						{							i++;							switch(tempBuffer[i])							{								case 'A':								{									i++;									switch(tempBuffer[i])									{										case 'B':										{											i++;											switch(tempBuffer[i])											{												case 'S':												{													GUI_LABS(&asmPB);													break;												}												default: Byte_ILLEGAL(&asmPB);											}											break;										}										default: Byte_ILLEGAL(&asmPB);									}									break;								}								case 'R':								{									i++;									switch(tempBuffer[i])									{										case 'P':										{											i++;											switch(tempBuffer[i])											{												case 'T':												{													GUI_LRPT(&asmPB);													break;												}												default: Byte_ILLEGAL(&asmPB);											}											break;										}										default: Byte_ILLEGAL(&asmPB);									}									break;								}								case 'S':								{									i++;									switch(tempBuffer[i])									{										case 'E':										{											i++;											switch(tempBuffer[i])											{												case 'C':												{													GUI_LSEC(&asmPB);													break;												}												default: Byte_ILLEGAL(&asmPB);											}											break;										}										case 'Y':										{											i++;											switch(tempBuffer[i])											{												case 'M':												{													GUI_LSYM(&asmPB);													break;												}												default: Byte_ILLEGAL(&asmPB);											}											break;										}										default: Byte_ILLEGAL(&asmPB);									}									break;								}								default: Byte_ILLEGAL(&asmPB);							}							break;						}						case 'S':						{							i++;							switch(tempBuffer[i])							{								case 'E':								{									i++;									switch(tempBuffer[i])									{										case 'C':										{											i++;											switch(tempBuffer[i])											{												case 'N':												{													GUI_SECN(&asmPB);													break;												}												default: Byte_ILLEGAL(&asmPB);											}											break;										}										default: Byte_ILLEGAL(&asmPB);									}									break;								}								case 'Y':								{									i++;									switch(tempBuffer[i])									{										case 'M':										{											i++;											switch(tempBuffer[i])											{												case 'B':												{													GUI_SYMB(&asmPB);													break;												}												case 'R':												{													GUI_SYMR(&asmPB);													break;												}												default: Byte_ILLEGAL(&asmPB);											}											break;										}										default: Byte_ILLEGAL(&asmPB);									}									break;								}								default: Byte_ILLEGAL(&asmPB);							}							break;						}						case 'V':						{							i++;							switch(tempBuffer[i])							{								case 'T':								{									i++;									switch(tempBuffer[i])									{										case 'B':										{											i++;											switch(tempBuffer[i])											{												case 'L':												{													GUI_VTBL(&asmPB);													break;												}												default: Byte_ILLEGAL(&asmPB);											}											break;										}										default: Byte_ILLEGAL(&asmPB);									}									break;								}								default: Byte_ILLEGAL(&asmPB);							}							break;						}					}					break;				}				default:break;			}															//			DisposePtr(slam);			break;			}	}}DisposePtr(asmPB.asmBuffer);//asmRecord->relocs = (short*)NewPtr((j*2)+8);//for(i=0;i<(j*2)+8;i++){((char*)asmRecord->relocs)[i]=((char*)asmPB.relocs)[i];}asmRecord->relocs = asmPB.relocs;asmRecord->reloccount = asmPB.rMarker;DisposePtr((char*)theFileName);return tempLCToReturn;}void GUI_DDAT(AssemblyPBlock  *asmPB){long arg,j,bmHolder;Str255 pStr;tempLC = LongChainAdd(&tempLCToReturn,0,kDDAT,"\p"/*"\pundef"*/,0);BuildRelocationCell(tempLC);GET_8BITtempLC->args->value[0] = arg;NumToString(arg,pStr);GET_6BITtempLC->args->value[1] = arg;}void GUI_CODE(AssemblyPBlock  *asmPB){long arg,j,bmHolder;Str255 pStr;tempLC = LongChainAdd(&tempLCToReturn,0,kCODE,"\p"/*"\pundef"*/,0);BuildRelocationCell(tempLC);GET_9BIT_M1tempLC->args->value[0] = arg;}void GUI_DATA(AssemblyPBlock  *asmPB){long arg,j,bmHolder;Str255 pStr;tempLC = LongChainAdd(&tempLCToReturn,0,kDATA,"\p"/*"\pundef"*/,0);BuildRelocationCell(tempLC);GET_9BIT_M1tempLC->args->value[0] = arg;}void GUI_DESC(AssemblyPBlock  *asmPB){long arg,j,bmHolder;Str255 pStr;tempLC = LongChainAdd(&tempLCToReturn,0,kDESC,"\p"/*"\pundef"*/,0);BuildRelocationCell(tempLC);GET_9BIT_M1tempLC->args->value[0] = arg;}void GUI_DSC2(AssemblyPBlock  *asmPB){long arg,j,bmHolder;Str255 pStr;tempLC = LongChainAdd(&tempLCToReturn,0,kDSC2,"\p"/*"\pundef"*/,0);BuildRelocationCell(tempLC);GET_9BIT_M1tempLC->args->value[0] = arg;}void GUI_VTBL(AssemblyPBlock  *asmPB){long arg,j,bmHolder;Str255 pStr;tempLC = LongChainAdd(&tempLCToReturn,0,kVTBL,"\p"/*"\pundef"*/,0);BuildRelocationCell(tempLC);GET_9BIT_M1tempLC->args->value[0] = arg;}void GUI_SYMR(AssemblyPBlock  *asmPB){long arg,j,bmHolder;Str255 pStr;tempLC = LongChainAdd(&tempLCToReturn,0,kSYMR,"\p"/*"\pundef"*/,0);BuildRelocationCell(tempLC);GET_9BIT_M1tempLC->args->value[0] = arg;}void GUI_SYMB(AssemblyPBlock  *asmPB){long arg,j,bmHolder;Str255 pStr;tempLC = LongChainAdd(&tempLCToReturn,0,kSYMB,"\p"/*"\pundef"*/,0);BuildRelocationCell(tempLC);GET_9BITtempLC->args->value[0] = arg;}void GUI_CDIS(AssemblyPBlock  *asmPB){long arg,j,bmHolder;Str255 pStr;tempLC = LongChainAdd(&tempLCToReturn,0,kCDIS,"\p"/*"\pundef"*/,0);BuildRelocationCell(tempLC);GET_9BITtempLC->args->value[0] = arg;}void GUI_DTIS(AssemblyPBlock  *asmPB){long arg,j,bmHolder;Str255 pStr;tempLC = LongChainAdd(&tempLCToReturn,0,kDTIS,"\p"/*"\pundef"*/,0);BuildRelocationCell(tempLC);GET_9BITtempLC->args->value[0] = arg;}void GUI_SECN(AssemblyPBlock  *asmPB){long arg,j,bmHolder;Str255 pStr;tempLC = LongChainAdd(&tempLCToReturn,0,kSECN,"\p"/*"\pundef"*/,0);BuildRelocationCell(tempLC);GET_9BITtempLC->args->value[0] = arg;}void GUI_DELT(AssemblyPBlock  *asmPB){long arg,j,bmHolder;Str255 pStr;tempLC = LongChainAdd(&tempLCToReturn,0,kDELT,"\p"/*"\pundef"*/,0);BuildRelocationCell(tempLC);GET_12BIT_M1tempLC->args->value[0] = arg;}void GUI_RPT(AssemblyPBlock  *asmPB){long arg,j,bmHolder;Str255 pStr;tempLC = LongChainAdd(&tempLCToReturn,0,kRPT,"\p"/*"\pundef"*/,0);BuildRelocationCell(tempLC);GET_4BIT_M1tempLC->args->value[0] = arg;GET_8BIT_M1tempLC->args->value[1] = arg;}void GUI_LABS(AssemblyPBlock  *asmPB){long arg,j,bmHolder;Str255 pStr;tempLC = LongChainAdd(&tempLCToReturn,0,kLABS,"\p"/*"\pundef"*/,0);BuildRelocationCell(tempLC);GET_26BITtempLC->args->value[0] = arg;}void GUI_LSYM(AssemblyPBlock  *asmPB){long arg,j,bmHolder;Str255 pStr;tempLC = LongChainAdd(&tempLCToReturn,0,kLSYM,"\p"/*"\pundef"*/,0);BuildRelocationCell(tempLC);GET_26BITtempLC->args->value[0] = arg;}void GUI_LRPT(AssemblyPBlock  *asmPB){long arg,j,bmHolder;Str255 pStr;tempLC = LongChainAdd(&tempLCToReturn,0,kDELT,"\p"/*"\pundef"*/,0);BuildRelocationCell(tempLC);GET_4BIT_M1tempLC->args->value[0] = arg;GET_22BITtempLC->args->value[1] = arg;}void GUI_LSEC(AssemblyPBlock  *asmPB){long arg,arg2,j,bmHolder;Str255 pStr;asmPB->relocs[asmPB->rMarker] += 0x2D<<10;GET_4BITarg2=arg;if(arg2==0){tempLC = LongChainAdd(&tempLCToReturn,0,kLSEC0,"\p"/*"\pundef"*/,0);BuildRelocationCell(tempLC);GET_22BITtempLC->args->value[0] = arg;}else if(arg2==1){tempLC = LongChainAdd(&tempLCToReturn,0,kLSEC1,"\p"/*"\pundef"*/,0);BuildRelocationCell(tempLC);GET_22BITtempLC->args->value[0] = arg;}else if(arg2==2){tempLC = LongChainAdd(&tempLCToReturn,0,kLSEC2,"\p"/*"\pundef"*/,0);BuildRelocationCell(tempLC);GET_22BITtempLC->args->value[0] = arg;}else{DebugFunction("\pGUI_LSEC error");}}