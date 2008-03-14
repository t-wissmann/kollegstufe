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
#include "ksplugininterface.h"
#include <core/ksconfigoption.h>
#include <core/kollegstufeparent.h>
#include <io/xmlparser.h>
#include <io/ksdebugoutput.h>
#include <QDateTime>
#include <QMessageBox>
#include <QMenu>

#include <core/configdialog.h>


ksPlugin::ksPlugin() :QObject()
{
    m_pConfigDia = NULL;
    m_pPluginCore = NULL;
    m_pCurrentConfig = &m_cGlobalConfig;
    m_nState = StateUnloaded;
    m_nWantedState = StateUnloaded;
    m_pInformation = NULL;
    setIsConfigurable(FALSE);
    setIsHavingAnAboutDialog(FALSE);
    setIdentifier("ksPlugin");
    setName(tr("Kollegstufe-Plugin"));
    setDescription(tr("A simple plugin for Kollegstufe"));
    setAuthor("Thorsten Wissmann");
    retranslateUi();
}
ksPlugin::ksPlugin(ksPluginInterface* pluginCore) : QObject()
{
    m_pConfigDia = NULL;
    m_pPluginCore = pluginCore;
    m_pCurrentConfig = &m_cGlobalConfig;
    m_nState = StateUnloaded;
    m_nWantedState = StateUnloaded;
    m_pInformation = NULL;
    setIsConfigurable(FALSE);
    setIsHavingAnAboutDialog(TRUE);
    setIdentifier("ksPlugin");
    if(m_pPluginCore)
    {
        setIdentifier(m_pPluginCore->identifier());
    }
    initPluginCore();
    retranslateUi();
}

void ksPlugin::initPluginCore(){
    if(!m_pPluginCore)
    {
        return;
    }
    m_pPluginCore->addMenuAction = &ksPlugin::addMenuAction;
    m_pPluginCore->removeMenuAction = &ksPlugin::removeMenuAction;
    m_pPluginCore->m_pParent = this;
    m_pPluginCore->printDebugMsg = &ksPlugin::printDebugMsg;
    m_pPluginCore->init();
    setIdentifier(m_pPluginCore->identifier());
}


ksPlugin::~ksPlugin()
{
    if(m_pConfigDia && !m_pInformation) // if there is m_pInformation
    {                                   // then there is a parent, that destroys config dialog
        delete m_pConfigDia;
    }
    if(m_pPluginCore)
    {
        delete m_pPluginCore;
    }
}


void ksPlugin::load()
{
    if(m_pPluginCore)
    {
        m_pPluginCore->load();
    }
}

void ksPlugin::refresh()
{
        
}

void ksPlugin::unload()
{
    if(m_pPluginCore)
    {
        m_pPluginCore->unload();
    }
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
    if(m_nState == StateLocalLoaded || m_nWantedState == StateLocalLoaded)
    {// if plugin was (un)loaded localy, the database has changed
        if(m_pInformation && m_pInformation->mainWindow())
        {
            m_pInformation->mainWindow()->setDatabaseChanged(TRUE);
                    
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
        connect(m_pConfigDia, SIGNAL(configurationChanged()), this, SLOT(configHasChanged()));
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
    if(m_pConfigDia != NULL && m_pConfigDia->isVisible()) // if config dialog is shown
    {
        configure(); // recreate configuration
    }
}

void ksPlugin::retranslate()
{
    if(m_pPluginCore)
    {
        m_pPluginCore->retranslate();
        setAuthor(m_pPluginCore->author());
        setDescription(m_pPluginCore->description());
        setName(m_pPluginCore->name());
    }
    
}

bool ksPlugin::operator==(const ksPlugin& other)
{
    return (identifier() == other.identifier());
}


void ksPlugin::configHasChanged()
{
    if(m_pInformation && m_pInformation->mainWindow())
    {
        if(state() == StateLocalLoaded
           ||(m_pConfigDia && (m_pConfigDia->currentConfigContainer() == &m_cLocalConfig)))
        {   // only set database changed, if config, that has been changed, is local config
            // we don't need mainWindow()->setDatabaseChanged(TRUE) if global config has changed,
            // because global config isn't in database.xml file !
            m_pInformation->mainWindow()->setDatabaseChanged(TRUE);
        }
    }
    if(isLoaded())
    {// if not is loaded, we don't need to refresh, because plugin will be refreshed when loading
        refresh(); // refresh from new configuration
    }
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
    {// local
        loadConfigFromXmlObject(&m_cLocalConfig, source);
        
        if((source && source->cGetAttributeByName("state"))) // always load , because localloaded has an higher priority
        {   // only load if is not loaded before, because localloaded has an higher priority
            QString newState = source->cGetAttributeByName("state")->value();
            if(newState == "loaded")
            {
                loadLocal();
            }
            else
            {
                // if wanted state in source is unloaded, and state() is global loaded
                // then let it be loaded, but if state() is StateLocalLoaded(), then unload it!
                if(state() == StateLocalLoaded)
                {
                    unloadGlobalLocal();
                }
            }
        }
    }
    else
    {// global
        loadConfigFromXmlObject(&m_cGlobalConfig, source);
        
        if((source && source->cGetAttributeByName("state")) && !isLoaded())
        {   // only load if is not loaded before, because localloaded has an higher priority
            QString newState = source->cGetAttributeByName("state")->value();
            if(newState == "loaded")
            {
                loadGlobal();
            }
        }
    }
}


void ksPlugin::addMenuAction(QAction* action)
{
    if(action && m_pInformation && m_pInformation->pluginMenu())
    {
        m_pInformation->pluginMenu()->addAction(action);
        if(m_pInformation->pluginMenu()->actions().size() == 1)
        {
            m_pInformation->pluginMenu()->setTitle(tr("&Plugins"));
        }
    }
}

void ksPlugin::removeMenuAction(QAction* action)
{
    if(action && m_pInformation && m_pInformation->pluginMenu())
    {
        m_pInformation->pluginMenu()->removeAction(action);
        if(m_pInformation->pluginMenu()->actions().size() <= 0)
        {
            m_pInformation->pluginMenu()->setTitle("");
        }
    }
}





void ksPlugin::printDebugMsg(QString message)
{
    ksDebugOutput::print(message);
}

