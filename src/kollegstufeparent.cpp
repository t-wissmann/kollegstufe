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
#include "kollegstufeparent.h"
#include "ksdebugoutput.h"
#include "dateConverter.h"
#include "ksplattformspec.h"
#include "ksdatabaseselection.h"
#include "ksdatabaseproperties.h"
#include "examitem.h"

// dialogs:
#include "kssubjectproperties.h"
#include "ksexamproperties.h"
#include "ksabout.h"

// normal widgets
#include <QComboBox>
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
#include <QModelIndex>

kollegstufeParent::kollegstufeParent(QWidget* parent)
 : QWidget(parent)
{
    initMembers();
    allocateWidgets();
    allocateDialogs();
    createMenuBar();
    connectSlots();
    createLayouts();
    initWidgets();
    
    loadConfigFile();
}


kollegstufeParent::~kollegstufeParent()
{
    if (diaExamProperties)
        delete diaExamProperties;
    
    delete debugOutput;
}

void kollegstufeParent::initMembers()
{
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
    debugOutput->enableDebugMode();
    connect(debugOutput, SIGNAL(printDebugLine(QString)), debugOutput, SLOT(putStandartQtDebugOutut(QString)));
}

void kollegstufeParent::allocateWidgets()
{
    //cathegory & subject selection
    cmbCathegory    = new QComboBox;
    lstSubjectList  = new QListWidget;
    wdgSubjectSelection = new QWidget;
    // subject tools:
    btnSubjectAdd       = new QPushButton(QString::fromLocal8Bit("Hinzufügen"));
    btnSubjectDelete    = new QPushButton("Entfernen");
    btnSubjectEdit      = new QPushButton("Fach Beartbeiten");
    
    
    //exam selection
    grpExamList     = new QGroupBox("Subject Properties");
    lstExamList     = new QTreeWidget;
    btnExamAdd      = new QPushButton(QString::fromLocal8Bit("Neue Leistung"));
    btnExamDelete   = new QPushButton("Entfernen");
    btnExamEdit     = new QPushButton("Bearbeiten");
    
}

void kollegstufeParent::allocateDialogs()
{
    diaExamProperties    = NULL;
    diaSubjectProperties = NULL;
    diaDatabaseSelection = NULL;
    diaDatabaseProperties= NULL;
    diaAbout             = NULL;
}

void kollegstufeParent::createMenuBar()
{
    // allocate menus
    mnbMenuBar  = new QMenuBar;
    mnbMenuBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    //add menus to menubar
    mnmFile     = mnbMenuBar->addMenu(tr("&Datei"));
    mnmExtras   = mnbMenuBar->addMenu(tr("&Extras"));
    
    //add actions to menus
    // 1. File - menu
    mnaLoadDatabase      = mnmFile->addAction(tr("Archiv Laden"));
    mnaDatabaseProperties= mnmFile->addAction(tr("Eigenschaften"));
    mnaSave              = mnmFile->addAction(tr("Speichern"));
    mnaQuit              = mnmFile->addAction(tr("Beenden"));
    mnaLoadDatabase->setShortcut(tr("Ctrl+O"));
    mnaSave->setShortcut(tr("Ctrl+S"));
    mnaQuit->setShortcut(tr("Ctrl+Q"));
    
    // 2. Extras - menu
    mnaAboutQt  = mnmExtras->addAction(QString::fromLocal8Bit("Über Qt"));
    mnaAboutKs  = mnmExtras->addAction(QString::fromLocal8Bit("Über Kollegstufe"));
    // icons will be set at initWidgets
    
}

void kollegstufeParent::createLayouts()
{
    // Subject selection layout
    layoutSubjectSelection = new QGridLayout;
    
    layoutSubjectSelection->addWidget(cmbCathegory, 0, 0, 1, 2);
    layoutSubjectSelection->addWidget(lstSubjectList, 1, 0, 1, 2);
    layoutSubjectSelection->addWidget(lstSubjectList, 1, 0, 1, 2);
    layoutSubjectSelection->addWidget(btnSubjectAdd, 3, 0, 1, 1);
    layoutSubjectSelection->addWidget(btnSubjectDelete, 3, 1, 1, 1);
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
    
    layoutParent = new QVBoxLayout;
    layoutParent->setMargin(0);
    layoutParent->addWidget(mnbMenuBar);
    layoutParent->addWidget(splitterParent);
    
    setLayout(layoutParent);
}


