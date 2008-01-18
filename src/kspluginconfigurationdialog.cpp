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
#include "kspluginconfigurationdialog.h"
#include "kspluginmanager.h"
#include "ksiconcatcher.h"
#include "ksplattformspec.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QAbstractButton>

#include <QMessageBox>
#include <QEvent>
#include <QWhatsThis>

ksPluginConfigurationDialog::ksPluginConfigurationDialog(QWidget *parent)
 : QDialog(parent)
{
    if(parent) // if has parent, then it can be closed automatically on main window close
    {
        setAttribute(Qt::WA_QuitOnClose, FALSE);
    }
    allocateWidgets();
    createLayouts();
    connectSlots();
    retranslateUi();
    reloadIcons();
}


ksPluginConfigurationDialog::~ksPluginConfigurationDialog()
{
}


void ksPluginConfigurationDialog::allocateWidgets()
{
    wdgPlugins = new ksPluginManager;
    
    btnBoxOkCancel = new QDialogButtonBox;
    btnOk =  new QPushButton;
    btnBoxOkCancel->addButton(btnOk, QDialogButtonBox::AcceptRole);
    btnApply = new QPushButton;
    btnBoxOkCancel->addButton(btnApply, QDialogButtonBox::ApplyRole);
    btnCancel = new QPushButton;
    btnBoxOkCancel->addButton(btnCancel, QDialogButtonBox::RejectRole);
    btnWhatsThis = new QPushButton;
    btnBoxOkCancel->addButton(btnWhatsThis, QDialogButtonBox::HelpRole);
}

void ksPluginConfigurationDialog::createLayouts()
{
    layoutParent = new QVBoxLayout;
    layoutParent->addWidget(wdgPlugins);
    layoutParent->addWidget(btnBoxOkCancel);
    setLayout(layoutParent);
}

void ksPluginConfigurationDialog::connectSlots()
{
    connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(this, SIGNAL(accepted()), this, SLOT(applyChanges()));
    connect(btnApply, SIGNAL(clicked()), this, SLOT(applyChanges()));
    connect(btnWhatsThis, SIGNAL(clicked()), this, SLOT(enterWhatsThisMode()));
}


void ksPluginConfigurationDialog::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if(event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
}

void ksPluginConfigurationDialog::setPluginEngine(ksPluginEngine* newPluginEngine)
{
    m_pluginEngine = newPluginEngine;
    wdgPlugins->setPluginEngine(m_pluginEngine);
}


void ksPluginConfigurationDialog::applyChanges()
{
    wdgPlugins->applyChanges();
}

void ksPluginConfigurationDialog::retranslateUi()
{
    btnOk->setText(tr("Ok"));
    btnApply->setText(tr("Apply"));
    btnCancel->setText(tr("Cancel"));
    btnWhatsThis->setText(tr("What's this ?"));
    setWindowTitle(tr("Plugin Configuration") + " - " + tr("Kollegstufe"));
}


void ksPluginConfigurationDialog::reloadIcons()
{
    btnOk->setIcon(ksIconCatcher::getIcon("button_ok", 16));
    btnApply->setIcon(ksIconCatcher::getIcon("apply", 16));
    btnCancel->setIcon(ksIconCatcher::getIcon("button_cancel", 16));
}


void ksPluginConfigurationDialog::enterWhatsThisMode()
{
    QWhatsThis::enterWhatsThisMode();
}


