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
#include "kspwidgettester.h"
#include "ksplugin.h"
#include "kspexporttohtml.h"
#include "ksplugininformation.h"
#include "xmlparser.h"
#include "xmlloader.h"

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
    addPlugin(new kspExportToHtml);
    addPlugin(new kspWidgetTester);
    addPlugin(new ksPlugin);
    
    
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
    
    target->nSetObjectCounter(m_listPlugins.size());
    xmlObject* currentObject = NULL;
    for(int i = 0; i < m_listPlugins.size(); i++)
    {
        currentObject = target->cGetObjectByIdentifier(i);
        m_listPlugins[i]->saveConfig(currentObject, localOrGlobal);
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



