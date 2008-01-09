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
#include "configdialog.h"
#include "ksconfigcontainer.h"
#include "ksconfigoptionwidget.h"
#include "xmlparser.h"
#include "xmlloader.h"

#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QScrollArea>
#include <QWhatsThis>
#include <QMenu>
#include <QAction>
#include <QActionGroup>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>


ConfigDialog::ConfigDialog(QWidget *parent)
 : QDialog(parent)
{
    if(parent) // if has parent, then it can be closed automatically on main window close
    {
        setAttribute(Qt::WA_QuitOnClose, FALSE);
    }
    m_pConfigContainer = NULL;
    allocateWidgets();
    createLayouts();
    connectSlots();
    retranslateUi();
    setConfigIdentifier("");
}


ConfigDialog::~ConfigDialog()
{
    
}


void  ConfigDialog::allocateWidgets()
{
    btnBoxBottom = new QDialogButtonBox;
    btnOk = new QPushButton;
    btnApply = new QPushButton;
    btnCancel = new QPushButton;
    btnWhatsThis = new QPushButton;
    lblHeader = new QLabel;
    lblHeader->setWordWrap(TRUE);
    lblHeader->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    scrollCentralOptions = new QScrollArea;
    
    
    btnConfigSelection = new QPushButton;
    lblConfigSelection = new QLabel;
    mnuConfigSelection = new QMenu;
    acgrpConfigSelection = new QActionGroup(this);
    acgrpConfigSelection->setExclusive(TRUE);
    btnConfigSelection->setMenu(mnuConfigSelection);
    
    mnaConfigSelectionSeparator = mnuConfigSelection->addSeparator();
    mnaConfigImport = mnuConfigSelection->addAction("");
    mnaConfigExport = mnuConfigSelection->addAction("");
    
    btnBoxBottom->addButton(btnOk, QDialogButtonBox::AcceptRole);
    btnBoxBottom->addButton(btnCancel, QDialogButtonBox::RejectRole);
    btnBoxBottom->addButton(btnApply, QDialogButtonBox::ApplyRole);
    btnBoxBottom->addButton(btnWhatsThis, QDialogButtonBox::HelpRole);
    
}

void  ConfigDialog::createLayouts()
{
    layoutOptionWidgets = new QVBoxLayout;
    layoutOptionWidgets->setMargin(0);
    
    layoutConfigSelection = new QHBoxLayout;
    layoutConfigSelection->setMargin(0);
    layoutConfigSelection->addStretch(0);
    layoutConfigSelection->addWidget(lblConfigSelection);
    layoutConfigSelection->addWidget(btnConfigSelection);
    
    
    
    wdgConfigOptions = new QWidget;
    wdgConfigOptions->setLayout(layoutOptionWidgets);
    wdgConfigOptions->setContentsMargins(0, 0, 0, 0);
    
    scrollCentralOptions->setWidget(wdgConfigOptions);
    scrollCentralOptions->setWidgetResizable(TRUE);
    
    layoutParent = new QVBoxLayout;
    layoutParent->addWidget(lblHeader);
    layoutParent->addWidget(scrollCentralOptions);
    layoutParent->addLayout(layoutConfigSelection);
    layoutParent->addWidget(btnBoxBottom);
    
    setLayout(layoutParent);
}

void  ConfigDialog::connectSlots()
{
    connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(btnApply, SIGNAL(clicked()), this, SLOT(applyChanges()));
    connect(btnWhatsThis, SIGNAL(clicked()), this, SLOT(enterWhatsThisMode()));
    connect(acgrpConfigSelection, SIGNAL(triggered(QAction*)), this, SLOT(configSelected(QAction*)));
    connect(this, SIGNAL(accepted()), this, SLOT(applyChanges()));
    
    connect(mnaConfigExport, SIGNAL(triggered()), this, SLOT(exportConfig()));
    connect(mnaConfigImport, SIGNAL(triggered()), this, SLOT(importConfig()));
}


void ConfigDialog::applyChanges()
{
    for(int i = 0; i < listConfigOptions.size(); i++)
    {
        listConfigOptions[i]->applyChanges();
    }
    emit configurationChanged();
}

void ConfigDialog::setTitle(QString title, QString description)
{
    m_szTitle =  title;
    m_szDescription = description;
    refreshHeaderFromTitle();
}


void  ConfigDialog::retranslateUi()
{
    btnOk->setText(tr("OK"));
    btnApply->setText(tr("Apply"));
    btnCancel->setText(tr("Cancel"));
    btnWhatsThis->setText(tr("What's this ?"));
    lblConfigSelection->setText(tr("Current Configuration:"));
    mnaConfigExport->setText(tr("Export Configuration"));
    mnaConfigImport->setText(tr("Import Configuration"));
    refreshHeaderFromTitle();
}

