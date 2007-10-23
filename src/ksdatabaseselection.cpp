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
#include "ksdatabaseselection.h"
#include "ksplattformspec.h"
#include "ksdatabaseproperties.h"
#include "xmlparser.h"

#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QTextEdit>
#include <QMenu>
#include <QAction>

// layouts
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSplitter>

#include <QEvent>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QFrame>
#include <QPalette>
#include <QFileDialog>
#include <QListWidgetItem>

ksDatabaseSelection::ksDatabaseSelection(QWidget *parent)
 : QDialog(parent)
{
    allocateWidgets();
    createLayouts();
    connectSlots();
    
    btnOk->setDefault(TRUE);
    
    //if(parent)
    //{  // we don't need this anymore, qt does it for us
    //    setWindowIcon(parent->windowIcon());
    //}
    retranslateUi();
}


ksDatabaseSelection::~ksDatabaseSelection()
{
}


void ksDatabaseSelection::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if(event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
}


void    ksDatabaseSelection::retranslateUi()
{
    btnNew->setText(tr("Create a new one"));
    btnExtras->setText(tr("archiv"));
    btnOk->setText(tr("Load archiv"));
    btnCancel->setText(tr("Cancel"));
    mnaExtrasDelete->setText(tr("Delete"));
    mnaExtrasImport->setText(tr("Import"));
    mnaExtrasExport->setText(tr("Export"));
    
    setWindowTitle(tr("Select Database to load - Kollegstufe"));
}

void    ksDatabaseSelection::allocateWidgets()
{
    // widgtes on bottom
    btnNew     = new QPushButton;
    //btnDelete  = new QPushButton(tr("Loeschen"));
    btnExtras  = new QPushButton;
    btnOk     = new QPushButton;
    btnCancel = new QPushButton;
    
    //Extras menu;
    QMenu*  mnmMenu = new QMenu;
    btnExtras->setMenu(mnmMenu);
    mnaExtrasDelete = mnmMenu->addAction("");
    mnaExtrasImport = mnmMenu->addAction("");
    mnaExtrasExport = mnmMenu->addAction("");
    
    
    // main widgets
    lstFileList = new QListWidget;
    lstFileList->setAlternatingRowColors(TRUE);
    txtInfo     = new QTextEdit;
    txtInfo->setReadOnly(TRUE);
    QPalette textPalette = palette();
    textPalette.setBrush(QPalette::Base, palette().brush(QPalette::Window));
    textPalette.setBrush(QPalette::Text, palette().brush(QPalette::WindowText));
    txtInfo->setPalette(textPalette);
    
    txtInfo->setFrameStyle(0);
    
}

void    ksDatabaseSelection::createLayouts()
{
    layoutBottom = new QHBoxLayout;
    layoutBottom->setMargin(0);
    layoutBottom->addWidget(btnNew);
    layoutBottom->addWidget(btnExtras);
    layoutBottom->addStretch(1);
    layoutBottom->addWidget(btnOk);
    layoutBottom->addWidget(btnCancel);
    
    layoutListOrInfo = new QSplitter(Qt::Horizontal);
    layoutListOrInfo->addWidget(lstFileList);
    layoutListOrInfo->addWidget(txtInfo);
    
    layoutParent = new QGridLayout;
    layoutParent->addWidget(layoutListOrInfo, 0, 0);
    layoutParent->addLayout(layoutBottom, 1, 0);
    
    setLayout(layoutParent);
}

