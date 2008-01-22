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
#ifndef KSPLUGINENGINE_H
#define KSPLUGINENGINE_H

#include <QObject>
#include <QList>

class ksPlugin;
class ksPluginInformation;
class xmlObject;

/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/
class ksPluginEngine : public QObject
{
Q_OBJECT
public:
    ksPluginEngine(QObject *parent = 0);
    ksPluginEngine(ksPluginInformation* newPluginInformation);

    ~ksPluginEngine();
    
    void setPluginInformation(ksPluginInformation* newPluginInformation);
    void retranslateUi();
    
    void applyPluginStateChanges();
    
    void savePluginConfigurations(xmlObject* target, bool localOrGlobal); // TRUE: LOCAL; FALSE: GLOBAL
    void loadPluginConfigurations(xmlObject* source, bool localOrGlobal); // TRUE: LOCAL; FALSE: GLOBAL
    
    void unloadAllPlugins();
    
    QList<ksPlugin*>  pluginList() const;
    
private:
    void  createPluginList();
    void  addPlugin(ksPlugin* newPlugin);
    void  deletePluginList();
    
    ksPluginInformation* m_pPluginInformation;
    
    QList<ksPlugin*>     m_listPlugins;

};

#endif
