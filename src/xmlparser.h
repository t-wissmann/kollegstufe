/***************************************************************************
 *   Copyright (C) 2007 by Thorsten Wissmann                               *
 *   E-Mail: kollegstufe@thorsten-wissmann.de                              *
 *   Homepage: www.thorsten-wissmann.de                                    *
 *                                                                         *
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

#ifndef __NM_FILE_IO_H_20060903
#define __NM_FILE_IO_H_20060903
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "error.h"


class xmlAttribute
{
    xmlAttribute(){
        SetName("");
        SetValue("");
    };
    public:
        char    szName[80];
        char    szValue[80];
        int     nValueToInt(void);
        float   nValueToFloat(void);
        double  nValueToDouble(void);
        void    SetValueToInt(int nNewValue);
        void    SetValueToFloat(float nNewValue);
        void    SetValueToDouble(double nNewValue);
        void    SetName(char* szNewName);
        void    SetValue(char* szNewValue);
        char*   name(){ return szName; };
        char*   value(){ return szValue; };
};

class xmlObject
{
    public:
        //MEMBER_Variables
        char           szName[80];
        
        //FUNCTIONS
        //CON-/DESTRUCTOR
        xmlObject();
        ~xmlObject();
        //MEMBER-Functions
        //about attributes
        long            nSetAttributeCounter (int nNewAttributeCounter);
        int             nGetAttributeCounter ();
        long            nSetAttributeValueByName (char* szAttributeName, char* szAttributeValue);
        xmlAttribute*   cGetAttributeByName (char* szAttributeName);
        long            nSetAttributeByIdentifier (int nIdentifier, char* szAttributeName, char* szAttributeValue);
        long            nAddAttribute (char* szAttributeName, char* szAttributeValue);
        int             nDeleteAttribute(int nIdentifier);
        xmlAttribute*   cGetAttributeByIdentifier (int nIdentifier);
        int             nGetAttributeIdentifierByName (char* szAttributeName);
        int             nGetAttributeIdentifierByValue (char* szAttributeValue);
        //about objects
        long            nSetObjectCounter (int nNewObjectCounter);
        int             nGetObjectCounter ();
        int             nAddObject(void);
        int             nAddObject(char* szNewName);
        int             nDeleteObject(int nIdentifier);
        int             nDeleteObject(xmlObject* objectToDelete);
        xmlObject*      cGetObjectByIdentifier (int nIdentifier);
        xmlObject*      cGetObjectByName (char szObjectName[80]);
        xmlObject*      cGetObjectByAttributeValue (char szAttributeName[80], char szAttributeValue[80]);
        int             nGetObjectIdentifierByAttributeValue (char szAttributeName[80], char szAttributeValue[80]);
        void            swapObjects(xmlObject* objectOne, xmlObject* objectTwo);
        void            swapObjects(int objectOne, int objectTwo);
        void            moveObjectTo(xmlObject* objectToMove, int newPosition);
        void            moveObjectTo(int idToMove, int newPosition);
        int             nGetIdentifierOf(xmlObject* objectToFind) const;
        //about content
        char*          szGetContent ( void );
        void            appendToContent(char* szStringToAppend, bool createSpaceBetween = 1);
        long            nSetContent ( char* szNewContent );
        long            nGetContentLength(){return nContentLength;};
        //about name
        char*          szGetName( void );
        char*          name(){ return szGetName(); };
        void           setName(char* szNewName);
        
//        xmlAttribute*  cGetAttribute(int n AttributeCounter);
        
    private:
        int            nAttributeCounter;
        xmlAttribute*  cAttributeList;
        int            nObjectCounter;
        xmlObject**    cObjectList; //the list is an array, constisting of pointers to other objects
        int            nContentLength; //Content contains the characters between start- and end-tag
        char*          szContent;     //a tag should have either objects or content, not both!

};



int ReadFileToClassOLD (char* szFilename, xmlObject* TargetObject);
int ReadFileToClass (char* szFilename, xmlObject* TargetObject);
int ReadObjectFromBuf (xmlObject* TargetObject, char* szBuf, unsigned long nBufLength, unsigned long* nBufPosition);
int WriteObjectToBuf (xmlObject* TargetObject, char** szBuf, unsigned  long* nBufLength);
int SeekToChar (char* szBuf, unsigned long nBufLength, unsigned long* nBufPosition,char cChar );
int WriteClassToFile (char* szFilename, xmlObject* SourceObject);
int PutObjectToScreen (xmlObject* SourceObject);
int EditLengthOfBuf (char** szBuf, unsigned long* nCurrentLength, long unsigned nNewLength);

#else
#endif

