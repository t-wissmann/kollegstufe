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
#include "ksdatabaseproperties.h"
#include "ksplattformspec.h"
#include "kscathegoryeditorwidget.h"

#include "xmlparser.h"
#include "dateConverter.h"

#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include <QRadioButton>
#include <QLineEdit>
#include <QGroupBox>
#include <QSpinBox>
#include <QListWidget>


// layouts
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QStackedWidget>

#include <QSizePolicy>

#include <QMessageBox>
#include <QEvent>
#include <QDate>
#include <QStringList>
#include <QList>


ksDatabaseProperties::ksDatabaseProperties(QWidget *parent)
 : QDialog(parent)
{
    allocateWidgets();
    createLayouts();
    connectSlots();
    resize(500, 300);
    
    databaseToEdit = NULL;
    propertiesToEdit = NULL;
    
    testDatabase = new xmlObject;
    testDatabase->cGetObjectByIdentifier(testDatabase->nAddObject("cathegory"))->nAddAttribute("name", "c eins");
    testDatabase->cGetObjectByIdentifier(testDatabase->nAddObject("cathegory"))->nAddAttribute("name", "c zwei");
    testDatabase->cGetObjectByIdentifier(testDatabase->nAddObject("cathegory"))->nAddAttribute("name", "c drei");
    
    
    btnOk->setDefault(TRUE);
    if(parent)
    {
        setWindowIcon(parent->windowIcon());
    }
    retranslateUi();
}


ksDatabaseProperties::~ksDatabaseProperties()
{
    delete testDatabase;
}

void ksDatabaseProperties::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if(event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
}


void ksDatabaseProperties::retranslateUi()
{
    
    btnOk->setText(tr("Ok"));
    btnCancel->setText(tr("Cancel"));
    lblName->setText(tr("Name of pupil:"));
    lblRating->setText(tr("Rating mode:"));
    optRatingMarks->setText(tr("Marks (1 - 6)"));
    optRatingPoints->setText(tr("Points (15 - 0 )"));
    optRatingOther->setText(tr("Special:"));
    lblRatingBest->setText(tr("Best:"));
    lblRatingWorst->setText(tr("Worst:"));
    grpSemesterList->setTitle(tr("Beginning and End of semesters"));
    lblStart->setText(tr("Beginning:"));
    lblEnd->setText(tr("End:"));
    grpCathegories->setTitle(tr("Edit Subject Cathegories"));
    
    setWindowTitle(tr("Edit properties of Mark-Database"));
}

void ksDatabaseProperties::allocateWidgets()
{
    btnOk     = new QPushButton;
    btnCancel = new QPushButton;
    lblName   = new QLabel;
    txtName   = new QLineEdit;
    
    lblRating       = new QLabel;
    optRatingMarks  = new QRadioButton;
    optRatingPoints = new QRadioButton;
    optRatingOther  = new QRadioButton;
    lblRatingBest   = new QLabel;
    lblRatingWorst  = new QLabel;
    spinRatingBest  = new QSpinBox;
    spinRatingWorst = new QSpinBox;
    
    
    grpSemesterList = new QGroupBox;
    lblStart = new QLabel;
    lblEnd   = new QLabel;
    lblStart->setAlignment(Qt::AlignHCenter);
    lblEnd->setAlignment(Qt::AlignHCenter);
    
    lbl121 = new QLabel("12 / 1:");
    lbl122 = new QLabel("12 / 2:");
    lbl131 = new QLabel("13 / 1:");
    lbl132 = new QLabel("13 / 2:");
    
    dteSemester121Start = new QDateEdit;
    dteSemester121End   = new QDateEdit;
    dteSemester122Start = new QDateEdit;
    dteSemester122End   = new QDateEdit;
    dteSemester131Start = new QDateEdit;
    dteSemester131End   = new QDateEdit;
    dteSemester132Start = new QDateEdit;
    dteSemester132End   = new QDateEdit;
    
    grpCathegories = new QGroupBox;
    wdgCathegoryEditor = new ksCathegoryEditorWidget;
    
    //lstStackMainControl = new QListWidget;
    //stackMain = new QStackedWidget;
}

