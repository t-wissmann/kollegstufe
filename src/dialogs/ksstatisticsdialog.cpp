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
#include "ksstatisticsdialog.h"
#include <widgets/ksstatisticswidget.h>
#include <widgets/ksstatisticsitem.h>
#include <core/ksplattformspec.h>
#include <widgets/kssubjectinformationwidget.h>

#include <io/xmlparser.h>
#include <core/dateConverter.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <QMessageBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

#include <QEvent>
#include <QDate>

ksStatisticsDialog::ksStatisticsDialog(QWidget *parent)
 : QDialog(parent)
{
    if(parent) // if has parent, then it can be closed automatically on main window close
    {
        setAttribute(Qt::WA_QuitOnClose, FALSE);
    }
    initMembers();
    allocateWidgets();
    createLayouts();
    connectSlots();
    initWidgets();
    resize(400, 200);
    retranslateUi();
}


ksStatisticsDialog::~ksStatisticsDialog()
{
}

void ksStatisticsDialog::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if(event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
}



void ksStatisticsDialog::initMembers()
{
    subject = NULL;
    properties = NULL;
    semesterListCode.clear();
    semesterListCode.append("all");
    semesterListCode.append("12/1");
    semesterListCode.append("12/2");
    semesterListCode.append("13/1");
    semesterListCode.append("13/2");
}

void ksStatisticsDialog::retranslateUi()
{
    int oldSemesterSelection = cmbSemesterSelection->currentIndex();
    if(oldSemesterSelection < 0)
    {
        oldSemesterSelection = 0;
    }
    semesterListUi.clear();
    semesterListUi.append(tr("All"));
    semesterListUi.append("12 / 1");
    semesterListUi.append("12 / 2");
    semesterListUi.append("13 / 1");
    semesterListUi.append("13 / 2");
    
    cmbSemesterSelection->clear();
    cmbSemesterSelection->addItems(semesterListUi);
    // restore old selection
    cmbSemesterSelection->setCurrentIndex(oldSemesterSelection);
    
    
    lblSemesterSelection->setText(tr("Semester:"));
    
    btnClose->setText(tr("Close"));
    
    if (subject)
    {
        ksPlattformSpec::addMissingSubjectAttributes(subject);
    
        QString subjectName = ksPlattformSpec::szToUmlauts(subject->cGetAttributeByName("name")->value());
        setWindowTitle(tr("Statistics of subject ") + subjectName);
    }
    else
    {
        setWindowTitle(tr("Statistics"));
    }
    
    
    btnIncreaseSelection->setText(tr("Next Exam >>"));
    btnDecreaseSelection->setText(tr("<< Exam before"));
}


void ksStatisticsDialog::allocateWidgets()
{
    btnClose = new QPushButton;
    cmbSemesterSelection = new QComboBox;
    statistics = new ksStatisticsWidget;
    information = new ksSubjectInformationWidget;
    lblSemesterSelection = new QLabel;
    
    btnIncreaseSelection = new QPushButton;
    btnDecreaseSelection = new QPushButton;
}


void ksStatisticsDialog::createLayouts()
{
    layoutBottom = new QHBoxLayout;
    layoutBottom->addWidget(btnDecreaseSelection);
    layoutBottom->addWidget(btnIncreaseSelection);
    layoutBottom->addStretch(1);
    layoutBottom->addWidget(btnClose);
    
    layoutParent = new QGridLayout;
    layoutParent->addWidget(statistics,   0, 0, 2, 1);
    layoutParent->addWidget(lblSemesterSelection,   0, 1);
    layoutParent->addWidget(cmbSemesterSelection,   0, 2);
    layoutParent->addWidget(information,   1, 1, 1, 2);
    layoutParent->addLayout(layoutBottom, 2, 0, 1, 3);
    layoutParent->setMargin(3);
    layoutParent->setSpacing(3);
    
    setLayout(layoutParent);
}

void ksStatisticsDialog::connectSlots()
{
    connect(btnClose, SIGNAL(clicked()), this, SLOT(reject()));
    connect(cmbSemesterSelection, SIGNAL(currentIndexChanged(int)), this, SLOT(currentSemesterChanged(int)));
    
    connect(btnIncreaseSelection, SIGNAL(clicked()), this, SLOT(increaseSelection()));
    connect(btnDecreaseSelection, SIGNAL(clicked()), this, SLOT(decreaseSelection()));
}

void ksStatisticsDialog::initWidgets()
{
    lblSemesterSelection->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    btnDecreaseSelection->setVisible(FALSE);
    btnIncreaseSelection->setVisible(FALSE);
}

void ksStatisticsDialog::setSubject(xmlObject* newSubject)
{
    subject = newSubject;
    if (subject)
    {
        ksPlattformSpec::addMissingSubjectAttributes(subject);
    
        QString subjectName = ksPlattformSpec::szToUmlauts(subject->cGetAttributeByName("name")->value());
        setWindowTitle(tr("Statistics of subject ") + subjectName);
    }
    else
    {
        setWindowTitle(tr("Statistics"));
        refreshUiAndChildren();
    }
}

void ksStatisticsDialog::setProperties(xmlObject* newProperties)
{
    properties = newProperties;
    if(!properties)
    {
        return;
    }
    ksPlattformSpec::addMissingPropertiesAttributes(properties);
    int best  = properties->cGetObjectByName("rating")->cGetAttributeByName("best")->nValueToInt();
    int worst = properties->cGetObjectByName("rating")->cGetAttributeByName("worst")->nValueToInt();
    
    statistics->setMinMaxY(worst, best);
    information->setProperties(properties);
    
}

void ksStatisticsDialog::currentSemesterChanged(int newSelection)
{
    if(newSelection >= 0 && newSelection < semesterListCode.count())
    {
        information->setSemesterToShow(semesterListCode[newSelection]);
    }
}

void ksStatisticsDialog::refreshUiAndChildren()
{
    //QMessageBox::information(this, "ksStatisticsDialog", "ksStatisticsDialog::refreshUiAndChildren()");
    information->setSubjectToShow(subject);
    refreshExamListFromXmlSubject();
}

void ksStatisticsDialog::refreshExamListFromXmlSubject()
{
    statistics->clearItemList();
    
    if(!subject)
    {
        statistics->update();
        return;
    }
    statistics->loadItemListFromSubject(subject);
}



void ksStatisticsDialog::increaseSelection()
{
    statistics->setSelectedItem(statistics->selectedItemIndex()+1);
}


void ksStatisticsDialog::decreaseSelection()
{
    
    statistics->setSelectedItem(statistics->selectedItemIndex()-1);
}


void ksStatisticsDialog::setSelectedExam(xmlObject* exam)
{
    int index = statistics->indexOfItem(exam);
    statistics->setSelectedItem(index);
}

