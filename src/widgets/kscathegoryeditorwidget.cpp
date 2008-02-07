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
#include "kscathegoryeditorwidget.h"
#include <core/ksplattformspec.h>
#include <io/xmlparser.h>
#include <io/ksiconcatcher.h>

// layouts
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSplitter>

#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>

// other qt-classes
#include <QApplication>
#include <QMessageBox>
#include <QString>
#include <QPixmap>
#include <QIcon>
#include <QListWidgetItem>
#include <QList>
#include <QEvent>


ksCathegoryEditorWidget::ksCathegoryEditorWidget(QWidget *parent)
 : QWidget(parent)
{
    
    initMembers();
    allocateWidgets();
    createLayouts();
    connectSlots();
    initWidgets();
    
    retranslateUi();
    reloadIcons();
    //lstCathegoryList->setEditTriggers(QAbstractItemView::AllEditTriggers);
}


ksCathegoryEditorWidget::~ksCathegoryEditorWidget()
{
}


void ksCathegoryEditorWidget::initMembers()
{
    bIsRenaming = FALSE;
    database = NULL;
}


void ksCathegoryEditorWidget::allocateWidgets()
{
    lstCathegoryList = new QListWidget;
    
    btnAdd = new QPushButton;
    btnDelete = new QPushButton;
    btnMoveUp = new QPushButton;
    btnMoveDown = new QPushButton;
    btnRename = new QPushButton;
    
    wdgRenameBar = new QWidget;
    //btnSubmitRename = new QPushButton;
    lblEnterNewName = new QLabel;
    txtNewName = new QLineEdit;
    
}



void ksCathegoryEditorWidget::createLayouts()
{
    layoutToolButtons = new QVBoxLayout;
    layoutToolButtons->setMargin(0);
    layoutToolButtons->addWidget(btnAdd);
    layoutToolButtons->addWidget(btnDelete);
    layoutToolButtons->addWidget(btnMoveUp);
    layoutToolButtons->addWidget(btnMoveDown);
    layoutToolButtons->addWidget(btnRename);
    
    layoutRenameBar = new QHBoxLayout;
    layoutRenameBar->setMargin(0);
    layoutRenameBar->addWidget(lblEnterNewName);
    layoutRenameBar->addWidget(txtNewName);
    //layoutRenameBar->addWidget(btnSubmitRename);
    wdgRenameBar->setLayout(layoutRenameBar);
    
    layoutListAndEnterNewName = new QVBoxLayout;
    layoutListAndEnterNewName->setMargin(0);
    layoutListAndEnterNewName->addWidget(lstCathegoryList);
    layoutListAndEnterNewName->addWidget(wdgRenameBar);
    
    layoutParent = new QHBoxLayout;
    layoutParent->setMargin(0);
    layoutParent->addLayout(layoutListAndEnterNewName); 
    layoutParent->addLayout(layoutToolButtons);
    
    setLayout(layoutParent);
    
}


void ksCathegoryEditorWidget::connectSlots()
{
    connect(btnAdd, SIGNAL(clicked()), this, SLOT(addCathegory()));
    connect(btnDelete, SIGNAL(clicked()), this, SLOT(deleteCathegory()));
    connect(btnMoveUp, SIGNAL(clicked()), this, SLOT(moveCathegoryUp()));
    connect(btnMoveDown, SIGNAL(clicked()), this, SLOT(moveCathegoryDown()));
    connect(btnRename, SIGNAL(clicked()), this, SLOT(renameCathegory()));
    connect(lstCathegoryList, SIGNAL(currentRowChanged(int)), this, SLOT(deactivateRenameMode()));
}


void ksCathegoryEditorWidget::initWidgets()
{
    wdgRenameBar->setContentsMargins(0, 0, 0, 0);
    deactivateRenameMode();
}


void ksCathegoryEditorWidget::retranslateUi()
{
    btnAdd->setText(tr("Add"));
    btnDelete->setText(tr("Delete"));
    btnMoveUp->setText(tr("Move Up"));
    btnMoveDown->setText(tr("Move Down"));
    btnRename->setText(tr("Rename"));
    
    lblEnterNewName->setText(tr("Enter a new name:"));
    
    setWindowTitle(tr("Edit Cathegories"));
}


