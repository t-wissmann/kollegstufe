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
#include "ksstatisticsconfigwidget.h"
#include "ksstatisticswidget.h"

#include <QString>
#include <QEvent>

#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>

ksStatisticsConfigWidget::ksStatisticsConfigWidget ( QWidget *parent , Qt::WindowFlags f )
		: QWidget ( parent , f)
{
    m_pStatisticsWidget = NULL;
    allocateWidgets();
    createLayouts();
    connectSlots();
    retranslateUi();
}


ksStatisticsConfigWidget::~ksStatisticsConfigWidget()
{
}



void ksStatisticsConfigWidget::allocateWidgets()
{
    cmbLabelVisibility = new QComboBox;
    for(int i = (int)ksStatisticsWidget::LabelAlwaysVisible; i <= (int)ksStatisticsWidget::LabelNeverVisible; i++)
    {
        cmbLabelVisibility->addItem(QString::number(i));
    }
    lblLabelVisibility = new QLabel;
}

void ksStatisticsConfigWidget::createLayouts()
{
    
    layoutParent = new QHBoxLayout;
    layoutParent->addWidget(lblLabelVisibility);
    layoutParent->addWidget(cmbLabelVisibility);
    setLayout(layoutParent);
}


void ksStatisticsConfigWidget::connectSlots()
{
    connectWidgetValueSlots();
}

void ksStatisticsConfigWidget::connectWidgetValueSlots()
{
    connect(cmbLabelVisibility, SIGNAL(currentIndexChanged(int)), this, SLOT(applyChanges()));
}

void ksStatisticsConfigWidget::disconnectWidgetValueSlots()
{
    disconnect(cmbLabelVisibility, SIGNAL(currentIndexChanged(int)), this, SLOT(applyChanges()));
}

void ksStatisticsConfigWidget::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if(event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
}

void ksStatisticsConfigWidget::retranslateUi()
{
    lblLabelVisibility->setText(tr("Item labels visible:"));
    cmbLabelVisibility->setItemText(ksStatisticsWidget::LabelAlwaysVisible, tr("Always"));
    cmbLabelVisibility->setItemText(ksStatisticsWidget::LabelSelectedVisible, tr("Only Selected"));
    cmbLabelVisibility->setItemText(ksStatisticsWidget::LabelNeverVisible, tr("Never"));
}


void ksStatisticsConfigWidget::setStatisticsWidget(ksStatisticsWidget* statistics)
{
    m_pStatisticsWidget = statistics;
    // disconnect slots
    disconnectWidgetValueSlots();
    
    //load values from statistics widget
    cmbLabelVisibility->setCurrentIndex(m_pStatisticsWidget->itemLabelVisibility());
    
    // reconnectSlots
    connectWidgetValueSlots();
}


void ksStatisticsConfigWidget::applyChanges()
{
    m_pStatisticsWidget->setItemLabelVisibility((ksStatisticsWidget::LabelVisibility)cmbLabelVisibility->currentIndex());
}