void  ConfigDialog::refreshHeaderFromTitle()
{
    setWindowTitle(tr("Configure %pluginname plugin").replace("%pluginname", m_szTitle));
    QString text;
    text += "<b>" + m_szTitle + "</b>";
    if(!m_szDescription.isEmpty() && !m_szTitle.isEmpty())
    {
        text += " - ";
    }
    text += m_szDescription;
    
    lblHeader->setText(text);
}


void ConfigDialog::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if(event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
}


void ConfigDialog::setConfigContainer(ksConfigContainer* config)
{
    m_pConfigContainer = config;
    refreshGuiFromConfig(m_pConfigContainer);
}

void ConfigDialog::configSelected(QAction* action)
{
    if(!action)
    {
        return;
    }
    // find config item
    for(int i = 0; i < listConfigMenuItems.size(); i++)
    {
        if(listConfigMenuItems[i].mnaMenuItem == action)
        {
            // if action was found, then set config container
            setConfigContainer(listConfigMenuItems[i].m_pConfig);
        }
            
    }
}

void ConfigDialog::refreshGuiFromConfig(ksConfigContainer* config)
{
     if(config)
     {
         setEnabled(TRUE);
     }
     else
     {
         setEnabled(FALSE);
         return;
     }
     ksConfigOptionWidget* currentItem = NULL;
     int sizeOfChangeableOptions = config->sizeOfUserChangableOptions();
     
     
     // 1. add missing optionwidgets
     while(listConfigOptions.size() < sizeOfChangeableOptions)
     {
         currentItem = new ksConfigOptionWidget;
         if(!currentItem)
         {
             break;
         }
         listConfigOptions.append(currentItem);
         layoutOptionWidgets->addWidget(currentItem);
     }
     
     // 2. remove optionwidgets while there are to much
     while(listConfigOptions.size() > sizeOfChangeableOptions)
     {
         currentItem = listConfigOptions.last();
         layoutOptionWidgets->removeWidget(currentItem);
         listConfigOptions.removeLast();
         if(!currentItem)
         {
             continue;
         }
         delete currentItem;
     }
     
     ksConfigOption* currentOption = NULL;
     
     int currentOptionWidgetIndex = 0;
     // 3. copy configoptions to configoptionwidgets
     for(int i = 0; i < config->size(); i++)
     {
         currentOption =  &((*config)[i]);
             
         if(currentOption->changeableByUser())
         {
            /*if(currentOptionWidgetIndex >= listConfigOptions.size())
            {
                qDebug("aah");
                break;
            }*/
            listConfigOptions[currentOptionWidgetIndex]->setConfigOption( currentOption );
            currentOptionWidgetIndex++;
         }
     }
     // 4. set btnConfigSelection text to name of current config
     // find current config item
     ConfigItem* foundItem = NULL;
     for(int i = 0; i < listConfigMenuItems.size(); i++)
     {
         if(listConfigMenuItems[i].m_pConfig == m_pConfigContainer)
         {
             foundItem = &(listConfigMenuItems[i]);
         }
     }
     if(foundItem == NULL)
     {
         btnConfigSelection->setText(tr("Unknown"));
     }
     else
     {
         if(foundItem->mnaMenuItem)
         {
            foundItem->mnaMenuItem->setChecked(TRUE);
            btnConfigSelection->setText(foundItem->mnaMenuItem->text());
         }
         else{
            btnConfigSelection->setText(tr("Unknown"));
         }
     }
}


void ConfigDialog::clearConfigSelectionMenu()
{
    ConfigItem* currentItem = NULL;
    for(int i = 0; i < listConfigMenuItems.size(); i++)
    {
        currentItem = &(listConfigMenuItems[i]);
        mnuConfigSelection->removeAction(currentItem->mnaMenuItem);
        acgrpConfigSelection->removeAction(currentItem->mnaMenuItem);
        delete currentItem->mnaMenuItem;
    }
    listConfigMenuItems.clear();
}

void ConfigDialog::addToConfigSelectionMenu(ksConfigContainer* config, const QString& name)
{
    ConfigItem newItem;
    // set item's config
    newItem.m_pConfig = config;
    
    // set item's action
    QAction* newAction = new QAction(acgrpConfigSelection);
    newAction->setText(name);
    newAction->setCheckable(TRUE);
    mnuConfigSelection->insertAction(mnaConfigSelectionSeparator, newAction);
    
    newItem.mnaMenuItem = newAction;
    listConfigMenuItems.append(newItem);
}

