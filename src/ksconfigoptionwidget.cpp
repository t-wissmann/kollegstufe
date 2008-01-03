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
#include "ksconfigoptionwidget.h"
#include "ksconfigoption.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include <QHBoxLayout>
#include <QSizePolicy>


ksConfigOptionWidget::ksConfigOptionWidget(QWidget *parent)
 : QWidget(parent)
{
    m_pConfigOption = NULL;
    allocateWidgets();
    createLayouts();
    connectSlots();
    retranslateUi();
}


ksConfigOptionWidget::~ksConfigOptionWidget()
{
}


void ksConfigOptionWidget::allocateWidgets()
{
    
    lblDescription = new QLabel;
    btnToDefault = new QPushButton;
    spinValueInt = new QSpinBox;
    spinValueDouble = new QDoubleSpinBox;
    txtValueString = new QLineEdit;
    chkValueBool = new QCheckBox("");
    
    btnToDefault->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    chkValueBool->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    txtValueString->setSizePolicy(spinValueInt->sizePolicy().horizontalPolicy(), QSizePolicy::Fixed);
}

void ksConfigOptionWidget::createLayouts()
{
    layoutParent = new QHBoxLayout;
    layoutParent->addWidget(lblDescription);
    layoutParent->addSpacing(0);
    layoutParent->addWidget(spinValueInt);
    layoutParent->addWidget(spinValueDouble);
    layoutParent->addWidget(txtValueString);
    layoutParent->addWidget(chkValueBool);
    layoutParent->addWidget(btnToDefault);
    
    setLayout(layoutParent);
}

void ksConfigOptionWidget::connectSlots()
{
    connect(btnToDefault, SIGNAL(clicked()), this, SLOT(setToDefault()));
}
    
void ksConfigOptionWidget::setConfigOption(ksConfigOption* configOption)
{
    m_pConfigOption = configOption;
    if(!m_pConfigOption)
    {
        setEnabled(FALSE);
        return;
    }
    // set visibilites
    ksConfigOption::OptionType type = m_pConfigOption->valueType();
    
    spinValueInt->setVisible(type == ksConfigOption::TypeInt);
    spinValueDouble->setVisible(type == ksConfigOption::TypeDouble);
    txtValueString->setVisible(type == ksConfigOption::TypeString);
    chkValueBool->setVisible(type == ksConfigOption::TypeBool);
    
    // set minmax at first
    spinValueInt->setMinimum((int)m_pConfigOption->minimum());
    spinValueInt->setMaximum((int)m_pConfigOption->maximum());
    spinValueDouble->setMinimum(m_pConfigOption->minimum());
    spinValueDouble->setMaximum(m_pConfigOption->maximum());
    // set values
    spinValueInt->setValue(m_pConfigOption->valueToInt());
    spinValueDouble->setValue(m_pConfigOption->valueToDouble());
    txtValueString->setText(m_pConfigOption->valueToString());
    chkValueBool->setChecked(m_pConfigOption->valueToBool());
    
    lblDescription->setText(m_pConfigOption->description());
    setWhatsThis(m_pConfigOption->whatsThis());
    
}

void ksConfigOptionWidget::setToDefault()
{
    if(m_pConfigOption == NULL)
    {
        return;
    }
    QString defaultvalue = m_pConfigOption->defaultValue();
    // set values
    spinValueInt->setValue(m_pConfigOption->valueToInt(defaultvalue));
    spinValueDouble->setValue(m_pConfigOption->valueToDouble(defaultvalue));
    txtValueString->setText(defaultvalue);
    chkValueBool->setChecked(m_pConfigOption->valueToBool(defaultvalue));
}

void ksConfigOptionWidget::applyChanges()
{
    if(m_pConfigOption == NULL)
    {
        return;
    }
    
    ksConfigOption::OptionType type = m_pConfigOption->valueType();
    switch(type)
    {
        case ksConfigOption::TypeInt: m_pConfigOption->setValue(spinValueInt->value()); break;
        case ksConfigOption::TypeDouble: m_pConfigOption->setValue(spinValueDouble->value()); break;
        case ksConfigOption::TypeBool: m_pConfigOption->setValue(chkValueBool->isChecked());  break;
        
        default: m_pConfigOption->setValue(txtValueString->text());
    }
    
}

void ksConfigOptionWidget::retranslateUi()
{
    btnToDefault->setText(tr("To Default"));
}

