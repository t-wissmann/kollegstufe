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
#include <core/ksplattformspec.h>
#include <io/ksiconcatcher.h>
#include <widgets/kscathegoryeditorwidget.h>
#include <widgets/kssemesterlisteditor.h>

#include <io/xmlparser.h>
#include <core/dateConverter.h>

#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include <QRadioButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QGroupBox>
#include <QSpinBox>
#include <QListWidget>


// layouts
#include <QDialogButtonBox>
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
    
    btnOk->setDefault(TRUE);
    if(parent)
    {
        setWindowIcon(parent->windowIcon());
    }
    retranslateUi();
    reloadIcons();
}


ksDatabaseProperties::~ksDatabaseProperties()
{
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
    grpCathegories->setTitle(tr("Edit Subject Cathegories"));
    
    setWindowTitle(tr("Edit properties of Mark-Database"));
}

void ksDatabaseProperties::reloadIcons()
{
    btnOk->setIcon(ksIconCatcher::getIcon("button_ok", 16));
    btnCancel->setIcon(ksIconCatcher::getIcon("button_cancel", 16));
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
    
    wdgSemesterList = new ksSemesterListEditor;
    
    grpSemesterList = new QGroupBox;
    
    grpCathegories = new QGroupBox;
    wdgCathegoryEditor = new ksCathegoryEditorWidget;
    
    //lstStackMainControl = new QListWidget;
    //stackMain = new QStackedWidget;
}

void ksDatabaseProperties::createLayouts()
{
    boxBottom = new QDialogButtonBox;
    boxBottom->addButton(btnOk, QDialogButtonBox::AcceptRole);
    boxBottom->addButton(btnCancel, QDialogButtonBox::RejectRole);
    
    
    layoutSemesterList = new QVBoxLayout;
    layoutSemesterList->addWidget(wdgSemesterList);
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
    layoutParent->addWidget(boxBottom, 5, 0, 1, 3);
    
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
    
    wdgSemesterList->setSemesterList(propertiesToEdit->cGetObjectByName("time"));
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
    int best = 0;
    int worst = 0;
    
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
    else // so: optRatingOther must be checked
    {
        best = spinRatingBest->value();
        worst = spinRatingWorst->value();
    }
    
    propertiesToEdit->cGetObjectByName("rating")->cGetAttributeByName("best")->SetValueToInt(best);
    propertiesToEdit->cGetObjectByName("rating")->cGetAttributeByName("worst")->SetValueToInt(worst);
    
    // Semester list
    wdgSemesterList->applyChanges();
}


