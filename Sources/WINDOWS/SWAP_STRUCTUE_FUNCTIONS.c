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

#define swaplong(X)				\
a = *((unsigned char*)X+0);		\
b = *((unsigned char*)X+1);		\
c = *((unsigned char*)X+2);		\
d = *((unsigned char*)X+3);		\
*X = (a<<24)+(b<<16)+(c<<8)+d;

#define swapshort(X)			\
a = *((unsigned char*)(X)+0);	\
b = *((unsigned char*)(X)+1);	\
*X = (a<<8)+b;

void Swap_PEF_CONTAINER_HEADER(PEFContainerHeader *header)
{
unsigned char a,b,c,d;

swaplong(&header->tag1)
swaplong(&header->tag2)
swaplong(&header->architecture)
swaplong(&header->formatVersion)
swaplong(&header->dateTimeStamp)
swaplong(&header->oldDefVersion)
swaplong(&header->oldImpVersion)
swaplong(&header->currentVersion)
swapshort(&header->sectionCount)
swapshort(&header->instSectionCount)
swaplong(&header->reservedA)
}

void Swap_PEF_LOADER_INFO_HEADER(PEFLoaderInfoHeader	*header)
{
unsigned char a,b,c,d;

swaplong(&header->mainSection)
swaplong(&header->mainOffset)
swaplong(&header->initSection)
swaplong(&header->initOffset)
swaplong(&header->termSection)
swaplong(&header->termOffset)
swaplong(&header->importedLibraryCount)
swaplong(&header->totalImportedSymbolCount)
swaplong(&header->relocSectionCount)
swaplong(&header->relocInstrOffset)
swaplong(&header->loaderStringsOffset)
swaplong(&header->exportHashOffset)
swaplong(&header->exportHashTablePower)
swaplong(&header->exportedSymbolCount)
}

void Swap_PEF_SECTION_HEADER(PEFSectionHeader	*header)
{
unsigned char a,b,c,d;

swaplong(&header->nameOffset)
swaplong(&header->defaultAddress)
swaplong(&header->totalLength)
swaplong(&header->unpackedLength)
swaplong(&header->containerLength)
swaplong(&header->containerOffset)
}

void Swap_PEF_IMPORTED_LIBRARY(PEFImportedLibrary	*implib)
{
unsigned char a,b,c,d;

swaplong(&implib->nameOffset)
swaplong(&implib->oldImpVersion)
swaplong(&implib->currentVersion)
swaplong(&implib->importedSymbolCount)
swaplong(&implib->firstImportedSymbol)
swapshort(&implib->reservedB)
}

void Swap_PEF_EXPORTED_SYMBOL(PEFExportedSymbol	*exsym)
{
unsigned char a,b,c,d;

swaplong(&exsym->classAndName)
swaplong(&exsym->symbolValue)
swapshort(&exsym->sectionIndex)
}

void Swap_PEFLoaderRelocationHeader(PEFLoaderRelocationHeader	*header)
{
unsigned char a,b,c,d;

swapshort(&header->sectionIndex)
swapshort(&header->reservedA)
swaplong(&header->relocCount)
swaplong(&header->firstRelocOffset)
}


void Swap_MACH_HEADER(struct mach_header *header)
{
unsigned char a,b,c,d;

swaplong(&header->magic)
swaplong(&header->cputype)
swaplong(&header->cpusubtype)
swaplong(&header->filetype)
swaplong(&header->ncmds)
swaplong(&header->sizeofcmds)
swaplong(&header->flags)
}

void Swap_MACBINARYIII_HEADER(MacBinaryIIIHeader *header)
{
unsigned char a,b,c,d;

swaplong(&header->fileType)
swaplong(&header->fileCreator)
swapshort(&header->windowPosV)
swapshort(&header->windowPosH)
swapshort(&header->folderID)
swaplong(&header->dataForkLength)
swaplong(&header->resourceForkLength)
swaplong(&header->creationDate)
swaplong(&header->modifiedDate)
swapshort(&header->getInfoLength)
swaplong(&header->signature)
swaplong(&header->totalFilesLength)
swapshort(&header->secondaryHeaderLength)
swapshort(&header->crc)
}

void Swap_LONG(void	*aLong)
{
unsigned char a,b,c,d;

swaplong((unsigned long*)aLong)
}

void Swap_SHORT(unsigned short	*aShort)
{
unsigned char a,b,c,d;

swapshort(aShort)
}
