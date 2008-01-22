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

#include <widgets/qclickablelabel.h>
#include <widgets/colorbutton.h>

#include <QPushButton>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QFocusEvent>

#include <QHBoxLayout>
#include <QSizePolicy>
#include <QPen>
#include <QPainter>
#include <QApplication>


ksConfigOptionWidget::ksConfigOptionWidget(QWidget *parent)
 : QWidget(parent)
{
    m_pConfigOption = NULL;
    m_bSelected = FALSE;
    allocateWidgets();
    createLayouts();
    connectSlots();
    retranslateUi();
    setSelected(FALSE);
    //setFocusPolicy(Qt::ClickFocus);
}


ksConfigOptionWidget::~ksConfigOptionWidget()
{
}


void ksConfigOptionWidget::allocateWidgets()
{
    
    lblDescription = new QClickableLabel;
    btnToDefault = new QPushButton;
    spinValueInt = new QSpinBox;
    spinValueDouble = new QDoubleSpinBox;
    txtValueString = new QLineEdit;
    btnValueColor = new ColorButton;
    chkValueBool = new QCheckBox("");
    
    btnToDefault->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    chkValueBool->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    spinValueInt->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    spinValueDouble->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
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
    layoutParent->addWidget(btnValueColor);
    layoutParent->addWidget(chkValueBool);
    layoutParent->addWidget(btnToDefault);
    
    setLayout(layoutParent);
}

void ksConfigOptionWidget::connectSlots()
{
    connect(btnToDefault, SIGNAL(clicked()), this, SLOT(setToDefault()));
    // slots for click event
    connect(btnToDefault, SIGNAL(clicked()), this, SLOT(childWasClicked()));
    connect(chkValueBool, SIGNAL(clicked(bool)), this, SLOT(childWasClicked()));
    connect(lblDescription, SIGNAL(clicked()), this, SLOT(childWasClicked()));
    connect(btnValueColor, SIGNAL(clicked()), this, SLOT(childWasClicked()));
    
    //connect(this, SIGNAL(clicked()), this, SLOT(setSelected()));
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
    btnValueColor->setVisible(type == ksConfigOption::TypeColor);
    
    // set minmax at first
    spinValueInt->setMinimum((int)m_pConfigOption->minimum());
    spinValueInt->setMaximum((int)m_pConfigOption->maximum());
    spinValueDouble->setMinimum(m_pConfigOption->minimum());
    spinValueDouble->setMaximum(m_pConfigOption->maximum());
    // set values
    spinValueInt->setValue(m_pConfigOption->valueToInt());
    spinValueDouble->setValue(m_pConfigOption->valueToDouble());
    txtValueString->setText(m_pConfigOption->valueToString());
    if(type == ksConfigOption::TypeColor)
    {// only copy color if type == color,,,
        // this if(){} was added to avoid a warning made by QColor on non color values
        btnValueColor->setColor(m_pConfigOption->valueToColor());
    }
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
    if(m_pConfigOption->valueType() == ksConfigOption::TypeColor)
    {// only copy color if type == color,,,
        // this if(){} was added to avoid a warning made by QColor on non color values
        btnValueColor->setColor(m_pConfigOption->valueToColor(defaultvalue));
    }
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
        case ksConfigOption::TypeColor: m_pConfigOption->setValue(btnValueColor->color());  break;
        
        default: m_pConfigOption->setValue(txtValueString->text());
    }
    
}

void ksConfigOptionWidget::childWasClicked()
{
    setFocus(Qt::MouseFocusReason);
    emit clicked();
}


void ksConfigOptionWidget::mousePressEvent(QMouseEvent* event)
{
    /*
    if(event->button() == Qt::LeftButton)
    {
        setFocus();
        emit clicked();
    }
    */
}


void ksConfigOptionWidget::paintEvent(QPaintEvent* event)
{
    if(m_bSelected)
    {
        QLinearGradient gradient(0, 0, 0, height());
        QColor main = palette().highlight().color();
        QColor c1 = main;
        QColor c2 = main;
        c1.setHsv(c1.hue(), c1.saturation(), c1.value()+30, main.alpha());
        c2.setHsv(c2.hue(), c2.saturation(), c2.value()-30, main.alpha());
        gradient.setColorAt(0, c1);
        gradient.setColorAt(1, c2);
        QBrush brush(gradient);
        QPen   pen;
        pen.setColor(palette().highlight().color());
        
        QPainter painter(this);
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.drawRect(0, 0, width()-1, height()-1);
    }
}

void ksConfigOptionWidget::setSelected(bool selected)
{
    if(selected != m_bSelected)
    {
        m_bSelected = selected;
        if(m_bSelected)
        {
            lblDescription->setForegroundRole(QPalette::HighlightedText);
        }
        else
        {
            lblDescription->setForegroundRole(QPalette::WindowText);
        }
    }
    update();
}

void ksConfigOptionWidget::retranslateUi()
{
    btnToDefault->setText(tr("To Default"));
}

bool ksConfigOptionWidget::hasMatchOn(QString text)
{
    bool result = FALSE;
    
    if(whatsThis().contains(text, Qt::CaseInsensitive))
    {
        result = TRUE;
    }
    if(lblDescription->text().contains(text, Qt::CaseInsensitive))
    {
        result = TRUE;
    }
    if(txtValueString->text().contains(text, Qt::CaseInsensitive))
    {
        result = TRUE;
    }
    
    return result;
}