void ksDatabaseProperties::createLayouts()
{
    layoutBottom = new QHBoxLayout;
    layoutBottom->addStretch(1);
    layoutBottom->addWidget(btnOk);
    layoutBottom->addWidget(btnCancel);
    
    layoutSemesterList = new QGridLayout;
    layoutSemesterList->addWidget(lblStart, 0, 1);
    layoutSemesterList->addWidget(lblEnd, 0, 2);
    lblStart->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    lblEnd->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    
    layoutSemesterList->addWidget(lbl121,              1, 0);
    layoutSemesterList->addWidget(dteSemester121Start, 1, 1);
    layoutSemesterList->addWidget(dteSemester121End,   1, 2);
    layoutSemesterList->addWidget(lbl122,              2, 0);
    layoutSemesterList->addWidget(dteSemester122Start, 2, 1);
    layoutSemesterList->addWidget(dteSemester122End,   2, 2);
    
    layoutSemesterList->addWidget(lbl131,              3, 0);
    layoutSemesterList->addWidget(dteSemester131Start, 3, 1);
    layoutSemesterList->addWidget(dteSemester131End,   3, 2);
    layoutSemesterList->addWidget(lbl132,              4, 0);
    layoutSemesterList->addWidget(dteSemester132Start, 4, 1);
    layoutSemesterList->addWidget(dteSemester132End,   4, 2);
    
    grpSemesterList->setLayout(layoutSemesterList);
    
    // rating
    layoutRatingOther = new QGridLayout;
    layoutRatingOther->addWidget(optRatingOther, 0, 0, 2, 1);
    layoutRatingOther->addWidget(lblRatingBest,  0, 1);
    layoutRatingOther->addWidget(spinRatingBest, 0, 2);
    layoutRatingOther->addWidget(lblRatingWorst,  1, 1);
    layoutRatingOther->addWidget(spinRatingWorst, 1, 2);
    
    layoutCathegories = new QHBoxLayout;
    layoutCathegories->addWidget(wdgCathegoryEditor);
    grpCathegories->setLayout(layoutCathegories);
    
    layoutParent = new QGridLayout;
    layoutParent->addWidget(lblName, 0, 0);
    layoutParent->addWidget(txtName, 0, 1, 1, 2);
    layoutParent->addWidget(lblRating, 1, 0);
    layoutParent->addWidget(optRatingMarks, 1, 1);
    layoutParent->addWidget(optRatingPoints, 1, 2);
    layoutParent->addLayout(layoutRatingOther, 2, 1, 1, 2);
    layoutParent->addWidget(grpSemesterList, 3, 0, 1, 3);
    layoutParent->addWidget(grpCathegories, 4, 0, 1, 3);
    layoutParent->addLayout(layoutBottom, 5, 0, 1, 3);
    
    setLayout(layoutParent);
}

void ksDatabaseProperties::connectSlots()
{
    connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(this, SIGNAL(accepted()), this, SLOT(writeWidgetAttributesToProperties()));
    
    //rating options:
    connect(optRatingPoints, SIGNAL(toggled(bool)), spinRatingBest,  SLOT(setDisabled(bool)));
    connect(optRatingPoints, SIGNAL(toggled(bool)), spinRatingWorst, SLOT(setDisabled(bool)));
    connect(optRatingPoints, SIGNAL(toggled(bool)), lblRatingBest,   SLOT(setDisabled(bool)));
    connect(optRatingPoints, SIGNAL(toggled(bool)), lblRatingWorst,  SLOT(setDisabled(bool)));
    connect(optRatingMarks,  SIGNAL(toggled(bool)), spinRatingBest,  SLOT(setDisabled(bool)));
    connect(optRatingMarks,  SIGNAL(toggled(bool)), spinRatingWorst, SLOT(setDisabled(bool)));
    connect(optRatingMarks,  SIGNAL(toggled(bool)), lblRatingBest,   SLOT(setDisabled(bool)));
    connect(optRatingMarks,  SIGNAL(toggled(bool)), lblRatingWorst,  SLOT(setDisabled(bool)));
    connect(optRatingOther,  SIGNAL(toggled(bool)), spinRatingBest,  SLOT(setEnabled(bool)));
    connect(optRatingOther,  SIGNAL(toggled(bool)), spinRatingWorst, SLOT(setEnabled(bool)));
    connect(optRatingOther,  SIGNAL(toggled(bool)), lblRatingBest,   SLOT(setEnabled(bool)));
    connect(optRatingOther,  SIGNAL(toggled(bool)), lblRatingWorst,  SLOT(setEnabled(bool)));
    
    connect(wdgCathegoryEditor, SIGNAL(renameModeChanged(bool)), this, SLOT(setBtnOkNotDefault(bool)));
    
    connectMinMaxDate();
}

