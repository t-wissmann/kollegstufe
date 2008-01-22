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
#include <core/ksplattformspec.h>
#include <QString>
#include <QChar>

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
    returnString.replace(QString("&lt;"), QString("<"));
    returnString.replace(QString("&gt;"), QString(">"));
    returnString.replace(QString("&nbsp;"), QString(" "));
    returnString.replace(QString("&auml;"), QString::fromLocal8Bit("ä"));
    returnString.replace(QString("&amp;"), QString("&"));
    returnString.replace(QString("&auml;"), QString::fromLocal8Bit("ä"));
    returnString.replace(QString("&ouml;"), QString::fromLocal8Bit("ö"));
    returnString.replace(QString("&uuml;"), QString::fromLocal8Bit("ü"));
    returnString.replace(QString("&Auml;"), QString::fromLocal8Bit("Ä"));
    returnString.replace(QString("&Ouml;"), QString::fromLocal8Bit("Ö"));
    returnString.replace(QString("&Uuml;"), QString::fromLocal8Bit("Ü"));
    returnString.replace(QString("&szlig;"), QString::fromLocal8Bit("ß"));
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
    returnString.replace(QString::fromLocal8Bit("ä"), QString("&auml;"));
    returnString.replace(QString::fromLocal8Bit("ö"), QString("&ouml;"));
    returnString.replace(QString::fromLocal8Bit("ü"), QString("&uuml;"));
    returnString.replace(QString::fromLocal8Bit("Ä"), QString("&Auml;"));
    returnString.replace(QString::fromLocal8Bit("Ö"), QString("&Ouml;"));
    returnString.replace(QString::fromLocal8Bit("Ü"), QString("&Uuml;"));
    returnString.replace(QString::fromLocal8Bit("ß"), QString("&szlig;"));
    return ksPlattformSpec::qstringToSz(returnString);
}


