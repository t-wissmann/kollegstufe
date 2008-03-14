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
#include "kspluginengine.h"
#include "ksplugininformation.h"
#include <io/ksdebugoutput.h>
#include <io/xmlparser.h>
#include <io/xmlloader.h>
#include <core/ksplattformspec.h>
#include <QDir>
#include <QApplication>
#include <QString>
#include <QPluginLoader>
#include <QStringList>

/// in main executable bin/kolllegstufe(.exe) built in plugins
#include "ksplugin.h"
#include "ksplugininterface.h"
//#include "kspwidgettester.h"
#include "builtinplugins/kspexporttohtml.h"
#include "builtinplugins/kspbavarianabitur.h"

ksPluginEngine::ksPluginEngine(QObject *parent)
 : QObject(parent)
{
    m_pPluginInformation = NULL;
    createPluginList();
}

ksPluginEngine::ksPluginEngine(ksPluginInformation* newPluginInformation)
    : QObject()
{
    m_pPluginInformation = newPluginInformation;
    createPluginList();
}



ksPluginEngine::~ksPluginEngine()
{
    deletePluginList();
}

void  ksPluginEngine::setPluginInformation(ksPluginInformation* newPluginInformation)
{
    m_pPluginInformation = newPluginInformation;
    for(int i = 0; i < m_listPlugins.size(); i++)
    {
        m_listPlugins[i]->setInformationSource(m_pPluginInformation);
    }
}


void  ksPluginEngine::createPluginList()
{
    
    /// add built-in - plugins
    addPlugin(new kspExportToHtml);
    addPlugin(new kspBavarianAbitur);
    //addPlugin(new kspWidgetTester);
    
    //addPlugin(new ksPlugin);
    
    QDir pluginDir(QApplication::applicationDirPath());
    pluginDir.cdUp();
    if(!pluginDir.cd("plugins"))
    {
        return;
    }
    foreach(QString fileName, pluginDir.entryList(QStringList() << "*kspwidg*", QDir::Files))
    {
        ksDebugOutput::print(QString("plugin %pluginname was found").replace("%pluginname", fileName));
        QPluginLoader loader(pluginDir.absoluteFilePath(fileName));
        QObject* instance = loader.instance();
        ksPluginInterface* newPluginInterface = qobject_cast<ksPluginInterface*>(instance);
        
        if(!newPluginInterface)
        {
            ksDebugOutput::print(QString("ERROR: %errorstring").replace("%errorstring", loader.errorString()));
        }
        else
        {
            
            // add plugin with the just catched plugin interface
            addPlugin(new ksPlugin(newPluginInterface));
        }
        
    }
    
}

void ksPluginEngine::retranslateUi()
{
    ksPlugin* currentPlugin = NULL; 
    for(int i = 0; i < m_listPlugins.size(); i++)
    {
        currentPlugin = m_listPlugins[i];
        if(currentPlugin == NULL)
        {
            continue;
        }
        currentPlugin->retranslateUi();
    }
}

void ksPluginEngine::applyPluginStateChanges()
{
    ksPlugin* currentPlugin = NULL; 
    for(int i = 0; i < m_listPlugins.size(); i++)
    {
        currentPlugin = m_listPlugins[i];
        if(currentPlugin == NULL)
        {
            continue;
        }
        currentPlugin->applyStateChange();
    }
}


void ksPluginEngine::addPlugin(ksPlugin* newPlugin)
{
    if(newPlugin == NULL)
    {
        return;
    }
    newPlugin->setInformationSource(m_pPluginInformation);
    m_listPlugins.append(newPlugin);
}


void ksPluginEngine::unloadAllPlugins()
{
    ksPlugin* currentPlugin = NULL;
    for(int i = 0; i < m_listPlugins.size(); i++)
    {
        currentPlugin = m_listPlugins[i];
        if(currentPlugin == NULL)
        {
            continue;
        }
        if(currentPlugin->isLoaded())
        {
            currentPlugin->setWantedState(ksPlugin::StateUnloaded);
            currentPlugin->applyStateChange();
        }
    }
}

void  ksPluginEngine::deletePluginList()
{
    
    ksPlugin* currentPlugin = NULL;
    for(int i = 0; i < m_listPlugins.size(); i++)
    {
        currentPlugin = m_listPlugins[i];
        if(currentPlugin == NULL)
        {
            continue;
        }
        if(currentPlugin->isLoaded())
        {
            // unload if is still loaded
            currentPlugin->setWantedState(ksPlugin::StateUnloaded);
            currentPlugin->applyStateChange();
        }
        delete currentPlugin;
    }
}

QList<ksPlugin*>  ksPluginEngine::pluginList() const
{
    return m_listPlugins;
}


void ksPluginEngine::savePluginConfigurations(xmlObject* target, bool localOrGlobal)
{
    if(!target)
    {
        return;
    }
    
    //target->nSetObjectCounter(m_listPlugins.size());
    xmlObject* currentObject = NULL;
    ksPlugin*  currentPlugin = NULL;
    for(int i = 0; i < m_listPlugins.size(); i++)
    {
        currentPlugin = m_listPlugins[i];
        if(!currentPlugin)
        {
            continue;
        }
        currentObject = target->cGetObjectByAttributeValue("name", ksPlattformSpec::qstringToSz(currentPlugin->identifier()));
        if(!currentObject)
        {
            currentObject = target->cAddObject("plugin-config");
            if(!currentObject)
            {
                return;
            }
        }
        currentPlugin->saveConfig(currentObject, localOrGlobal);
    }
}

void ksPluginEngine::loadPluginConfigurations(xmlObject* source, bool localOrGlobal) // TRUE: LOCAL; FALSE: GLOBAL
{
    if(!source)
    {
        return;
    }
    for(int i = 0; i < m_listPlugins.size(); i++)
    {
        m_listPlugins[i]->loadConfig(source->cGetObjectByAttributeValue("name", m_listPlugins[i]->identifier().toAscii().data()), localOrGlobal);
    }
}



