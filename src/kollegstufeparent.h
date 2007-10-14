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


#ifndef KS_IS_MAIN_WINDOW
 // defines wether kollegstufeParent shall be a QWidget with a menubar in its layout
 // or if it shall be a QMainWindow with a central widget and with using setMenubar(mnbMenuBar)
#define KS_IS_MAIN_WINDOW
#endif

#ifdef KS_IS_MAIN_WINDOW
    #include <QMainWindow>
#else
    #include <QWidget>
#endif

#include <QTranslator>
#include "xmlparser.h"

// own qobjects
class ksDebugOutput;
// own dialogs
class ksExamProperties;
class ksSubjectProperties;
class ksDatabaseSelection;
class ksStatisticsDialog;
class ksDatabaseProperties;
class ksAbout;
class ksConfigure;


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
class QEvent;

/**
	@author Thorsten Wissmann <towi89@web.de>
*/
class kollegstufeParent
#ifdef KS_IS_MAIN_WINDOW
    : public QMainWindow
#else
    :public QWidget
#endif
{
    Q_OBJECT
            
signals:
    //void languageChanged();
public slots:
    void showAboutDialog();
    void showConfigureDialog();
    // Save/load
    void loadConfigFile();
    void saveConfigFile();
    void showDatabaseSelection();
    void showDatabaseProperties();
    
    void refreshMnaStatisticsChecked();
    // view editor:
    void refreshCathegoryList();
    void refreshSubjectList(int rowToSelectAfterRefresh = -1); // -1 if currentRow shall be selected after refresh
    void refreshExamList();
    void selectedCathegoryChanged();
    void selectedSubjectChanged();
    void selectedExamChanged();
    
    void saveFile(QString newFilename = ""); 
    
    // subject control:
    void subjectAdd();
    void subjectDelete();
    void subjectEdit();
    void subjectMoveUp();
    void subjectMoveDown();
    
    // exam control:
    void examAdd();
    void examDelete();
    void examEdit();
    
    void retranslateUi();
    
    
public:
    kollegstufeParent(QWidget* parentWidget = 0);
    ~kollegstufeParent();
    
    bool wantsToBeShown(){ return bWantsToBeShown;};
    void dontShowMe(){ bWantsToBeShown = FALSE;};
    
    void loadFile(QString newFilename, bool showErrorMsg = FALSE); // returns true on success, otherwise false
    
    bool askForSavingChangedDatabase();//returns true if everything is okey, false if file mustn't be closed
    void setDatabaseChanged( bool newChangeState = TRUE) {  bDatabaseChanged = newChangeState; };
    bool databaseChanged(){ return bDatabaseChanged; };
    
    QString getFilename(){ return szFilename; };
    void reloadTranslator();
protected:
    virtual void closeEvent(QCloseEvent* event);
    virtual void changeEvent(QEvent* event);

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
    // extras - menu
    QAction*        mnaStatistics;
    
    QAction*        mnaConfigureKs;
    QAction*        mnaAboutKs;
    QAction*        mnaAboutQt;
    // file - menu
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
    QPushButton*    btnSubjectMoveUp;
    QPushButton*    btnSubjectMoveDown;
    
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
    ksConfigure*            diaConfigureKs;
    ksStatisticsDialog*     diaStatistics;
    
    // member variables
    
    QTranslator appTranslator;
    bool        bWantsToBeShown;
    
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
