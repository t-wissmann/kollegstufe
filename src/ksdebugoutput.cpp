/***************************************************************************
 *   Copyright (C) 2007 by Thorsten W.	                                   *
 *   towi89@web.de towi16.piranho.de                                       *
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
#include <QtGlobal>
#include <QChar>
#include <string.h>

// ensure that there's a debug output
#ifndef QT_NO_DEBUG_OUTPUT
#define QT_NO_DEBUG_OUTPUT
#endif

ksDebugOutput::ksDebugOutput()
 : QObject()
{
    bDebugMode = FALSE;
}


ksDebugOutput::~ksDebugOutput()
{
}

void ksDebugOutput::putStandartQtDebugOutut(QString outputString)
{
    // only put debug string if debug mode is enabled
    if (!bDebugMode)
        return;
    qDebug(outputString.toAscii().constData());
    
}
void ksDebugOutput::putDebugOutput(QString outputString)
{
    emit printDebugLine(outputString);
}


