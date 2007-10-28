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
#include "ksdebugoutput.h"
#include "dateConverter.h"
#include "ksplattformspec.h"
#include "ksdatabaseselection.h"
#include "ksdatabaseproperties.h"
#include "ksstatisticsdialog.h"
#include "ksconfigure.h"
#include "examitem.h"

// dialogs:
#include "kssubjectproperties.h"
#include "ksexamproperties.h"
#include "ksabout.h"

// normal widgets
#include <QComboBox>
#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QTreeWidget>

// widget containing widgets
#include <QGroupBox>
#include <QMenuBar>
#include <QAction>

// layouts
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSplitter>

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
    
    
    debugOutput->putDebugOutput("Loading configuration");
    retranslateUi(); // ensure, that there are texts
    loadConfigFile();
    debugOutput->putDebugOutput("MainWindow created");
    
    //update window's pixels
    update();
}


kollegstufeParent::~kollegstufeParent()
{
    
    debugOutput->putDebugOutput("MainWindow destroyed");
    if (diaExamProperties)
        delete diaExamProperties;
    
    delete debugOutput;
}


void kollegstufeParent::initMembers()
{
    bWantsToBeShown = TRUE;
    
    // init pointers
    currentDataPart     = NULL;
    currentPropertyPart = NULL;
    currentCathegory    = NULL;
    currentSubject      = NULL;
    currentExam         = NULL;
    
    bDatabaseChanged    = FALSE;
    
    // get application path
    szApplicationPath = QApplication::applicationDirPath();
    if( !szApplicationPath.endsWith(QDir::separator()))
    {
        szApplicationPath += QDir::separator();
    }
    
    // init output
    debugOutput = new ksDebugOutput;
    connect(debugOutput, SIGNAL(printDebugLine(QString)), debugOutput, SLOT(putStandartQtDebugOutut(QString)));
}

