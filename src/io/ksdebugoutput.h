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
#ifndef KSDEBUGOUTPUT_H
#define KSDEBUGOUTPUT_H

#include <stdlib.h>
#include <QObject>
#include <QString>

/**
	@author Thorsten Wissmann <towi89@web.de>
*/
class ksDebugOutput : public QObject
{
    Q_OBJECT
signals:
    void printDebugLine(QString);
public slots:
    void putStandartQtDebugOutut(QString outputString);
    void putDebugOutput(QString outputString);
public:
    ksDebugOutput();
    ~ksDebugOutput();
    bool isDebugMode(){ return bDebugMode; };
    void setDebugMode(bool newDebugMode){ bDebugMode = newDebugMode; };
    void enableDebugMode(){ setDebugMode(TRUE); };
    void disableDebugMode(){ setDebugMode(FALSE); };
    static void print(QString msg);
    
private:
    bool bDebugMode;
    static ksDebugOutput* lastInstance;
};

#endif
