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
#include "kssubjectstatusbar.h"
#include "ksplattformspec.h"

// widgets
#include <QPushButton>
#include <QLabel>

// layouts
#include <QHBoxLayout>

// other classes
#include <QEvent>

ksSubjectStatusbar::ksSubjectStatusbar(QWidget *parent)
 : QWidget(parent)
{
    initMembers();
    allocateWidgets();
    createLayouts();
    initWidgets();
    connectSlots();
    
    retranslateUi();
}


ksSubjectStatusbar::~ksSubjectStatusbar()
{
}

void ksSubjectStatusbar::initMembers()
{
    m_cSubjectToShow = NULL;
    layoutParent = NULL;
    setAlignment(Qt::AlignLeft);
}


void ksSubjectStatusbar::allocateWidgets()
{
    btnStatistics = new QPushButton;
}

void ksSubjectStatusbar::createLayouts()
{
    layoutParent = new QHBoxLayout;
    layoutParent->setMargin(0);
    layoutParent->addStretch(1);
    layoutParent->addWidget(btnStatistics);
    
    setLayout(layoutParent);
}

void ksSubjectStatusbar::connectSlots()
{
    connect(btnStatistics, SIGNAL(toggled(bool)), this, SLOT(btnStatisticsToggled(bool)));
}

void ksSubjectStatusbar::initWidgets()
{
    btnStatistics->setCheckable(TRUE);
}

void ksSubjectStatusbar::resetLayout()
{
    
}


void ksSubjectStatusbar::setEntireAverageTitle(double average)
{
    average *= 100;
    average = (int) average;
    average /= 100;
    btnStatistics->setText(tr("Entire Average"));
    if(average != -1)
    {
        btnStatistics->setText(btnStatistics->text() + ": " + QString::number(average));
    }
}

void ksSubjectStatusbar::calculateAverage()
{
    double average = ksPlattformSpec::computeEntireAverageOfSubject(m_cSubjectToShow, "all", NULL);
    setEntireAverageTitle(average);
}


void ksSubjectStatusbar::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if(event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
}

void ksSubjectStatusbar::toggleStatisticsButton(bool pressed)
{
    if(pressed != btnStatistics->isChecked())
    {
        btnStatistics->setChecked(pressed);
    }
    
}

void ksSubjectStatusbar::retranslateUi()
{
    calculateAverage();
}


//getter and setter functions
void ksSubjectStatusbar::setAlignment(Qt::Alignment nNewAlignment)
{
    m_nAlignment = nNewAlignment;
    if(!layoutParent)
    {
        return;
    }
    if(m_nAlignment == Qt::AlignLeft)
    {
        layoutParent->setDirection(QBoxLayout::RightToLeft);
    }
    else
    {
        layoutParent->setDirection(QBoxLayout::LeftToRight);
    }
}

Qt::Alignment ksSubjectStatusbar::alignment()
{
    return m_nAlignment;
}


void ksSubjectStatusbar::setSubject(xmlObject* newSubject)
{
    m_cSubjectToShow = newSubject;
    calculateAverage();
}

xmlObject* ksSubjectStatusbar::subject()
{
    return m_cSubjectToShow;
}




