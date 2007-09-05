/***************************************************************************
 *   Copyright (C) 2007 by Thorsten Wissmann   *
 *   towi89@web.de  http://towi16.piranho.de *
 *   http://www.kde-look.org/usermanager/search.php?username=twi           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/



#include "xmlparser.h"

int nLayer; //nLayer shows the depth of the current object


int xmlAttribute::nValueToInt()
{
    int nReturnValue;
    sscanf(szValue, "%d", &nReturnValue);
    return nReturnValue;
}

float  xmlAttribute::nValueToFloat()
{
    float nReturnValue;
    sscanf(szValue, "%f", &nReturnValue);
    return nReturnValue;
}

double  xmlAttribute::nValueToDouble()
{
    double nReturnValue;
    sscanf(szValue, "%lf", &nReturnValue);
    return nReturnValue;
}

void    xmlAttribute::SetValueToInt(int nNewValue)
{
    sprintf(szValue, "%d", nNewValue);
}
void    xmlAttribute::SetValueToFloat(float nNewValue)
{
    sprintf(szValue, "%f", nNewValue);
}
void    xmlAttribute::SetValueToDouble(double nNewValue)
{
    sprintf(szValue, "%lf", nNewValue);
}

void    xmlAttribute::SetName(char* szNewName)
{
    strncpy(szName, szNewName, 79);
    szName[79] = '\0';
}

void    xmlAttribute::SetValue(char* szNewValue)
{
    strncpy(szValue, szNewValue, 79);
    szValue[79] = '\0';
}

xmlObject::xmlObject()
{
    nAttributeCounter = 0;
    nObjectCounter = 0;
    nContentLength = 0;
    
    cAttributeList = NULL;
    cObjectList = NULL;
    szContent = NULL;
}

xmlObject::~xmlObject()
{
    free(cAttributeList);
    if (nContentLength)
        free(szContent);
    
    int i;
    for (i = 0; i < nObjectCounter; i++)
    {
        delete cObjectList[i];
    }
    delete cObjectList;
}


char*          xmlObject::szGetName( void )
{
    return szName;
}

void           xmlObject::setName(char* szNewName)
{
    strncpy(szName, szNewName, 79);
    szName[79] = '\0';
}

/* START OF ATTRIBUTE FUNCTIONS */

long           xmlObject::nSetAttributeCounter(int nNewAttributeCounter)
{
    if (nNewAttributeCounter < 0)
        return ErrorInvalidParameters;
    if(nNewAttributeCounter == nAttributeCounter)
        return 0;
    
    xmlAttribute* cOldAttributeList = cAttributeList;
    int nOldAttributeCounter = nAttributeCounter;
    int nCurrentAttributeItem = 0;
    
    cAttributeList = (xmlAttribute*) malloc(nNewAttributeCounter * sizeof(xmlAttribute));
    if (cAttributeList == NULL)
        return ErrorNoMemory;
    
    while (nCurrentAttributeItem < nOldAttributeCounter && nCurrentAttributeItem < nNewAttributeCounter)
    {
        cAttributeList[nCurrentAttributeItem] = cOldAttributeList[nCurrentAttributeItem];
        nCurrentAttributeItem++;
    }
    free(cOldAttributeList);
    nAttributeCounter = nNewAttributeCounter;
    
    return 0;
}

long            xmlObject::nSetAttributeValueByName (char* szAttributeName, char* szAttributeValue)
{
    int i;
    for ( i = 0; i < nAttributeCounter; i++)
    {
        if(!strcmp(szAttributeName, cAttributeList[i].szName))
        {
            strcpy(cAttributeList[i].szValue, szAttributeValue);
        }
    }
    return ErrorInvalidParameters;
}

xmlAttribute*   xmlObject::cGetAttributeByName (char* szAttributeName)
{
    int i;
    for ( i = 0; i < nAttributeCounter; i++)
    {
        if(!strcmp(szAttributeName, cAttributeList[i].szName))
            return &cAttributeList[i];
    }
    return NULL;
}

int             xmlObject::nGetAttributeIdentifierByName (char* szAttributeName)
{
    int i;
    for ( i = 0; i < nAttributeCounter; i++)
    {
        if(!strcmp(szAttributeName, cAttributeList[i].szName))
            return i;
    }
    return -1;
}

