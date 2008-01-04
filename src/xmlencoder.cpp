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

#include "xmlencoder.h"
#include "ksplattformspec.h"
#include <QString>

xmlEncoder::xmlEncoder()
{
}


xmlEncoder::~xmlEncoder()
{
}


char* xmlEncoder::xmlCodeToString(char* string)
{
    QString returnString = ksPlattformSpec::szToUmlauts(string);
    returnString.replace(QString("&quot;"), QString("\""));
    returnString.replace(QString("&apos;"), QString("\'"));
    returnString.replace(QString("&amp;"), QString("&"));
    returnString.replace(QString("&lt;"), QString("<"));
    returnString.replace(QString("&gt;"), QString(">"));
    return ksPlattformSpec::qstringToSz(returnString);
}

char* xmlEncoder::stringToXmlCode(char* string)
{
    QString returnString = ksPlattformSpec::szToUmlauts(string);
    returnString.replace(QString("&"), QString("&amp;"));
    returnString.replace(QString("\""), QString("&quot;"));
    returnString.replace(QString("\'"), QString("&apos;"));
    returnString.replace(QString("<"), QString("&lt;"));
    returnString.replace(QString(">"), QString("&gt;"));
    return ksPlattformSpec::qstringToSz(returnString);
}