void ksCathegoryEditorWidget::reloadIcons()
{
    btnAdd->setIcon(ksIconCatcher::getIcon("add", 16));
    btnDelete->setIcon(ksIconCatcher::getIcon("remove", 16));
    btnMoveUp->setIcon(ksIconCatcher::getIcon("up", 16));
    btnMoveDown->setIcon(ksIconCatcher::getIcon("down", 16));
    //m_renameIcon = QIcon(); // empty icon
    m_submitIcon = ksIconCatcher::getIcon("button_ok", 16);
    if(isInRenameMode())
    {
        btnRename->setIcon(m_submitIcon);
    }
    else
    {
        btnRename->setIcon(m_renameIcon);
    }
}


void ksCathegoryEditorWidget::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if(event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
}


void ksCathegoryEditorWidget::setDatabaseToEdit(xmlObject* newDatabase)
{
    database = newDatabase;
    cathList.clear();
    if (!database)
    {
        return;
    }
    
    for(int i = 0; i < database->nGetObjectCounter(); i++)
    {
        xmlObject* currentCathegory = database->cGetObjectByIdentifier(i);
        
        ksPlattformSpec::addMissingCathegoryAttributes(currentCathegory);
        ksCathegoryHistoryItem itemToAdd;
        itemToAdd.setName(ksPlattformSpec::szToUmlauts(currentCathegory->cGetAttributeByName("name")->value()));
        itemToAdd.setOldIndex(i);
        itemToAdd.setSourceObject(currentCathegory);
        
        cathList.append(itemToAdd);
    }
    refreshListWidget();
    deactivateRenameMode();
    
}


void ksCathegoryEditorWidget::refreshListWidget()
{
    int oldSelection = lstCathegoryList->currentRow();
    lstCathegoryList->clear();
    
    for(int i = 0; i < cathList.count(); i++)
    {
        if(cathList[i].isToDelete())
        {
            // only draw not deleted items
            continue;
        }
        QListWidgetItem* item = new QListWidgetItem(cathList[i].newName());
        lstCathegoryList->addItem(item);
    }
    if(cathList.isValidIndex(oldSelection))
    {
        // if valid, then restore old selection
        lstCathegoryList->setCurrentRow(oldSelection);
    }
    else
    {   // else: select first row
        lstCathegoryList->setCurrentRow(0);
    }
}

void ksCathegoryEditorWidget::applyChanges()
{
    if(!database)
    {
        return;
    }
    
    for(int i = 0; i < cathList.count(); i++)
    {
        applyAddingItem(cathList[i], database);
        applyRenamingItem(cathList[i]);
        database->moveObjectTo(cathList[i].sourceObject(), i);
    }
    
    // delete items, that must be deleted
    for(int i = 0; i < cathList.count(); i++)
    {
        applyDeletingItem(cathList[i], database);
    }
}

void ksCathegoryEditorWidget::applyAddingItem  (ksCathegoryHistoryItem& item, xmlObject*  parentObject, bool alsoAddIfNotIsNew)
{
    if(parentObject == NULL)
    {
        return;
    }
    if(alsoAddIfNotIsNew || item.isNew())
    {
        // only add, if necessary or wanted
        xmlObject* newCathegory = parentObject->cGetObjectByIdentifier(parentObject->nAddObject("cathegory"));
        item.setSourceObject(newCathegory);
        item.setIfIsNew(FALSE);
    }
    ksPlattformSpec::addMissingCathegoryAttributes(item.sourceObject());
    
}

void ksCathegoryEditorWidget::applyRenamingItem(ksCathegoryHistoryItem& item)
{
    xmlObject* source = item.sourceObject();
    if(!source)
    {
        return;
    }
    source->cGetAttributeByName("name")->SetValue(ksPlattformSpec::qstringToSz(item.newName()));
}

void ksCathegoryEditorWidget::applyDeletingItem(ksCathegoryHistoryItem& item, xmlObject* parentObject)
{
    if(!parentObject)
    {
        return;
    }
    //only delete, if necessary
    if(item.isToDelete())
    {
        parentObject->nDeleteObject(item.sourceObject());
        item.setIfToDelete(FALSE);
    }
    item.setSourceObject(NULL);
}


void ksCathegoryEditorWidget::moveCathegoryUp()
{
    QList<QListWidgetItem*> list = lstCathegoryList->selectedItems();
    if(list.count() < 1)
    {
        return;
    }
    QString nameToMove = list.first()->text();
    int     indexToMove = cathList.indexOf(ksCathegoryHistoryItem(nameToMove));
    int     newIndex = indexToMove -1;
    
    if(!cathList.isValidIndex(indexToMove) || !cathList.isValidIndex(newIndex))
    {
        // don't move an invalid index
        return;
    }
    // move cathegory in internal buf
    cathList.move(indexToMove, newIndex);
    
    // refresh cathegory List on screen
    refreshListWidget();
    // restore selection of current item
    lstCathegoryList->setCurrentRow(cathList.indexOf(ksCathegoryHistoryItem(nameToMove)));
}