void kollegstufeParent::connectSlots()
{
    connect(cmbCathegory, SIGNAL(currentIndexChanged(int)), this, SLOT(selectedCathegoryChanged()));
    connect(lstSubjectList, SIGNAL(currentRowChanged(int)), this, SLOT(selectedSubjectChanged()));
    connect(lstExamList, SIGNAL(itemSelectionChanged()), this, SLOT(selectedExamChanged()));
    
    //subject control
    connect(btnSubjectAdd, SIGNAL(clicked()), this, SLOT(subjectAdd()));
    connect(btnSubjectDelete, SIGNAL(clicked()), this, SLOT(subjectDelete()));
    connect(btnSubjectEdit, SIGNAL(clicked()), this, SLOT(subjectEdit()));
    
    //menubar-menu-actions
    // mnmFile
    connect(mnaQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(mnaSave, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(mnaLoadDatabase, SIGNAL(triggered()), this, SLOT(showDatabaseSelection()));
    connect(mnaDatabaseProperties, SIGNAL(triggered()), this, SLOT(showDatabaseProperties()));
    // mnmExtras
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
    examListHeader->setText(2, "Datum");
    examListHeader->setText(3, "Nr");
    examListHeader->setText(4, "Typ");
    examListHeader->setText(5, "Punkte");
    lstExamList->setHeaderItem(examListHeader);
    lstExamList->hideColumn(0);
    lstExamList->header()->setClickable(TRUE);
    lstExamList->header()->setMovable(TRUE);
    lstExamList->setSortingEnabled(TRUE);
    
    // subject selection widgets:
    layoutSubjectSelection->setMargin(1);
    
    // init main window / this
    splitterParent->setChildrenCollapsible(FALSE);
    
    QDir iconDir(szApplicationPath);
    iconDir.cdUp();
    iconDir.cd("pic");
    setWindowIcon(QPixmap(iconDir.filePath("kollegstufe.png")));
    setWindowTitle("Kollegstufe");
    
    // set Icons for MenuBar:
    mnaAboutKs->setIcon(windowIcon());
    
    
    //set alternatingrowcolors
    lstSubjectList->setAlternatingRowColors(TRUE);
    lstExamList->setAlternatingRowColors(TRUE);
    

}

void kollegstufeParent::loadFile(QString newFilename, bool showErrorMsg)
{
    // clear current database
    currentDatabase.nSetAttributeCounter(0);
    currentDatabase.nSetObjectCounter(0);
    
    int nResult = ReadFileToClass(newFilename.toAscii().data(), &currentDatabase);
    szFilename = newFilename;
    if( nResult != 0)
    {
        splitterParent->setEnabled(FALSE);
        szFilename = "";
        if ( showErrorMsg )
        {
            QMessageBox::critical(this, tr("Fehler beim Datei-Laden - Kollegstufe"),
                                "Fehler beim Laden der Date <i>" + newFilename + "</i>\n"
                                + QString::fromLocal8Bit("Anscheinend haben sie nicht die nötigen Leserechte dazu oder diese existiert nicht!"));
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
        QMessageBox::critical(this, tr("Fehler beim Ordner-Erstellen - Kollegstufe"),
                              "Fehler beim Erstellen des Ordners <i>" + ksPlattformSpec::getKsDir() + "</i>\n"
                                      + QString::fromLocal8Bit("Anscheinend haben sie nicht die nötigen Schreibrechte dazu!"));
        return;
    }
    //create config File home/.kollegstufe
    if(0 != WriteClassToFile(newFilename.toAscii().data(), &currentDatabase))
    {
        QMessageBox::critical(this, tr("Fehler beim Datei-Speichern - Kollegstufe"),
                              "Fehler beim Speichern der Date <i>" + newFilename + "</i>\n"
                                      + QString::fromLocal8Bit("Anscheinend haben sie nicht die nötigen Schreibrechte dazu!"));
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
                                      + QString::fromLocal8Bit("Anscheinend haben sie nicht die nötigen Schreibrechte dazu!"));
        return;
    }
    //create config File home/.kollegstufe
    if(!ksPlattformSpec::createConfigFile())
    {
        QMessageBox::critical(this, tr("Fehler beim Datei-Laden/Erstellen - Kollegstufe"),
                              "Fehler beim Erstellen oder Laden der Date <i>" + ksPlattformSpec::getKsDir() + "config.xml</i>\n"
                                      + QString::fromLocal8Bit("Anscheinend haben sie nicht die nötigen Schreib- und/oder Leserechte dazu!"));
        return;
    }
    
    //clear old config;
    xmlConfig.nSetObjectCounter(0);
    xmlConfig.nSetAttributeCounter(0);
    if(0 != ReadFileToClass(QString(ksPlattformSpec::getKsDir() + "config.xml").toAscii().data(), &xmlConfig))
    {
        QMessageBox::critical(this, tr("Fehler beim Datei-Laden - Kollegstufe"),
                              "Fehler beim Laden der Date <i>" + ksPlattformSpec::getKsDir() + "config.xml</i>\n"
                                      + QString::fromLocal8Bit("Anscheinend haben sie nicht die nötigen Leserechte dazu!"));
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
    // 3. file
    
    loadFile(xmlConfig.cGetObjectByName("session")->cGetObjectByName("file")->szGetContent());
    
}

void kollegstufeParent::saveConfigFile()
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
    // 3. file
    xmlConfig.cGetObjectByName("session")->cGetObjectByName("file")->nSetContent(szFilename.toAscii().data());
    // create .kollegstufe direcotry if it doesnt exist
    if(!ksPlattformSpec::createKsDir())
    {
        QMessageBox::critical(this, tr("Fehler beim Ordner-Erstellen - Kollegstufe"),
                              "Fehler beim Erstellen des Ordners <i>" + ksPlattformSpec::getKsDir() + "</i>\n"
                                      + QString::fromLocal8Bit("Anscheinend haben sie nicht die nötigen Schreibrechte dazu!"));
        return;
    }
    // write config  to file
    if(0 != WriteClassToFile(QString(ksPlattformSpec::getKsDir() + "config.xml").toAscii().data(), &xmlConfig))
    {
        QMessageBox::critical(this, tr("Fehler beim Datei-Schreiben - Kollegstufe"),
                              "Fehler beim Schreiben der Date <i>" + ksPlattformSpec::getKsDir() + "config.xml</i>\n"
                                      + QString::fromLocal8Bit("Anscheinend haben sie nicht die nötigen Schreibrechte dazu!"));
        return;
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
    }
    else
    {
        event->ignore();
    }
}

bool kollegstufeParent::askForSavingChangedDatabase()
{
    
    // only ask to quit if data has been changed
    
    if (databaseChanged())
    {
        QMessageBox questionBox;
        questionBox.addButton(tr("Speichern"), QMessageBox::ApplyRole);
        questionBox.addButton(tr("Verwerfen"), QMessageBox::DestructiveRole);
        questionBox.addButton(tr("Nicht Beenden"), QMessageBox::RejectRole);
        questionBox.setIcon(QMessageBox::Question);
        questionBox.setText(QString::fromLocal8Bit("Seit dem letzten Speichern haben sich Daten verändert!\nMöchten Sie diese vor dem Beenden speichern oder verwerfen ?"));
        questionBox.setWindowTitle(QString::fromLocal8Bit("Beenden bestätigen - Kollegstufe"));
        
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
    diaDatabaseProperties->exec();
    if(diaDatabaseProperties->result() == QDialog::Accepted)
    {
        setDatabaseChanged();
    }
}

void kollegstufeParent::showAboutDialog()
{
    if (!diaAbout)
        diaAbout = new ksAbout(this);
    diaAbout->exec();
}


void kollegstufeParent::subjectAdd()
{
    
    QString    szNewSubject = "Neues Fach";
    int        nNewSubjectId;
    xmlObject* newSubject;
    
    //Find first "free" Subject Name
    int SubjectNumber = 1;
    while(currentCathegory->cGetObjectByAttributeValue("name", szNewSubject.toAscii().data()))
    {
        SubjectNumber++;
        szNewSubject = "Neues Fach " + QString::number(SubjectNumber);
    }
    
    if ((nNewSubjectId = currentCathegory->nAddObject("subject")) < 0)
        return;
    newSubject = currentCathegory->cGetObjectByIdentifier(nNewSubjectId);
    newSubject->nAddAttribute("name", szNewSubject.toAscii().data());
    ksPlattformSpec::addMissingSubjectAttributes(newSubject);
    
    
    //run subject dialog
    if(!diaSubjectProperties)
    {
        diaSubjectProperties = new ksSubjectProperties;
    }
    
    diaSubjectProperties->setSubjectToEdit(newSubject);
    diaSubjectProperties->setCathegoryOfSubject(currentCathegory);
    diaSubjectProperties->exec();
    
    switch(diaSubjectProperties->result())
    {
        case QDialog::Accepted:
            setDatabaseChanged();
            refreshSubjectList();
            if(lstSubjectList->findItems(szNewSubject, Qt::MatchExactly).size() > 0)
            {
                lstSubjectList->setCurrentItem(lstSubjectList->findItems(szNewSubject, Qt::MatchExactly).first());
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
    
    QString message = QString::fromLocal8Bit("Möchten Sie das Fach \'");
    message += lstSubjectList->currentItem()->text();
    message += QString::fromLocal8Bit("\' wirklich löschen?");
    if(QMessageBox::question ( this, QString::fromLocal8Bit("Löschen eines Faches"), message , QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        currentCathegory->nDeleteObject(currentCathegory->nGetObjectIdentifierByAttributeValue("name", lstSubjectList->currentItem()->text().toAscii().data()));

        
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
    while (currentSubject->cGetObjectByAttributeValue("id", newId.toAscii().data()) != NULL)
    {
        i++;
        newId.setNum(i);
    }
    newExam->nAddAttribute("id",newId.toAscii().data());
    ksPlattformSpec::addMissingExamAttributes(newExam);
    
    // run exam property dialog
    if(!diaExamProperties)
    {
        diaExamProperties = new ksExamProperties(this);
    }
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
    
    QString message = QString::fromLocal8Bit("Möchten Sie ");
    message += ksPlattformSpec::getArticleForNoun(currentExam->cGetObjectByAttributeValue("name", "type")->cGetAttributeByName("value")->value(), ksPlattformSpec::kasusAkkusativ);
    if(currentExam->cGetObjectByAttributeValue("name", "number")->cGetAttributeByName("value")->nValueToInt() > 0)
    {
        message += currentExam->cGetObjectByAttributeValue("name", "number")->cGetAttributeByName("value")->value();
        message += ". ";
    }
    message += currentExam->cGetObjectByAttributeValue("name", "type")->cGetAttributeByName("value")->value();
    message += " vom ";
    cDateConverter date;
    date.setDateString(currentExam->cGetObjectByAttributeValue("name", "date")->cGetAttributeByName("value")->value());
    message += date.humanDate();
    message += QString::fromLocal8Bit(" wirklich löschen?");
    if(QMessageBox::question ( this, QString::fromLocal8Bit("Löschen einer Leistung"), message , QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
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
        currentCathegory = currentDataPart->cGetObjectByAttributeValue("name", (cmbCathegory->currentText().toAscii().data()));
    }
    else
    {
        currentCathegory = NULL;
    }
    refreshSubjectList();
}

void kollegstufeParent::selectedSubjectChanged()
{
    if (lstSubjectList->currentItem())
    {
        grpExamList->setEnabled(TRUE);
        btnSubjectDelete->setEnabled(TRUE);
        currentSubject = currentCathegory->cGetObjectByAttributeValue("name", lstSubjectList->currentItem()->text().toAscii().data());
        grpExamList->setTitle(lstSubjectList->currentItem()->text());
    }
    else
    {
        grpExamList->setEnabled(FALSE);
        btnSubjectDelete->setEnabled(FALSE);
        grpExamList->setTitle( QString::fromLocal8Bit("Kein Fach ausgewählt"));
        currentSubject = NULL;
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
        currentExam = currentSubject->cGetObjectByAttributeValue("id", selectedID.toAscii().data());
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
            cmbCathegory->addItem(currentObjectToAdd->cGetAttributeByName("name")->value());
        }
    }
    
    // restore Selection from before refresh
    cmbCathegory->setCurrentIndex(nBackupSelectedIndex);
    currentCathegory = currentDataPart->cGetObjectByAttributeValue("name", cmbCathegory->currentText().toAscii().data());
}

void kollegstufeParent::refreshSubjectList()
{
    //backup last Selection
    int nBackupSelectedIndex = lstSubjectList->currentRow();
    if(nBackupSelectedIndex < 0)
    {
        nBackupSelectedIndex = 0;
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
            lstSubjectList->addItem(currentObjectToAdd->cGetAttributeByName("name")->value());
        }
    }
    
    // restore Selection from before refresh
    lstSubjectList->setCurrentRow(nBackupSelectedIndex);
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
    if(currentSubject == NULL)
    {
        return;
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
        
        currentSemester = QString::fromLocal8Bit(currentXmlExam->cGetObjectByAttributeValue("name", "semester")->cGetAttributeByName("value")->value());
        currentDate     = QString::fromLocal8Bit(currentXmlExam->cGetObjectByAttributeValue("name", "date")->cGetAttributeByName("value")->value());
        currentNumber   = QString::fromLocal8Bit(currentXmlExam->cGetObjectByAttributeValue("name", "number")->cGetAttributeByName("value")->value());
        currentType     = QString::fromLocal8Bit(currentXmlExam->cGetObjectByAttributeValue("name", "type")->cGetAttributeByName("value")->value());
        currentPoints   = QString::fromLocal8Bit(currentXmlExam->cGetObjectByAttributeValue("name", "points")->cGetAttributeByName("value")->value());
        
        // edit number:
        if (currentNumber == "0")
            currentNumber = "";
        //if(currentSemester == "auto")
        //    currentSemester = cFrmStatistic::getSemesterContainigDate( pCurrentArchive->cGetObjectByName("properties")->cGetObjectByName("time"),  Date.getDateString());
        if(currentSemester == "12/1")
            currentSemester = "12 / 1";
        if(currentSemester == "12/2")
            currentSemester = "12 / 2";
        if(currentSemester == "13/1")
            currentSemester = "13 / 1";
        if(currentSemester == "13/2")
            currentSemester = "13 /2";
        currentDateClass.setDateString(currentDate.toAscii().data());
        currentDate = currentDateClass.humanDate();
        
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
    
}




