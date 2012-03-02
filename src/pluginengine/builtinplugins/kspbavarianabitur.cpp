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
#include "../../pluginengine/builtinplugins/kspbavarianabitur.h"
#include <QString>
#include <QMessageBox>
#include <QAction>
#include "kssemesterachievementsdialog.h"
#include "../ksplugininformation.h"
#include <core/ksplattformspec.h>
#include <io/xmlparser.h>

kspBavarianAbitur::kspBavarianAbitur() 
        
{
    mnaSemesterAchievements = NULL;
    //twi: entfernt am 2012-03-02
    //ksPlugin::ksPlugin();
    setIdentifier("kspBavarianAbitur");
    setIsHavingAnAboutDialog(FALSE);
    setIsConfigurable(TRUE);
    mnaSemesterAchievements = new QAction(this);
    diaSemesterAchievementsDialog = NULL;
    retranslate();
}


kspBavarianAbitur::~kspBavarianAbitur()
{
    delete mnaSemesterAchievements;
}


void kspBavarianAbitur::retranslate()
{
    setName(tr("Bavarian Abitur"));
    setAuthor("Thorsten Wissmann");
    setDescription(tr("Gives some specific features for the Bavarian Abitur"));
    if(mnaSemesterAchievements)
    {
        mnaSemesterAchievements->setText(tr("Show Semester Achievements"));
    }
}


void kspBavarianAbitur::load()
{
    addMenuAction(mnaSemesterAchievements);
    createConfiguration(m_pCurrentConfig);
    
    connect(mnaSemesterAchievements, SIGNAL(triggered()),
            this, SLOT(showSemesterAchievements()));
    connect(m_pInformation, SIGNAL(currentDatabaseChanged()), this, SLOT(refreshSemesterAchievementsDialog()));
    
    // restore state of dialog
    if(diaSemesterAchievementsDialog)
    {
        diaSemesterAchievementsDialog->loadUiConfig(m_pCurrentConfig);
    }
    ksConfigOption* opt;
    if(m_pCurrentConfig && (opt = m_pCurrentConfig->getOption("semester_achievement_dialog.isVisible")))
    {
        if(opt->valueToBool() == TRUE)
        {
            // show dialog if it was saved
            showSemesterAchievements();
        }
    }
}

void kspBavarianAbitur::refresh()
{
    
}

void kspBavarianAbitur::unload()
{
    // save state of dialog
    if(diaSemesterAchievementsDialog)
    {
        diaSemesterAchievementsDialog->saveUiStateToConfig();
    }
    disconnect(mnaSemesterAchievements, SIGNAL(triggered()),
               this, SLOT(showSemesterAchievements()));
    disconnect(m_pInformation, SIGNAL(currentDatabaseChanged()), this, SLOT(refreshSemesterAchievementsDialog()));
    removeMenuAction(mnaSemesterAchievements);
    if(diaSemesterAchievementsDialog && diaSemesterAchievementsDialog->isVisible())
    {
        diaSemesterAchievementsDialog->hide();
    }
}


void kspBavarianAbitur::createConfiguration(ksConfigContainer* config)
{
    if(!config)
    {
        return;
    }
    // options for dialog
    config->createOption(ksConfigOption("semester_achievement_dialog.ascendingorder", FALSE));
    config->createOption(ksConfigOption("semester_achievement_dialog.semestershowcount", 22));
    config->createOption(ksConfigOption("semester_achievement_dialog.colcount", 5));
    config->createOption(ksConfigOption("semester_achievement_dialog.controldockposition", QString("left")));
    config->createOption(ksConfigOption("semester_achievement_dialog.isVisible", FALSE));
    ksConfigOption* option;
    config->setInGuiOptionPart(TRUE);
    option = config->createOption(ksConfigOption("useCustomCategory", FALSE));
    option->setDescription(tr("Use a custom default category"));
    option->setWhatsThis(tr("Select this, if you want to have a special category selected, "
            "when starting the \"Semester Achievement\" dialog.\n"
            "If this is not selected, the currently selected category in the main window will be selected."));
    option = config->createOption(ksConfigOption("customCategoryName", QString("")));
    option->setDescription(tr("Name of the custom default category"));
    option->setWhatsThis(tr("Enter the name of the category, that shall be selected, "
            "when the \"Semester Achievement\" dialog gets started."));
    config->setInGuiOptionPart(FALSE);
}

void kspBavarianAbitur::showSemesterAchievements()
{
    if(!m_pInformation || !m_pInformation->currentPropertyPart() || !m_pCurrentConfig
    || !m_pInformation->currentDataPart())
    {
        QMessageBox::critical(NULL, identifier() + ": Error",
                              "Internal pointer error:\n"
                                      "m_pInformation = NULL or m_pInformation->currentPropertyPart() = NULL");
        return;
    }
    createConfiguration(m_pCurrentConfig);
    
    if(!diaSemesterAchievementsDialog)
    {
        diaSemesterAchievementsDialog = new ksSemesterAchievementsDialog
                ((QWidget*)(m_pInformation->mainWindow()));
        connect(diaSemesterAchievementsDialog, SIGNAL(visibilityChanged(bool)),
                this, SLOT(dialogVisibilityChanged(bool)));
    }
    xmlObject* properties = m_pInformation->currentPropertyPart();
    xmlObject* categoryToSelect = m_pInformation->currentCategory();
    if(m_pCurrentConfig->getOption("useCustomCategory")->valueToBool())
    {
        QString name = m_pCurrentConfig->getOption("customCategoryName")->valueToString();
        xmlObject* customCategory = m_pInformation->currentDataPart()->cGetObjectByAttributeValue("name", ksPlattformSpec::qstringToSz(name));
        //qDebug("name is %s\ncustomcategory is %d", name.toAscii().data(), customCategory);
        if(customCategory != NULL && name != "")
        {
            categoryToSelect = customCategory;
        }
    }
    ksPlattformSpec::addMissingPropertiesAttributes(properties);
    diaSemesterAchievementsDialog->setPluginParent(NULL); // set parent = 0, to avoid the configHasChanged() call when loading config
    diaSemesterAchievementsDialog->loadUiConfig(m_pCurrentConfig);
    diaSemesterAchievementsDialog->setPluginParent(this); 
    diaSemesterAchievementsDialog->setArchive(m_pInformation->currentDataPart(),
                                              properties->cGetObjectByName("time"),
                                                      categoryToSelect);
    diaSemesterAchievementsDialog->show();
}


void kspBavarianAbitur::refreshSemesterAchievementsDialog()
{
    if(diaSemesterAchievementsDialog && diaSemesterAchievementsDialog->isVisible())
    {
        // if dia is shown, then refresh it
        showSemesterAchievements();
    }
}


void kspBavarianAbitur::dialogVisibilityChanged(bool isVisibleNow)
{
    if(!m_pCurrentConfig)
    {
        return;
    }
    ksConfigOption* visibility = m_pCurrentConfig->getOption("semester_achievement_dialog.isVisible");
    if(!visibility)
    {
        return;
    }
    visibility->setValue(isVisibleNow); // store visibility in config
}