void ksCathegoryEditorWidget::moveCathegoryDown()
{
    QList<QListWidgetItem*> list = lstCathegoryList->selectedItems();
    if(list.count() < 1)
    {
        return;
    }
    QString nameToMove = list.first()->text();
    int     indexToMove = cathList.indexOf(ksCathegoryHistoryItem(nameToMove));
    int     newIndex = indexToMove + 1; // calculate new Index
    if(!cathList.isValidIndex(indexToMove) || !cathList.isValidIndex(newIndex))
    {
        // don't move an invalid index
        return;
    }
    // move cathegory in internal buf
    cathList.move(indexToMove, newIndex);
    
    // refresh cathegory List on screen
    refreshListWidget();
    // restore selection of current item
    lstCathegoryList->setCurrentRow(cathList.indexOf(ksCathegoryHistoryItem(nameToMove)));
    
}

void ksCathegoryEditorWidget::deleteCathegory()
{
    
    QList<QListWidgetItem*> list = lstCathegoryList->selectedItems();
    if(list.count() < 1)
    {
        return;
    }
    QString nameToDelete = list.first()->text();
    int     indexToDelete = cathList.indexOf(ksCathegoryHistoryItem(nameToDelete));
    
    if(cathList.isValidIndex(indexToDelete))
    {
        // just switch value : not deleted will be deleted,,, deleted will be restored
        cathList[indexToDelete].setIfToDelete(!cathList[indexToDelete].isToDelete());
    }
    
    refreshListWidget();
    
}

void ksCathegoryEditorWidget::addCathegory()
{
    // find free cathegory name
    QString basicName = tr("New Cathegory");
    QString finalName = basicName;
    if(cathList.indexOf(ksCathegoryHistoryItem(finalName)) >= 0)
    {
        int i = 1;
        do
        {
            finalName = (basicName + " %1").arg(i);
            i++;
        } while(cathList.indexOf(ksCathegoryHistoryItem(finalName)) >= 0);
    }
    ksCathegoryHistoryItem newItem(finalName);
    newItem.setIfIsNew(TRUE);
    cathList.append(newItem);
    
    refreshListWidget();
    
    // select new cathegory
    lstCathegoryList->setCurrentRow(cathList.indexOf(ksCathegoryHistoryItem(finalName)));
}

void ksCathegoryEditorWidget::renameCathegory()
{
    QList<QListWidgetItem*> list = lstCathegoryList->selectedItems();
    if(list.count() < 1)
    {
        return;
    }
    QString itemToRename = list.first()->text();
    int     indexToRename = cathList.indexOf(ksCathegoryHistoryItem(itemToRename));
    
    if(!isInRenameMode())
    {
        
        txtNewName->setText(itemToRename);
        txtNewName->selectAll();
    
        activateRenameMode();
    }else
    {
        QString basicName = txtNewName->text();
        QString finalName = basicName;
        int     foundIndex = cathList.indexOf(ksCathegoryHistoryItem(finalName));
        // search an unique name
        if(cathList.isValidIndex(foundIndex) && foundIndex != indexToRename)
        {
            int i = 1;
            do
            {
                finalName = (basicName + " %1").arg(i);
                i++;
                foundIndex = cathList.indexOf(ksCathegoryHistoryItem(finalName));
            } while(cathList.isValidIndex(foundIndex) && ( foundIndex!= indexToRename));
        }
        if(cathList.isValidIndex(indexToRename))
        {
            cathList[indexToRename].setNewName(finalName);
        }
        refreshListWidget();
        deactivateRenameMode();
        
    }
}


void ksCathegoryEditorWidget::activateRenameMode()
{
    bIsRenaming = TRUE;
    wdgRenameBar->setVisible(TRUE);
    
    txtNewName->setFocus();
    btnRename->setText(tr("Submit"));
    btnRename->setIcon(m_submitIcon);
    btnRename->setDefault(TRUE);
    
    emit renameModeChanged(bIsRenaming);
}

void ksCathegoryEditorWidget::deactivateRenameMode()
{
    wdgRenameBar->setVisible(FALSE);
    
    
    btnRename->setText(tr("Rename"));
    btnRename->setIcon(m_renameIcon);
    
    btnRename->setDefault(FALSE);
    lstCathegoryList->setFocus();
    emit renameModeChanged(FALSE);
    bIsRenaming = FALSE;
}



