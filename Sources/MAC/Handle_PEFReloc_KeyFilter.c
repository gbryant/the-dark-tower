Copyright 2016 Gregory BryantLicensed under the Apache License, Version 2.0 (the "License");you may not use this file except in compliance with the License.You may obtain a copy of the License at    http://www.apache.org/licenses/LICENSE-2.0Unless required by applicable law or agreed to in writing, softwaredistributed under the License is distributed on an "AS IS" BASIS,WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.See the License for the specific language governing permissions andlimitations under the License./***********************************************************************/#include <ProjectHeaders.h>#define ERROR_4BIT "Argument Must Be <= 15"#define ERROR_4BIT_M1 "Argument Must Be <= 16"#define ERROR_6BIT "Argument Must Be <= 63"#define ERROR_8BIT "Argument Must Be <= 255"#define ERROR_8BIT_M1 "Argument Must Be <= 256"#define ERROR_9BIT "Argument Must Be <= 511"#define ERROR_9BIT_M1 "Argument Must Be <= 512"#define ERROR_12BIT_M1 "Argument Must Be <= 4096"#define ERROR_22BIT "Argument Must Be <= 4194303"#define ERROR_26BIT "Argument Must Be <= 67108863"/*#define DEFAULT_FILTER														\																			\if(*charCode==kDelete||*charCode==kRightArrow||*charCode==kLeftArrow		\||*charCode==kUpArrow||*charCode==kDownArrow)								\{returnVal=true;goto end;}														\if(*charCode>0x39||*charCode<0x30){returnVal=false;goto end;}*/ControlKeyFilterResult Handle_PEFReloc_KeyFilter(ControlRef theControl, SInt16 *keyCode, SInt16 *charCode, EventModifiers *modifiers){unsigned long					size;long							data,textSize,outSize,i,j;char							*charBuffer=0,*tempBuffer=0;ControlEditTextSelectionRec		theSelection;unsigned long					arg;CFStringRef						cfStr;Boolean							returnVal=true;long							controlType;GetControlProperty(theControl,kCreatorCode,kControlType,sizeof(long),&size,&controlType);//-----if(*charCode==kDelete||*charCode==kRightArrow||*charCode==kLeftArrow||*charCode==kUpArrow||*charCode==kDownArrow){returnVal=true;goto end;}if(*charCode>0x39||*charCode<0x30){returnVal=false;goto end;}//-----GetControlDataSize(theControl,kControlEditTextPart,kControlEditTextTextTag,&textSize);charBuffer = NewPtr(textSize);GetControlData(theControl,kControlEditTextPart,kControlEditTextTextTag,textSize,charBuffer,&outSize);GetControlData(theControl,kControlEditTextPart,kControlEditTextSelectionTag,sizeof(ControlEditTextSelectionRec),&theSelection,&outSize);if(theSelection.selEnd-theSelection.selStart){if(theSelection.selStart==0 && theSelection.selEnd!=textSize && *charCode=='0'){returnVal=false;goto end;}tempBuffer = NewPtr(textSize-(theSelection.selEnd-theSelection.selStart)+1);for(i=0;i<theSelection.selStart;i++){tempBuffer[i]=charBuffer[i];}tempBuffer[i]=*charCode;i++;j=i;for(;i<theSelection.selEnd;i++){}for(;j<textSize;i++,j++){tempBuffer[j]=charBuffer[i];}tempBuffer[j]=0;}else{if(theSelection.selStart==0 && textSize && *charCode=='0'){returnVal=false;goto end;}if(theSelection.selStart==1 && charBuffer[0]=='0'){returnVal=false;goto end;}tempBuffer = NewPtr(textSize+1);for(i=0;i<theSelection.selStart;i++){tempBuffer[i]=charBuffer[i];}tempBuffer[i]=*charCode;i++;for(;i<textSize+1;i++){tempBuffer[i]=charBuffer[i-1];}tempBuffer[i]=0;}cfStr = CFStringCreateWithCString(CFAllocatorGetDefault(),tempBuffer,kCFStringEncodingMacRoman);arg = (unsigned long)CFStringGetIntValue(cfStr);switch(controlType){	case k4BIT:	{		if(arg>0xF)		{		SetControlData(gAppControls.rlocMessageText, kControlEditTextPart,kControlStaticTextTextTag,sizeof(ERROR_4BIT),ERROR_4BIT);		Draw1Control(gAppControls.rlocMessageText);		gMustClearRelocMessage=true;		returnVal=false;		}		else		{		gSelectedRelocItem->args->value[gSelectedRelocItem->args->selector]=arg;		}	break;	}	case k4BIT_M1:	{		if(arg>0x10)		{		SetControlData(gAppControls.rlocMessageText, kControlEditTextPart,kControlStaticTextTextTag,sizeof(ERROR_4BIT_M1),ERROR_4BIT_M1);		Draw1Control(gAppControls.rlocMessageText);		gMustClearRelocMessage=true;		returnVal=false;		}		else		{		gSelectedRelocItem->args->value[gSelectedRelocItem->args->selector]=arg;		}	break;	}	case k6BIT:	{		if(arg>0x3F)		{		SetControlData(gAppControls.rlocMessageText, kControlEditTextPart,kControlStaticTextTextTag,sizeof(ERROR_6BIT),ERROR_6BIT);		Draw1Control(gAppControls.rlocMessageText);		gMustClearRelocMessage=true;		returnVal=false;		}		else		{		gSelectedRelocItem->args->value[gSelectedRelocItem->args->selector]=arg;		}	break;	}	case k8BIT:	{		if(arg>0xFF)		{		SetControlData(gAppControls.rlocMessageText, kControlEditTextPart,kControlStaticTextTextTag,sizeof(ERROR_8BIT),ERROR_8BIT);		Draw1Control(gAppControls.rlocMessageText);		gMustClearRelocMessage=true;		returnVal=false;		}		else		{		gSelectedRelocItem->args->value[gSelectedRelocItem->args->selector]=arg;		}	break;	}	case k8BIT_M1:	{		if(arg>0x100)		{		SetControlData(gAppControls.rlocMessageText, kControlEditTextPart,kControlStaticTextTextTag,sizeof(ERROR_8BIT_M1),ERROR_8BIT_M1);		Draw1Control(gAppControls.rlocMessageText);		gMustClearRelocMessage=true;		returnVal=false;		}		else		{		gSelectedRelocItem->args->value[gSelectedRelocItem->args->selector]=arg;		}	break;	}	case k9BIT:	{		if(arg>0x1FF)		{		SetControlData(gAppControls.rlocMessageText, kControlEditTextPart,kControlStaticTextTextTag,sizeof(ERROR_9BIT),ERROR_9BIT);		Draw1Control(gAppControls.rlocMessageText);		gMustClearRelocMessage=true;		returnVal=false;		}		else		{		gSelectedRelocItem->args->value[gSelectedRelocItem->args->selector]=arg;		}	break;	}	case k9BIT_M1:	{		if(arg>0x200)		{		SetControlData(gAppControls.rlocMessageText, kControlEditTextPart,kControlStaticTextTextTag,sizeof(ERROR_9BIT_M1),ERROR_9BIT_M1);		Draw1Control(gAppControls.rlocMessageText);		gMustClearRelocMessage=true;		returnVal=false;		}		else		{		gSelectedRelocItem->args->value[gSelectedRelocItem->args->selector]=arg;		}	break;	}	case k12BIT_M1:	{		if(arg>0x1000)		{		SetControlData(gAppControls.rlocMessageText, kControlEditTextPart,kControlStaticTextTextTag,sizeof(ERROR_12BIT_M1),ERROR_12BIT_M1);		Draw1Control(gAppControls.rlocMessageText);		gMustClearRelocMessage=true;		returnVal=false;		}		else		{		gSelectedRelocItem->args->value[gSelectedRelocItem->args->selector]=arg;		}	break;	}	case k22BIT:	{		if(arg>0x003FFFFF)		{		SetControlData(gAppControls.rlocMessageText, kControlEditTextPart,kControlStaticTextTextTag,sizeof(ERROR_22BIT),ERROR_22BIT);		Draw1Control(gAppControls.rlocMessageText);		gMustClearRelocMessage=true;		returnVal=false;		}		else		{		gSelectedRelocItem->args->value[gSelectedRelocItem->args->selector]=arg;		}	break;	}	case k26BIT:	{		if(arg>0x03FFFFFF)		{		SetControlData(gAppControls.rlocMessageText, kControlEditTextPart,kControlStaticTextTextTag,sizeof(ERROR_26BIT),ERROR_26BIT);		Draw1Control(gAppControls.rlocMessageText);		gMustClearRelocMessage=true;		returnVal=false;		}		else		{		gSelectedRelocItem->args->value[gSelectedRelocItem->args->selector]=arg;		}	break;	}	default:	{	returnVal=false;	break;	}}end:;if(tempBuffer){DisposePtr(tempBuffer);}if(charBuffer){DisposePtr(charBuffer);}return returnVal;}