int             xmlObject::nGetAttributeIdentifierByValue (char* szAttributeValue)
{
    int i;
    for ( i = 0; i < nAttributeCounter; i++)
    {
        if(!strcmp(szAttributeValue, cAttributeList[i].szValue))
            return i;
    }
    return -1;
}

xmlAttribute*   xmlObject::cGetAttributeByIdentifier (int nIdentifier)
{
    if (nIdentifier >= nAttributeCounter || nIdentifier < 0 )
        return NULL;
    return &cAttributeList[nIdentifier];
}

long            xmlObject::nSetAttributeByIdentifier (int nIdentifier, char* szAttributeName, char* szAttributeValue)
{
    if (nIdentifier >= nAttributeCounter || nIdentifier < 0 )
        return ErrorInvalidParameters;
    strcpy(cAttributeList[nIdentifier].szName, szAttributeName);
    strcpy(cAttributeList[nIdentifier].szValue, szAttributeValue);
    return 0;
}


long            xmlObject::nAddAttribute (char* szAttributeName, char* szAttributeValue)
{
    int nNewAttributeId;
    int nReturnValue = 0;
    
    if ((nReturnValue = nSetAttributeCounter(nAttributeCounter + 1)) != 0)
        return nReturnValue;
    nNewAttributeId = nAttributeCounter - 1; //new attribute is the last one in the list
    strcpy(cAttributeList[nNewAttributeId].szName, szAttributeName);
    strcpy(cAttributeList[nNewAttributeId].szValue, szAttributeValue);
    
    
    return 0;
}
int             xmlObject::nDeleteAttribute(int nIdentifier)
{
    if (!(nIdentifier < nAttributeCounter && nIdentifier >= 0))
        return ErrorInvalidParameters;
    if (nAttributeCounter <= 0)
        return ErrorInvalidParameters;
    
    xmlAttribute* cOldAttributeList = cAttributeList;
    int nOldAttributeCounter = nAttributeCounter;
    int nNewAttributeCounter = nAttributeCounter -1;
    int nCurrentAttributeItem = 0;
    
    cAttributeList = (xmlAttribute*) malloc(nNewAttributeCounter * sizeof(xmlAttribute));
    if (cAttributeList == NULL)
        return ErrorNoMemory;
    
    while (nCurrentAttributeItem < nOldAttributeCounter)
    {
        if ( nCurrentAttributeItem == nIdentifier )
            break;
        cAttributeList[nCurrentAttributeItem] = cOldAttributeList[nCurrentAttributeItem];
        nCurrentAttributeItem++;
    }
    nCurrentAttributeItem++;
    while (nCurrentAttributeItem < nOldAttributeCounter)
    {
        cAttributeList[nCurrentAttributeItem-1] = cOldAttributeList[nCurrentAttributeItem];
        nCurrentAttributeItem++;
    }
    free(cOldAttributeList);
    nAttributeCounter = nNewAttributeCounter;
    
    return 0;
}


int            xmlObject::nGetAttributeCounter()
{
    return nAttributeCounter;
}

/* END OF ATTRIBUTE FUNCTIONS */

/* START OF OBJECT FUNCTIONS */

long           xmlObject::nSetObjectCounter(int nNewObjectCounter)
{
    if (nNewObjectCounter < 0)
        return ErrorInvalidParameters;
    if(nNewObjectCounter == nObjectCounter)
        return 0;
    
    xmlObject** cOldObjectList = cObjectList;
    int nOldObjectCounter = nObjectCounter;
    int nCurrentObjectItem = 0;
    
    cObjectList = (xmlObject**) malloc(nNewObjectCounter * sizeof(xmlObject*));
    if (cObjectList == NULL)
        return ErrorNoMemory;
    
    while (nCurrentObjectItem < nOldObjectCounter && nCurrentObjectItem < nNewObjectCounter)
    {
        cObjectList[nCurrentObjectItem] = cOldObjectList[nCurrentObjectItem];
        nCurrentObjectItem++;
    }
    while (nCurrentObjectItem < nOldObjectCounter)        //free/delete not copied and so not used items from the Old List
    {
        delete cObjectList[nCurrentObjectItem];
        nCurrentObjectItem++;
    }
    while (nCurrentObjectItem < nNewObjectCounter)        //initialize new pointers in our new list
    {
        cObjectList[nCurrentObjectItem] = new xmlObject;
        nCurrentObjectItem++;
    }
    free(cOldObjectList);
    nObjectCounter = nNewObjectCounter;
    
    return 0;
}