void ksDatabaseProperties::connectMinMaxDate()
{
    QList<QDateEdit*>  DateBuf;
    DateBuf.append(dteSemester121Start);
    DateBuf.append(dteSemester121End);
    DateBuf.append(dteSemester122Start);
    DateBuf.append(dteSemester122End);
    DateBuf.append(dteSemester131Start);
    DateBuf.append(dteSemester131End);
    DateBuf.append(dteSemester132Start);
    DateBuf.append(dteSemester132End);
    for (int i = 0; i < DateBuf.size(); i++)
    {
        connect(DateBuf[i], SIGNAL(dateChanged(QDate)), this, SLOT(setMinMaxDate()));
    }
}

void ksDatabaseProperties::disconnectMinMaxDate()
{
    QList<QDateEdit*>  DateBuf;
    DateBuf.append(dteSemester121Start);
    DateBuf.append(dteSemester121End);
    DateBuf.append(dteSemester122Start);
    DateBuf.append(dteSemester122End);
    DateBuf.append(dteSemester131Start);
    DateBuf.append(dteSemester131End);
    DateBuf.append(dteSemester132Start);
    DateBuf.append(dteSemester132End);
    for (int i = 0; i < DateBuf.size(); i++)
    {
        disconnect(DateBuf[i], SIGNAL(dateChanged(QDate)), this, SLOT(setMinMaxDate()));
    }
}


void ksDatabaseProperties::setBtnOkNotDefault(bool newDefaultValue)
{
    btnOk->setDefault(!newDefaultValue);
}

void ksDatabaseProperties::setDatabasePropertiesToEdit(xmlObject* newProperties)
{
    propertiesToEdit = newProperties;
    if(propertiesToEdit == NULL)
    {
        return;
    }
    ksPlattformSpec::addMissingPropertiesAttributes(propertiesToEdit);
    
    // name
    txtName->setText(ksPlattformSpec::szToUmlauts(propertiesToEdit->cGetObjectByName( "author" )->szGetContent()));
    
    // rating
    int best  = propertiesToEdit->cGetObjectByName("rating")->cGetAttributeByName("best")->nValueToInt();
    int worst = propertiesToEdit->cGetObjectByName("rating")->cGetAttributeByName("worst")->nValueToInt();
    
    spinRatingBest ->setValue(best);
    spinRatingWorst->setValue(worst);
    
    if(best == 1 && worst == 6)
    {
        optRatingMarks->setChecked(TRUE);
    }
    if(best == 15 && worst == 0)
    {
        optRatingPoints->setChecked(TRUE);
    }
    
    
    //READ DATES
    QList<QDateEdit*>  DateBuf;
    DateBuf.append(dteSemester121Start);
    DateBuf.append(dteSemester121End);
    DateBuf.append(dteSemester122Start);
    DateBuf.append(dteSemester122End);
    DateBuf.append(dteSemester131Start);
    DateBuf.append(dteSemester131End);
    DateBuf.append(dteSemester132Start);
    DateBuf.append(dteSemester132End);
    
    QStringList SemesterName;
    SemesterName.append("12/1");
    SemesterName.append("12/2");
    SemesterName.append("13/1");
    SemesterName.append("13/2");
    
    int             i = 0;
    cDateConverter  dateToCopy;
    xmlObject*      currentSemester;
    QDate           qdateToCopy;
    xmlObject*      currentAttributeObject;
    disconnectMinMaxDate();
    currentAttributeObject = propertiesToEdit->cGetObjectByName( "time" );
    for(i=0; i < 8; i+=1)
    {
        currentSemester = currentAttributeObject->cGetObjectByAttributeValue("name", ksPlattformSpec::qstringToSz(SemesterName[(int)(i/2)]));
        if(!currentSemester)
            continue;
        if(i % 2)//if is 0 , 2, 4, 6 than i % 2 is 0 , so current date is a start date, else it is a end tag
            dateToCopy.setDateString(currentSemester->cGetAttributeByName("end")->value());
        else
            dateToCopy.setDateString(currentSemester->cGetAttributeByName("start")->value());
        qdateToCopy.setYMD( dateToCopy.Year(), dateToCopy.Month(), dateToCopy.Day());
        DateBuf[i]->setDate(qdateToCopy);
    }
    connectMinMaxDate();
    
}

