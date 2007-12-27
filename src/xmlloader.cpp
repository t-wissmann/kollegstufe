/***************************************************************************
 *   Copyright (C) 2007 by Thorsten Wissmann                               *
 *   E-Mail: kollegstufe@thorsten-wissmann.de                              *
 *   Homepage: www.thorsten-wissmann.de                                    *
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
#include "xmlloader.h"
#include "xmlparser.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"


#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif



xmlLoader::xmlLoader()
{
    // initing members about buf
    m_nParsingPosition = 0;
    m_szBuf = NULL;
    m_nBufSize = 0;
}


xmlLoader::~xmlLoader()
{
    resizeBuf(0);
}


bool xmlLoader::resizeBuf(unsigned int newBufSize)
{
    if(m_szBuf != NULL)
    {
        free(m_szBuf);
        m_szBuf = NULL;
    }
    m_nBufSize = newBufSize;
    
    if(m_nBufSize > 0)
    {
        m_szBuf = (char*) malloc(sizeof(char) * m_nBufSize);
        if( m_szBuf == NULL)
        {
            return FALSE;
        }
    }
    else
    {
        m_szBuf = NULL;
    }
    return TRUE;
}




    
bool xmlLoader::loadFileToClass(char* szPath, xmlObject* targetClass)
{
    if(targetClass == NULL)
    {
        return FALSE;
    }
    if(!loadFileToBuf(szPath))
    {
        return FALSE;
    }
    m_nParsingPosition = 0; // init Position
    // seek start of information:
    goToNextRelevantPosition();
    if(m_szBuf[m_nParsingPosition] == '\0')
    {
        // file is empty , there is nothing to do
        return TRUE;
    }
    
    return parseBufToObject(targetClass);
}


bool xmlLoader::parseBufToObject(xmlObject* target)
{
    if(m_szBuf == NULL || target == NULL)
    {
        return FALSE;
    }
    
    return parseNextTag(target);
    
}

bool xmlLoader::parseNextTag(xmlObject* target)
{
    if(m_szBuf == NULL || target == NULL)
    {
        return FALSE;
    }
    goToNextRelevantPosition();
    seekInBufTo('<');
    m_nParsingPosition++; // if tag looks like this: "<  header>"
    seekInBufTo(' ', FALSE);
    // parse tag name
    if(!parseTagName(target))
    {
        return FALSE;
    }
    goToNextRelevantPosition();
    if(!parseAttributes(target))
    {
        return FALSE;
    }
    if(!goToNextRelevantPosition())
    {
        return FALSE;
    }
    
    if(m_szBuf[m_nParsingPosition] == '/')
    {
        m_nParsingPosition++; // <tag/->><-  ...
        m_nParsingPosition++;// go out of tag: <tag/>-> <-  ..
        // now the tag has finished -> we can leave successful
        return TRUE;
        
    }
    if(m_szBuf[m_nParsingPosition] == '\0')
    {// tag wasn't closed in file
        return FALSE;
    }
    
    if(m_szBuf[m_nParsingPosition] == '>')
    {
        m_nParsingPosition++;// go out of tag: <tag\>-> <-
    }
    
    
    
    while(1)
    {
        
        if(!goToNextRelevantPosition())
        {
            return FALSE;
        }
        if(m_szBuf[m_nParsingPosition] == '<' && m_szBuf[m_nParsingPosition+1] == '/')
        {// if there is an end tag
            // go into it
            m_nParsingPosition += 2; // </->t<-ag>
            // search for end of tag
            seekInBufTo('>'); // </tag-><<-
            //go out of tag
            m_nParsingPosition++; // </tag>-> <-
            // there are no more things to do
            return TRUE;
        }
        
        // CONTENT
        if(m_szBuf[m_nParsingPosition] != '<')
        {
            // only parse content if there is no tag
            if(!parseContent(target))
            {
                return FALSE;
            }
            continue;
        
        }
        
        // CHILD TAGS
        // if next tag isn't an end tag
        // there is a child tag:
        xmlObject* newTag = target->cAddObject();
        // 2. Parse it:
        if(!parseNextTag(newTag))
        {
            return FALSE;
        }
    }
    return TRUE;
}


bool xmlLoader::parseTagName(xmlObject* target)
{
    if(m_szBuf == NULL || target == NULL)
    {
        return FALSE;
    }
    unsigned int nameStartPosition = m_nParsingPosition;
    seekInBufTo(' ', '/', '>'); // seek to end of name
    
    char oldChar = m_szBuf[m_nParsingPosition]; // backup char at this position
    m_szBuf[m_nParsingPosition] = '\0'; // simulate string end
    // write name to target
    target->setName(&(m_szBuf[nameStartPosition]));
    //restore old buf
    m_szBuf[m_nParsingPosition] = oldChar;
    return TRUE;
}


bool xmlLoader::parseAttributes(xmlObject* target)
{
    if(m_szBuf == NULL || target == NULL)
    {
        return FALSE;
    }
    
    goToNextRelevantPosition();
    
    while(m_szBuf[m_nParsingPosition] != '/'
          && m_szBuf[m_nParsingPosition] != '>'
          && m_szBuf[m_nParsingPosition] != '\0') // until tag end
    {
        if(!parseNextAttribute(target))
        {
            return FALSE;
        }
        
        if(!goToNextRelevantPosition())
        {
            return FALSE;    
        }
            
    }
    return TRUE;
}

bool xmlLoader::parseNextAttribute(xmlObject* target)
{
    if(m_szBuf == NULL || target == NULL)
    {
        return FALSE;
    }
    if(m_szBuf[m_nParsingPosition] == '\0' || m_szBuf[m_nParsingPosition] == '>')
    {
        return TRUE;
    }
    
    long attributeid = target->nAddAttribute("", "");
    xmlAttribute* attribute = target->cGetAttributeByIdentifier(attributeid);
    
    if(attribute == NULL)
    {
        return FALSE;
    }
    
    unsigned int nameposition = m_nParsingPosition; //backup start of name
    seekInBufTo(' ', '=', '\0');//seek to end of name
    char oldChar = m_szBuf[m_nParsingPosition]; // backup char
    m_szBuf[m_nParsingPosition] = '\0';
    
    attribute->SetName(&(m_szBuf[nameposition]));
    //reset old char at m_nParsingPosition
    m_szBuf[m_nParsingPosition] = oldChar;
    
    //seek to start of value
    seekInBufTo('\"', '\'', '\0');
    char valueindicator = m_szBuf[m_nParsingPosition]; // if value is in ' or in "
    
    
    m_nParsingPosition++; // go into value: name="->V<-alue"
    unsigned int valueposition = m_nParsingPosition;
    
    //seek to end of value:
    seekInBufTo(valueindicator); // name="Value->"<-
    
    
    m_szBuf[m_nParsingPosition] = '\0'; //replace valueindicator by '\0'
    attribute->SetValue(&(m_szBuf[valueposition])); // write value to attributeclass
    m_szBuf[m_nParsingPosition] = valueindicator; //restore old char
    
    m_nParsingPosition++; // leave ' or ": name="value"-> <-
    
    return TRUE;
}

bool xmlLoader::parseContent(xmlObject* target)
{
    if(target == NULL || m_szBuf == NULL)
    {
        return FALSE;
    }
    
    seekInBufTo(' ', FALSE); // seek to start of content
    while(m_szBuf[m_nParsingPosition] == '\n') // repeat it if there is just a newline
    {
        m_nParsingPosition++;
        seekInBufTo(' ', FALSE); // seek to start of content
    }
    unsigned int contentstart = m_nParsingPosition; // backup start of content
    seekInBufTo('<'); // seek to start of next tag or end-tag
    char oldChar = m_szBuf[m_nParsingPosition]; // backup char at this position
    m_szBuf[m_nParsingPosition] = '\0'; // simulate string end
    if(contentstart >= m_nParsingPosition) // if there is no real content
    {
        // do nothing
    }else
    {
        
        target->appendToContent(&(m_szBuf[contentstart])); // write content to target class
    }
    m_szBuf[m_nParsingPosition] = oldChar; // restore old buf
    // done
    return TRUE;
}


bool xmlLoader::goToNextRelevantPosition()
{
    if(m_szBuf == NULL || m_nParsingPosition > m_nBufSize || m_szBuf[m_nParsingPosition] == '\0')
    {
        return FALSE;
    }
    seekInBufTo(' ', FALSE);
    while(m_szBuf[m_nParsingPosition] == '\n' || m_szBuf[m_nParsingPosition] == '\t')
    {
        m_nParsingPosition++;
        seekInBufTo(' ', FALSE);
    }
    return TRUE;
}

bool xmlLoader::seekInBufTo(char c1, char c2, char c3, char c4, char c5, char c6, char c7, char c8, char c9)
{
    if(m_szBuf == NULL)
    {
        return FALSE;
    }
    char cc = '\0'; // current char
    for( ;m_nParsingPosition < m_nBufSize ; m_nParsingPosition++ )
    {
         // search while char at parsing position is (un)equal character
        cc = m_szBuf[m_nParsingPosition];
        if(cc == c1 ||cc == c2 ||cc == c3 ||cc == c4 ||cc == c5 ||cc == c6 ||cc == c7 ||cc == c8 ||cc == c9)
        {
            return TRUE;
        }
    }
    return FALSE;
}

bool xmlLoader::seekInBufTo(char  character,   bool equalOrUnequal)
{
    if(m_szBuf == NULL)
    {
        return FALSE;
    }
    for( ;m_nParsingPosition < m_nBufSize ; m_nParsingPosition++ )
    {
         // search while char at parsing position is (un)equal character
        if(m_szBuf[m_nParsingPosition] == '\0')
        {
            return FALSE;
        }
        if(equalOrUnequal)// TRUE : search for char that is equal, search for char that is unequal
        {
            if(m_szBuf[m_nParsingPosition] == character)
            {
                return TRUE;
            }
        }
        else
        {
            if(m_szBuf[m_nParsingPosition] != character)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

bool xmlLoader::seekInBufTo(char* string)
{
    if(m_szBuf == NULL)
    {
        return FALSE;
    }
    if(string == NULL)
    {
        return FALSE;
    }
    if(strlen(string) == 0)
    {
        return FALSE;
    }
    if(strlen(string) == 1)
    {
        return seekInBufTo(string[0]);
    }
    unsigned int stringlength = strlen(string);
    
    seekInBufTo(string[0]);
    while(0 != strncmp(string, &(m_szBuf[m_nParsingPosition]), stringlength))
    {
        m_nParsingPosition++;
        if(m_nParsingPosition >= m_nBufSize)
        {
            return FALSE;
        }
        seekInBufTo(string[0]);
    }
    
    return TRUE;
}


bool xmlLoader::loadFileToBuf(char* szPath)
{
    
    FILE*   pXmlFile;
    unsigned long    nFileLength;
    
    
    if(szPath == NULL)
    {
        resizeBuf(0);
        return FALSE;
    }
    // Try to open file
    if ((pXmlFile = fopen(szPath, "r")) == NULL)
    {
        return FALSE;
    }
    // get file length and resize buf to filelength
    fseek(pXmlFile, 0, SEEK_END);
    nFileLength = ftell(pXmlFile);
    fseek(pXmlFile, 0, SEEK_SET);
    // one byte longer to ensure that there is a \0 at the end
    if(!resizeBuf(nFileLength+1))
    {
        return FALSE;
    }
    m_szBuf[m_nBufSize-1] = '\0';
    
    // read file to buf, maximal nFileLength bytes, NOT m_nBufSize!!!
    if ( !fread(m_szBuf, sizeof(char), nFileLength, pXmlFile) )
    {
        fclose( pXmlFile );
        return FALSE;
    }
    fclose( pXmlFile ); //now, we don't need the file anymore
    return TRUE;
}