int             xmlObject::nAddObject()
{
    if(nSetObjectCounter(nObjectCounter+1) != 0)
        return (-1);
    return (nObjectCounter-1);
    
}

int             xmlObject::nAddObject(char* szNewName)
{
    if(nSetObjectCounter(nObjectCounter+1) != 0)
        return (-1);
    if(szNewName)
        strncpy(cGetObjectByIdentifier(nObjectCounter-1)->szName, szNewName, 79);
    return (nObjectCounter-1);
    
}

int             xmlObject::nDeleteObject(int nIdentifier)
{
    if (!(nIdentifier < nObjectCounter && nIdentifier >= 0))
        return ErrorInvalidParameters;
    if (nObjectCounter <= 0)
        return ErrorInvalidParameters;
    
    xmlObject** cOldObjectList = cObjectList;
    int nOldObjectCounter = nObjectCounter;
    int nNewObjectCounter = nObjectCounter-1;
    int nCurrentObjectItem = 0;
    
    cObjectList = (xmlObject**) malloc(nNewObjectCounter * sizeof(xmlObject*));
    
    delete cOldObjectList[nIdentifier];
    
    if (cObjectList == NULL)
        return ErrorNoMemory;
    
    while (nCurrentObjectItem < nOldObjectCounter)
    {
        if(nCurrentObjectItem == nIdentifier)
            break;
        cObjectList[nCurrentObjectItem] = cOldObjectList[nCurrentObjectItem];
        nCurrentObjectItem++;
    }
    nCurrentObjectItem++;  //we don't want to copy cObjectList[nIdentifier]
    while (nCurrentObjectItem < nOldObjectCounter)
    {
        cObjectList[nCurrentObjectItem-1] = cOldObjectList[nCurrentObjectItem];
        nCurrentObjectItem++;
    }
    
    free(cOldObjectList);
    nObjectCounter = nNewObjectCounter;
    
    return 0;
}

int xmlObject::nDeleteObject(xmlObject* objectToDelete)
{
    int nIdentifier = -1;
    for(int i = 0; i < nObjectCounter; i++)
    {
        if(cObjectList[i] == objectToDelete)
        {
            nIdentifier = i;
            break;
        }
    }
    return nDeleteObject(nIdentifier);
}

xmlObject*      xmlObject::cGetObjectByIdentifier (int nIdentifier)
{
    if (nIdentifier >= nObjectCounter || nIdentifier < 0)
        return NULL;
    return cObjectList[nIdentifier];
}
xmlObject*      xmlObject::cGetObjectByName (char szObjectName[80])
{
    int i;
    for ( i = 0; i < nObjectCounter; i++)
    {
        if(!strcmp(szObjectName, cObjectList[i]->szName))
            return cObjectList[i];
    }
    return NULL;
}

int      xmlObject::nGetObjectIdentifierByAttributeValue (char szAttributeName[80], char szAttributeValue[80])
{
    int nCurrentObject;
    for ( nCurrentObject = 0; nCurrentObject < nObjectCounter; nCurrentObject++)
    {
        if(!strcmp(cObjectList[nCurrentObject]->cGetAttributeByName(szAttributeName)->szValue, szAttributeValue))
            return nCurrentObject;
    }
    return -1;
}

xmlObject*      xmlObject::cGetObjectByAttributeValue (char szAttributeName[80], char szAttributeValue[80])
{
    int nCurrentObject;
    for ( nCurrentObject = 0; nCurrentObject < nObjectCounter; nCurrentObject++)
    {
        if (cObjectList[nCurrentObject]->cGetAttributeByName(szAttributeName) == NULL)
            break;
        if(!strcmp(cObjectList[nCurrentObject]->cGetAttributeByName(szAttributeName)->szValue, szAttributeValue))
            return cObjectList[nCurrentObject];
    }
    return NULL;
}

int            xmlObject::nGetObjectCounter()
{
    return nObjectCounter;
}

/* END OF OBJECT FUNCTIONS */

/* START OF CONTENT FUNCTIONS */
char*          xmlObject::szGetContent ()
{
    if (nContentLength)
        return szContent;
    return NULL;
}