void ksDatabaseProperties::setDatabaseToEdit(xmlObject* newDatabase)
{
    wdgCathegoryEditor->setDatabaseToEdit(newDatabase);
}

void ksDatabaseProperties::writeWidgetAttributesToProperties()
{
    // apply new cathegories:
    wdgCathegoryEditor->applyChanges();
    
    // apply new property-values:
    // name
    propertiesToEdit->cGetObjectByName( "author" )->nSetContent(txtName->text().toLocal8Bit().data());
    
    // rating mode
    int best;
    int worst;
    
    if(optRatingMarks->isChecked())
    {
        best = 1;
        worst = 6;
    }
    else if(optRatingPoints->isChecked())
    {
        best = 15;
        worst = 0;
    }
    if(optRatingOther->isChecked())
    {
        best = spinRatingBest->value();
        worst = spinRatingWorst->value();
    }
    
    propertiesToEdit->cGetObjectByName("rating")->cGetAttributeByName("best")->SetValueToInt(best);
    propertiesToEdit->cGetObjectByName("rating")->cGetAttributeByName("worst")->SetValueToInt(worst);
    
    // date
    QList<QDateEdit*>  DateBuf;
    DateBuf.append(dteSemester121Start);
    DateBuf.append(dteSemester121End);
    DateBuf.append(dteSemester122Start);
    DateBuf.append(dteSemester122End);
    DateBuf.append(dteSemester131Start);
    DateBuf.append(dteSemester131End);
    DateBuf.append(dteSemester132Start);
    DateBuf.append(dteSemester132End);
    
    QStringList SemesterName;
    SemesterName.append("12/1");
    SemesterName.append("12/2");
    SemesterName.append("13/1");
    SemesterName.append("13/2");
    
    int             i = 0;
    cDateConverter  dateToCopy;
    xmlObject*      currentSemester;
    QDate           qdateToCopy;
    xmlObject*      currentAttributeObject;
    
    currentAttributeObject = propertiesToEdit->cGetObjectByName( "time" );
    for(i=0; i < 8; i+=1)
    {
        currentSemester = currentAttributeObject->cGetObjectByAttributeValue("name", ksPlattformSpec::qstringToSz(SemesterName[(int)(i/2)]));
        if(!currentSemester)
            continue;
        qdateToCopy = DateBuf[i]->date();
        
        dateToCopy.setDay(qdateToCopy.day());
        dateToCopy.setMonth(qdateToCopy.month());
        dateToCopy.setYear(qdateToCopy.year());
        if(i % 2)//if is 0 , 2, 4, 6 than i % 2 is 0 , so current date is a start date, else it is a end tag
            currentSemester->cGetAttributeByName("end")->SetValue(dateToCopy.getDateString());
        else
            currentSemester->cGetAttributeByName("start")->SetValue(dateToCopy.getDateString());
        
    }
}

void ksDatabaseProperties::setMinMaxDate()
{
    QList<QDateEdit*>  DateBuf;
    DateBuf.append(dteSemester121Start);
    DateBuf.append(dteSemester121End);
    DateBuf.append(dteSemester122Start);
    DateBuf.append(dteSemester122End);
    DateBuf.append(dteSemester131Start);
    DateBuf.append(dteSemester131End);
    DateBuf.append(dteSemester132Start);
    DateBuf.append(dteSemester132End);
    
    for(int i = 1; i < (DateBuf.size()); i++)
    {
        DateBuf[i-1]->setMaximumDate(DateBuf[i]->date());
        DateBuf[i]->setMinimumDate(DateBuf[i-1]->date());
    }
    
}


