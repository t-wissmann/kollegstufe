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
#include "kspwidgettesterinterface.h"
//#include "kspwidgettester.h"

#include <QtPlugin>

Q_EXPORT_PLUGIN2 (kspwidgettesterinterface, kspWidgetTesterInterface);


kspWidgetTesterInterface::kspWidgetTesterInterface()
 : QObject()
{
    qDebug("plugin says hello");
}


kspWidgetTesterInterface::~kspWidgetTesterInterface()
{
    qDebug("plugin says goodbye");
}

ksPlugin* kspWidgetTesterInterface::createNewInstance()
{
    //return "es funktioniert";
    //return new kspWidgetTester;
    return (ksPlugin*)289;
}


