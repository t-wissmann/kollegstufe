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
#include "kollegstufeparent.h"
#include <io/ksdebugoutput.h>
#include "dateConverter.h"
#include "ksplattformspec.h"
#include <widgets/examitem.h>
#include <pluginengine/kspluginengine.h>
#include <io/ksiconcatcher.h>

// dialogs:
#include <dialogs/ksdatabaseselection.h>
#include <dialogs/ksdatabaseproperties.h>
#include <dialogs/ksstatisticsdialog.h>
#include <dialogs/ksconfigure.h>
#include <dialogs/kscategorychooserdialog.h>
#include <pluginengine/kspluginconfigurationdialog.h>
#include <dialogs/kssubjectproperties.h>
#include <dialogs/ksexamproperties.h>
#include <dialogs/ksabout.h>
#include <dialogs/ksdebugdialog.h>

// own widgets
#include <widgets/kssubjectstatusbar.h>
#include <widgets/ksstatisticswidget.h>
#include <widgets/ksfilterwidget.h>

// normal widgets
#include <QComboBox>
#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QToolButton>
#include <QTreeWidget>
#include <QLabel>
#include <QStatusBar>

// widget containing widgets
#include <QGroupBox>
#include <QMenuBar>
#include <QAction>
#include <QStackedWidget>

// layouts
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSplitter>
#include <QScrollArea>

// other qt-classes
#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QPixmap>
#include <QIcon>
#include <QListWidgetItem>
#include <QList>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QSizePolicy>
#include <QCloseEvent>
#include <QEvent>
#include <QFileOpenEvent>
#include <QModelIndex>

kollegstufeParent::kollegstufeParent(QWidget* parentWidget)

#ifdef KS_IS_MAIN_WINDOW
    : QMainWindow(parentWidget)
#else
    : QWidget(parentWidget)
#endif
{
    initMembers();
    handleArguments();
    debugOutput->putDebugOutput("Creating GUI");
    allocateWidgets();
    allocateDialogs();
    createMenuBar();
    createLayouts();
    connectSlots();
    initWidgets();
    initPluginEngine();
    reloadIcons();
    
    debugOutput->putDebugOutput("Loading configuration");
    retranslateUi(); // ensure, that there are texts
    loadConfigFile();
    debugOutput->putDebugOutput("MainWindow created");
    
}


kollegstufeParent::~kollegstufeParent()
{
    
    debugOutput->putDebugOutput("MainWindow destroyed");
    
    delete pPluginEngine;
    
    delete debugOutput;
}


void kollegstufeParent::initMembers()
{
    bWantsToBeShown = TRUE;
    
    
    // init pointers
    pPluginEngine       = NULL;
    /*
    currentDataPart     = NULL;
    currentPropertyPart = NULL;
    currentCathegory    = NULL;
    currentSubject      = NULL;
    currentExam         = NULL;
    */
    bDatabaseChanged    = FALSE;
    
    // get application path
    szApplicationPath = QApplication::applicationDirPath();
    if( !szApplicationPath.endsWith(QDir::separator()))
    {
        szApplicationPath += QDir::separator();
    }
    
    // init output
    debugOutput = new ksDebugOutput;
}

void kollegstufeParent::handleArguments()
{
    QStringList args = QApplication::arguments();
    if(args.contains("--debug"))
    {
        debugOutput->enableDebugMode();
    connect(debugOutput, SIGNAL(printDebugLine(QString)), debugOutput, SLOT(putStandartQtDebugOutut(QString)));
    }
    if(args.contains("--help"))
    {
        debugOutput->putStandartQtDebugOutut("Usage: " + args[0] + " [--debug]");
        debugOutput->putStandartQtDebugOutut("Possible arguments:");
        debugOutput->putStandartQtDebugOutut("\t--version\t\tprints version");
        debugOutput->putStandartQtDebugOutut("\t--debug\t\tprints debug information to screen");
        debugOutput->putStandartQtDebugOutut("\t--help\t\tprints this help");
        dontShowMe();
    }
    if(args.contains("--version"))
    {
        debugOutput->putStandartQtDebugOutut("version of kollegstufe: " + ksPlattformSpec::versionAsString());
        debugOutput->putStandartQtDebugOutut("made by Thorsten Wissmann kollegstufe@thorsten-wissmann.de www.thorsten-wissmann.de");
        dontShowMe();
    }
}

void kollegstufeParent::allocateWidgets()
{
    //cathegory & subject selection & parent Widgets
    cmbCathegory    = new QComboBox;
    lstSubjectList  = new QListWidget;
    wdgSubjectSelection = new QWidget;
    statusbar = new QStatusBar;
    // subject tools:
    btnSubjectAdd       = new QToolButton;
    btnSubjectDelete    = new QToolButton;
    btnSubjectEdit      = new QPushButton;
    btnSubjectMoveUp    = new QToolButton;
    btnSubjectMoveDown  = new QToolButton;
    
    //exam selection
    grpExamList     = new QGroupBox;
    stackedExamLists = new QStackedWidget;
    lblExamListStyle = new QLabel;
    cmbExamListStyle = new QComboBox;
    cmbExamListStyle->addItem("table");
    cmbExamListStyle->addItem("chart");
    wdgExamFilter    = new ksFilterWidget;
    wdgSubjectStatusbar = new ksSubjectStatusbar;
    lstExamList     = new QTreeWidget;
    statisticsExamList = new ksStatisticsWidget;
    scrollStatisticsExamList = new QScrollArea;
    scrollStatisticsExamList->setWidget(statisticsExamList);
    btnExamAdd      = new QPushButton;
    btnExamDelete   = new QPushButton;
    btnExamEdit     = new QPushButton;
    
    stackedExamLists->addWidget(lstExamList);
    stackedExamLists->addWidget(scrollStatisticsExamList);
    
}

void kollegstufeParent::allocateDialogs()
{
    diaExamProperties    = NULL;
    diaSubjectProperties = NULL;
    diaCategoryChooser   = NULL;
    diaDatabaseSelection = NULL;
    diaDatabaseProperties= NULL;
    diaAbout             = NULL;
    diaConfigureKs       = NULL;
    diaPluginConfig      = NULL;
    diaDebugDialog = new ksDebugDialog(this);
    connect(debugOutput, SIGNAL(printDebugLine(QString)), diaDebugDialog, SLOT(putDebugMessage(QString)));
    
    diaStatistics = new ksStatisticsDialog(this);
}

void kollegstufeParent::createMenuBar()
{
    // allocate menus
    mnbMenuBar  = new QMenuBar;
    mnbMenuBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    //add menus to menubar
    mnmFile     = mnbMenuBar->addMenu("&File");
    mnmEdit     = mnbMenuBar->addMenu("&Edit");
    mnmPlugins  = mnbMenuBar->addMenu("");
    mnmSettings = mnbMenuBar->addMenu("&Settings");
    mnmWindow   = mnbMenuBar->addMenu("&Window");
    mnmHelp     = mnbMenuBar->addMenu("&Help");
    
    
    //add actions to menus
    // 1. File - menu
    mnaLoadDatabase      = mnmFile->addAction("");
    mnaSave              = mnmFile->addAction("");
    mnaDatabaseProperties= mnmFile->addAction("");
    mnmFile->addSeparator();
    mnaQuit              = mnmFile->addAction("");
    
    // 2. Edit - menu
    mnmEditSubject      = mnmEdit->addMenu("");
    mnaEditSubjectAdd   = mnmEditSubject->addAction("");
    mnaEditSubjectDelete= mnmEditSubject->addAction("");
    mnaEditSubjectEdit  = mnmEditSubject->addAction("");
    mnaEditSubjectMoveUp= mnmEditSubject->addAction("");
    mnaEditSubjectMoveDown= mnmEditSubject->addAction("");
    mnaEditSubjectMoveToCategory = mnmEditSubject->addAction("");
    
    mnmEditExam         = mnmEdit->addMenu("");
    mnaEditExamAdd      = mnmEditExam->addAction("");
    mnaEditExamDelete   = mnmEditExam->addAction("");
    mnaEditExamEdit     = mnmEditExam->addAction("");
    
    
    // 3. Window - menu
    mnaStatistics = mnmWindow->addAction("statistics");
    mnaStatistics->setCheckable(TRUE);
    
    // 4. settings - menu
    mnaConfigurePlugins = mnmSettings->addAction("");
    mnaConfigureKs      = mnmSettings->addAction("");
    
    // 5. Help - menu
    mnaShowHelp   = mnmHelp->addAction("help");
                    mnmHelp->addSeparator();
    mnaShowDebugDialog = mnmHelp->addAction("debugdialog");
    mnaAboutQt    = mnmHelp->addAction("about qt");
    mnaAboutKs    = mnmHelp->addAction("about kollegstufe");
    
    // INFO: icons will be set at reloadIcons()
    // INFO: texts will be set at retranslateUi();
    
}

