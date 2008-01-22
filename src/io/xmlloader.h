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
#ifndef XMLLOADER_H
#define XMLLOADER_H

/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/
class xmlObject;

class xmlLoader{
public:
    xmlLoader();
    ~xmlLoader();
    
    bool loadFileToClass(char* path, xmlObject* targetClass);
    bool loadFileToBuf(char* path);
    
    char* buf() { return m_szBuf; };
    unsigned int bufSize() const { return m_nBufSize; };
    
    unsigned int parsingPosition() const { return m_nParsingPosition; };
    
    
    
private: 
    bool parseBufToObject(xmlObject* target);
    bool parseNextTag(xmlObject* target);
    bool parseTagName(xmlObject* target);
    bool parseAttributes(xmlObject* target);
    bool parseNextAttribute(xmlObject* target);
    bool parseContent(xmlObject* target);
    
    
    //member about parsing buf
    bool goToNextRelevantPosition();
    bool seekInBufTo(char  character,   bool equalOrUnequal = 1); // TRUE search for string equal,, FALSE : search for string unequal string
    bool seekInBufTo(char c1, char c2, char c3, char c4 = '\0',
                     char c5 = '\0', char c6 = '\0', char c7 = '\0', char c8 = '\0', char c9 = '\0');
    bool seekInBufTo(char* string);
    
    //memberfunctions about buf
    bool resizeBuf(unsigned int newBufSize);
    
    // members
    unsigned int m_nParsingPosition;
    char*        m_szBuf;
    unsigned int m_nBufSize;

};

#endif