long           xmlObject::nSetContent ( char* szNewContent )
{
    if (nContentLength != (strlen(szNewContent) + 1))
    {
        nContentLength = (strlen(szNewContent) + 1);
        if(nContentLength)
            free(szContent);
        szContent = (char*) malloc(nContentLength * sizeof(char));
        if (szContent == NULL)
            return ErrorNoMemory;
    }
    strcpy(szContent, szNewContent);
    return 0;
}
/* END OF CONTENT FUNCTIONS */
        
        
        
/* START OF INPUT FUNCTIONS */

int ReadObjectFromBuf (xmlObject* TargetObject, char* szBuf, unsigned long nBufLength, unsigned long *nBufPosition)   
{
    unsigned long nBufPosMark = 0;
    long nCopyLength = 0;
    long nReturnValue = 0;
    char szStringBuf1[80];
    char szStringBuf2[80];
    //printf("tag at pos: %d\n", *nBufPosition);
    if (szBuf[*nBufPosition] != '<')
        SeekToChar(szBuf, nBufLength, nBufPosition, '<');
    (*nBufPosition)++;
    /* START: GET NAME OF TAG */
    while (szBuf[*nBufPosition] == ' ') //SEEK end of blanks = start of name
    {
        (*nBufPosition)++;
    }
    nBufPosMark = *nBufPosition;
    while ((szBuf[*nBufPosition] != '>') && (szBuf[*nBufPosition] != ' '))  //SEEK end of name = start of blanks
    {
        (*nBufPosition)++;
    } 
    nCopyLength = *nBufPosition - nBufPosMark ;
    if (nCopyLength > 80)
        nCopyLength = 80;
    strncpy(TargetObject->szName, &szBuf[nBufPosMark], nCopyLength);
    TargetObject->szName[nCopyLength] = '\0';
    /* END: GET NAME OF TAG */
    /* START: GET ATTRIBUTES OF TAG */
    while ((szBuf[*nBufPosition] != '>') && (szBuf[*nBufPosition] != '/') )
    {
        while (szBuf[*nBufPosition] == ' ') //SEEK end of blanks = start of firstAttribute
            (*nBufPosition)++;
        if ((szBuf[*nBufPosition] == '/') || (szBuf[*nBufPosition] == '>'))
            break;
        
        nBufPosMark = *nBufPosition; // 1. GET ATTRIBUTE NAME
        SeekToChar(szBuf, nBufLength, nBufPosition, '=');
        nCopyLength = *nBufPosition - nBufPosMark;
        if (nCopyLength > 80)
            nCopyLength = 80;
        strncpy(szStringBuf1, &szBuf[nBufPosMark], nCopyLength); //BACKUP attribute name
        szStringBuf1[nCopyLength] = '\0';
        //printf("found attribute: ->%s<-\n", szStringBuf1);
        while ((szBuf[*nBufPosition] != '\'') && (szBuf[*nBufPosition] != '\"'))
            (*nBufPosition)++;
        nBufPosMark = (*nBufPosition) +1; //mark NEXT index, because the current is the " or '   !
        if (szBuf[*nBufPosition] == '\'')
        {
            (*nBufPosition)++;
            SeekToChar(szBuf, nBufLength, nBufPosition, '\'');
        }
        if (szBuf[*nBufPosition] == '\"')
        {
            (*nBufPosition)++;
            SeekToChar(szBuf, nBufLength, nBufPosition, '\"');
        }
        nCopyLength = *nBufPosition - nBufPosMark;
        
        if (nCopyLength > 80)
            nCopyLength = 80;
        strncpy(szStringBuf2, &szBuf[nBufPosMark], nCopyLength); //BACKUP attribute value
        szStringBuf2[nCopyLength] = '\0';
        //printf("stringvalue: ->%s<-\n",szStringBuf2);
        
        TargetObject->nSetAttributeCounter(TargetObject->nGetAttributeCounter()+1);
        TargetObject->nSetAttributeByIdentifier(TargetObject->nGetAttributeCounter() - 1, szStringBuf1, szStringBuf2);
        (*nBufPosition)++;
    }
    /* END: GET ATTRIBUTES OF TAG */
    if (szBuf[*nBufPosition] == '/')
    {
        if (szBuf[*nBufPosition+1] != '>')
            return ErrorInvalidDataFormat;
        else
            return 0;
    }
    (*nBufPosition)++; //leave the >
    /* START: GET CONTENT OF TAG */
    while ((szBuf[*nBufPosition] == ' ')||(szBuf[*nBufPosition] == '\n')||(szBuf[*nBufPosition] == '\t')) //SEEK end of blanks = start of CONTENT
    {
        //printf("szBuf[*nBufPosition]=\'%d\'\n", szBuf[*nBufPosition]);
        (*nBufPosition)++;
    }
    nBufPosMark = *nBufPosition;
    
    SeekToChar(szBuf, nBufLength, nBufPosition, '<'); //SEEK start of next tag
    if (*nBufPosition != nBufPosMark)
    {
        szBuf[*nBufPosition] = '\0'; //CHEAT AND SIMULATE String-end
        TargetObject->nSetContent(&szBuf[nBufPosMark]);
        szBuf[*nBufPosition] = '<'; //re-get original character
    }
    /* END: GET CONTENT OF TAG */
    /* START: GET OBJECTS OF TAG */
    while (!(szBuf[*nBufPosition] == '<' && szBuf[*nBufPosition+1] == '/'))
    {
        TargetObject->nSetObjectCounter (TargetObject->nGetObjectCounter() + 1);
        if( (nReturnValue = ReadObjectFromBuf (TargetObject->cGetObjectByIdentifier(TargetObject->nGetObjectCounter() - 1), szBuf, nBufLength, nBufPosition))
             != 0)
            return nReturnValue;
        SeekToChar(szBuf, nBufLength, nBufPosition, '<'); //SEEK start of next tag
    }
    /* END: GET OBJECTS OF TAG */
    /* START: ANALYZE END-TAG */
    *nBufPosition += 2;
    while (szBuf[*nBufPosition] == ' ') //SEEK end of blanks = start of name of End-Tag
        (*nBufPosition)++;
    nBufPosMark = *nBufPosition;
    while ((szBuf[*nBufPosition] != ' ') && (szBuf[*nBufPosition] != '>')) //SEEK end of name of End-Tag
        (*nBufPosition)++;
    nCopyLength = *nBufPosition - nBufPosMark;
    strncpy(szStringBuf1, &szBuf[nBufPosMark], nCopyLength);
    szStringBuf1[nCopyLength] = '\0';
    if (strcmp( szStringBuf1, TargetObject->szName))
    {
        return ErrorInvalidDataFormat;
    }
    while (szBuf[*nBufPosition] != '>')
        (*nBufPosition)++;
    /* END: ANALYZE END-TAG */
    while ((szBuf[*nBufPosition] != '<') && (*nBufPosition < nBufLength)) //Seek next Tag
        (*nBufPosition)++;
    if (*nBufPosition > nBufLength)
        return ErrorEndOfFile;
    
    //printf("marker in end-tag of '%s'\n", TargetObject->szName);
    return 0;
}

