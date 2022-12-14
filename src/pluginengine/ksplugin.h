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
#ifndef KSPLUGIN_H
#define KSPLUGIN_H

#include <QObject>
#include <QString>
#include <core/ksconfigcontainer.h>
#include <QtGui>
#include "ksplugininterface.h"

class ksPluginInformation;

class xmlObject;
class QAction;
class ConfigDialog;

/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/

class ksPlugin : public QObject
{
    Q_OBJECT
public:
    ksPlugin();
    ksPlugin(ksPluginInterface* pluginCore);

    virtual ~ksPlugin();
    
    enum PluginState {
        StateUnloaded,
        StateGlobalLoaded,
        StateLocalLoaded
    };
    
    void retranslateUi();
    
    void loadGlobal();
    void loadLocal();
    void unloadGlobalLocal();
    
public slots:
    void configHasChanged();
    
public:
    
    virtual void configure();
    virtual void about();
    
    
    static QString stateToString(PluginState state);
    static PluginState stringToState(QString statestring);
    
    PluginState state() const;
    
    void    setWantedState(PluginState newWantedState);
    void    applyStateChange();
    
    // GETTER & SETTER
    bool    hasAboutDialog() const;
    bool    isConfigurable() const;
    bool    isLoaded() const;
    
    QString identifier() const;
    QString name() const;
    QString author() const;
    QString description() const;
    void setInformationSource(ksPluginInformation*   newInformation);
    // operators
    bool    operator==(const ksPlugin& other);
    
    static void loadConfigFromXmlObject(ksConfigContainer* targetConfig, xmlObject* sourceObject);
    void loadConfig(xmlObject* source, bool localOrGlobal); //TRUE: local ; FALSE: global
    
    void saveConfigToXmlObject(ksConfigContainer* sourceConfig, xmlObject* targetObject);
    
    void saveConfig(xmlObject* target, bool localOrGlobal); //TRUE: local ; FALSE: global
    
    void printDebugMsg(QString message);
    
protected:
    // core functions
    virtual void load();
    virtual void refresh();
    virtual void unload();
    virtual void retranslate();
    virtual void createConfiguration(ksConfigContainer* config);
    
    void addMenuAction(QAction* action);
    void removeMenuAction(QAction* action);
    
protected:
    void    setIdentifier(QString newIdentifier);
    void    setName(QString newName);
    void    setAuthor(QString newAuthor);
    void    setDescription(QString newDescription);
    void    setIsConfigurable(bool newIsConfigurable);
    void    setIsHavingAnAboutDialog(bool newHasAboutDialog);
    
    void    initPluginCore();
    
    // members
    ksPluginInformation*    m_pInformation;
    
    ksConfigContainer*      m_pCurrentConfig;
    
    
private:
    ksConfigContainer       m_cGlobalConfig;
    ksConfigContainer       m_cLocalConfig;
    
    ksPluginInterface*      m_pPluginCore;
    PluginState m_nState;
    PluginState m_nWantedState;
    bool    m_bIsConfigurable;
    bool    m_bHasAboutDialog;
    QString m_szIdentifier;
    QString m_szName;
    QString m_szAuthor;
    QString m_szDescription;
    
    ConfigDialog*   m_pConfigDia;
    
};

Q_DECLARE_INTERFACE(ksPlugin, "kspluginnummbereins");

            
#endif
