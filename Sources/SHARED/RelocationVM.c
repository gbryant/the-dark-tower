Copyright 2016 Gregory BryantLicensed under the Apache License, Version 2.0 (the "License");you may not use this file except in compliance with the License.You may obtain a copy of the License at    http://www.apache.org/licenses/LICENSE-2.0Unless required by applicable law or agreed to in writing, softwaredistributed under the License is distributed on an "AS IS" BASIS,WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.See the License for the specific language governing permissions andlimitations under the License./***********************************************************************/#include "ProjectHeaders.h"void RelocationVM(RelocOpWriterPBlock *rowPB){long blockCount,repeatCount,savedPos;LabelList	**curr;long		bMarker,bmHolder,symSelect,libSelect,i,symT,idex,j,raddr,k;curr = &rowPB->rMachine.sym;rowPB->rMachine.relocAddress=0;rowPB->rMachine.importIndex=0;rowPB->rMachine.sectionC=0;rowPB->rMachine.sectionD=0;rowPB->pos=0;while(rowPB->pos < rowPB->relocSize){//relocOpCodeVM[(rowPB->relocInst[rowPB->pos]&0xFE00)>>9](rowPB);switch((rowPB->relocInst[rowPB->pos]&0xFE00)>>9){	//ROP_DDAT_VM	case 0x00:	case 0x01:	case 0x02:	case 0x03:	case 0x04:	case 0x05:	case 0x06:	case 0x07:	case 0x08:	case 0x09:	case 0x0A:	case 0x0B:	case 0x0C:	case 0x0D:	case 0x0E:	case 0x0F:	case 0x10:	case 0x11:	case 0x12:	case 0x13:	case 0x14:	case 0x15:	case 0x16:	case 0x17:	case 0x18:	case 0x19:	case 0x1A:	case 0x1B:	case 0x1C:	case 0x1D:	case 0x1E:	case 0x1F:	{	rowPB->rMachine.relocAddress += (((rowPB->relocInst[rowPB->pos]&0x3FC0)>>6)*4);	rowPB->rMachine.relocAddress += ((rowPB->relocInst[rowPB->pos]&0x3F)*4);	rowPB->pos+=1;	break;	}		//ROP_CODE_VM	case 0x20:	{	rowPB->rMachine.relocAddress += (((rowPB->relocInst[rowPB->pos]&0x1FF)+1)*4);	rowPB->pos+=1;	break;	}		//ROP_DATA_VM	case 0x21:	{	rowPB->rMachine.relocAddress += (((rowPB->relocInst[rowPB->pos]&0x1FF)+1)*4);	rowPB->pos+=1;	break;	}		//ROP_DESC_VM	case 0x22:	{	rowPB->rMachine.relocAddress += (((rowPB->relocInst[rowPB->pos]&0x1FF)+1)*12);	rowPB->pos+=1;	break;	}		//ROP_DSC2_VM	case 0x23:	{	rowPB->rMachine.relocAddress += (((rowPB->relocInst[rowPB->pos]&0x1FF)+1)*8);	rowPB->pos+=1;	break;	}		//ROP_VTBL_VM	case 0x24:	{	rowPB->rMachine.relocAddress += (((rowPB->relocInst[rowPB->pos]&0x1FF)+1)*8);	rowPB->pos+=1;	break;	}		//ROP_SYMR_VM	case 0x25:	{		idex = rowPB->rMachine.importIndex;		raddr = rowPB->rMachine.relocAddress;		i=(rowPB->relocInst[rowPB->pos]&0x1FF)+1;		for(i=0;i<((rowPB->relocInst[rowPB->pos]&0x1FF)+1);i++)		{			symT=0;			for(libSelect=0;libSelect<rowPB->llpb->libcount;libSelect++)			{				symSelect = rowPB->llpb->lib[libSelect].firstImportedSymbol;				for(k=0;k<rowPB->llpb->lib[libSelect].importedSymbolCount;k++,symSelect++,symT++)				{					if(symT==idex)					{						(*curr) = (LabelList*)NewPtr(sizeof(LabelList));						(*curr)->index = raddr;						(*curr)->next=nil;						bMarker = rowPB->llpb->sym[symSelect].classAndName & kPEFImpSymNameOffsetMask;						bmHolder = bMarker;						while(rowPB->llpb->name[bMarker]!=0){bMarker++;}						(*curr)->label = NewPtr((bMarker-bmHolder)+1);						for(j=0;j<bMarker-bmHolder;j++){(*curr)->label[j+1]=rowPB->llpb->name[bmHolder+j];}(*curr)->label[0]=bMarker-bmHolder;						idex++;						raddr+=4;						curr = &((*curr)->next);						goto end;					}				}			}		end:;		}		rowPB->rMachine.relocAddress += (((rowPB->relocInst[rowPB->pos]&0x1FF)+1)*4);		rowPB->rMachine.importIndex += (rowPB->relocInst[rowPB->pos]&0x1FF)+1;		rowPB->pos+=1;	break;	}		//ROP_SYMB_VM	case 0x30:	{		idex = (rowPB->relocInst[rowPB->pos]&0x1FF);		raddr = rowPB->rMachine.relocAddress;		{			symT=0;			for(libSelect=0;libSelect<rowPB->llpb->libcount;libSelect++)			{				symSelect = rowPB->llpb->lib[libSelect].firstImportedSymbol;				for(i=0;i<rowPB->llpb->lib[libSelect].importedSymbolCount;i++,symSelect++,symT++)				{					if(symT==idex)					{						(*curr) = (LabelList*)NewPtr(sizeof(LabelList));						(*curr)->index = raddr;						(*curr)->next = nil;						bMarker = rowPB->llpb->sym[symSelect].classAndName & kPEFImpSymNameOffsetMask;						bmHolder = bMarker;						while(rowPB->llpb->name[bMarker]!=0){bMarker++;}						(*curr)->label = NewPtr((bMarker-bmHolder)+1);						for(j=0;j<bMarker-bmHolder;j++){(*curr)->label[j+1]=rowPB->llpb->name[bmHolder+j];}(*curr)->label[0]=bMarker-bmHolder;						idex++;						raddr+=4;						curr = &((*curr)->next);						goto end2;					}				}			}		end2:;		}		rowPB->rMachine.relocAddress += 4;		rowPB->rMachine.importIndex = (rowPB->relocInst[rowPB->pos]&0x1FF)+1;		rowPB->pos+=1;	break;	}		//ROP_CDIS_VM	case 0x31:	{	rowPB->pos+=1;	break;	}		//ROP_DTIS_VM	case 0x32:	{	rowPB->pos+=1;	break;	}		//ROP_SECN_VM	case 0x33:	{	rowPB->rMachine.relocAddress += (((rowPB->relocInst[rowPB->pos]&0x1FF)+1)*4);	rowPB->pos+=1;	break;	}		//ROP_DELT_VM	case 0x40:	case 0x41:	case 0x42:	case 0x43:	case 0x44:	case 0x45:	case 0x46:	case 0x47:	{	rowPB->rMachine.relocAddress += ((rowPB->relocInst[rowPB->pos]&0xFFF)+1);	rowPB->pos+=1;	break;	}		//ROP_RPT_VM	case 0x48:	case 0x49:	case 0x4A:	case 0x4B:	case 0x4C:	case 0x4D:	case 0x4E:	case 0x4F:	{		blockCount = ((rowPB->relocInst[rowPB->pos]&0xF00)>>8)+1;	repeatCount = (rowPB->relocInst[rowPB->pos]&0xFF)+1;	savedPos = rowPB->pos;	for(i=0;i<repeatCount;i++)	{		rowPB->pos -= blockCount;		for(j=0;j<blockCount;j++)		{		//relocOpCodeVM[(rowPB->relocInst[rowPB->pos]&0xFE00)>>9](rowPB);		RelocationVMEngine(rowPB,curr);		}	}	rowPB->pos=savedPos;	rowPB->pos+=1;	break;	}		//ROP_LABS_VM	case 0x50:	case 0x51:	{	rowPB->rMachine.relocAddress += (*((long*)(&rowPB->relocInst[rowPB->pos])))&0x03FFFFFF;	rowPB->pos+=2;	break;	}		//ROP_LSYM_VM	case 0x52:	case 0x53:	{		idex = ((*((long*)(&rowPB->relocInst[rowPB->pos])))&0x03FFFFFF);		raddr = rowPB->rMachine.relocAddress;		{			symT=0;			for(libSelect=0;libSelect<rowPB->llpb->libcount;libSelect++)			{				symSelect = rowPB->llpb->lib[libSelect].firstImportedSymbol;				for(i=0;i<rowPB->llpb->lib[libSelect].importedSymbolCount;i++,symSelect++,symT++)				{					if(symT==idex)					{						(*curr) = (LabelList*)NewPtr(sizeof(LabelList));						(*curr)->index = raddr;						(*curr)->next = nil;						bMarker = rowPB->llpb->sym[symSelect].classAndName & kPEFImpSymNameOffsetMask;						bmHolder = bMarker;						while(rowPB->llpb->name[bMarker]!=0){bMarker++;}						(*curr)->label = NewPtr((bMarker-bmHolder)+1);						for(j=0;j<bMarker-bmHolder;j++){(*curr)->label[j+1]=rowPB->llpb->name[bmHolder+j];}(*curr)->label[0]=bMarker-bmHolder;						idex++;						raddr+=4;						curr = &((*curr)->next);						goto end3;					}				}			}			end3:;		}		rowPB->rMachine.relocAddress += 4;		rowPB->rMachine.importIndex = ((*((long*)(&rowPB->relocInst[rowPB->pos])))&0x03FFFFFF)+1;		rowPB->pos+=2;	break;	}		//ROP_LRPT_VM	case 0x58:	case 0x59:	{	blockCount = (((*((long*)(&rowPB->relocInst[rowPB->pos])))&0x03C00000)>>22)+1;	repeatCount = (*((long*)(&rowPB->relocInst[rowPB->pos])))&0x003FFFFF;	savedPos = rowPB->pos;	for(i=0;i<repeatCount;i++)	{		rowPB->pos -= blockCount;		for(j=0;j<blockCount;j++)		{		//relocOpCodeVM[(rowPB->relocInst[rowPB->pos]&0xFE00)>>9](rowPB);		RelocationVMEngine(rowPB,curr);		}	}	rowPB->pos=savedPos;	rowPB->pos+=2;	break;	}		//ROP_LSEC_VM	case 0x5A:	{	rowPB->rMachine.relocAddress += (*((long*)(&rowPB->relocInst[rowPB->pos])))&0x003FFFFF;	rowPB->pos+=2;	break;	}}}}/*void ROP_DDAT_VM(RelocOpWriterPBlock *rowPB){rowPB->rMachine.relocAddress += (((rowPB->relocInst[rowPB->pos]&0x3FC0)>>6)*4);rowPB->rMachine.relocAddress += ((rowPB->relocInst[rowPB->pos]&0x3F)*4);rowPB->pos+=1;}void ROP_CODE_VM(RelocOpWriterPBlock *rowPB){rowPB->rMachine.relocAddress += (((rowPB->relocInst[rowPB->pos]&0x1FF)+1)*4);rowPB->pos+=1;}void ROP_DATA_VM(RelocOpWriterPBlock *rowPB){rowPB->rMachine.relocAddress += (((rowPB->relocInst[rowPB->pos]&0x1FF)+1)*4);rowPB->pos+=1;}void ROP_DESC_VM(RelocOpWriterPBlock *rowPB){rowPB->rMachine.relocAddress += (((rowPB->relocInst[rowPB->pos]&0x1FF)+1)*12);rowPB->pos+=1;}void ROP_DSC2_VM(RelocOpWriterPBlock *rowPB){rowPB->rMachine.relocAddress += (((rowPB->relocInst[rowPB->pos]&0x1FF)+1)*8);rowPB->pos+=1;}void ROP_VTBL_VM(RelocOpWriterPBlock *rowPB){rowPB->rMachine.relocAddress += (((rowPB->relocInst[rowPB->pos]&0x1FF)+1)*8);rowPB->pos+=1;}void ROP_SYMR_VM(RelocOpWriterPBlock *rowPB){idex = rowPB->rMachine.importIndex;raddr = rowPB->rMachine.relocAddress;for(i=0;i<((rowPB->relocInst[rowPB->pos]&0x1FF)+1);i++){	symT=0;	for(libSelect=0;libSelect<rowPB->llpb->libcount;libSelect++)	{		symSelect = rowPB->llpb->lib[libSelect].firstImportedSymbol;		for(k=0;k<rowPB->llpb->lib[libSelect].importedSymbolCount;k++,symSelect++,symT++)		{			if(symT==idex)			{				(*curr) = (LabelList*)NewPtr(sizeof(LabelList));				(*curr)->index = raddr;				(*curr)->next=nil;				bMarker = rowPB->llpb->sym[symSelect].classAndName & kPEFImpSymNameOffsetMask;				bmHolder = bMarker;				while(rowPB->llpb->name[bMarker]!=0){bMarker++;}				(*curr)->label = NewPtr((bMarker-bmHolder)+1);				for(j=0;j<bMarker-bmHolder;j++){(*curr)->label[j]=rowPB->llpb->name[bmHolder+j];}(*curr)->label[j]=0;				idex++;				raddr+=4;				curr = &((*curr)->next);				goto end;			}		}	}end:;}rowPB->rMachine.relocAddress += (((rowPB->relocInst[rowPB->pos]&0x1FF)+1)*4);rowPB->rMachine.importIndex += (rowPB->relocInst[rowPB->pos]&0x1FF)+1;rowPB->pos+=1;}void ROP_SYMB_VM(RelocOpWriterPBlock *rowPB){idex = (rowPB->relocInst[rowPB->pos]&0x1FF);raddr = rowPB->rMachine.relocAddress;{	symT=0;	for(libSelect=0;libSelect<rowPB->llpb->libcount;libSelect++)	{		symSelect = rowPB->llpb->lib[libSelect].firstImportedSymbol;		for(i=0;i<rowPB->llpb->lib[libSelect].importedSymbolCount;i++,symSelect++,symT++)		{			if(symT==idex)			{				(*curr) = (LabelList*)NewPtr(sizeof(LabelList));				(*curr)->index = raddr;				(*curr)->next = nil;				bMarker = rowPB->llpb->sym[symSelect].classAndName & kPEFImpSymNameOffsetMask;				bmHolder = bMarker;				while(rowPB->llpb->name[bMarker]!=0){bMarker++;}				(*curr)->label = NewPtr((bMarker-bmHolder)+1);				for(j=0;j<bMarker-bmHolder;j++){(*curr)->label[j]=rowPB->llpb->name[bmHolder+j];}(*curr)->label[j]=0;				idex++;				raddr+=4;				curr = &((*curr)->next);				goto end2;			}		}	}end2:;}rowPB->rMachine.relocAddress += 4;rowPB->rMachine.importIndex = (rowPB->relocInst[rowPB->pos]&0x1FF)+1;rowPB->pos+=1;}void ROP_CDIS_VM(RelocOpWriterPBlock *rowPB){rowPB->pos+=1;}void ROP_DTIS_VM(RelocOpWriterPBlock *rowPB){rowPB->pos+=1;}void ROP_SECN_VM(RelocOpWriterPBlock *rowPB){rowPB->rMachine.relocAddress += (((rowPB->relocInst[rowPB->pos]&0x1FF)+1)*4);rowPB->pos+=1;}void ROP_DELT_VM(RelocOpWriterPBlock *rowPB){rowPB->rMachine.relocAddress += ((rowPB->relocInst[rowPB->pos]&0xFFF)+1);rowPB->pos+=1;}void ROP_RPT_VM(RelocOpWriterPBlock *rowPB){long blockCount,repeatCount,i,j,savedPos;blockCount = ((rowPB->relocInst[rowPB->pos]&0xF00)>>8)+1;repeatCount = (rowPB->relocInst[rowPB->pos]&0xFF)+1;savedPos = rowPB->pos;for(i=0;i<repeatCount;i++){	rowPB->pos -= blockCount;	for(j=0;j<blockCount;j++)	{	relocOpCodeVM[(rowPB->relocInst[rowPB->pos]&0xFE00)>>9](rowPB);	}}rowPB->pos=savedPos;rowPB->pos+=1;}void ROP_LABS_VM(RelocOpWriterPBlock *rowPB){rowPB->rMachine.relocAddress += (*((long*)(&rowPB->relocInst[rowPB->pos])))&0x03FFFFFF;rowPB->pos+=2;}void ROP_LSYM_VM(RelocOpWriterPBlock *rowPB){idex = ((*((long*)(&rowPB->relocInst[rowPB->pos])))&0x03FFFFFF);raddr = rowPB->rMachine.relocAddress;{	symT=0;	for(libSelect=0;libSelect<rowPB->llpb->libcount;libSelect++)	{		symSelect = rowPB->llpb->lib[libSelect].firstImportedSymbol;		for(i=0;i<rowPB->llpb->lib[libSelect].importedSymbolCount;i++,symSelect++,symT++)		{			if(symT==idex)			{				(*curr) = (LabelList*)NewPtr(sizeof(LabelList));				(*curr)->index = raddr;				(*curr)->next = nil;				bMarker = rowPB->llpb->sym[symSelect].classAndName & kPEFImpSymNameOffsetMask;				bmHolder = bMarker;				while(rowPB->llpb->name[bMarker]!=0){bMarker++;}				(*curr)->label = NewPtr((bMarker-bmHolder)+1);				for(j=0;j<bMarker-bmHolder;j++){(*curr)->label[j]=rowPB->llpb->name[bmHolder+j];}(*curr)->label[j]=0;				idex++;				raddr+=4;				curr = &((*curr)->next);				goto end3;			}		}	}end3:;}rowPB->rMachine.relocAddress += 4;rowPB->rMachine.importIndex = ((*((long*)(&rowPB->relocInst[rowPB->pos])))&0x03FFFFFF)+1;rowPB->pos+=2;}void ROP_LRPT_VM(RelocOpWriterPBlock *rowPB){long blockCount,repeatCount,i,j,savedPos;blockCount = (((*((long*)(&rowPB->relocInst[rowPB->pos])))&0x03C00000)>>22)+1;repeatCount = (*((long*)(&rowPB->relocInst[rowPB->pos])))&0x003FFFFF;savedPos = rowPB->pos;for(i=0;i<repeatCount;i++){	rowPB->pos -= blockCount;	for(j=0;j<blockCount;j++)	{	relocOpCodeVM[(rowPB->relocInst[rowPB->pos]&0xFE00)>>9](rowPB);	}}rowPB->pos=savedPos;rowPB->pos+=2;}void ROP_LSEC_VM(RelocOpWriterPBlock *rowPB){rowPB->rMachine.relocAddress += (*((long*)(&rowPB->relocInst[rowPB->pos])))&0x003FFFFF;rowPB->pos+=2;}*/