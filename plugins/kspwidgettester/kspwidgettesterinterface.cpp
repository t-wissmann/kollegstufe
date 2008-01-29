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
#include <QDateTime>
#include <QtPlugin>
#include <QAction>

Q_EXPORT_PLUGIN2 (kspwidgettesterinterface, kspWidgetTesterInterface);


kspWidgetTesterInterface::kspWidgetTesterInterface()
 : QObject()
{
    m_szIdentifier = "kspWidgetTester";
    qDebug("plugin says i am loaded");
    mnaHello = 0;
}


kspWidgetTesterInterface::~kspWidgetTesterInterface()
{
    qDebug("plugin says goodbye");
    if(mnaHello)
    {
        disconnect(mnaHello, SIGNAL(triggered()), this, SLOT(sayHello()));
        delete mnaHello;
    }
}




void kspWidgetTesterInterface::init()
{
}

void kspWidgetTesterInterface::load()
{
    if(!m_pParent)
    {
        return;
    }
    QString msg = "Plugin \'%pluginname\' gets loaded at %datetime";
    msg.replace("%pluginname", identifier());
    msg.replace("%datetime", QDateTime::currentDateTime().toString());
    print(msg);
    
    if(!mnaHello)
    {
        mnaHello = new QAction(this);
        mnaHello->setText("Say Hello");
        connect(mnaHello, SIGNAL(triggered()), this, SLOT(sayHello()));
    }
    (m_pParent->*addMenuAction)(mnaHello);
    
    
}
void kspWidgetTesterInterface::unload()
{
    if(!m_pParent)
    {
        return;
    }
    QString msg = "Plugin \'%pluginname\' gets unloaded at %datetime";
    msg.replace("%pluginname", identifier());
    msg.replace("%datetime", QDateTime::currentDateTime().toString());
    print(msg);
    
    (m_pParent->*removeMenuAction)(mnaHello);
}

void kspWidgetTesterInterface::retranslate()
{
    m_szName = tr("Widget Tester");
    m_szDescription = tr("Tests some custom widgets of kollegstufe");
    m_szAuthor = "Thorsten Wissmann";
}
void kspWidgetTesterInterface::sayHello()
{
    print("Juhuuuuuu es klapppppt :))");
}


