/***************************************************************************
 *   Copyright (C) 2007 by Thorsten W.	                                   *
 *   towi89@web.de towi16.piranho.de                                       *
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
#include "ksdatabaseselection.h"
#include "ksplattformspec.h"

#include <QPushButton>
#include <QListWidget>
#include <QLabel>

// layouts
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QMessageBox>

ksDatabaseSelection::ksDatabaseSelection(QWidget *parent)
 : QDialog(parent)
{
    allocateWidgets();
    createLayouts();
    connectSlots();
    
    setWindowTitle(QString::fromLocal8Bit("Noten-Archiv zum Laden auswählen"));
}


ksDatabaseSelection::~ksDatabaseSelection()
{
}

void    ksDatabaseSelection::allocateWidgets()
{
    // widgtes on bottom
    btnOk     = new QPushButton("Archiv Laden");
    btnCancel = new QPushButton("Abbrechen");
    
    // main widgets
    lstFileList = new QListWidget;
    lstFileList->setAlternatingRowColors(TRUE);
    lblInfo     = new QLabel;
}

void    ksDatabaseSelection::createLayouts()
{
    layoutBottom = new QHBoxLayout;
    layoutBottom->setMargin(0);
    layoutBottom->addStretch(1);
    layoutBottom->addWidget(btnOk);
    layoutBottom->addWidget(btnCancel);
    
    layoutParent = new QGridLayout;
    layoutParent->addWidget(lstFileList, 0, 0);
    layoutParent->addWidget(lblInfo,     0, 1);
    layoutParent->addLayout(layoutBottom, 1, 0, 1, 2);
    
    setLayout(layoutParent);
}

void    ksDatabaseSelection::connectSlots()
{
    connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(lstFileList, SIGNAL(currentRowChanged(int)), this, SLOT(selectedFileChanged()));
}



void    ksDatabaseSelection::setCurrentFile(QString szNewFile)
{
    szCurrentFile = szNewFile;
    refreshFileList();
}

QString ksDatabaseSelection::getCurrentFile()
{
    return szCurrentFile;
}

void ksDatabaseSelection::refreshFileList()
{
    lstFileList->clear();
    catchStringList();
    
    // disconnect to prevent resetting of szCurrentFile
    disconnect(lstFileList, SIGNAL(currentRowChanged(int)), this, SLOT(selectedFileChanged()));
    for (int i = 0; i < szFileList.size(); i++)
    {
        lstFileList->addItem(szFileList.at(i));
    }
    connect(lstFileList, SIGNAL(currentRowChanged(int)), this, SLOT(selectedFileChanged()));
    // select currentFile
    for (int i = 0; i < szFileList.size(); i++)
    {
        if (szFileList.at(i) == szCurrentFile)
        {
            lstFileList->setCurrentRow(i);
        }
    }
}

void ksDatabaseSelection::catchStringList()
{
    ksPlattformSpec::catchFileList(&szFileList);
}

void ksDatabaseSelection::selectedFileChanged()
{
    if(lstFileList->selectedItems().size() < 1)
    {
        btnOk->setEnabled(FALSE);
    }
    else
    {
        btnOk->setEnabled(TRUE);
        szCurrentFile = szFileList.at(lstFileList->currentRow());
    }
}


