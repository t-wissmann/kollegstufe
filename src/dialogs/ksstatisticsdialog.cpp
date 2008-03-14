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
#include <io/ksiconcatcher.h>
#include <widgets/ksfilterwidget.h>

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
    reloadIcons();
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
}

void ksStatisticsDialog::retranslateUi()
{
    cmbSemesterSelection->setItemText(0, tr("All"));
    
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

void ksStatisticsDialog::reloadIcons()
{
    btnClose->setIcon(ksIconCatcher::getIcon("fileclose", 16));
}

void ksStatisticsDialog::allocateWidgets()
{
    btnClose = new QPushButton;
    cmbSemesterSelection = new QComboBox;
    statistics = new ksStatisticsWidget;
    information = new ksSubjectInformationWidget;
    lblSemesterSelection = new QLabel;
    wdgFilter = new ksFilterWidget;
    
    btnIncreaseSelection = new QPushButton;
    btnDecreaseSelection = new QPushButton;
}


void ksStatisticsDialog::createLayouts()
{
    layoutBottom = new QHBoxLayout;
    layoutBottom->addWidget(btnDecreaseSelection);
    layoutBottom->addWidget(btnIncreaseSelection);
    layoutBottom->addWidget(wdgFilter);
    layoutBottom->addStretch(1);
    layoutBottom->addWidget(btnClose);
    
    
    layoutSemesterSelection = new QHBoxLayout;
    layoutSemesterSelection->setMargin(0);
    layoutSemesterSelection->addWidget(lblSemesterSelection);
    layoutSemesterSelection->addWidget(cmbSemesterSelection);
    
    layoutInformation = new QVBoxLayout;
    layoutInformation->setMargin(0);
    layoutInformation->addLayout(layoutSemesterSelection);
    layoutInformation->addWidget(information);
    
    layoutChartAndInformation = new QHBoxLayout;
    layoutChartAndInformation->setMargin(0);
    layoutChartAndInformation->addWidget(statistics);
    layoutChartAndInformation->addLayout(layoutInformation);
    
    layoutParent = new QVBoxLayout;
    layoutParent->addLayout(layoutChartAndInformation);
    layoutParent->addLayout(layoutBottom);
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
    connect(wdgFilter, SIGNAL(filterChanged(QString)), statistics, SLOT(setFilter(QString)));
}

void ksStatisticsDialog::initWidgets()
{
    lblSemesterSelection->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    btnDecreaseSelection->setVisible(FALSE);
    btnIncreaseSelection->setVisible(FALSE);
    cmbSemesterSelection->addItem("all");
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
    
    xmlObject* semesterList = properties->cGetObjectByName("time");
    // remove items in combo box if there are to much
    while((cmbSemesterSelection->count() - 1) > semesterList->nGetObjectCounter())
    { // count() - 1 because the "all" item mustn't be contained in semesterList
        cmbSemesterSelection->removeItem(cmbSemesterSelection->count() - 1); // remove last
    }
    while((cmbSemesterSelection->count() - 1) < semesterList->nGetObjectCounter())
    {// add items if there are to few in the combo box
        cmbSemesterSelection->addItem("foo");
    }
    if(semesterList->nGetObjectCounter()  <= 0)
    {// if semester list is empty
        // then hide semester-selection-combo-box
        cmbSemesterSelection->setVisible(FALSE);
        lblSemesterSelection->setVisible(FALSE);
    }
    else
    {
        // show combo box
        lblSemesterSelection->setVisible(TRUE);
        cmbSemesterSelection->setVisible(TRUE);
        // copy names
        for(int i = 0; i < semesterList->nGetObjectCounter(); i++)
        {
        cmbSemesterSelection->setItemText(i+1, semesterList->cGetObjectByIdentifier(i)->cGetAttributeByName("name")->value()); 
        }
    }
}

void ksStatisticsDialog::currentSemesterChanged(int newSelection)
{
    if(newSelection >= 1 && newSelection < cmbSemesterSelection->count())
    {
        information->setSemesterToShow(cmbSemesterSelection->itemText(newSelection));
    }
    else
    {
        // if first item is selection , then "all" semesters are selected
        information->setSemesterToShow("all");
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


