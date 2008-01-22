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
#ifndef KSPWIDGETTESTERINTERFACE_H
#define KSPWIDGETTESTERINTERFACE_H

#include <pluginengine/ksplugininterface.h>
#include <QObject>
#include <QtPlugin> // needed for  Q_INTERFACES macro

#include <QString>
#include <pluginengine/ksplugin.h>


/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/
class kspWidgetTesterInterface : public QObject, public ksPluginInterface
{
Q_OBJECT
Q_INTERFACES(ksPluginInterface);
public:
    kspWidgetTesterInterface();
    virtual ~kspWidgetTesterInterface();
    
    virtual void init();
    
    virtual void load() ;
    virtual void unload();
    
    virtual void retranslate();
public slots:
    void sayHello();
    
private:
    QAction* mnaHello;
    

};

#endif