void kollegstufeParent::createLayouts()
{
    // Subject selection layout
    layoutSubjectSelection = new QVBoxLayout;
    layoutSubjectToolBar   = new QHBoxLayout;
    layoutSubjectToolBar->setMargin(0);
    
    layoutSubjectSelection->addWidget(cmbCathegory);
    layoutSubjectSelection->addWidget(lstSubjectList);
    layoutSubjectToolBar->addWidget(btnSubjectAdd);
    layoutSubjectToolBar->addWidget(btnSubjectDelete);
    layoutSubjectToolBar->addWidget(btnSubjectMoveUp);
    layoutSubjectToolBar->addWidget(btnSubjectMoveDown);
    layoutSubjectSelection->addLayout(layoutSubjectToolBar);
    wdgSubjectSelection->setLayout(layoutSubjectSelection);
    
    //exam List "toolbar" layout
    layoutExamToolbar = new QHBoxLayout;
    layoutExamToolbar->setMargin(0);
    layoutExamToolbar->addWidget(lblExamListStyle);
    layoutExamToolbar->addWidget(cmbExamListStyle);
    layoutExamToolbar->addWidget(wdgExamFilter);
    layoutExamToolbar->addStretch(0);
    layoutExamToolbar->addWidget(wdgSubjectStatusbar);
    
    // layout for exam selection group box
    
    layoutExamList = new QGridLayout;
    //layoutExamList->setMargin(2);
    layoutExamList->addLayout(layoutExamToolbar, 0, 0, 1, 4);
    layoutExamList->addWidget(stackedExamLists, 1, 0,1, 4);
    layoutExamList->addWidget(btnSubjectEdit, 2, 0, 1, 1);
    layoutExamList->addWidget(btnExamAdd, 2, 1, 1, 1);
    layoutExamList->addWidget(btnExamEdit, 2, 2, 1, 1);
    layoutExamList->addWidget(btnExamDelete, 2, 3, 1, 1);
    
    grpExamList->setLayout(layoutExamList);
    
    //parent Layout and Splitter
    splitterParent = new QSplitter(Qt::Horizontal);
    splitterParent->setContentsMargins(3, 0, 3, 3);
    splitterParent->addWidget(wdgSubjectSelection);
    splitterParent->addWidget(grpExamList);
    
#ifndef KS_IS_MAIN_WINDOW
    layoutParent = new QVBoxLayout;
    layoutParent->setMargin(0);
    layoutParent->setSpacing(0);
    layoutParent->addWidget(mnbMenuBar);
    mnbMenuBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    layoutParent->addWidget(splitterParent);
    layoutParent->addWidget(statusbar);
    statusbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    setLayout(layoutParent);
#else
    setCentralWidget(splitterParent);
    setMenuBar(mnbMenuBar);
    setStatusBar(statusbar);
#endif
    
}