void kollegstufeParent::handleArguments()
{
    QStringList args = QApplication::arguments();
    if(args.contains("--debug"))
    {
        debugOutput->enableDebugMode();
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
    //cathegory & subject selection
    cmbCathegory    = new QComboBox;
    lstSubjectList  = new QListWidget;
    wdgSubjectSelection = new QWidget;
    // subject tools:
    /*btnSubjectAdd       = new QPushButton(tr("Hinzufuegen"));
    btnSubjectDelete    = new QPushButton("Entfernen");*/
    btnSubjectAdd       = new QPushButton("");
    btnSubjectDelete    = new QPushButton("");
    btnSubjectEdit      = new QPushButton;
    btnSubjectMoveUp    = new QPushButton("");
    btnSubjectMoveDown  = new QPushButton("");
    
    //exam selection
    grpExamList     = new QGroupBox;
    lstExamList     = new QTreeWidget;
    btnExamAdd      = new QPushButton;
    btnExamDelete   = new QPushButton;
    btnExamEdit     = new QPushButton;
    
}

void kollegstufeParent::allocateDialogs()
{
    diaExamProperties    = NULL;
    diaSubjectProperties = NULL;
    diaDatabaseSelection = NULL;
    diaDatabaseProperties= NULL;
    diaAbout             = NULL;
    diaConfigureKs       = NULL;
    
    diaStatistics = new ksStatisticsDialog(this);
}

void kollegstufeParent::createMenuBar()
{
    // allocate menus
    mnbMenuBar  = new QMenuBar;
    mnbMenuBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    //add menus to menubar
    mnmFile     = mnbMenuBar->addMenu("&File");
    mnmExtras   = mnbMenuBar->addMenu("&Extras");
    
    //add actions to menus
    // 1. File - menu
    mnaLoadDatabase      = mnmFile->addAction("");
    mnaSave              = mnmFile->addAction("");
    mnaDatabaseProperties= mnmFile->addAction("");
    mnmFile->addSeparator();
    mnaQuit              = mnmFile->addAction("");
    
    // 2. Extras - menu
    mnaStatistics = mnmExtras->addAction("statistics");
    mnmExtras->addSeparator();
    mnaConfigureKs= mnmExtras->addAction("");
    mnmExtras->addSeparator();
    mnaAboutQt    = mnmExtras->addAction("");
    mnaAboutKs    = mnmExtras->addAction("");
    mnaStatistics->setCheckable(TRUE);
    
    // INFO: icons will be set at initWidgets
    
}

void kollegstufeParent::createLayouts()
{
    // Subject selection layout
    layoutSubjectSelection = new QGridLayout;
    
    layoutSubjectSelection->addWidget(cmbCathegory, 0, 0, 1, 4);
    layoutSubjectSelection->addWidget(lstSubjectList, 1, 0, 1, 4);
    layoutSubjectSelection->addWidget(btnSubjectAdd, 3, 0, 1, 1);
    layoutSubjectSelection->addWidget(btnSubjectDelete, 3, 1, 1, 1);
    layoutSubjectSelection->addWidget(btnSubjectMoveUp, 3, 2, 1, 1);
    layoutSubjectSelection->addWidget(btnSubjectMoveDown, 3, 3, 1, 1);
    wdgSubjectSelection->setLayout(layoutSubjectSelection);
    
    //exam List layout
    layoutExamList = new QGridLayout;
    layoutExamList->setMargin(2);
    layoutExamList->addWidget(lstExamList, 0, 0,1, 4);
    layoutExamList->addWidget(btnSubjectEdit, 1, 0, 1, 1);
    layoutExamList->addWidget(btnExamAdd, 1, 1, 1, 1);
    layoutExamList->addWidget(btnExamEdit, 1, 2, 1, 1);
    layoutExamList->addWidget(btnExamDelete, 1, 3, 1, 1);
    
    grpExamList->setLayout(layoutExamList);
    
    //parent Layout and Splitter
    splitterParent = new QSplitter(Qt::Horizontal);
    splitterParent->setContentsMargins(3, 0, 3, 3);
    splitterParent->addWidget(wdgSubjectSelection);
    splitterParent->addWidget(grpExamList);
    
#ifndef KS_IS_MAIN_WINDOW
    layoutParent = new QVBoxLayout;
    layoutParent->setMargin(0);
    layoutParent->addWidget(mnbMenuBar);
    layoutParent->addWidget(splitterParent);
    
    setLayout(layoutParent);
#else
    setCentralWidget(splitterParent);
    setMenuBar(mnbMenuBar);
#endif
    
}

void kollegstufeParent::connectSlots()
{
    
    // selection-change-handling
    connect(cmbCathegory, SIGNAL(currentIndexChanged(int)), this, SLOT(selectedCathegoryChanged()));
    connect(lstSubjectList, SIGNAL(currentRowChanged(int)), this, SLOT(selectedSubjectChanged()));
    connect(lstExamList, SIGNAL(itemSelectionChanged()), this, SLOT(selectedExamChanged()));
    
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
    // mnmExtras
    connect(mnaStatistics, SIGNAL(toggled(bool)), diaStatistics, SLOT(setVisible(bool)));
    connect(diaStatistics, SIGNAL(rejected()), this, SLOT(refreshMnaStatisticsChecked()));
    connect(diaStatistics, SIGNAL(accepted()), this, SLOT(refreshMnaStatisticsChecked()));
    connect(mnaConfigureKs, SIGNAL(triggered()), this, SLOT(showConfigureDialog()));
    connect(mnaAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(mnaAboutKs, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
    
    //exam control
    connect(btnExamAdd, SIGNAL(clicked()), this, SLOT(examAdd()));
    connect(btnExamDelete, SIGNAL(clicked()), this, SLOT(examDelete()));
    connect(btnExamEdit, SIGNAL(clicked()), this, SLOT(examEdit()));
    connect(lstExamList, SIGNAL(doubleClicked( const QModelIndex &)), this, SLOT(examEdit()));
    
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
    
    // init main window / this
    splitterParent->setChildrenCollapsible(FALSE);
    
    QDir iconDir(szApplicationPath);
    iconDir.cdUp();
    iconDir.cd("pic");
    setWindowIcon(QPixmap(iconDir.filePath("kollegstufe.png")));
    setWindowTitle(tr("Kollegstufe"));
    
    // set Icons for MenuBar:
    mnaLoadDatabase->setIcon(QIcon(QPixmap(iconDir.filePath("fileopen.png"))));
    mnaSave->setIcon(QIcon(QPixmap(iconDir.filePath("filesave.png"))));
    mnaQuit->setIcon(QIcon(QPixmap(iconDir.filePath("exit.png"))));
    mnaAboutKs->setIcon(QIcon(QPixmap(iconDir.filePath("kollegstufe16.png"))));
    // set Icons for Subject-control-buttons
    btnSubjectAdd->setIcon(QIcon(QPixmap(iconDir.filePath("add.png"))));
    btnSubjectDelete->setIcon(QIcon(QPixmap(iconDir.filePath("remove.png"))));
    btnSubjectMoveUp->setIcon(QIcon(QPixmap(iconDir.filePath("up.png"))));
    btnSubjectMoveDown->setIcon(QIcon(QPixmap(iconDir.filePath("down.png"))));
    
    
    //set alternatingrowcolors
    lstSubjectList->setAlternatingRowColors(TRUE);
    lstExamList->setAlternatingRowColors(TRUE);
    
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
    currentDataPart     = NULL;
    currentPropertyPart = NULL;
    currentCathegory    = NULL;
    currentSubject      = NULL;
    currentExam         = NULL;
    
    
    ksPlattformSpec::addMissingDatabaseAttributes(&currentDatabase);
    currentPropertyPart = currentDatabase.cGetObjectByName("properties");
    ksPlattformSpec::addMissingPropertiesAttributes(currentPropertyPart);
    
    // reset Database change state:
    setDatabaseChanged(FALSE);
    //refresh GUI
    refreshCathegoryList();
}

void kollegstufeParent::saveFile(QString newFilename)
{
    if( newFilename == "")
    {
        newFilename = szFilename;
    }
    
    
    //create kollegstufe dir in home
    if(!ksPlattformSpec::createKsDir())
    {
        QMessageBox::critical(this, tr("Error when creating Folder - Kollegstufe"),
                              "Error during creating folder <i>" + ksPlattformSpec::getKsDir() + "</i>\n"
                                      + tr("It seems, you haven't got the necessary Write-Rights!"));
        return;
    }
    //create config File home/.kollegstufe
    if(0 != WriteClassToFile( ksPlattformSpec::qstringToSz(newFilename), &currentDatabase))
    {
        QMessageBox::critical(this, tr("Error during saving file- Kollegstufe"),
                              "Error during saving file <i>" + newFilename + "</i>\n"
                                      + tr("It seems, you haven't got the necessary Write-Rights!"));
        return;
    }
    else
    {
        //on success:
        setDatabaseChanged(FALSE);
    }
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
    // add missing Attributes in config File
    ksPlattformSpec::addMissingConfigAttributes(&xmlConfig);
    
    
    // write config Attributes to widget-attributes:
    // 1. window size
    int nWidth  = xmlConfig.cGetObjectByName("window-settings")->cGetAttributeByName("width")->nValueToInt();
    int nHeight = xmlConfig.cGetObjectByName("window-settings")->cGetAttributeByName("height")->nValueToInt();
    resize(nWidth, nHeight);
    // 2. splitter position
    int nSplitterCoord = xmlConfig.cGetObjectByName("window-settings")->cGetObjectByName("splitterMain")->cGetAttributeByName("coord")->nValueToInt();
    QList<int> list = splitterParent->sizes();
    QList<int>::Iterator it = list.begin();
    *it = nSplitterCoord;
    ++it;
    *it = this->width() - nSplitterCoord;
    splitterParent->setSizes(list);
    
    // 3. update Language
    reloadTranslator();
    
    // 4. file
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

void kollegstufeParent::saveConfigFile()
{
    debugOutput->putDebugOutput("Saving Config File");
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
    // 3. file
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
    //subject selection
        btnSubjectEdit->setText(tr("Edit Subject"));
        btnSubjectAdd->setToolTip(tr("Add Subject"));
        btnSubjectDelete->setToolTip(tr("Delete Subject"));
        btnSubjectMoveUp->setToolTip(tr("Move Subject Up"));
        btnSubjectMoveDown->setToolTip(tr("Move Subject Down"));
    // exam selection
        grpExamList->setTitle(tr("Subject Properties"));
        btnExamAdd->setText(tr("New Exam"));
        btnExamDelete->setText(tr("Delete"));
        btnExamEdit->setText(tr("Edit"));
    
    // menubar - menus
        mnmFile->setTitle(tr("&File"));
        mnmExtras->setTitle(tr("&Extras"));
    //file menu
        mnaLoadDatabase->setText(tr("Load archiv"));
        mnaLoadDatabase->setShortcut(tr("Ctrl+O"));
        mnaSave->setShortcut(tr("Ctrl+S"));
        mnaQuit->setShortcut(tr("Ctrl+Q"));
        mnaSave->setText(tr("Save"));
        mnaDatabaseProperties->setText(tr("Eigenschaften"));
        mnaQuit->setText(tr("Quit"));
    // extras menu
        mnaStatistics->setText(tr("Statistics"));
        mnaConfigureKs->setText(tr("Configure Kollegstufe"));
        mnaAboutQt->setText(tr("About Qt"));
        mnaAboutKs->setText(tr("About Kollegstufe"));
    
    // exam list table
    QTreeWidgetItem* examListHeader = lstExamList->headerItem();
    examListHeader->setText(1, tr("Semester"));
    examListHeader->setText(2, tr("Date"));
    examListHeader->setText(3, tr("Nr"));
    examListHeader->setText(4, tr("Type"));
    examListHeader->setText(5, tr("Points"));
    
    selectedCathegoryChanged();
    
}

void kollegstufeParent::reloadTranslator()
{
    if(xmlConfig.cGetObjectByName("language")->szGetContent())
    {
        ksPlattformSpec::setLanguage(xmlConfig.cGetObjectByName("language")->szGetContent(), &appTranslator);
    }
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
        diaStatistics->close();
        if (diaAbout)
        {
            diaAbout->close();
        }
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
        questionBox.addButton(tr("Save"), QMessageBox::ApplyRole);
        questionBox.addButton(tr("Discard"), QMessageBox::DestructiveRole);
        questionBox.addButton(tr("Don't Close"), QMessageBox::RejectRole);
        questionBox.setIcon(QMessageBox::Question);
        questionBox.setText(tr("Seit dem letzten Speichern haben sich Daten verandert!\nMoechten Sie diese vor dem Schliessen speichern oder verwerfen ?"));
        questionBox.setWindowTitle(tr("Closing Database - Kollegstufe"));
        
        switch (questionBox.exec()) {
            case 0:
                saveFile();
                return TRUE;
                break;
            case 1:
                return TRUE;
                break;
            case 2:
                return FALSE;
                break;
            default:
                return TRUE;
                break;
        }
    }
    else
    {
        return TRUE;
    }
}

void kollegstufeParent::showDatabaseProperties()
{
    if(!diaDatabaseProperties)
    {
        diaDatabaseProperties = new ksDatabaseProperties(this);
    }
    diaDatabaseProperties->setDatabasePropertiesToEdit(currentDatabase.cGetObjectByName("properties"));
    diaDatabaseProperties->setDatabaseToEdit(currentDataPart);
    diaDatabaseProperties->exec();
    if(diaDatabaseProperties->result() == QDialog::Accepted)
    {
        refreshCathegoryList();
        if(diaStatistics)
        {
            diaStatistics->setProperties(currentPropertyPart);
        }
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
    if( currentCathegory == NULL)
    {
        return;
    }
    
    QString    szNewSubjectName;
    QString    szBasicName;
    
    int        nNewSubjectId;
    xmlObject* newSubject;
    if ((nNewSubjectId = currentCathegory->nAddObject("subject")) < 0)
        return;
    
    newSubject = currentCathegory->cGetObjectByIdentifier(nNewSubjectId);
    ksPlattformSpec::addMissingSubjectAttributes(newSubject);
    
     // set strings to current subject name
    szNewSubjectName = newSubject->cGetAttributeByName("name")->value();
    szBasicName = szNewSubjectName;
    
    //Find first "free" Subject Name
    int SubjectNumber = 1;
    xmlObject*  foundedSubject;
    while((foundedSubject=currentCathegory->cGetObjectByAttributeValue("name", ksPlattformSpec::qstringToSz(szNewSubjectName))) && foundedSubject != newSubject)
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
    diaSubjectProperties->setCathegoryOfSubject(currentCathegory);
    diaSubjectProperties->exec();
    
    switch(diaSubjectProperties->result())
    {
        case QDialog::Accepted:
            setDatabaseChanged();
            refreshSubjectList();
            szNewSubjectName = newSubject->cGetAttributeByName("name")->value();
            if(lstSubjectList->findItems(szNewSubjectName, Qt::MatchExactly).size() > 0)
            {
                lstSubjectList->setCurrentItem(lstSubjectList->findItems(szNewSubjectName, Qt::MatchExactly).first());
            }
            break;
        case QDialog::Rejected:
            currentCathegory->nDeleteObject(newSubject);
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
    
    QString message = tr("Do you really want to delete the subject\'");
    message += lstSubjectList->currentItem()->text();
    message += tr("\' ?");
    if(QMessageBox::question ( this, tr("Deleting a subject"), message , QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        currentCathegory->nDeleteObject(currentCathegory->cGetObjectByAttributeValue("name", ksPlattformSpec::qstringToSz(lstSubjectList->currentItem()->text())));

        
        refreshSubjectList();
        setDatabaseChanged();
    }
}

void kollegstufeParent::subjectEdit()
{
    if(!diaSubjectProperties)
    {
        diaSubjectProperties = new ksSubjectProperties(this);
    }
    if(!currentSubject)
    {
        return;
    }
    
    diaSubjectProperties->setSubjectToEdit(currentSubject);
    diaSubjectProperties->setCathegoryOfSubject(currentCathegory);
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
    if(!currentSubject || !currentCathegory)
    {
        return;
    }
    if(lstSubjectList->count() <= 1)
    {
        return;
    }
    currentCathegory->moveObjectTo(currentSubject, lstSubjectList->currentRow()-1);
    refreshSubjectList(lstSubjectList->currentRow()-1);
    setDatabaseChanged();
}

void kollegstufeParent::subjectMoveDown()
{
    if(!currentSubject || !currentCathegory)
    {
        return;
    }
    if(lstSubjectList->count() <= 1)
    {
        return;
    }
    
    currentCathegory->moveObjectTo(currentSubject, lstSubjectList->currentRow()+1);
    
    refreshSubjectList(lstSubjectList->currentRow()+1);
    setDatabaseChanged();
}

void kollegstufeParent::examAdd()
{
    if(currentSubject == NULL)
    {
        // return if there is no subject selected
        return;
    }
    int nNewExamIdentifier = currentSubject->nAddObject("exam");
    xmlObject* newExam = currentSubject->cGetObjectByIdentifier(nNewExamIdentifier);
    if (newExam == NULL)
    {
        return; 
    }
    int i = 0;
    QString newId;
    newId.setNum(i);
    while (currentSubject->cGetObjectByAttributeValue("id", ksPlattformSpec::qstringToSz(newId)) != NULL)
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
    diaExamProperties->setProperties(currentPropertyPart);
    diaExamProperties->setExamToEdit(newExam);
    diaExamProperties->exec();
    switch(diaExamProperties->result())
    {
        case QDialog::Accepted:
            setDatabaseChanged();
            break;
        case QDialog::Rejected:
            currentSubject->nDeleteObject(newExam);
            refreshExamList();
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
    
    return;
}

void kollegstufeParent::examDelete()
{
    
    if(currentSubject == NULL)
    {
        // return if there is no subject selected
        return;
    }
    if(currentExam == NULL)
    {
        // return if there is no exam selected
        return;
    }
    
    QString message = tr("Do you really want to delete ");
    message += ksPlattformSpec::getArticleForNoun(currentExam->cGetObjectByAttributeValue("name", "type")->cGetAttributeByName("value")->value(), ksPlattformSpec::kasusAkkusativ);
    if(currentExam->cGetObjectByAttributeValue("name", "number")->cGetAttributeByName("value")->nValueToInt() > 0)
    {
        message += currentExam->cGetObjectByAttributeValue("name", "number")->cGetAttributeByName("value")->value();
        message += ". ";
    }
    message += currentExam->cGetObjectByAttributeValue("name", "type")->cGetAttributeByName("value")->value();
    message += tr(" from ");
    cDateConverter date;
    date.setDateString(currentExam->cGetObjectByAttributeValue("name", "date")->cGetAttributeByName("value")->value());
    message += date.humanDate();
    message += tr(" ? Really Delete this exam ?");
    if(QMessageBox::question ( this, tr("Deleting of an Exam"), message , QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        currentSubject->nDeleteObject(currentExam);
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
    if(!currentExam)
    {
        return;
    }
    diaExamProperties->setProperties(currentPropertyPart);
    diaExamProperties->setExamToEdit(currentExam);
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
}

void kollegstufeParent::selectedCathegoryChanged()
{
    if (currentDataPart)
    {
        currentCathegory = currentDataPart->cGetObjectByAttributeValue("name", (ksPlattformSpec::qstringToSz(cmbCathegory->currentText())));
    }
    else
    {
        currentCathegory = NULL;
    }
    refreshSubjectList();
    //to refresh everything
    selectedSubjectChanged();
}

void kollegstufeParent::selectedSubjectChanged()
{
    if (lstSubjectList->currentItem() && currentCathegory)
    {
        grpExamList->setEnabled(TRUE);
        btnSubjectDelete->setEnabled(TRUE);
        currentSubject = currentCathegory->cGetObjectByAttributeValue("name", ksPlattformSpec::qstringToSz(lstSubjectList->currentItem()->text()));
        grpExamList->setTitle(tr("Properties of subject ") + lstSubjectList->currentItem()->text());
        
    }
    else
    {
        grpExamList->setEnabled(FALSE);
        btnSubjectDelete->setEnabled(FALSE);
        grpExamList->setTitle( tr("No Subject Selected"));
        currentSubject = NULL;
    }
    if(diaStatistics)
    {
        diaStatistics->setSubject(currentSubject);
        diaStatistics->setProperties(currentPropertyPart);
    }
    refreshExamList();
}

void kollegstufeParent::selectedExamChanged()
{
    
    //backup last Selection
    QString selectedID = "foo";
    QList<QTreeWidgetItem *> itemList = lstExamList->selectedItems ();
    if( itemList.size() > 0)
    {
        selectedID = itemList.first()->text(0);
    }
    else
    {
        currentExam = NULL;
        return;
    }
    if (currentSubject == NULL)
    {
        currentExam = NULL;
    }
    else
    {
        currentExam = currentSubject->cGetObjectByAttributeValue("id", ksPlattformSpec::qstringToSz(selectedID));
    }
    
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
    if (!currentDataPart)
    {
        if(currentDatabase.cGetObjectByName("data"))
        {
            currentDataPart = currentDatabase.cGetObjectByName("data");
        }
        else
        {
            currentDataPart = currentDatabase.cGetObjectByIdentifier(currentDatabase.nAddObject("data"));
        }
    }
    for(int i = 0; i < currentDataPart->nGetObjectCounter(); i++)
    {
        if(!currentDataPart->cGetObjectByIdentifier(i))
        {
            continue;
        }
        xmlObject* currentObjectToAdd = currentDataPart->cGetObjectByIdentifier(i);
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
    currentCathegory = currentDataPart->cGetObjectByAttributeValue("name", ksPlattformSpec::qstringToSz(cmbCathegory->currentText()));
    
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
    if( !currentCathegory)
    {
        currentSubject = NULL;
        return;
    }
    for (int i = 0; i < currentCathegory->nGetObjectCounter(); i++)
    {
        if(!currentCathegory->cGetObjectByIdentifier(i))
        {
            continue;
        }
        xmlObject* currentObjectToAdd = currentCathegory->cGetObjectByIdentifier(i);
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
    QString BackupSelectedID = "foo";
    if( lstExamList->currentItem() != NULL)
    {
        BackupSelectedID = lstExamList->currentItem()->text(0);
    }
    
    // refresh list
    lstExamList->clear();
    if(!currentPropertyPart || !currentSubject)
    {
        return;
    }
    
    int best  = currentPropertyPart->cGetObjectByName("rating")->cGetAttributeByName("best")->nValueToInt();
    int worst = currentPropertyPart->cGetObjectByName("rating")->cGetAttributeByName("worst")->nValueToInt();
    
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
    
    for (int i = 0; i < currentSubject->nGetObjectCounter(); i++)
    {
        xmlObject*          currentXmlExam;
        if(!(currentXmlExam = currentSubject->cGetObjectByIdentifier(i)))
        {
            continue;
        }
        ksPlattformSpec::addMissingExamAttributes(currentXmlExam);
        
        currentSemester = ksPlattformSpec::szToUmlauts(currentXmlExam->cGetObjectByAttributeValue("name", "semester")->cGetAttributeByName("value")->value());
        currentDate     = ksPlattformSpec::szToUmlauts(currentXmlExam->cGetObjectByAttributeValue("name", "date")->cGetAttributeByName("value")->value());
        currentNumber   = ksPlattformSpec::szToUmlauts(currentXmlExam->cGetObjectByAttributeValue("name", "number")->cGetAttributeByName("value")->value());
        currentType     = ksPlattformSpec::szToUmlauts(currentXmlExam->cGetObjectByAttributeValue("name", "type")->cGetAttributeByName("value")->value());
        currentPoints   = ksPlattformSpec::szToUmlauts(currentXmlExam->cGetObjectByAttributeValue("name", "points")->cGetAttributeByName("value")->value());
        
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
        if(currentSemester == "12/1")
            currentSemester = "12 / 1";
        if(currentSemester == "12/2")
            currentSemester = "12 / 2";
        if(currentSemester == "13/1")
            currentSemester = "13 / 1";
        if(currentSemester == "13/2")
            currentSemester = "13 /2";
        
        examToAdd = new ExamItem(lstExamList);
        examToAdd->setText(0, currentXmlExam->cGetAttributeByName("id")->value());
        examToAdd->setText(1, currentSemester);
        examToAdd->setText(2, currentDate);
        examToAdd->setText(3, currentNumber);
        examToAdd->setText(4, currentType);
        examToAdd->setText(5, currentPoints);
        
    }
    
    //resize columns so that texts can be seen:
    for(int i = 0; i < lstExamList->columnCount(); i++)
    {
        lstExamList->resizeColumnToContents(i);
    }
    
    
    // restore Selection from before refresh
    QList <QTreeWidgetItem*> itemList = lstExamList->findItems(BackupSelectedID ,0);
    if (itemList.size() > 0)
        lstExamList->setCurrentItem(itemList.first());
    else
        lstExamList->setCurrentItem(lstExamList->itemAt(0,0));
    
    diaStatistics->refreshUiAndChildren();
}




