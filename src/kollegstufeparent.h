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

#ifndef KOLLEGSTUFEPARENT_H
#define KOLLEGSTUFEPARENT_H

#include <QWidget>
#include "xmlparser.h"

// own qobjects
class ksDebugOutput;
// own dialogs
class ksExamProperties;
class ksSubjectProperties;
class ksDatabaseSelection;
class ksDatabaseProperties;
class ksAbout;


// normal widgets
class QLabel;
class QComboBox;
class QListWidget;
class QPushButton;
class QTreeWidget;

// widget containing widgets
class QGroupBox;
class QStatusBar;
// menubar:
class QMenuBar;
class QMenu;
class QAction;


// layouts
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QSplitter;

// other Q_Objects
class QCloseEvent;

/**
	@author Thorsten Wissmann <towi89@web.de>
*/
class kollegstufeParent : public QWidget
{
    Q_OBJECT
public slots:
    void showAboutDialog();
    // Save/load
    void loadConfigFile();
    void saveConfigFile();
    void showDatabaseSelection();
    void showDatabaseProperties();
    // view editor:
    void refreshCathegoryList();
    void refreshSubjectList();
    void refreshExamList();
    void selectedCathegoryChanged();
    void selectedSubjectChanged();
    void selectedExamChanged();
    
    void saveFile(QString newFilename = ""); 
    
    // subject control:
    void subjectAdd();
    void subjectDelete();
    void subjectEdit();
    
    // exam control:
    void examAdd();
    void examDelete();
    void examEdit();
    
    
public:
    kollegstufeParent(QWidget* parent = 0);
    ~kollegstufeParent();
    
    void loadFile(QString newFilename, bool showErrorMsg = FALSE); // returns true on success, otherwise false
    
    bool askForSavingChangedDatabase();//returns true if everything is okey, false if file mustn't be closed
    void setDatabaseChanged( bool newChangeState = TRUE) {  bDatabaseChanged = newChangeState; };
    bool databaseChanged(){ return bDatabaseChanged; };
    
    QString getFilename(){ return szFilename; };
    
protected:
    virtual void closeEvent(QCloseEvent* event);
private:
    void    initMembers();
    void    allocateWidgets();
    void    allocateDialogs();
    void    createMenuBar();
    void    createLayouts();
    void    connectSlots();
    void    initWidgets();
    
    // parentWidgets
    QSplitter*      splitterParent;
    QVBoxLayout*    layoutParent;
    //menubar:
    QMenuBar*       mnbMenuBar;
    // menus
    QMenu*          mnmFile;
    QMenu*          mnmExtras;
    //actions
    QAction*        mnaAboutKs;
    QAction*        mnaAboutQt;
    QAction*        mnaLoadDatabase;
    QAction*        mnaDatabaseProperties;
    QAction*        mnaSave;
    QAction*        mnaQuit;
    
    
    // widgets for subject selection
    QGridLayout*    layoutSubjectSelection;
    QComboBox*      cmbCathegory;
    QListWidget*    lstSubjectList;
    QWidget*        wdgSubjectSelection;
    QPushButton*    btnSubjectAdd;
    QPushButton*    btnSubjectDelete;
    QPushButton*    btnSubjectEdit;
    
    // widgets for exam selection;
    QGridLayout*    layoutExamList;
    QGroupBox*      grpExamList;
    QTreeWidget*    lstExamList;
    QPushButton*    btnExamAdd;
    QPushButton*    btnExamDelete;
    QPushButton*    btnExamEdit;
    
    // debug output:
    ksDebugOutput*  debugOutput;
    
    // dialog windows
    ksExamProperties*       diaExamProperties;
    ksSubjectProperties*    diaSubjectProperties;
    ksDatabaseProperties*   diaDatabaseProperties;
    ksDatabaseSelection*    diaDatabaseSelection;
    ksAbout*                diaAbout;
    
    // member variables
    QString     szApplicationPath;
    QString     szFilename;
    bool        bDatabaseChanged;
    
    xmlObject   currentDatabase;
    xmlObject*  currentPropertyPart;
    xmlObject*  currentDataPart;
    xmlObject*  currentCathegory;
    xmlObject*  currentSubject;
    xmlObject*  currentExam;
    
    xmlObject   xmlConfig;
    
    
};

#endif