int SeekToChar (char* szBuf, unsigned long nBufLength, unsigned long* nBufPosition, char cChar )
{
    if ( *nBufPosition >= nBufLength)
        return ErrorEndOfFile;
    
    while( szBuf[*nBufPosition] != cChar)
    {
        (*nBufPosition)++;
        if ( *nBufPosition >= nBufLength)
        {
            return ErrorEndOfFile;
        }
    }
    return 0;
}

int ReadFileToClass (char* szFilename, xmlObject* TargetObject)
{
    FILE*   pXmlFile;
    unsigned long    nFileLength;
    char*   szFileBuf;
    unsigned long    nFilePosition=0;
    
    if ((pXmlFile = fopen(szFilename, "r")) == NULL)
    {
        return ErrorFileNotFound;
    }
    fseek(pXmlFile, 0, SEEK_END);
    nFileLength = ftell(pXmlFile);
    fseek(pXmlFile, 0, SEEK_SET);
    szFileBuf = (char*) malloc( nFileLength * sizeof(char) );
    if ( !fread(szFileBuf, sizeof(char), nFileLength, pXmlFile) )
        return ErrorFileNotFound;
    fclose( pXmlFile ); //now, we don't need the file anymore, just the buffer
    
    /*analyzing our Buffer*/
    SeekToChar(szFileBuf, nFileLength, &nFilePosition, '<');
    nFilePosition++;
    //printf("INFO: Length: %d characters\n", nFileLength);
    if (szFileBuf[nFilePosition] == '?') //if its the header, then seek to end of tag
    {
        while (szFileBuf[nFilePosition] != '>')
        {
            if (szFileBuf[nFilePosition] == '\"')
            {
                SeekToChar(szFileBuf, nFileLength, &nFilePosition, '\"');
            }
            if (szFileBuf[nFilePosition] == '\'')
            {
                SeekToChar(szFileBuf, nFileLength, &nFilePosition, '\'');
            }
            nFilePosition++;
        }
        nFilePosition++;
    }
    else
        nFilePosition = 0;
    //szFileBuf[nFilePosition] = '\0';
    
    ReadObjectFromBuf (TargetObject, szFileBuf,  nFileLength, &nFilePosition);
    
    //printf("attribute of titel: ->%s<-\n", TargetObject->cGetObjectByName("titel")->cGetAttributeByName("name")->szValue);
//     printf("%s\n", szFileBuf);
    free(szFileBuf);
    return 0;
}

