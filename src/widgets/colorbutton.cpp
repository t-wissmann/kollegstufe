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
#include "colorbutton.h"
#include <QSizePolicy>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QPalette>
#include <QRect>
#include <QColorDialog>

ColorButton::ColorButton(QWidget *parent)
 : QPushButton(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(this, SIGNAL(clicked()), this, SLOT(getColorFromDialog()));
}


ColorButton::~ColorButton()
{
}

void ColorButton::paintEvent(QPaintEvent* event)
{
    QPushButton::paintEvent(event);
    QBrush   bgBrush(Qt::SolidPattern);
    QPen     pen;
    pen.setColor(palette().highlight().color());
    pen.setWidth(1);
    
    int marginleft, margintop, marginright, marginbottom;
    getContentsMargins(&marginleft, &margintop, &marginright, &marginbottom);
    //qDebug("%d, %d, %d, %d", marginleft, margintop, marginright, marginbottom);
    
    // simulate border
    marginleft   += 5; // 5px space on the left
    margintop    += 5;
    marginright  += 5;
    marginbottom += 5;
    
    QRect rect(marginleft, margintop, width()-marginleft-marginright, height()-margintop-marginbottom);
    
    QPainter painter(this);
    // white background
    bgBrush = palette().base();
    if(isDown())
    {
        bgBrush.setColor(bgBrush.color().darker(120));
    }
    else if(underMouse())
    {
        bgBrush.setColor(bgBrush.color().lighter(120));
    }
    painter.setPen(Qt::NoPen);
    painter.setBrush(bgBrush);
    painter.drawRect(rect);
    
    // highlighted border
    rect.adjust(1, 1, -2, -2);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(rect);
    
    // draw current m_cColor
    rect.adjust(2, 2, -1, -1);
    bgBrush.setColor(m_cColor);
    
    painter.setPen(Qt::NoPen);
    if(isDown())
    {
        bgBrush.setColor(bgBrush.color().darker(120));
    }
    else if(underMouse())
    {
        bgBrush.setColor(bgBrush.color().lighter(120));
    }
    painter.setBrush(bgBrush);
    painter.drawRect(rect);
    
                     
}



void ColorButton::getColorFromDialog()
{
    QColor newColor = QColorDialog::getColor(m_cColor, this);
    if(newColor.isValid())
    {// only apply changes if color is valid, i.e. user didn't cancel the color dialog
        m_cColor = newColor;
    }
}


void ColorButton::setColor(const QColor& newColor)
{
    m_cColor = newColor;
    update();
}

