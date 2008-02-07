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
#ifndef KSCATHEGORYEDITORWIDGET_H
#define KSCATHEGORYEDITORWIDGET_H

#include <QWidget>
#include <QList>
#include <QIcon>


// normal widgets
class QLabel;
class QComboBox;
class QListWidget;
class QPushButton;
class QTreeWidget;
class QLineEdit;



// layouts
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QSplitter;

// other Q_Objects
class QEvent;

class xmlObject;
/**
	@author Thorsten Wissmann <towi89@web.de>
*/

class ksCathegoryHistoryItem // stores information if how a cathegory should be renamed or deleted
{
public:
    ksCathegoryHistoryItem(QString newOldName = "", int newOldIndex = -1)
    {
        bIsToDelete = FALSE;
        nOldIndex = newOldIndex;
        bIsNew = FALSE;
        setName(newOldName);
        source = NULL;
    };
    ksCathegoryHistoryItem(const ksCathegoryHistoryItem &other){ operator=(other); };
    ~ksCathegoryHistoryItem(){   };
    
    bool   isToDelete() const { return bIsToDelete; };
    void   setIfToDelete(bool newIsToDelete = TRUE){ bIsToDelete = newIsToDelete; };
    
    QString oldName() const { return szOldName; };
    void setOldName(QString newOldName) { szOldName = newOldName; };
    
    void setName(QString newName){ setOldName(newName); setNewName(newName); };
    
    QString newName() const { return szNewName; };
    void setNewName(QString newNewName) { szNewName = newNewName; };
    
    void setIfIsNew(bool newIsNew){ bIsNew = newIsNew; };
    bool isNew() const { return bIsNew; };
    void setOldIndex(int newOldIndex){ nOldIndex = newOldIndex; };
    int oldIndex() const { return nOldIndex; };
    
    xmlObject*  sourceObject() const { return source; };
    void  setSourceObject(xmlObject* newSource) { source = newSource; };
    
    void operator=( const ksCathegoryHistoryItem &other)
    {
        setOldName(other.oldName());
        setNewName(other.newName());
        setIfToDelete(other.isToDelete());
        setIfIsNew(other.isNew());
        setOldIndex(other.oldIndex());
        setSourceObject(other.sourceObject());
    }
    
    bool operator==( const ksCathegoryHistoryItem &other)
    {
        return newName() == other.newName();
    }
    
private:
    QString szOldName;
    QString szNewName;
    int     nOldIndex;
    bool    bIsNew;
    bool    bIsToDelete; // if this cathegory should be deleted in ksCathegoryEditorWidget::applyChanges();
    xmlObject*  source;
};


class ksCathegoryHistory : public QList<ksCathegoryHistoryItem>
{
public:
    ksCathegoryHistory(){
        QList<ksCathegoryHistoryItem>::QList();
    };
    ~ksCathegoryHistory(){};
    
    ksCathegoryHistoryItem* findItemByNewName(QString newName)
    {
        for (int i = 0; i < count(); i++)
        {
            if((*this)[i].newName() == newName)
            {
                return &((*this)[i]);
            }
        }
        return NULL;
    };
    
    bool isValidIndex( const int indexToCheck)
    {
        return (indexToCheck >= 0) && (indexToCheck < count());
    }
    
};

class ksCathegoryEditorWidget : public QWidget
{
    Q_OBJECT
    
signals:
    void renameModeChanged(bool newRenameMode);
public:
    ksCathegoryEditorWidget(QWidget *parent = 0);

    ~ksCathegoryEditorWidget();
    void retranslateUi();
    void reloadIcons();
    
    void setDatabaseToEdit(xmlObject* newDatabase);
    
    bool isInRenameMode(){ return bIsRenaming;};
    
    
public slots:
    void applyChanges();
    void moveCathegoryUp();
    void moveCathegoryDown();
    void deleteCathegory();
    void addCathegory();
    void renameCathegory();
    void refreshListWidget();
    
    void activateRenameMode();
    void deactivateRenameMode();
    
    
protected:
    virtual void changeEvent(QEvent* event);
private:
    
    void    initMembers();
    void    allocateWidgets();
    void    createLayouts();
    void    connectSlots();
    void    initWidgets();
    
    // parts of applyChanges()
    static void applyRenamingItem(ksCathegoryHistoryItem& item);
    static void applyDeletingItem(ksCathegoryHistoryItem& item, xmlObject* parentObject);
    static void applyAddingItem  (ksCathegoryHistoryItem& item, xmlObject*  parentObject, bool alsoAddIfNotIsNew = FALSE);
    
    // widgets
    
    //layouts
    QHBoxLayout*    layoutParent;
    QVBoxLayout*    layoutListAndEnterNewName;
    QHBoxLayout*    layoutRenameBar;
    QVBoxLayout*    layoutToolButtons;
    
    
    QListWidget*    lstCathegoryList;
    
    QWidget*        wdgRenameBar;
    QLabel*         lblEnterNewName;
    QLineEdit*      txtNewName;
    QPushButton*    btnSubmitRename;
    
    // ToolButtons
    QPushButton*    btnAdd;
    QPushButton*    btnDelete;
    QPushButton*    btnMoveUp;
    QPushButton*    btnMoveDown;
    QPushButton*    btnRename;
    
    //members
    bool                bIsRenaming;
    
    xmlObject*          database;
    ksCathegoryHistory  cathList;
    QIcon               m_renameIcon;
    QIcon               m_submitIcon;
};

#endif