void ConfigDialog::enterWhatsThisMode(bool on)
{
    if(on)
    {
        QWhatsThis::enterWhatsThisMode();
    }
    else
    {
        QWhatsThis::leaveWhatsThisMode();
    }
}


void ConfigDialog::setConfigIdentifier(QString id)
{
    m_szConfigIdentifier = id;
    mnaConfigImport->setVisible(!id.isEmpty());
    mnaConfigExport->setVisible(!id.isEmpty());
    
};

void ConfigDialog::importConfig()
{
    if(m_pConfigContainer == NULL)
    {// can't export if there is nothing
        return;
    }
    QFileDialog filedia(this);
    filedia.setDirectory(QDir::home());
    filedia.setFilter(tr("Xml Files (*.xml)"));
    filedia.setWindowTitle(tr("Import Plugin Configuration"));
    filedia.setConfirmOverwrite(TRUE);
    filedia.setAcceptMode(QFileDialog::AcceptOpen);
    if(filedia.exec() == QDialog::Rejected)
    {
        return;
    }
    QString fileName = filedia.selectedFiles().first();
    if(fileName.isEmpty())
    {// then, there is nothing to do
        return;
    }
    xmlObject obj;
    xmlLoader loader;
    // load file
    if(!loader.loadFileToClass(fileName.toAscii().data(), &obj))
    {
        QMessageBox::critical(this, tr("Error during reading file"),
                  tr("Couldn't import configuration from %filename").replace("%filename", fileName)
                + tr("Probably you haven't got enough read rights."));
    }
    QMessageBox msgbox(this);
    msgbox.setIcon(QMessageBox::Warning);
    msgbox.setWindowTitle(tr("Importing a configuration"));
    msgbox.setText(tr("If you import an configuration, then the changes are applied immediately and your old configuration gets lost.")
                  + "\n" + tr("Do you really want to import?"));
    msgbox.addButton(tr("Yes, import and overwrite configuration"), QMessageBox::YesRole);
    msgbox.addButton(tr("No, don't import"), QMessageBox::RejectRole);
    
    switch(msgbox.exec())
    {// return if rejected was clicked
        case QMessageBox::RejectRole: return;
    }
    // checking for right identifier
    xmlAttribute* name = obj.cGetAttributeByName("name");
    QString id = name ? name->value() : "";
    if(id.isEmpty() || id != m_szConfigIdentifier)
    {
        QMessageBox questionbox(this);
        questionbox.setIcon(QMessageBox::Warning);
        questionbox.setWindowTitle(tr("Importing a configuration"));
        
        questionbox.setText(
                  tr("The configuration in the file is incompatible to the configuration for %pluginname.\n"
                      "To import it is very risky and not recommended.\n"
                      "Do you really want to continue?").replace("%pluginname", m_szTitle)
                           );
    
        questionbox.addButton(tr("Yes, import"), QMessageBox::YesRole);
        questionbox.addButton(tr("No, don't take the risk"), QMessageBox::RejectRole);
        questionbox.setDefaultButton(QMessageBox::Cancel);
        
        switch(questionbox.exec())
        {// return if rejected was clicked
            case QMessageBox::RejectRole: return;
        }
    }
    m_pConfigContainer->loadFromXmlObject(&obj);
    refreshGuiFromConfig(m_pConfigContainer);
    emit configurationChanged();
}

void ConfigDialog::exportConfig()
{
    if(m_pConfigContainer == NULL)
    {// can't export if there is nothing
        return;
    }
    QFileDialog filedia(this);
    filedia.setDirectory(QDir::home());
    filedia.setFilter(tr("Xml Files (*.xml)"));
    filedia.setWindowTitle(tr("Export Plugin Configuration"));
    filedia.setConfirmOverwrite(TRUE);
    filedia.setAcceptMode(QFileDialog::AcceptSave);
    if(filedia.exec() == QDialog::Rejected)
    {
        return;
    }
    QString fileName = filedia.selectedFiles().first();
    if(fileName.isEmpty())
    {// then, there is nothing to do
        return;
    }
    if(!fileName.endsWith(".xml"))
    {
        fileName += ".xml";
    }
    xmlObject obj;
    obj.setName("configuration");
    obj.nAddAttribute("name", configIdentifier().toAscii().data());
    // add options to xmlObject
    m_pConfigContainer->saveToXmlObject(&obj);
    // write xmlObject to file
    
    if(WriteClassToFile(fileName.toAscii().data(), &obj) != 0)
    {
        QMessageBox::critical(this, tr("Error during writing file"),
                  tr("Couldn't export configuration to \'%filename\'.\n"
                    "Probably you haven't got enough write rights."
                    ).replace("%filename", fileName)
                             );
    }
}





