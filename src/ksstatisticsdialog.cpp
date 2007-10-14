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
#include "ksstatisticsdialog.h"
#include "ksstatisticswidget.h"
#include "ksstatisticsitem.h"
#include "ksplattformspec.h"

#include "xmlparser.h"
#include "dateConverter.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <QMessageBox>
#include <QPushButton>

#include <QEvent>
#include <QDate>


ksStatisticsDialog::ksStatisticsDialog(QWidget *parent)
 : QDialog(parent)
{
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
}

void ksStatisticsDialog::retranslateUi()
{
    
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
}


void ksStatisticsDialog::allocateWidgets()
{
    btnClose = new QPushButton;
    statistics = new ksStatisticsWidget;
}


void ksStatisticsDialog::createLayouts()
{
    layoutBottom = new QHBoxLayout;
    layoutBottom->addStretch(1);
    layoutBottom->addWidget(btnClose);
    
    layoutParent = new QGridLayout;
    layoutParent->addWidget(statistics,   0, 0);
    layoutParent->addLayout(layoutBottom, 1, 0);
    layoutParent->setMargin(3);
    layoutParent->setSpacing(3);
    
    setLayout(layoutParent);
}

void ksStatisticsDialog::connectSlots()
{
    connect(btnClose, SIGNAL(clicked()), this, SLOT(reject()));
}

void ksStatisticsDialog::initWidgets()
{
    
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
    }
    refreshExamListFromXmlSubject();
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
    //statistics->setMinMaxY(6, 1);
    
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
    
    xmlObject* currentExam;
    
    for (int i = 0; currentExam = subject->cGetObjectByIdentifier(i); i++)
    {
        //only use "real" exams
        if(ksPlattformSpec::szToUmlauts(currentExam->name()) != "exam")
        {
            continue;
        }
        // ensure, that there are the required attributes
        ksPlattformSpec::addMissingExamAttributes(currentExam);
        
        ksStatisticsItem itemToAdd;
        QString currentCaption;
        
        int currentNumber = currentExam->cGetObjectByAttributeValue("name", "number")->cGetAttributeByName("value")->nValueToInt();
        if(currentNumber != 0)
        {
            currentCaption += QString::number(currentNumber);
            currentCaption += ". ";
        }
        
        // add exam type
        currentCaption += ksPlattformSpec::szToUmlauts(currentExam->
                cGetObjectByAttributeValue("name", "type")->cGetAttributeByName("value")->value());
        
        // get points
        int currentPoints = currentExam->cGetObjectByAttributeValue("name", "points")
                                          ->cGetAttributeByName("value")->nValueToInt();
        
        //get date
        cDateConverter cDate;
        cDate.setDateString(currentExam->
                cGetObjectByAttributeValue("name", "date")->cGetAttributeByName("value")->value());
        QDate   currentDate;
        currentDate.setYMD(cDate.Year(), cDate.Month(), cDate.Day());
        
        //write variants to ksStatisticItem class
        itemToAdd.setX(currentDate);
        itemToAdd.setY(currentPoints);
        itemToAdd.setCaption(currentCaption);
        
        // add itemToAdd to statistics widget
        statistics->addItem(itemToAdd);
        
    }
    
    statistics->update();
}


