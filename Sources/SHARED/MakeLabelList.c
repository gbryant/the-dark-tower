Copyright 2016 Gregory BryantLicensed under the Apache License, Version 2.0 (the "License");you may not use this file except in compliance with the License.You may obtain a copy of the License at    http://www.apache.org/licenses/LICENSE-2.0Unless required by applicable law or agreed to in writing, softwaredistributed under the License is distributed on an "AS IS" BASIS,WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.See the License for the specific language governing permissions andlimitations under the License./***********************************************************************/#include "ProjectHeaders.h"LabelList* MakeLabelList(LabelListPBlock*	llPB){LabelList				*topLabelList;topLabelList = nil;if(gGlobalSettings.labels){	if(gGlobalSettings.bfspeed)	{	topLabelList = BranchFinder2(llPB->codeLength,llPB->codeSection,llPB->rm);	}	else	{	topLabelList = BranchFinder(llPB->codeLength,llPB->codeSection,llPB->rm);	}}if(!gAborted){	InsertFragPoints(&topLabelList,&llPB->fragPoint);	if(llPB->codeExport)	{	InsertExports(&topLabelList,llPB->codeExport);	}}return topLabelList;}