void kollegstufeParent::connectSlots()
{
    
    // selection-change-handling
    connect(cmbCathegory, SIGNAL(currentIndexChanged(int)), this, SLOT(selectedCathegoryChanged()));
    connect(lstSubjectList, SIGNAL(currentRowChanged(int)), this, SLOT(selectedSubjectChanged()));
    connect(lstExamList, SIGNAL(itemSelectionChanged()), this, SLOT(selectedExamChanged()));
    connect(statisticsExamList, SIGNAL(currentItemChanged(int)), this, SLOT(selectedExamChanged()));
    
    //subject control
    connect(btnSubjectAdd, SIGNAL(clicked()), this, SLOT(subjectAdd()));
    connect(btnSubjectDelete, SIGNAL(clicked()), this, SLOT(subjectDelete()));
    connect(btnSubjectEdit, SIGNAL(clicked()), this, SLOT(subjectEdit()));
    connect(btnSubjectMoveUp, SIGNAL(clicked()), this, SLOT(subjectMoveUp()));
    connect(btnSubjectMoveDown, SIGNAL(clicked()), this, SLOT(subjectMoveDown()));
    
    //menubar-menu-actions
    // mnmFile
    connect(mnaQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(mnaSave, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(mnaLoadDatabase, SIGNAL(triggered()), this, SLOT(showDatabaseSelection()));
    connect(mnaDatabaseProperties, SIGNAL(triggered()), this, SLOT(showDatabaseProperties()));
    // mnmEdit
    connect(mnaEditSubjectAdd, SIGNAL(triggered()), this, SLOT(subjectAdd()));
    connect(mnaEditSubjectDelete, SIGNAL(triggered()), this, SLOT(subjectDelete()));
    connect(mnaEditSubjectEdit, SIGNAL(triggered()), this, SLOT(subjectEdit()));
    connect(mnaEditSubjectMoveUp, SIGNAL(triggered()), this, SLOT(subjectMoveUp()));
    connect(mnaEditSubjectMoveDown, SIGNAL(triggered()), this, SLOT(subjectMoveDown()));
    connect(mnaEditSubjectMoveToCategory, SIGNAL(triggered()), this, SLOT(subjectMoveToCategory()));
    
    connect(mnaEditExamAdd, SIGNAL(triggered()), this, SLOT(examAdd()));
    connect(mnaEditExamDelete, SIGNAL(triggered()), this, SLOT(examDelete()));
    connect(mnaEditExamEdit, SIGNAL(triggered()), this, SLOT(examEdit()));
    
    // mnmSettings
    connect(mnaConfigureKs, SIGNAL(triggered()), this, SLOT(showConfigureDialog()));
    connect(mnaConfigurePlugins, SIGNAL(triggered()), this, SLOT(showPluginConfigDialog()));
    
    // mnmWindow
    connect(mnaStatistics, SIGNAL(toggled(bool)), diaStatistics, SLOT(setVisible(bool)));
    connect(mnaStatistics, SIGNAL(toggled(bool)), wdgSubjectStatusbar, SLOT(toggleStatisticsButton(bool)));
    connect(wdgSubjectStatusbar, SIGNAL(statisticsButtonToggled(bool)), mnaStatistics, SLOT(setChecked(bool)));
    connect(diaStatistics, SIGNAL(rejected()), this, SLOT(refreshMnaStatisticsChecked()));
    connect(diaStatistics, SIGNAL(accepted()), this, SLOT(refreshMnaStatisticsChecked()));
    // mnmHelp
    connect(mnaShowHelp, SIGNAL(triggered()), this, SLOT(showHelpDialog()));
    connect(mnaShowDebugDialog, SIGNAL(triggered()), this, SLOT(showDebugDialog()));
    connect(mnaAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(mnaAboutKs, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
    
    //exam control
    connect(wdgExamFilter, SIGNAL(filterChanged(QString)), this, SLOT(setExamListFilter(QString)));
    connect(cmbExamListStyle, SIGNAL(currentIndexChanged(int)), stackedExamLists, SLOT(setCurrentIndex(int)));
    connect(btnExamAdd, SIGNAL(clicked()), this, SLOT(examAdd()));
    connect(btnExamDelete, SIGNAL(clicked()), this, SLOT(examDelete()));
    connect(btnExamEdit, SIGNAL(clicked()), this, SLOT(examEdit()));
    connect(lstExamList, SIGNAL(doubleClicked( const QModelIndex &)), this, SLOT(examEdit()));
    connect(statisticsExamList, SIGNAL(itemDoubleClicked(int)), this, SLOT(examEdit()));
    connect(statisticsExamList, SIGNAL(itemTriggered(int)), this, SLOT(examEdit()));
    
}

void kollegstufeParent::initWidgets()
{
    // exam selection widgets:
    QTreeWidgetItem* examListHeader = new QTreeWidgetItem(QTreeWidgetItem::UserType);
    examListHeader->setText(0, "id");
    examListHeader->setText(1, "Semester");
    examListHeader->setText(2, "Date");
    examListHeader->setText(3, "Nr");
    examListHeader->setText(4, "Type");
    examListHeader->setText(5, "Points");
    lstExamList->setHeaderItem(examListHeader);
    lstExamList->hideColumn(0);
    lstExamList->header()->setClickable(TRUE);
    lstExamList->header()->setMovable(TRUE);
    lstExamList->setSortingEnabled(TRUE);
    lstExamList->sortByColumn(2, Qt::AscendingOrder);
    
    // subject selection widgets:
    layoutSubjectSelection->setMargin(1);
    // subject control
    btnSubjectAdd->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    btnSubjectDelete->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    btnSubjectMoveUp->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    btnSubjectMoveDown->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    
    // init scrollarea of statisticsExamList
    scrollStatisticsExamList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollStatisticsExamList->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollStatisticsExamList->setBackgroundRole(QPalette::Base);
    scrollStatisticsExamList->setAutoFillBackground(FALSE);
    scrollStatisticsExamList->setWidgetResizable(TRUE);
    statisticsExamList->setFrameStyle(QFrame::NoFrame);
    // init main window / this
    splitterParent->setChildrenCollapsible(FALSE);
    
    
    
    //set alternatingrowcolors
    lstSubjectList->setAlternatingRowColors(TRUE);
    lstExamList->setAlternatingRowColors(TRUE);
    
}

void kollegstufeParent::reloadIcons()
{
    
    setWindowIcon(ksIconCatcher::getIcon("kollegstufe"));
    // set Icons for MenuBar:
    mnaLoadDatabase->setIcon(ksIconCatcher::getIcon(QString("fileopen"), 16));
    mnaSave->setIcon(ksIconCatcher::getIcon(QString("filesave"), 16));
    mnaQuit->setIcon(ksIconCatcher::getIcon(QString("exit"), 16));
    mnaAboutKs->setIcon(ksIconCatcher::getIcon(QString("kollegstufe"), 16));
    
    mnaEditSubjectAdd->setIcon(ksIconCatcher::getIcon(QString("add"), 16));
    mnaEditSubjectDelete->setIcon(ksIconCatcher::getIcon(QString("remove"), 16));
    mnaEditSubjectEdit->setIcon(ksIconCatcher::getIcon(QString("configure"), 16));
    mnaEditSubjectMoveUp->setIcon(ksIconCatcher::getIcon(QString("up"), 16));
    mnaEditSubjectMoveDown->setIcon(ksIconCatcher::getIcon(QString("down"), 16));
    
    mnaEditExamAdd->setIcon(ksIconCatcher::getIcon(QString("add"), 16));
    mnaEditExamDelete->setIcon(ksIconCatcher::getIcon(QString("remove"), 16));
    mnaEditExamEdit->setIcon(ksIconCatcher::getIcon(QString("edit"), 16));
    
    mnaConfigureKs->setIcon(ksIconCatcher::getIcon(QString("configure"), 16));
    mnaConfigurePlugins->setIcon(ksIconCatcher::getIcon(QString("configure"), 16));
    
    // set Icons for Subject-control-buttons
    btnSubjectAdd->setIcon(ksIconCatcher::getIcon(QString("add"), 16));
    btnSubjectDelete->setIcon(ksIconCatcher::getIcon(QString("remove"), 16));
    btnSubjectMoveUp->setIcon(ksIconCatcher::getIcon(QString("up"), 16));
    btnSubjectMoveDown->setIcon(ksIconCatcher::getIcon(QString("down"), 16));
    btnSubjectEdit->setIcon(ksIconCatcher::getIcon(QString("configure"), 16));
    
    // set Icons for Exam-controlButtons
    btnExamAdd->setIcon(ksIconCatcher::getIcon(QString("add"), 16));
    btnExamDelete->setIcon(ksIconCatcher::getIcon(QString("remove"), 16));
    btnExamEdit->setIcon(ksIconCatcher::getIcon(QString("edit"), 16));
    // icons for exam view mode
    cmbExamListStyle->setItemIcon(0, ksIconCatcher::getIcon(QString("view_detailed"), 16));
    cmbExamListStyle->setItemIcon(1, ksIconCatcher::getIcon(QString("visualizations"), 16));
    
    
    //set icons for dialogs
    if(diaStatistics)
    {
        diaStatistics->setWindowIcon(this->windowIcon());
    }
    
}

void kollegstufeParent::initPluginEngine()
{
    
    pluginInformation.setMainWindow(this);
    pluginInformation.setPluginMenu(mnmPlugins);
    
    pPluginEngine = new ksPluginEngine(&pluginInformation);
}

void kollegstufeParent::loadFile(QString newFilename, bool showErrorMsg)
{
    debugOutput->putDebugOutput("Loading file \'" + newFilename + "\'");
    
    // clear current database
    currentDatabase.nSetAttributeCounter(0);
    currentDatabase.nSetObjectCounter(0);
    
    int nResult = ReadFileToClass( ksPlattformSpec::qstringToSz(newFilename), &currentDatabase);
    szFilename = newFilename;
    if( nResult != 0)
    {
        splitterParent->setEnabled(FALSE);
        szFilename = "";
        if ( showErrorMsg )
        {
            QMessageBox::critical(this, tr("Error when loading File - Kollegstufe"),
                                tr("Error when trying to load File <i>") + newFilename + "</i>\n"
                                  + tr("It seems, you haven't got enough Read-Rights"));
        }
    }
    else
    {
        splitterParent->setEnabled(TRUE);
    }
    // re-init pointers
    pluginInformation.setAllDatabasePointersToNull();
    /*
    currentDataPart     = NULL;
    currentPropertyPart = NULL;
    currentCathegory    = NULL;
    currentSubject      = NULL;
    currentExam         = NULL;
    */
    
    ksPlattformSpec::addMissingDatabaseAttributes(&currentDatabase);
    pluginInformation.setCurrentPropertyPart(currentDatabase.cGetObjectByName("properties"));
    ksPlattformSpec::addMissingPropertiesAttributes(pluginInformation.currentPropertyPart());
    
    applyPropertyChanges(); // load properties to gui
    
    //refresh GUI
    refreshCathegoryList();
    pluginInformation.setCurrentDatabase(&currentDatabase);
    
    // load plugin configs to plugin engine
    pPluginEngine->loadPluginConfigurations(currentDatabase.cGetObjectByName("plugins"), TRUE);  // local
    
    // reset Database change state:
    setDatabaseChanged(FALSE);
}


void kollegstufeParent::saveFile(QString newFilename)
{
    if( newFilename == "")
    {
        newFilename = szFilename;
    }
    
    
    debugOutput->putDebugOutput("Saving file \'" + newFilename + "\'");
    
    
    ksPlattformSpec::addMissingDatabaseAttributes(&currentDatabase);
    // write local plugin config
    xmlObject* pluginConfig = currentDatabase.cGetObjectByName("plugins");
    pPluginEngine->savePluginConfigurations(pluginConfig, TRUE); // save local configurations
    
    //create kollegstufe dir in home
    if(!ksPlattformSpec::createKsDir())
    {
        QMessageBox::critical(this, tr("Error when creating Folder - Kollegstufe"),
                              "Error during creating folder <i>" + ksPlattformSpec::getKsDir() + "</i>\n"
                                      + tr("It seems, you haven't got the necessary Write-Rights!"));
        return;
    }
    //create archive File home/.kollegstufe
    if(0 != WriteClassToFile( ksPlattformSpec::qstringToSz(newFilename), &currentDatabase))
    {
        QMessageBox::critical(this, tr("Error during saving file- Kollegstufe"),
                              "Error during saving file <i>" + newFilename + "</i>\n"
                                      + tr("It seems, you haven't got the necessary Write-Rights!"));
        return;
    }
    //on success:
    setDatabaseChanged(FALSE);
    
    QString dbName = currentWindowTitle;
    statusbar->showMessage(tr("Archive \"%filename\" was succesfully saved").replace("%filename", dbName) , 5000);
    
    return;
}

void kollegstufeParent::loadConfigFile()
{
    //create kollegstufe dir in home
    if(!ksPlattformSpec::createKsDir())
    {
        QMessageBox::critical(this, tr("Fehler beim Ordner-Erstellen - Kollegstufe"),
                              "Fehler beim Erstellen des Ordners <i>" + ksPlattformSpec::getKsDir() + "</i>\n"
                                      + tr("It seems, you haven't got the necessary Write-Rights!"));
        return;
    }
    //create config File home/.kollegstufe
    if(!ksPlattformSpec::createConfigFile())
    {
        QMessageBox::critical(this, tr("Fehler beim Datei-Laden/Erstellen - Kollegstufe"),
                              "Fehler beim Erstellen oder Laden der Date <i>" + ksPlattformSpec::getKsDir() + "config.xml</i>\n"
                                      + tr("It seems, you haven't got the necessary Write- or Read-Rights!"));
        return;
    }
    
    //clear old config;
    xmlConfig.nSetObjectCounter(0);
    xmlConfig.nSetAttributeCounter(0);
    if(0 != ReadFileToClass(ksPlattformSpec::qstringToSz(QString(ksPlattformSpec::getKsDir() + "config.xml")), &xmlConfig))
    {
        QMessageBox::critical(this, tr("Fehler beim Datei-Laden - Kollegstufe"),
                              "Fehler beim Laden der Date <i>" + ksPlattformSpec::getKsDir() + "config.xml</i>\n"
                                      + tr("It seems, you haven't got the necessary Read-Rights!"));
        return;
    }
    
    loadGuiConfigToWidgets(); // load gui config to widgets
                    // there, the ksPlattformSpec::addMissingConfigAttributes() happens
    // 4. load plugins
    
    pPluginEngine->loadPluginConfigurations(xmlConfig.cGetObjectByName("plugins"), FALSE); // load global
    
    // 5. file
    if(ksPlattformSpec::fileExists(xmlConfig.cGetObjectByName("session")->cGetObjectByName("file")->szGetContent()))
    {
        loadFile(xmlConfig.cGetObjectByName("session")->cGetObjectByName("file")->szGetContent(), TRUE);
    }
    else
    {
        if(!diaDatabaseSelection)
        {
            diaDatabaseSelection = new ksDatabaseSelection(this);
        }
        diaDatabaseSelection->setCurrentFile("");
        diaDatabaseSelection->exec();
        switch(diaDatabaseSelection->result())
        {
            case QDialog::Accepted:
                loadFile(diaDatabaseSelection->getCurrentFile(), TRUE);
                break;
            case QDialog::Rejected:
                dontShowMe();
                break;
            default:
                break;
        }
    }
}

void kollegstufeParent::loadGuiConfigToWidgets()
{
    // add missing Attributes in config File
    ksPlattformSpec::addMissingConfigAttributes(&xmlConfig);
    
    
    // write config Attributes to widget-attributes:
    // 1. window size
    int nWidth  = xmlConfig.cGetObjectByName("window-settings")->cGetAttributeByName("width")->nValueToInt();
    int nHeight = xmlConfig.cGetObjectByName("window-settings")->cGetAttributeByName("height")->nValueToInt();
    resize(nWidth, nHeight);
    // 2. splitter position
    int nSplitterCoord = xmlConfig.cGetObjectByName("window-settings")->cGetObjectByName("splitterMain")->
            cGetAttributeByName("coord")->nValueToInt();
    QList<int> list = splitterParent->sizes();
    QList<int>::Iterator it = list.begin();
    *it = nSplitterCoord;
    ++it;
    *it = this->width() - nSplitterCoord;
    splitterParent->setSizes(list);
    
    // 3. exam list style
    QString examListViewMode = xmlConfig.cGetObjectByName("window-settings")->cGetObjectByName("examlist")->
            cGetAttributeByName("viewmode")->value();
    if(examListViewMode == "table")
    {
        cmbExamListStyle->setCurrentIndex(0);
    }
    else if(examListViewMode == "chart")
    {
        cmbExamListStyle->setCurrentIndex(1);
    }
    
    // 4. update Language
    reloadTranslator();
    
    
}

void kollegstufeParent::saveWidgetAttributesToConfig()
{
    // add missing Attributes in config File
    ksPlattformSpec::addMissingConfigAttributes(&xmlConfig);
    // write widget-attributes to config Attributes:
    // 1. window size
    xmlConfig.cGetObjectByName("window-settings")->cGetAttributeByName("width")->SetValueToInt(width());
    xmlConfig.cGetObjectByName("window-settings")->cGetAttributeByName("height")->SetValueToInt(height());
    
    // 2. splitter position
    QList<int> list = splitterParent->sizes();
    QList<int>::Iterator it = list.begin();
    xmlConfig.cGetObjectByName("window-settings")->cGetObjectByName("splitterMain")->cGetAttributeByName("coord")->SetValueToInt(*it);
    // 3. exam list view mode
    QString examListViewMode = "table";
    if(cmbExamListStyle->currentIndex() == 1) // if the chart is shown
    {
        examListViewMode = "chart";
    }
    xmlConfig.cGetObjectByName("window-settings")->cGetObjectByName("examlist")->
            cGetAttributeByName("viewmode")->SetValue(ksPlattformSpec::qstringToSz(examListViewMode));
    
    // 4. language:
    // language is already saved to xmlconifig
}

void kollegstufeParent::saveConfigFile()
{
    debugOutput->putDebugOutput("Saving Config File");
    saveWidgetAttributesToConfig(); // save gui options to config
                //there the ksPlattformSpec::addMissingConfigAttributes() will happen
    // 4. plugins
    pPluginEngine->savePluginConfigurations(xmlConfig.cGetObjectByName("plugins"), FALSE); // FALSE : global
    
    // 5. file
    xmlConfig.cGetObjectByName("session")->cGetObjectByName("file")->nSetContent(ksPlattformSpec::qstringToSz(szFilename));
    // create .kollegstufe direcotry if it doesnt exist
    if(!ksPlattformSpec::createKsDir())
    {
        QMessageBox::critical(this, tr("Fehler beim Ordner-Erstellen - Kollegstufe"),
                              "Fehler beim Erstellen des Ordners <i>" + ksPlattformSpec::getKsDir() + "</i>\n"
                                      + tr("Anscheinend haben sie nicht die noetigen Schreibrechte dazu!"));
        return;
    }
    // write config  to file
    if(0 != WriteClassToFile(ksPlattformSpec::qstringToSz(QString(ksPlattformSpec::getKsDir() + "config.xml")), &xmlConfig))
    {
        QMessageBox::critical(this, tr("Fehler beim Datei-Schreiben - Kollegstufe"),
                              "Fehler beim Schreiben der Date <i>" + ksPlattformSpec::getKsDir() + "config.xml</i>\n"
                                      + tr("Anscheinend haben sie nicht die noetigen Schreibrechte dazu!"));
        return;
    }
}

void kollegstufeParent::retranslateUi()
{
    // subjects
    btnSubjectEdit->setText(tr("Edit Subject"));
    // exam selection
    grpExamList->setTitle(tr("Subject Properties"));
    btnExamAdd->setText(tr("New Exam"));
    btnExamDelete->setText(tr("Delete"));
    btnExamEdit->setText(tr("Edit"));

    // menubar - menus
    mnmFile->setTitle(tr("&File"));
    mnmEdit->setTitle(tr("&Edit"));
    // pluginsmenu
    if(!mnmPlugins->title().isEmpty())
    {
        mnmPlugins->setTitle(tr("Plugins"));
    }
    
    mnmSettings->setTitle(tr("&Settings"));
    mnmWindow->setTitle(tr("&Window"));
    mnmHelp->setTitle(tr("&Help"));
    //file menu
    mnaLoadDatabase->setText(tr("Load archiv"));
    mnaLoadDatabase->setShortcut(tr("Ctrl+O"));
    mnaSave->setShortcut(tr("Ctrl+S"));
    mnaQuit->setShortcut(tr("Ctrl+Q"));
    mnaSave->setText(tr("Save"));
    mnaDatabaseProperties->setText(tr("Properties"));
    mnaQuit->setText(tr("Quit"));
    // edit menu
    
    mnmEditSubject->setTitle(tr("Subject"));
    mnaEditSubjectAdd->setText(tr("Add"));
    mnaEditSubjectDelete->setText(tr("Delete"));
    mnaEditSubjectEdit->setText(tr("Edit"));
    mnaEditSubjectMoveUp->setText(tr("Move Up"));
    mnaEditSubjectMoveDown->setText(tr("Move Down"));
    mnaEditSubjectMoveToCategory->setText(tr("Move to category"));
    
    mnmEditExam->setTitle(tr("Exam"));
    mnaEditExamAdd->setText(tr("Add"));
    mnaEditExamDelete->setText(tr("Delete"));
    mnaEditExamEdit->setText(tr("Edit"));
    
    // settings menu
    mnaConfigurePlugins->setText(tr("Configure Plugins"));
    mnaConfigureKs->setText(tr("Configure Kollegstufe"));
    // window menu
    mnaStatistics->setText(tr("Statistics"));
    // help menu
    mnaShowDebugDialog->setText(tr("Debug-Window"));
    mnaShowHelp->setText(tr("User Manual"));
    mnaAboutQt->setText(tr("About Qt"));
    mnaAboutKs->setText(tr("About Kollegstufe"));
    
    // exam list table
    lblExamListStyle->setText(tr("List style:"));
    cmbExamListStyle->setItemText(0, tr("Table"));
    cmbExamListStyle->setItemText(1, tr("Chart"));
    QTreeWidgetItem* examListHeader = lstExamList->headerItem();
    examListHeader->setText(1, tr("Semester"));
    examListHeader->setText(2, tr("Date"));
    examListHeader->setText(3, tr("Nr"));
    examListHeader->setText(4, tr("Type"));
    examListHeader->setText(5, tr("Points"));
    
    // retranslate plugin engine
    pPluginEngine->retranslateUi();
    
    retranslateTooltips();
    resetWindowTitle();
    selectedCathegoryChanged();
}

void kollegstufeParent::retranslateTooltips()
{
    //subject selection
    btnSubjectEdit->setToolTip(tr("Click to edit the properties of currently selected subject"));
    btnSubjectAdd->setToolTip(tr("Add Subject"));
    btnSubjectDelete->setToolTip(tr("Delete Subject"));
    btnSubjectMoveUp->setToolTip(tr("Move Subject Up"));
    btnSubjectMoveDown->setToolTip(tr("Move Subject Down"));
    //exam selection
    btnExamAdd->setToolTip(tr("Adds a new exam to currently selected subject"));
    btnExamEdit->setToolTip(tr("Click to edit the properties of currently selected exam"));
    btnExamDelete->setToolTip(tr("Removes the currently selected exam"));
    
}

void kollegstufeParent::resetWindowTitle()
{
    QString newWindowTitle = currentWindowTitle;
    if (!newWindowTitle.isEmpty())
    {
        newWindowTitle += " - ";
    }
    newWindowTitle += tr("Kollegstufe");
    setWindowTitle(newWindowTitle);
}

void kollegstufeParent::reloadTranslator()
{
    if(!xmlConfig.cGetObjectByName("language")->szGetContent())
    {
        // if there is no language choose english
        xmlConfig.cGetObjectByName("language")->nSetContent("english");
    }
    QString language = xmlConfig.cGetObjectByName("language")->szGetContent();
    ksPlattformSpec::setLanguage(language, &appTranslator);
    // ensure that there are example archivs in current language
    ksPlattformSpec::catchKsDatabaseTemplates(language);
}

void kollegstufeParent::showDatabaseSelection()
{
    if(!diaDatabaseSelection)
    {
        diaDatabaseSelection = new ksDatabaseSelection(this);
    }
    
    diaDatabaseSelection->setCurrentFile(szFilename);
    diaDatabaseSelection->exec();
    switch(diaDatabaseSelection->result())
    {
        case QDialog::Accepted:
            if(askForSavingChangedDatabase())
            {
                loadFile(diaDatabaseSelection->getCurrentFile());
            }
            break;
        case QDialog::Rejected:
            
            break;
        default:
            break;
    }
}

void kollegstufeParent::closeEvent(QCloseEvent* event)
{
    if(askForSavingChangedDatabase())
    {
        saveConfigFile();
        event->accept();
        /*diaStatistics->close();
        if (diaAbout)
        {
            diaAbout->close();
        }
        if (diaPluginConfig)
        {
            diaPluginConfig->close();
        }*/
    }
    else
    {
        event->ignore();
    }
}

void kollegstufeParent::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if(event->type() == QEvent::LanguageChange)
    {
        QString newLanguage = "unknown language";
        if(xmlConfig.cGetObjectByName("language")->szGetContent())
        {
            newLanguage = xmlConfig.cGetObjectByName("language")->szGetContent();
        }
        debugOutput->putDebugOutput("Translating GUI to \'" + newLanguage + "\'");
        retranslateUi();
    }
}

bool kollegstufeParent::askForSavingChangedDatabase()
{
    
    // only ask to quit if data has been changed
    
    if (databaseChanged())
    {
        QMessageBox questionBox(this);
        QPushButton* saveButton = new QPushButton(ksIconCatcher::getIcon("filesave", 16), tr("Save"));
        QPushButton* discardButton = new QPushButton(tr("Discard"));
        QPushButton* rejectButton = new QPushButton(ksIconCatcher::getIcon("button_cancel", 16), tr("Don't Close"));
        questionBox.addButton(saveButton, QMessageBox::ApplyRole);
        questionBox.addButton(discardButton, QMessageBox::DestructiveRole);
        questionBox.addButton(rejectButton, QMessageBox::RejectRole);
        questionBox.setIconPixmap(ksIconCatcher::getIconPixmap("exit", 48));
        questionBox.setText(tr("Seit dem letzten Speichern haben sich Daten verandert!\nMoechten Sie diese vor dem Schliessen speichern oder verwerfen ?"));
        questionBox.setWindowTitle(tr("Closing Database - Kollegstufe"));
        
        questionBox.exec();
        if(questionBox.clickedButton() == saveButton) {
                saveFile();
                return TRUE;
        } else if(questionBox.clickedButton() == discardButton){
                return TRUE;
        } else if(questionBox.clickedButton() == rejectButton){
            return FALSE;
        }
    }
    // else
    return TRUE;
}

void kollegstufeParent::showDatabaseProperties()
{
    if(!diaDatabaseProperties)
    {
        diaDatabaseProperties = new ksDatabaseProperties(this);
    }
    diaDatabaseProperties->setDatabasePropertiesToEdit(currentDatabase.cGetObjectByName("properties"));
    diaDatabaseProperties->setDatabaseToEdit(pluginInformation.currentDataPart());
    diaDatabaseProperties->exec();
    if(diaDatabaseProperties->result() == QDialog::Accepted)
    {
        refreshCathegoryList();
        applyPropertyChanges();
        setDatabaseChanged();
        refreshExamList();
    }
}

void kollegstufeParent::showAboutDialog()
{
    if (!diaAbout)
    {
        diaAbout = new ksAbout(this);
    }
    diaAbout->show();
}

void kollegstufeParent::showHelpDialog()
{
    QString helpPath = szApplicationPath + ".." + QDir::separator() + "doc" + QDir::separator() + "help.html";
    
    QFileOpenEvent event(helpPath);
    QApplication::sendEvent(this, &event);
}

void kollegstufeParent::showConfigureDialog()
{
    if(!diaConfigureKs)
    {
        diaConfigureKs = new ksConfigure(this);
    }
    diaConfigureKs->setConfigToEdit(&xmlConfig);
    if(diaConfigureKs->exec())
    {
        reloadTranslator();
    }
    
}

void kollegstufeParent::showPluginConfigDialog()
{
    if(!diaPluginConfig)
    {
        diaPluginConfig = new ksPluginConfigurationDialog(this);
    }
    diaPluginConfig->setPluginEngine(pPluginEngine);
    diaPluginConfig->show();
}

void kollegstufeParent::showDebugDialog()
{
    if(!diaDebugDialog)
    {
        diaDebugDialog = new ksDebugDialog(this);
        connect(debugOutput, SIGNAL(printDebugLine(QString)), diaDebugDialog, SLOT(putDebugMessage(QString)));
    }
    diaDebugDialog->show();
}

void kollegstufeParent::refreshMnaStatisticsChecked()
{
    // only set, if there is something to change.
    //this had been done to avoid infinite recursion
    
    if(mnaStatistics->isChecked() != diaStatistics->isVisible())
    {
        mnaStatistics->setChecked(diaStatistics->isVisible());
    }
}

void kollegstufeParent::subjectAdd()
{
    if( pluginInformation.currentCategory() == NULL)
    {
        return;
    }
    
    QString    szNewSubjectName;
    QString    szBasicName;
    
    int        nNewSubjectId;
    xmlObject* newSubject;
    debugOutput->putDebugOutput("Adding Subject...");
    if ((nNewSubjectId = pluginInformation.currentCategory()->nAddObject("subject")) < 0)
        return;
    
    newSubject = pluginInformation.currentCategory()->cGetObjectByIdentifier(nNewSubjectId);
    ksPlattformSpec::addMissingSubjectAttributes(newSubject);
    
     // set strings to current subject name
    szNewSubjectName = ksPlattformSpec::szToUmlauts(newSubject->cGetAttributeByName("name")->value());
    szBasicName = szNewSubjectName;
    
    //Find first "free" Subject Name
    int SubjectNumber = 1;
    xmlObject*  foundedSubject;
    while((foundedSubject=pluginInformation.currentCategory()->cGetObjectByAttributeValue("name", ksPlattformSpec::qstringToSz(szNewSubjectName))) && foundedSubject != newSubject)
    {
        SubjectNumber++;
        szNewSubjectName = szBasicName + " " + QString::number(SubjectNumber);
    }
    newSubject->cGetAttributeByName("name")->SetValue(ksPlattformSpec::qstringToSz(szNewSubjectName));
    
    //run subject dialog
    if(!diaSubjectProperties)
    {
        diaSubjectProperties = new ksSubjectProperties(this);
    }
    
    diaSubjectProperties->setSubjectToEdit(newSubject);
    diaSubjectProperties->setCathegoryOfSubject(pluginInformation.currentCategory());
    diaSubjectProperties->exec();
    
    switch(diaSubjectProperties->result())
    {
        case QDialog::Accepted:
            setDatabaseChanged();
            refreshSubjectList();
            szNewSubjectName = ksPlattformSpec::szToUmlauts(newSubject->cGetAttributeByName("name")->value());
            if(lstSubjectList->findItems(szNewSubjectName, Qt::MatchExactly).size() > 0)
            {
                lstSubjectList->setCurrentItem(lstSubjectList->findItems(szNewSubjectName, Qt::MatchExactly).first());
            }
            debugOutput->putDebugOutput("Subject \'" + szNewSubjectName + "\' successfully added");
            break;
        case QDialog::Rejected:
            pluginInformation.currentCategory()->nDeleteObject(newSubject);
            break;
        default:
            refreshSubjectList();
            break;
    }
}

void kollegstufeParent::subjectDelete()
{
    // if no item is selected
    if(!lstSubjectList->currentItem())
        return;
    
    QString subjectNameToDelete = lstSubjectList->currentItem()->text();
    
    debugOutput->putDebugOutput(QString("Deleting Subject \'%subjectname\' ...").replace("%subjectname", subjectNameToDelete));
    QString message = tr("Do you really want to delete the subject \'%subjectname\' ?").replace("%subjectname", subjectNameToDelete);
    
    // create messagebox
    QMessageBox msgBox(this);
    QPushButton* yesButton = new QPushButton(ksIconCatcher::getIconPixmap("button_ok" , 16), tr("Yes"));
    msgBox.setWindowTitle(tr("Deleting a subject"));
    msgBox.setText(message);
    msgBox.setIconPixmap(ksIconCatcher::getIconPixmap("editdelete", 48));
    msgBox.addButton(yesButton,
                     (QMessageBox::YesRole));
    msgBox.addButton(new QPushButton(ksIconCatcher::getIconPixmap("button_cancel" , 16), tr("No")),
                     (QMessageBox::NoRole));
    msgBox.exec();
    if(msgBox.clickedButton() == yesButton)
    {
        pluginInformation.currentCategory()->nDeleteObject(pluginInformation.currentCategory()->cGetObjectByAttributeValue("name",
                                        ksPlattformSpec::qstringToSz(subjectNameToDelete)));

        
        refreshSubjectList();
        setDatabaseChanged();
        
        debugOutput->putDebugOutput("Subject \'" + subjectNameToDelete + "\' successfully deleted");
    }
}

void kollegstufeParent::subjectEdit()
{
    if(!diaSubjectProperties)
    {
        diaSubjectProperties = new ksSubjectProperties(this);
    }
    if(!pluginInformation.currentSubject())
    {
        return;
    }
    
    diaSubjectProperties->setSubjectToEdit(pluginInformation.currentSubject());
    diaSubjectProperties->setCathegoryOfSubject(pluginInformation.currentCategory());
    diaSubjectProperties->exec();
    switch(diaSubjectProperties->result())
    {
        case QDialog::Accepted:
            setDatabaseChanged();
            refreshSubjectList();
            break;
        case QDialog::Rejected:
            
            break;
        default:
            refreshSubjectList();
            break;
    }
}


void kollegstufeParent::subjectMoveUp()
{
    if(!pluginInformation.currentSubject() || !pluginInformation.currentCategory())
    {
        return;
    }
    if(lstSubjectList->count() <= 1)
    {
        return;
    }
    pluginInformation.currentCategory()->moveObjectTo(pluginInformation.currentSubject(), lstSubjectList->currentRow()-1);
    refreshSubjectList(lstSubjectList->currentRow()-1);
    setDatabaseChanged();
}

void kollegstufeParent::subjectMoveDown()
{
    if(!pluginInformation.currentSubject() || !pluginInformation.currentCategory())
    {
        return;
    }
    if(lstSubjectList->count() <= 1)
    {
        return;
    }
    
    pluginInformation.currentCategory()->moveObjectTo(pluginInformation.currentSubject(), lstSubjectList->currentRow()+1);
    
    refreshSubjectList(lstSubjectList->currentRow()+1);
    setDatabaseChanged();
}

void kollegstufeParent::subjectMoveToCategory()
{
    xmlObject* currentSubject = pluginInformation.currentSubject();
    xmlObject* currentDataPart = pluginInformation.currentDataPart();
    if(!currentSubject || !currentDataPart)
    {
        statusbar->showMessage(tr("Please select a subject first"), 3000);
        return;
    }
    if(!diaCategoryChooser)
    {
        diaCategoryChooser = new ksCategoryChooserDialog(this);
    }
    diaCategoryChooser->setData(currentDataPart, currentSubject);
    if(diaCategoryChooser->exec() == QDialog::Accepted)
    {
        setDatabaseChanged(TRUE);
        // select new category
        cmbCathegory->setCurrentIndex(cmbCathegory->findText(diaCategoryChooser->categorySelected()));
    }
}

void kollegstufeParent::setExamListFilter(QString filter)
{
    //qDebug("Exam list Filter has been set to \'%s\'", filter.toAscii().data());
    // filter for chart
    statisticsExamList->setFilter(filter);
    
    // filter for table
    setExamListTableFilter(filter);
    
}

void kollegstufeParent::setExamListTableFilter(QString filter)
{
    QStringList keys = filter.split(" ", QString::SkipEmptyParts);
    ExamItem*        currentExam = !lstExamItems.isEmpty() ? lstExamItems.first() : NULL;
    
    for(int i = 0; i < lstExamItems.size(); i++)
    {
        currentExam = lstExamItems[i];
        if(!currentExam)
        {
            continue;
        }
        
        
        QString keyword;
        bool    hasMatch = TRUE;
        for(int k = 0; k < keys.size(); k++)
        {
            keyword = keys[k];
            if(!currentExam->hasMatchOn(keyword))
            {
                hasMatch = FALSE;
                break;
            }
        }
        currentExam->setHidden(!hasMatch);
        
    }
}

void kollegstufeParent::examAdd()
{
    
    if(pluginInformation.currentSubject() == NULL)
    {
        // return if there is no subject selected
        return;
    }
    int nNewExamIdentifier = pluginInformation.currentSubject()->nAddObject("exam");
    xmlObject* newExam = pluginInformation.currentSubject()->cGetObjectByIdentifier(nNewExamIdentifier);
    if (newExam == NULL)
    {
        return; 
    }
    int i = 0;
    QString newId;
    newId.setNum(i);
    while (pluginInformation.currentSubject()->cGetObjectByAttributeValue("id", ksPlattformSpec::qstringToSz(newId)) != NULL)
    {
        i++;
        newId.setNum(i);
    }
    newExam->nAddAttribute("id",ksPlattformSpec::qstringToSz(newId));
    ksPlattformSpec::addMissingExamAttributes(newExam);
    
    // run exam property dialog
    if(!diaExamProperties)
    {
        diaExamProperties = new ksExamProperties(this);
    }
    diaExamProperties->setProperties(pluginInformation.currentPropertyPart());
    diaExamProperties->setExamToEdit(newExam);
    diaExamProperties->exec();
    switch(diaExamProperties->result())
    {
        case QDialog::Accepted:
            setDatabaseChanged();
            break;
        case QDialog::Rejected:
            pluginInformation.currentSubject()->nDeleteObject(newExam);
            refreshExamList();
            // set dia-pointer to NULL, so that there is no SEGMENTAION fault
            // if the current Exam is deleted and retranslateUi() is called
            diaExamProperties->setProperties(NULL);
            diaExamProperties->setExamToEdit(NULL);
            return;
            break;
        default:
            break;
    }
    refreshExamList();
    
    // set Selection to new exam
    QList <QTreeWidgetItem*> itemList = lstExamList->findItems(newExam->cGetAttributeByName("id")->value() ,0);
    if (itemList.size() > 0)
    {
        lstExamList->setCurrentItem(itemList.first());
    }
    else
    {
        lstExamList->setCurrentItem(lstExamList->itemAt(0,0));
    }
    
    // set dia-pointer to NULL, so that there is no SEGMENTAION fault
    // if the current Exam is deleted and retranslateUi() is called
    diaExamProperties->setProperties(NULL);
    diaExamProperties->setExamToEdit(NULL);
    return;
}

void kollegstufeParent::examDelete()
{
    
    if(pluginInformation.currentSubject() == NULL)
    {
        // return if there is no subject selected
        statusbar->showMessage(tr("Select a subject first"), 3000);
        return;
    }
    if(pluginInformation.currentExam() == NULL)
    {
        statusbar->showMessage(tr("Select an exam first"), 3000);
        // return if there is no exam selected
        return;
    }
    
    QString message = tr("Do you really want to delete ");
    message += ksPlattformSpec::getArticleForNoun(pluginInformation.currentExam()->cGetObjectByAttributeValue("name", "type")->cGetAttributeByName("value")->value(), ksPlattformSpec::kasusAkkusativ);
    if(pluginInformation.currentExam()->cGetObjectByAttributeValue("name", "number")->cGetAttributeByName("value")->nValueToInt() > 0)
    {
        message += pluginInformation.currentExam()->cGetObjectByAttributeValue("name", "number")->cGetAttributeByName("value")->value();
        message += ". ";
    }
    message += pluginInformation.currentExam()->cGetObjectByAttributeValue("name", "type")->cGetAttributeByName("value")->value();
    cDateConverter date;
    date.setDateString(pluginInformation.currentExam()->cGetObjectByAttributeValue("name", "date")->cGetAttributeByName("value")->value());
    
    message += tr(" from %date ?").replace("%date", date.humanDate());
    
    // create messagebox
    QMessageBox msgBox(this);
    QPushButton* yesButton = new QPushButton(ksIconCatcher::getIconPixmap("button_ok" , 16), tr("Yes"));
    msgBox.setWindowTitle( tr("Deleting an Exam"));
    msgBox.setText(message);
    msgBox.setIconPixmap(ksIconCatcher::getIconPixmap("editdelete", 48));
    msgBox.addButton(yesButton,
                     (QMessageBox::YesRole));
    msgBox.addButton(new QPushButton(ksIconCatcher::getIconPixmap("button_cancel" , 16), tr("No")),
                     (QMessageBox::NoRole));
    msgBox.exec();
    if(msgBox.clickedButton() == yesButton)
    {
        pluginInformation.currentSubject()->nDeleteObject(pluginInformation.currentExam());
        pluginInformation.setCurrentExam(NULL);
        refreshExamList();
        setDatabaseChanged();
    }
    
}

void kollegstufeParent::examEdit()
{
    if(!diaExamProperties)
    {
        diaExamProperties = new ksExamProperties(this);
    }
    if(!pluginInformation.currentExam())
    {
        statusbar->showMessage(tr("Select an exam first"), 3000);
        return;
    }
    diaExamProperties->setProperties(pluginInformation.currentPropertyPart());
    diaExamProperties->setExamToEdit(pluginInformation.currentExam());
    diaExamProperties->exec();
    switch(diaExamProperties->result())
    {
        case QDialog::Accepted:
            setDatabaseChanged();
            refreshExamList();
            break;
        case QDialog::Rejected:
            
            break;
        default:
            refreshExamList();
            break;
    }
    // set dia-pointer to NULL, so that there is no SEGMENTAION fault
    // if the current Exam is deleted and retranslateUi() is called
    diaExamProperties->setProperties(NULL);
    diaExamProperties->setExamToEdit(NULL);
    
}

void kollegstufeParent::selectedCathegoryChanged()
{
    if (pluginInformation.currentDataPart())
    {
        pluginInformation.setCurrentCategory( pluginInformation.currentDataPart()->cGetObjectByAttributeValue("name", (ksPlattformSpec::qstringToSz(cmbCathegory->currentText()))));
    }
    else
    {
        pluginInformation.setCurrentCategory(NULL);
    }
    refreshSubjectList();
    //to refresh everything
    selectedSubjectChanged();
}

void kollegstufeParent::selectedSubjectChanged()
{
    if (lstSubjectList->currentItem() && pluginInformation.currentCategory())
    {
        grpExamList->setEnabled(TRUE);
        btnSubjectDelete->setEnabled(TRUE);
        pluginInformation.setCurrentSubject(pluginInformation.currentCategory()->cGetObjectByAttributeValue("name", ksPlattformSpec::qstringToSz(lstSubjectList->currentItem()->text())));
        grpExamList->setTitle(tr("Properties of subject ") + lstSubjectList->currentItem()->text());
        
    }
    else
    {
        grpExamList->setEnabled(FALSE);
        btnSubjectDelete->setEnabled(FALSE);
        grpExamList->setTitle( tr("No Subject Selected"));
        pluginInformation.setCurrentSubject(NULL);
    }
    if(diaStatistics)
    {
        diaStatistics->setSubject(pluginInformation.currentSubject());
        diaStatistics->setProperties(pluginInformation.currentPropertyPart());
    }
    wdgSubjectStatusbar->setSubject(pluginInformation.currentSubject());
    refreshExamList();
    selectExam(NULL); // unselect current Exam
}

void kollegstufeParent::selectedExamChanged()
{
    if (pluginInformation.currentSubject() == NULL)
    {// if no subject is selected, then there can't be an currentExam
        pluginInformation.setCurrentExam(NULL);
        return;
    }
    
    xmlObject* currentExam = NULL;
    if(cmbExamListStyle->currentIndex() == 0) // if table is shown
    {
        QString selectedID = "foo";
        QList<QTreeWidgetItem *> itemList = lstExamList->selectedItems();
        
        if( itemList.size() > 0)
        {
            selectedID = itemList.first()->text(0);
        }
        else
        {
            pluginInformation.setCurrentExam(NULL);
            return;
            
        }
        
        currentExam = pluginInformation.currentSubject()->cGetObjectByAttributeValue("id", ksPlattformSpec::qstringToSz(selectedID));
    }
    else if(cmbExamListStyle->currentIndex() == 1) // if chart is shown
    {
        currentExam = statisticsExamList->selectedXmlSource();
        //qDebug("statistik hat selektiert: %d", (int)currentExam);
    }
    else
    {
        return;
    } // return if nothing valid is chosen in cmbExamListStyle
    
    selectExam(currentExam);
        
    
    
}

void kollegstufeParent::refreshCathegoryList()
{
    int nBackupSelectedIndex = cmbCathegory->currentIndex();
    if(nBackupSelectedIndex < 0)
    {
        nBackupSelectedIndex = 0;
    }
    cmbCathegory->clear();
    // add dataPart if is missing
    if (!pluginInformation.currentDataPart())
    {
        if(currentDatabase.cGetObjectByName("data"))
        {
            pluginInformation.setCurrentDataPart( currentDatabase.cGetObjectByName("data"));
        }
        else
        {
            pluginInformation.setCurrentDataPart( currentDatabase.cGetObjectByIdentifier(currentDatabase.nAddObject("data")));
        }
    }
    for(int i = 0; i < pluginInformation.currentDataPart()->nGetObjectCounter(); i++)
    {
        if(!pluginInformation.currentDataPart()->cGetObjectByIdentifier(i))
        {
            continue;
        }
        xmlObject* currentObjectToAdd = pluginInformation.currentDataPart()->cGetObjectByIdentifier(i);
        if (QString("cathegory") == currentObjectToAdd->szGetName())
        {
            if(!currentObjectToAdd->cGetAttributeByName("name"))
            {
                currentObjectToAdd->nAddAttribute("name", "Unbekannt");
            }
            cmbCathegory->addItem(ksPlattformSpec::szToUmlauts(currentObjectToAdd->cGetAttributeByName("name")->value()));
        }
    }
    
    // restore Selection from before refresh
    cmbCathegory->setCurrentIndex(nBackupSelectedIndex);
    pluginInformation.setCurrentCategory(pluginInformation.currentDataPart()->cGetObjectByAttributeValue("name", ksPlattformSpec::qstringToSz(cmbCathegory->currentText())));
    
    selectedSubjectChanged();
}

void kollegstufeParent::refreshSubjectList(int rowToSelectAfterRefresh)
{
    if(rowToSelectAfterRefresh < 0 || rowToSelectAfterRefresh >= lstSubjectList->count())
    {
        //backup last Selection if there is no special row wanted
        rowToSelectAfterRefresh = lstSubjectList->currentRow();
    }
    if(rowToSelectAfterRefresh < 0)
    {
        rowToSelectAfterRefresh = 0;
    }
    lstSubjectList->clear();
    if( !pluginInformation.currentCategory())
    {
        pluginInformation.setCurrentSubject(NULL);
        return;
    }
    for (int i = 0; i < pluginInformation.currentCategory()->nGetObjectCounter(); i++)
    {
        if(!pluginInformation.currentCategory()->cGetObjectByIdentifier(i))
        {
            continue;
        }
        xmlObject* currentObjectToAdd = pluginInformation.currentCategory()->cGetObjectByIdentifier(i);
        if (QString("subject") == currentObjectToAdd->szGetName())
        {
            if(!currentObjectToAdd->cGetAttributeByName("name"))
            {
                currentObjectToAdd->nAddAttribute("name", "Unbekannt");
            }
            lstSubjectList->addItem(ksPlattformSpec::szToUmlauts(currentObjectToAdd->cGetAttributeByName("name")->value()));
        }
    }
    
    // restore Selection from before refresh
    lstSubjectList->setCurrentRow(rowToSelectAfterRefresh);
}

void kollegstufeParent::refreshExamList()
{
    //backup last Selection
    xmlObject* backupCurrentExam = pluginInformation.currentExam();
    
    // refresh list
    lstExamItems.clear();
    lstExamList->clear();
    if(!pluginInformation.currentPropertyPart() || !pluginInformation.currentSubject())
    {
        return;
    }
    
    int best  = pluginInformation.currentPropertyPart()->cGetObjectByName("rating")->cGetAttributeByName("best")->nValueToInt();
    int worst = pluginInformation.currentPropertyPart()->cGetObjectByName("rating")->cGetAttributeByName("worst")->nValueToInt();
    
    if(currentDatabase.cGetObjectByName("properties")->cGetObjectByName("time")->nGetObjectCounter() <= 0)
    {//if semester list is empty, then we don't need the semester column
        lstExamList->hideColumn(1);
    }
    else
    {// else show it again
        lstExamList->showColumn(1);
    }
    
    if (best > worst)
    {
        // point  mode
        QTreeWidgetItem* head = lstExamList->headerItem();
        head->setText(head->columnCount()-1, tr("Points"));
    }else
    {
        // mark- mode
        QTreeWidgetItem* head = lstExamList->headerItem();
        head->setText(head->columnCount()-1, tr("Mark"));
    }
    
    ExamItem*           examToAdd;
    QString             currentSemester;
    QString             currentDate;
    cDateConverter      currentDateClass;
    QString             currentNumber;
    QString             currentType;
    QString             currentPoints;
    
    for (int i = 0; i < pluginInformation.currentSubject()->nGetObjectCounter(); i++)
    {
        xmlObject*          currentXmlExam;
        if(!(currentXmlExam = pluginInformation.currentSubject()->cGetObjectByIdentifier(i)))
        {
            continue;
        }
        ksPlattformSpec::addMissingExamAttributes(currentXmlExam);
        
        currentSemester = ksPlattformSpec::szToUmlauts(currentXmlExam->cGetObjectByAttributeValue("name", "semester")->cGetAttributeByName("value")->value());
        currentDate     = ksPlattformSpec::szToUmlauts(currentXmlExam->cGetObjectByAttributeValue("name", "date")->cGetAttributeByName("value")->value());
        currentNumber   = ksPlattformSpec::szToUmlauts(currentXmlExam->cGetObjectByAttributeValue("name", "number")->cGetAttributeByName("value")->value());
        currentType     = ksPlattformSpec::szToUmlauts(currentXmlExam->cGetObjectByAttributeValue("name", "type")->cGetAttributeByName("value")->value());
        currentPoints   = ksPlattformSpec::szToUmlauts(currentXmlExam->cGetObjectByAttributeValue("name", "mark")->cGetAttributeByName("value")->value());
        
        // transform date
        currentDateClass.setDateString(ksPlattformSpec::qstringToSz(currentDate));
        currentDate = currentDateClass.humanDate();
        
        // edit number:
        if (currentNumber == "0")
            currentNumber = "";
        else
            currentNumber += ".";
        
        // edit semester
        if(currentSemester == "auto")
            currentSemester = ksPlattformSpec::getSemesterContainigDate( currentDatabase.cGetObjectByName("properties")->cGetObjectByName("time"),  QString(currentDateClass.getDateString()));
        /*
        if(currentSemester == "12/1")
            currentSemester = "12 / 1";
        if(currentSemester == "12/2")
            currentSemester = "12 / 2";
        if(currentSemester == "13/1")
            currentSemester = "13 / 1";
        if(currentSemester == "13/2")
            currentSemester = "13 / 2";
        */
        
        examToAdd = new ExamItem(lstExamList);
        examToAdd->setText(0, currentXmlExam->cGetAttributeByName("id")->value());
        examToAdd->setText(1, currentSemester);
        examToAdd->setText(2, currentDate);
        examToAdd->setText(3, currentNumber);
        examToAdd->setText(4, currentType);
        examToAdd->setText(5, currentPoints);
        lstExamItems.append(examToAdd);
    }
    
    //resize columns so that texts can be seen:
    for(int i = 0; i < lstExamList->columnCount(); i++)
    {
        lstExamList->resizeColumnToContents(i);
    }
    
    // refresh exam list in chart i.e. ksStatisticsWidget
    statisticsExamList->loadItemListFromSubject(pluginInformation.currentSubject());
    
    
    // refresh average computing components
    diaStatistics->refreshUiAndChildren();
    wdgSubjectStatusbar->calculateAverage();
    
    // restore Selection from before refresh
    selectExam(backupCurrentExam);
    
    // refresh filter for examlist table
    setExamListTableFilter(wdgExamFilter->filter());
    
}

void kollegstufeParent::selectExam(xmlObject* exam)
{
    
    // refresh for statistics widget:
    // get index in list of chart widget i.e. ksStatisticswidget i.e. exam list
    int statIndex = statisticsExamList->indexOfItem(exam);
    //qDebug("selected exam index is %d", statIndex);
    // select new selected item in exam list chart
    statisticsExamList->setSelectedItem(statIndex);
        
    
    if(exam == pluginInformation.currentExam())
    {
        return; // return if nothing would change
    }
    if(pluginInformation.currentSubject() == NULL)
    {
        // if there isn't a current subject,
        // then there can't be any exams
        return;
    }
    // apply new exam to pluginInformation object
    pluginInformation.setCurrentExam(exam);
    
    
    // select new selected item in exam list table
    QString id = "foo";
    if(exam && exam->cGetAttributeByName("id"))
    {
        id = exam->cGetAttributeByName("id")->value();
    }
    QList <QTreeWidgetItem*> itemList = lstExamList->findItems(id ,0);
    if (itemList.size() > 0)
    {
        lstExamList->setCurrentItem(itemList.first());
    }
    else
    {
        lstExamList->setCurrentItem(NULL);
    }
    //qDebug("id %s was selected, pointer = %d", id.toAscii().data(), (int)exam);
        
    if(diaStatistics)
    {
        diaStatistics->setSelectedExam(exam);
    }
}

void kollegstufeParent::applyPropertyChanges()
{
    xmlObject* properties = pluginInformation.currentPropertyPart();
    if(!properties)
    {
        return;
    }
    ksPlattformSpec::addMissingPropertiesAttributes(properties);
    int best  = properties->cGetObjectByName("rating")->cGetAttributeByName("best")->nValueToInt();
    int worst = properties->cGetObjectByName("rating")->cGetAttributeByName("worst")->nValueToInt();
    
    statisticsExamList->setMinMaxY(worst, best);
    
    if(diaStatistics)
    {
        diaStatistics->setProperties(pluginInformation.currentPropertyPart());
    }
    if(pluginInformation.currentPropertyPart() && pluginInformation.currentPropertyPart()->cGetObjectByName("author"))
    {
        currentWindowTitle = ksPlattformSpec::szToUmlauts(pluginInformation.currentPropertyPart()->cGetObjectByName("author")->szGetContent());
    }
    resetWindowTitle();
}



