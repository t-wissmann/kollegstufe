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
#include "ksdebugoutput.h"
#include <core/ksplattformspec.h>
#include <QtGlobal>
#include <QChar>
#include <string.h>

// ensure that there's a debug output
#ifndef QT_NO_DEBUG_OUTPUT
#define QT_NO_DEBUG_OUTPUT
#endif

ksDebugOutput *ksDebugOutput::lastInstance = 0;

ksDebugOutput::ksDebugOutput()
 : QObject()
{
    bDebugMode = FALSE;
    lastInstance = this;
}


ksDebugOutput::~ksDebugOutput()
{
    if(lastInstance == this)
    {
        lastInstance = 0;
    }
}

void ksDebugOutput::putStandartQtDebugOutut(QString outputString)
{
    // only put debug string if debug mode is enabled
    qDebug(ksPlattformSpec::qstringToSz(outputString));
}
void ksDebugOutput::putDebugOutput(QString outputString)
{
    if (!bDebugMode)
        return;
    emit printDebugLine(outputString);
}
void ksDebugOutput::print(QString msg)
{
    if(lastInstance)
    {
        lastInstance->putDebugOutput(msg);
    }
    else
    {
        qDebug("%s", msg.toAscii().data());
    }
}