/* END OF INPUT FUNCTIONS */
/* START OF OUTPUT FUNCTIONS */
int WriteClassToFile (char* szFilename, xmlObject* SourceObject)
{
    FILE*   pXmlFile;
    char* szBuf;
    //unsigned long nBufPosition = 0;
    unsigned long nBufLength = 1;
//    unsigned long nBytesCopied = 0;
    int nReturnValue;
    if ((pXmlFile = fopen(szFilename, "w")) == NULL)
    {
        return ErrorFileNotFound;
    }
    nLayer = 0;
    szBuf = (char*) malloc(nBufLength * sizeof(char));
    EditLengthOfBuf (&szBuf, &nBufLength, nBufLength +4);
    szBuf[0] = '\0';
    if ((nReturnValue = WriteObjectToBuf (SourceObject, &szBuf, &nBufLength)) != 0)
        return nReturnValue;
    if (strlen(szBuf) != fwrite (szBuf, sizeof(char), strlen(szBuf), pXmlFile))
        return ErrorGeneral;
    fclose(pXmlFile);
    free (szBuf);
    //printf("File was written\n");
    return 0;
}
int WriteObjectToBuf (xmlObject* TargetObject, char** szBuf, unsigned long* nBufLength)
{
    int nResult = 0;
    int nCurrentAttribute;
    int nAttributeNameLength;
    int nAttributeValueLength;
    int nCurrentObject;
    int nCurrentLayer;
    nLayer++;
    if (nLayer > 1)
    {
        if ( (*nBufLength) < (strlen(*szBuf) + nLayer ))
        {
            if ((nResult = EditLengthOfBuf (szBuf, nBufLength, strlen(*szBuf) + nLayer ) ) != 0)
                return nResult;
        }
        for (nCurrentLayer = 1; nCurrentLayer < nLayer; nCurrentLayer++)
            strcat(*szBuf," ");
    }
    //printf("%s has layer number: %d\n", TargetObject->szName, nLayer);
    if ( (*nBufLength) < (strlen(*szBuf) + 2))
    {
        if ((nResult = EditLengthOfBuf (szBuf, nBufLength, strlen(*szBuf) +2)) != 0)
            return nResult;
    }
    strcat(*szBuf, "<");
    //write tag-name
    if ( (*nBufLength) < (strlen(*szBuf) + strlen(TargetObject->szName) +1) )
    {
        if ((nResult = EditLengthOfBuf (szBuf, nBufLength, strlen(*szBuf)  + strlen(TargetObject->szName) +1)) != 0)
            return nResult;
    }
    strcat( *szBuf, TargetObject->szName);
    //write Attributes
    
    for (nCurrentAttribute = 0; nCurrentAttribute < TargetObject->nGetAttributeCounter(); nCurrentAttribute++)
    {
        nAttributeNameLength = strlen(TargetObject->cGetAttributeByIdentifier(nCurrentAttribute)->szName);
        nAttributeValueLength = strlen(TargetObject->cGetAttributeByIdentifier(nCurrentAttribute)->szValue);
        
        if ( (*nBufLength) < (strlen(*szBuf) + nAttributeNameLength + nAttributeValueLength +5) )
        {
            if ((nResult = EditLengthOfBuf (szBuf, nBufLength, strlen(*szBuf) + nAttributeNameLength + nAttributeValueLength +5)) != 0)
                return nResult;
        }
        strcat( *szBuf, " ");
        strcat( *szBuf, TargetObject->cGetAttributeByIdentifier(nCurrentAttribute)->szName);
        strcat( *szBuf, "=\"");
        strcat( *szBuf, TargetObject->cGetAttributeByIdentifier(nCurrentAttribute)->szValue);
        strcat( *szBuf, "\"");
    }
    if ( (*nBufLength) < (strlen(*szBuf) + 2))
    {
        if ((nResult = EditLengthOfBuf (szBuf, nBufLength, strlen(*szBuf) +2)) != 0)
            return nResult;
    }
    strcat( *szBuf, ">");
    
    //Write Content
    if (TargetObject->szGetContent() != NULL)
    {
        if ( (*nBufLength) < (strlen(*szBuf) + strlen(TargetObject->szGetContent()) + 1) )
        {
            if ((nResult = EditLengthOfBuf (szBuf, nBufLength, strlen(*szBuf)  + strlen(TargetObject->szGetContent()) +1 )) != 0)
                return nResult;
        }
        strcat(*szBuf, TargetObject->szGetContent());
    
    }
    //write contained OBJECTS
    if (TargetObject->nGetObjectCounter() != 0)
    {
        if ( (*nBufLength) < (strlen(*szBuf) + 2) )
        {
            if ((nResult = EditLengthOfBuf (szBuf, nBufLength, strlen(*szBuf) +2 ) ) != 0)
                return nResult;
        }
        strcat( *szBuf, "\n");
        for (nCurrentObject = 0; nCurrentObject < (TargetObject->nGetObjectCounter()); nCurrentObject++)
        {
            
            if( ( nResult = WriteObjectToBuf (TargetObject->cGetObjectByIdentifier(nCurrentObject), szBuf, nBufLength)) != 0)
                return nResult;
            
            if ( (*nBufLength) < (strlen(*szBuf) + 2) )
            {
                if ((nResult = EditLengthOfBuf (szBuf, nBufLength, strlen(*szBuf) +2 ) ) != 0)
                    return nResult;
            }
            strcat(*szBuf,"\n");
        }
        
        if (nLayer > 1)
        {
            if ( (*nBufLength) < (strlen(*szBuf) + nLayer ))
            {
                if ((nResult = EditLengthOfBuf (szBuf, nBufLength, strlen(*szBuf) + nLayer ) ) != 0)
                    return nResult;
            }
            for (nCurrentLayer = 1; nCurrentLayer < nLayer; nCurrentLayer++)
                strcat(*szBuf," ");
        }
        //printf("%s has layer number: %d\n", TargetObject->szName, nLayer);
    }
        
    
    
    //write END-TAG  
    if ( (*nBufLength) < (strlen(*szBuf) + strlen(TargetObject->szName) +4) )
    {
        if ((nResult = EditLengthOfBuf (szBuf, nBufLength, strlen(*szBuf)  + strlen(TargetObject->szName)+4) ) != 0)
        {
            return nResult;
        }
    }
    strcat( *szBuf, "</");
    strcat( *szBuf, TargetObject->szName);
    strcat( *szBuf, ">");
    
    nLayer--;
    //    printf("strlen: %d\n", strlen(*szBuf));
    //    printf("BufLen: %d\n", *nBufLength);
    return 0;
}

