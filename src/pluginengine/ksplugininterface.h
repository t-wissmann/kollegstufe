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
#ifndef KSPLUGININTERFACE_H
#define KSPLUGININTERFACE_H

#include <QtPlugin> // needed for  Q_DECLARE_INTERFACE macro

class QAction;
class ksPlugin;

typedef void (ksPlugin::*stringSetter) (QString);
typedef void (ksPlugin::*qactionSetter) (QAction*);


/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/

class ksPluginInterface{
    
public:
    ksPluginInterface(){
        m_pParent = 0;
        addMenuAction = 0;
        removeMenuAction = 0;
        printDebugMsg = 0;
    };
    
    virtual void init() {};
    virtual ~ksPluginInterface() {};
    
    virtual void load() {} ;
    virtual void unload() {};
    
    virtual void retranslate() = 0;
    
    QString identifier() const { return m_szIdentifier;};
    QString name() const {return m_szName; };
    QString description() const {return m_szDescription; };
    QString author() const {return m_szAuthor; };
    
    qactionSetter addMenuAction;
    qactionSetter removeMenuAction;
    
    stringSetter  printDebugMsg;
    
    void print(QString message) {
        if(m_pParent)
        {
            (m_pParent->*printDebugMsg)(message);
        }else{
            qDebug("%s", message.toAscii().data());
        }
    };
    
    ksPlugin*     m_pParent;
    
protected:
    QString m_szIdentifier;
    QString m_szName;
    QString m_szDescription;
    QString m_szAuthor;
    
};

Q_DECLARE_INTERFACE(ksPluginInterface , "thorsten.wissmann.kollegstufe.ksPluginInterface/1.0");


#endif
