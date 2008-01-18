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
#include "ksconfigure.h"
#include "ksplattformspec.h"
#include "ksiconcatcher.h"
#include "xmlparser.h"

#include <QPushButton>
#include <QApplication>
#include <QLabel>
#include <QFrame>
#include <QTextEdit>
#include <QGroupBox>
#include <QListWidget>

// layouts
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QApplication>
#include <QString>
#include <QEvent>
#include <QListWidgetItem>
#include <QDir>
#include <QMessageBox>

ksConfigure::ksConfigure(QWidget *parent)
 : QDialog(parent)
{
    if(parent) // if has parent, then it can be closed automatically on main window close
    {
        setAttribute(Qt::WA_QuitOnClose, FALSE);
    }
    config = NULL;
    
    allocateWidgets();
    createLayouts();
    connectSlots();
    initWidgets();
    
    retranslateUi();
    reloadIcons();
}


ksConfigure::~ksConfigure()
{
}


void ksConfigure::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if(event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
}

void ksConfigure::allocateWidgets()
{
    btnOk = new QPushButton;
    btnCancel = new QPushButton;
    
    lstLanguageSelection = new QListWidget;
    grpLanguageSelection = new QGroupBox;
    
    
    
}


void ksConfigure::createLayouts()
{
    
    layoutBottom = new QHBoxLayout;
    layoutBottom->addStretch();
    layoutBottom->addWidget(btnOk);
    layoutBottom->addWidget(btnCancel);
    
    layoutLanguageSelection = new QGridLayout;
    layoutLanguageSelection->addWidget(lstLanguageSelection);
    grpLanguageSelection->setLayout(layoutLanguageSelection);
    
    layoutParent = new QGridLayout;
    
    layoutParent->addWidget(grpLanguageSelection, 1, 0);
    layoutParent->addLayout(layoutBottom, 2, 0);
    
    
    setLayout(layoutParent);
}

void ksConfigure::connectSlots()
{
    connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(this, SIGNAL(accepted()), this, SLOT(applyChanges()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void ksConfigure::initWidgets()
{
    btnOk->setDefault(TRUE);
    resize(330, 150);
}

void ksConfigure::retranslateUi()
{
    btnOk->setText(tr("&Ok"));
    btnCancel->setText(tr("&Cancel"));
    
    grpLanguageSelection->setTitle(tr("Select your Language"));
    
    setWindowTitle(tr("Configure Kollegstufe"));
}

void ksConfigure::reloadIcons()
{
    btnOk->setIcon(ksIconCatcher::getIcon("button_ok", 16));
    btnCancel->setIcon(ksIconCatcher::getIcon("button_cancel", 16));
}



void ksConfigure::setConfigToEdit(xmlObject* newConfig)
{
    config = newConfig;
    
    ksPlattformSpec::addMissingConfigAttributes(config);
    catchLanguageList();
    QString language;
    language = config->cGetObjectByName("language")->szGetContent();
    QList<QListWidgetItem *> list = lstLanguageSelection->findItems(language, Qt::MatchStartsWith);
    if (list.count() > 0)
    {
        list.first()->setSelected(TRUE);
    }
    
}
    
void ksConfigure::applyChanges()
{
    if(!config)
    {
        return;
    }
    
    QList<QListWidgetItem *> itemList = lstLanguageSelection->selectedItems();
    QString newLanguage = itemList.first()->text();
    config->cGetObjectByName("language")->nSetContent(ksPlattformSpec::qstringToSz(newLanguage));
    
    
}

void ksConfigure::catchLanguageList()
{
    languageList.clear();
    
    languageList = ksPlattformSpec::getAvailibleLanguages();
    lstLanguageSelection->clear();
    for(int i = 0; i < languageList.count(); i++)
    {
        lstLanguageSelection->addItem(languageList[i]);
    }
    
}

