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

#include "ksplugin.h"
#include "ksplugininformation.h"
#include "ksconfigoption.h"
#include <QMessageBox>
#include "xmlparser.h"
#include <QDateTime>

#include "configdialog.h"


ksPlugin::ksPlugin() :QObject()
{
    m_pCurrentConfig = &m_cGlobalConfig;
    m_nState = StateUnloaded;
    m_nWantedState = StateUnloaded;
    m_pInformation = NULL;
    setIsConfigurable(TRUE);
    setIsHavingAnAboutDialog(FALSE);
    setIdentifier("ksPlugin");
    retranslateUi();
    m_pConfigDia = NULL;
}


ksPlugin::~ksPlugin()
{
    if(m_pConfigDia && !m_pInformation) // if there is m_pInformation, then there is a parent
    {
        delete m_pConfigDia;
    }
}



void ksPlugin::load()
{
    /*
    QMessageBox::information(m_pInformation ? (QWidget*)(m_pInformation->mainWindow()) : NULL,
                             QString("Kollegstufe") + QString(" - ") + name(),
                            name() + " gets loaded!");
    */
    m_pCurrentConfig->setOption(ksConfigOption("lastload", QDateTime::currentDateTime().toString()));
}

void ksPlugin::refresh()
{
        
}

void ksPlugin::unload()
{
    /*
    QMessageBox::information(m_pInformation ? (QWidget*)(m_pInformation->mainWindow()) : NULL,
                             QString("Kollegstufe") + QString(" - ") + name(),
                                     name() + " gets unloaded!");
    */
    m_pCurrentConfig->setOption(ksConfigOption("lastunload", QDateTime::currentDateTime().toString()));
}


void ksPlugin::createConfiguration(ksConfigContainer* config)
{
    ksConfigContainer* foo;
    foo = config;
}

void ksPlugin::setWantedState(PluginState newWantedState)
{
    m_nWantedState = newWantedState;
}


void ksPlugin::loadGlobal()
{
    setWantedState(StateGlobalLoaded);
    applyStateChange();
}

void ksPlugin::loadLocal()
{
    setWantedState(StateLocalLoaded);
    applyStateChange();
}

void ksPlugin::unloadGlobalLocal()
{
    setWantedState(StateUnloaded);
    applyStateChange();
}


void ksPlugin::applyStateChange()
{
    if(m_nWantedState == m_nState)
    {
        // if state hasn't changed, we don't have to do anything
        return;
    }
    if(m_nWantedState == StateUnloaded)
    {
        unload();
    }
    else if(m_nWantedState == StateLocalLoaded || m_nWantedState == StateGlobalLoaded)
    {
        if(!isLoaded())
        {// if wasn't loaded before, just give right information
            //set config file
            switch(m_nWantedState)
            {
                case StateLocalLoaded:  m_pCurrentConfig = &m_cLocalConfig;  break;
                case StateGlobalLoaded: m_pCurrentConfig = &m_cGlobalConfig; break;
                default:                m_pCurrentConfig = &m_cGlobalConfig; break;
                        
            }
            // then load
            load();
        }
        else
        {// if is currently running, i.e. switching from local to global or from global to local
            //1. unload
            unload();
            //2. set Config File
            switch(m_nWantedState)
            {
                case StateLocalLoaded:  m_pCurrentConfig = &m_cLocalConfig;  break;
                case StateGlobalLoaded: m_pCurrentConfig = &m_cGlobalConfig; break;
                default:                m_pCurrentConfig = &m_cGlobalConfig; break;
                        
            }
            //3. load again
            load();
            
        }
    }
    // apply new state
    m_nState = m_nWantedState;
}

void ksPlugin::configure()
{
    if(!m_pConfigDia)
    {
        if(m_pInformation)
        {
            m_pConfigDia = new ConfigDialog((QWidget*)m_pInformation->mainWindow());
        }
        else
        {
            m_pConfigDia = new ConfigDialog;
        }
    }
    createConfiguration(&m_cGlobalConfig); // create global config
    createConfiguration(&m_cLocalConfig); // create local config
    m_pConfigDia->setTitle(name(), description());
    m_pConfigDia->clearConfigSelectionMenu();
    m_pConfigDia->setConfigIdentifier(identifier());
    m_pConfigDia->addToConfigSelectionMenu(&m_cGlobalConfig, tr("Global"));
    m_pConfigDia->addToConfigSelectionMenu(&m_cLocalConfig, tr("Local"));
    m_pConfigDia->setConfigContainer(m_pCurrentConfig);
    m_pConfigDia->show();
}


void ksPlugin::about()
{
    QString text = "<b>";
    text += name();
    text += "</b> ";
    text += tr("by") + " ";
    text += author();
    text += "<br>";
    text += description();
    QMessageBox::information(m_pInformation ? (QWidget*)(m_pInformation->mainWindow()) : NULL,
                             tr("Kollegstufe") + " - " + name(), text);
}