void    ksDatabaseSelection::connectSlots()
{
    connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(btnNew, SIGNAL(clicked()), this, SLOT(createNewDatabase()));
    connect(mnaExtrasDelete, SIGNAL(triggered()), this, SLOT(deleteDatabase()));
    connect(mnaExtrasImport, SIGNAL(triggered()), this, SLOT(importDatabase()));
    connect(mnaExtrasExport, SIGNAL(triggered()), this, SLOT(exportDatabase()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(lstFileList, SIGNAL(currentRowChanged(int)), this, SLOT(selectedFileChanged(int)));
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
    disconnect(lstFileList, SIGNAL(currentRowChanged(int)), this, SLOT(selectedFileChanged(int)));
    for (int i = 0; i < szFileList.size(); i++)
    {
        //lstFileList->addItem(getFileListLineForPath(szFileList.at(i)));
        new QListWidgetItem(getFileListLineForPath(szFileList.at(i)), lstFileList);
    }
    // restore connection
    connect(lstFileList, SIGNAL(currentRowChanged(int)), this, SLOT(selectedFileChanged(int)));
    
    
    // refresh button state
    if(szFileList.size() < 1)
    {
        selectedFileChanged(-1);
    }
    // select currentFile
    for (int i = 0; i < szFileList.size(); i++)
    {
        if (szFileList.at(i) == szCurrentFile)
        {
            lstFileList->setCurrentRow(i);
            selectedFileChanged(i);
        }
    }
}
void ksDatabaseSelection::createNewDatabase()
{
    // create new File
    xmlObject newFile;
    ksPlattformSpec::addMissingDatabaseAttributes(&newFile);
    ksPlattformSpec::addMissingPropertiesAttributes(newFile.cGetObjectByName("properties"));
    
    ksDatabaseProperties properties(this);
    properties.setDatabasePropertiesToEdit(newFile.cGetObjectByName("properties"));
    properties.setDatabaseToEdit(newFile.cGetObjectByName("data"));
    properties.exec();
    if(properties.result() == QDialog::Accepted)
    {
        // get not used Filename
        QString newFilename;
        newFilename = ksPlattformSpec::getNewFilename("archiv", ".xml");
        if( 0 != WriteClassToFile(newFilename.toAscii().data(), &newFile))
        {
            QMessageBox::critical(this, tr("Fehler beim Datei-Speichern - Kollegstufe"),
                                    tr("Fehler beim Speichern der Date <i>") + newFilename + "</i>\n"
                                            + tr("Anscheinend haben sie nicht die noetigen Schreibrechte dazu!"));
        }
    }
    refreshFileList();
}

void ksDatabaseSelection::deleteDatabase()
{
    if(getCurrentFile() == "")
    {
        return;
    }
    
    QString msgToAsk;
    msgToAsk =  tr("Moechten Sie Das Archiv \'");
    msgToAsk += getFileListLineForPath(getCurrentFile());
    msgToAsk += tr("\' wirklich loeschen?");
    QString title = tr("Loeschen eines Notenarchivs - Kollegstufe");
    
    if(QMessageBox::Yes == QMessageBox::question(this, title, msgToAsk, QMessageBox::Yes, QMessageBox::No))
    {
        if(!ksPlattformSpec::deleteFile(getCurrentFile()))
        {
            title = tr("Fehler - Kollegstufe");
            msgToAsk  = tr("Die Datei \'");
            msgToAsk += getCurrentFile();
            msgToAsk += tr("\' konnte nicht geloescht werden.\n");
            msgToAsk += tr("Warscheinlich besitzen sie nicht die noetigen Schreibrechte dazu");
            
            QMessageBox::critical(this, title, msgToAsk);
        }
    }
    refreshFileList();
}


void ksDatabaseSelection::exportDatabase()
{
    if (szCurrentFile == "")
    {
        return;
    }
    QFileDialog fdia(this);
    fdia.setFilter(tr("Xml - Dateien ( *.xml )"));
    fdia.setAcceptMode(QFileDialog::AcceptSave);
    fdia.setWindowTitle(tr("Notenarchiv exportieren nach ..."));
    fdia.setDirectory(ksPlattformSpec::getKsDir());
    fdia.setConfirmOverwrite(TRUE);
    fdia.selectFile(szCurrentFile);
    if(fdia.exec())
    {
        if(fdia.selectedFiles().isEmpty())
        {
            return;
        }
        QString newFilename = fdia.selectedFiles()[0];
        if(QFile::exists(newFilename))
        {
            if (!QFile::remove(newFilename))
            {
                QString msg = tr("Es geschah ein Fehler beim ueberschreiben von \'");
                msg += newFilename;
                msg += tr("\' mit \'");
                msg += szCurrentFile;
                msg += "\'\n";
                msg += tr("Warscheinlich besitzen Sie nicht die noetigen Schreibrechte dazu");
                QMessageBox::critical(this, tr("Fehler beim Datei ueberschreiben - Kollegstufe"), msg);
            }
        }
        if(!QFile::copy(szCurrentFile, newFilename))
        {
            QString msg = tr("Es geschah ein Fehler beim ueberschreiben von \'");
            msg += newFilename;
            msg += tr("\' mit \'");
            msg += szCurrentFile;
            msg += tr("\'\n");
            msg += tr("Warscheinlich besitzen Sie nicht die notigen Schreibrechte dazu");
            QMessageBox::critical(this, tr("Fehler beim Datei uberschreiben - Kollegstufe"), msg);
        }
    }
}

void ksDatabaseSelection::importDatabase()
{
    
    QFileDialog fdia(this);
    fdia.setFilter(tr("Xml - Dateien ( *.xml )"));
    fdia.setAcceptMode(QFileDialog::AcceptOpen);
    fdia.setWindowTitle(tr("Notenarchiv importieren von ..."));
    fdia.setDirectory(ksPlattformSpec::getKsDir());
    fdia.setConfirmOverwrite(TRUE);
    fdia.selectFile(szCurrentFile);
    if(fdia.exec())
    {
        if(fdia.selectedFiles().isEmpty())
        {
            return;
        }
        // import-file-path and import-file-filename
        QString fileToImport = fdia.selectedFiles()[0];
        QString filename = fileToImport.split(QDir::separator()).last();
        filename  = filename.split(".").first(); // just get name without file extension
        QString filenameInKsDir = filename;
        QDir   dir(ksPlattformSpec::getKsDir());
        if(dir.exists(filename + ".xml"))
        {
            //get free filename
            int currentIndex = 2;
            filenameInKsDir = filename + QString::number(currentIndex) + ".xml";
            while(dir.exists(filenameInKsDir))
            {
                currentIndex++;
                filenameInKsDir = filename + QString::number(currentIndex) + ".xml";
            }
        }
        // copy fileToImport to ksDir
        if (!QFile::copy(fileToImport, ksPlattformSpec::getKsDir() + filenameInKsDir))
        {
            QString msg = tr("Es geschah ein Fehler beim Erstellen der Datei \'");
            msg += filenameInKsDir;
            msg += tr("\' im Ordner \'");
            msg += ksPlattformSpec::getKsDir();
            msg += tr("\'\n");
            msg += tr("Warscheinlich besitzen Sie nicht die notigen Schreibrechte dazu");
            QMessageBox::critical(this, tr("Fehler beim Datei Erstellen - Kollegstufe"), msg);
        }
    }
    refreshFileList();
}


QString ksDatabaseSelection::getFileListLineForPath(QString path)
{
    QString onError;
    QString onSuccess;
    onError = path.split(QDir::separator()).last(); // on error, just return Filename
    
    xmlObject  file;
    if(0 != ReadFileToClass(path.toAscii().data(), &file))
    {
        return onError;
    }
    if(!file.cGetObjectByName("properties"))
    {
        return onError;
    }
    if(!file.cGetObjectByName("properties")->cGetObjectByName("author"))
    {
        return onError;
    }
    if(QString(file.cGetObjectByName("properties")->cGetObjectByName("author")->szGetContent()) == "")
    {
        return onError;
    }
    
    onSuccess += ksPlattformSpec::szToUmlauts(file.cGetObjectByName("properties")->cGetObjectByName("author")->szGetContent());
    
    
    return onSuccess;
}
QString ksDatabaseSelection::getHtmlInfoForPath(QString path)
{
    QString onError = tr("<i>konnte Archiv-Information nicht lesen, warscheinlich haben sie nicht die notigen Leserechte dazu.</i>");
    QString onSuccess;
    
    xmlObject  file;
    if(0 != ReadFileToClass(path.toAscii().data(), &file))
    {
        return onError;
    }
    // get name
    xmlObject* currentObject;
    if (currentObject = file.cGetObjectByName("properties"))
    {
        if (currentObject = currentObject->cGetObjectByName("author"))
        {
            if(currentObject->nGetContentLength() > 0)
            {
                onSuccess += tr("<u>Name des Schulers:</u> <b>");
                onSuccess += ksPlattformSpec::szToUmlauts(currentObject->szGetContent());
                onSuccess += tr("</b><br>");
            }
        }
    }
    if (currentObject = file.cGetObjectByName("data"))
    {
        // get all cathegories
        int i = 0;
        xmlObject* currentCathegory;
        while(currentObject->cGetObjectByIdentifier(i))
        {
            currentCathegory = currentObject->cGetObjectByIdentifier(i);
            if (currentCathegory->szGetName() == QString("cathegory"))
            {
                onSuccess += "<p><u>";
                onSuccess += ksPlattformSpec::szToUmlauts(currentCathegory->cGetAttributeByName("name")->value());
                onSuccess += ":</u> ";
                
                int subjectCount = 0;
                xmlObject* currentSubject;
                bool subjectAlreadyAppended = FALSE;
                
                while(currentCathegory->cGetObjectByIdentifier(subjectCount))
                {
                    currentSubject = currentCathegory->cGetObjectByIdentifier(subjectCount);
                    if(currentSubject->szGetName() == QString("subject"))
                    {
                        if(subjectAlreadyAppended)
                        {
                            onSuccess += ", ";
                        }
                        onSuccess += ksPlattformSpec::szToUmlauts(currentSubject->cGetAttributeByName("name")->value());
                        subjectAlreadyAppended = TRUE;
                    }
                    subjectCount++;
                }
                //QMessageBox::information(NULL, "hi", "hi");
                onSuccess += "</p>";
            }
            i++;
        }
    }
    
    return onSuccess;
}


void ksDatabaseSelection::catchStringList()
{
    ksPlattformSpec::catchFileList(&szFileList);
}

void ksDatabaseSelection::selectedFileChanged(int newCurrentFile)
{
    if ((newCurrentFile < szFileList.size()) && (newCurrentFile >= 0))
    {
        btnOk->setEnabled(TRUE);
        szCurrentFile = szFileList.at(lstFileList->currentRow());
        txtInfo->setText(getHtmlInfoForPath(szCurrentFile));
    }
    else
    {
        btnOk->setEnabled(FALSE);
    }
}