int EditLengthOfBuf (char** szBuf, unsigned long* nCurrentLength, unsigned long nNewLength)
{
    if ((*nCurrentLength) == nNewLength)
        return 0;
    char* szNewBuf;
    int i;
    
    if ((szNewBuf = (char*) malloc(nNewLength * sizeof(char))) == NULL)
        return ErrorNoMemory;
    for (i = 0; (i < (*nCurrentLength)) && (i < nNewLength); i++)
    {
        szNewBuf[i] = (*szBuf)[i];
    }
    free (*szBuf);
    *nCurrentLength = nNewLength;
    (*szBuf) = szNewBuf;
    return 0;
}
        
int PutObjectToScreen (xmlObject* SourceObject)
{
    char* szBuf;
//    unsigned long nBufPosition = 0;
    unsigned long nBufLength = 1;
    
    nLayer = 0;
    szBuf = (char*) malloc(nBufLength * sizeof(char));
    EditLengthOfBuf (&szBuf, &nBufLength, nBufLength +4);
    szBuf[0] = '\0';
    WriteObjectToBuf (SourceObject, &szBuf, &nBufLength);
    printf("Content of file:\n%s\nEND OF FILE\n", szBuf);
    free (szBuf);
    return 0;
}
/* END OF OUTPUT FUNCTIONS */