QString ksPlugin::stateToString(PluginState state)
{
    switch(state){
        case StateGlobalLoaded: return "globalloaded";
        case StateLocalLoaded:  return "localloaded";
        default: return "unloaded";
    }
}

ksPlugin::PluginState ksPlugin::stringToState(QString statestring)
{
    PluginState returnState = StateUnloaded;
    if(statestring == "globalloaded")
        returnState = StateGlobalLoaded;
    if(statestring == "localloaded")
        returnState = StateLocalLoaded;
    return returnState;
}



ksPlugin::PluginState ksPlugin::state() const
{
    return m_nState;
}

bool ksPlugin::isLoaded() const
{
    return m_nState == StateLocalLoaded || m_nState == StateGlobalLoaded;
}



void    ksPlugin::setInformationSource(ksPluginInformation*   newInformation)
{
    m_pInformation = newInformation;
    if(m_pInformation && m_pConfigDia)
    {
        m_pConfigDia->setParent((QWidget*)m_pInformation->mainWindow());
    }
}

void ksPlugin::retranslateUi()
{
    retranslate();
    
}

void ksPlugin::retranslate()
{
    
    setName(tr("Kollegstufe-Plugin"));
    setDescription(tr("A simple plugin for Kollegstufe"));
    setAuthor("Thorsten Wissmann");
}

bool ksPlugin::operator==(const ksPlugin& other)
{
    return (identifier() == other.identifier());
}

void ksPlugin::setIdentifier(QString newIdentifier)
{
    m_szIdentifier = newIdentifier;
}

QString ksPlugin::identifier() const
{
    return m_szIdentifier;
}


void ksPlugin::setName(QString newName)
{
    m_szName = newName;
}


QString ksPlugin::name() const
{
    return m_szName;
}


bool    ksPlugin::isConfigurable() const
{
    return m_bIsConfigurable;
}

void    ksPlugin::setIsConfigurable(bool newIsConfigurable)
{
    m_bIsConfigurable = newIsConfigurable;
}

bool    ksPlugin::hasAboutDialog() const
{
    return m_bHasAboutDialog;
}


void ksPlugin::setIsHavingAnAboutDialog(bool newHasAboutDialog)
{
    m_bHasAboutDialog = newHasAboutDialog;
}


void    ksPlugin::setAuthor(QString newAuthor)
{
    m_szAuthor = newAuthor;
}

QString ksPlugin::author() const
{
    return m_szAuthor;
}

void    ksPlugin::setDescription(QString newDescription)
{
    m_szDescription = newDescription;
}

QString ksPlugin::description() const
{
    return m_szDescription;
}

void ksPlugin::loadConfigFromXmlObject(ksConfigContainer* targetConfig, xmlObject* sourceObject)
{
    if(targetConfig)
    {
        targetConfig->loadFromXmlObject(sourceObject);
    }
}

void ksPlugin::saveConfigToXmlObject(ksConfigContainer* sourceConfig, xmlObject* targetObject)
{
    if(!sourceConfig)
    {
        return;
    }
    sourceConfig->saveToXmlObject(targetObject);
    
    xmlAttribute* nameAttribute = targetObject->cGetAttributeByName("name");
    if(nameAttribute == NULL)
    {
        nameAttribute = targetObject->cAddAttribute("name", identifier().toAscii().data());
    }
    else
    {
        nameAttribute->SetValue(identifier().toAscii().data());
    }
    
    
}

void ksPlugin::saveConfig(xmlObject* target, bool localOrGlobal) //TRUE: local ; FALSE: global
{
    if(target == NULL)
    {
        return;
    }
    if(localOrGlobal)
    {
        saveConfigToXmlObject(&m_cLocalConfig, target);
        // save if plugin was loaded locally or no
        xmlAttribute* stateAttribute = target->cGetAttributeByName("state");
        QString state =  m_nState == StateLocalLoaded ? "loaded" : "unloaded";
        if(stateAttribute == NULL)
        {
            stateAttribute = target->cAddAttribute("state",state.toAscii().data());
        }
        else
        {
            stateAttribute->SetValue(state.toAscii().data());
        }
    
    }
    else
    {
        saveConfigToXmlObject(&m_cGlobalConfig, target);
        // save if plugin was loaded globally or no
        xmlAttribute* stateAttribute = target->cGetAttributeByName("state");
        QString state =  m_nState == StateGlobalLoaded ? "loaded" : "unloaded";
        if(stateAttribute == NULL)
        {
            stateAttribute = target->cAddAttribute("state",state.toAscii().data());
        }
        else
        {
            stateAttribute->SetValue(state.toAscii().data());
        }
    }
}

void ksPlugin::loadConfig(xmlObject* source, bool localOrGlobal) //TRUE: local ; FALSE: global
{
    if(localOrGlobal)
    {
        loadConfigFromXmlObject(&m_cLocalConfig, source);
    }
    else
    {
        loadConfigFromXmlObject(&m_cGlobalConfig, source);
    }
}



