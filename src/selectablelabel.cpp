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
#include "selectablelabel.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QFocusEvent>
#include <QPainter>
#include <QPalette>
#include <QRect>

SelectableLabel::SelectableLabel(QWidget *parent)
 : QWidget(parent)
{
    m_bSelected = FALSE;
    setFocusPolicy(Qt::StrongFocus);
    setMinimumWidth(80);
    setMinimumHeight(80);
}


SelectableLabel::~SelectableLabel()
{
    
}

QString SelectableLabel::text() const
{
    return m_szText;
}
void SelectableLabel::setText(QString text)
{
    m_szText = text;
    update();
}

void SelectableLabel::focusInEvent(QFocusEvent* event)
{
    if(event->gotFocus())
    {
        setSelected(TRUE);
    }
}

void SelectableLabel::focusOutEvent(QFocusEvent* event)
{
    if(event->lostFocus())
    {
        setSelected(FALSE);
    }
}

/*void SelectableLabel::mousePressEvent(QMouseEvent* event)
{
    
}*/

void SelectableLabel::paintEvent(QPaintEvent* event)
{
    drawLabelAt(0, 0, width(), height(), m_szText, m_bSelected);   
    
}

void SelectableLabel::drawLabelAt(int x, int y, int width, int height, QString text, bool selected)
{
    if(width  <= 0 || height <= 0)
    {
        return;
    }
    QRect rect(x,y, width, height);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    QPen      pen;
    // draw background
    if(selected)
    {
        // create gradient
        QLinearGradient gradient(x, y, x, height);
        QColor maincolor = palette().highlight().color();
        QColor c1 = maincolor;
        QColor c2 = maincolor;
        
        c1.setHsv(c1.hue(), c1.saturation(), c1.value()+25, maincolor.alpha());
        c2.setHsv(c2.hue(), c2.saturation(), c2.value()-35, maincolor.alpha());
        
        gradient.setColorAt(0, c1);
        gradient.setColorAt(1, c2);
        
        // draw..
        QBrush   bgBrush(gradient);
        painter.setBrush(bgBrush);
        painter.setPen(Qt::NoPen);
        //painter.drawRect(rect);
        int cornerradius = 30;
        painter.drawRoundRect(rect, cornerradius, (int)(((double)cornerradius) * width / height));
        
        // init pen
        pen.setColor(palette().highlightedText().color());
    }
    else
    {
        // if is not selected, then we don't need a background
        
        // init pen
        pen.setColor(palette().windowText().color());
    }
    // draw text
    
    painter.setBrush(Qt::NoBrush);
    painter.setPen(pen);
    painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, text);
}


void SelectableLabel::setSelected(bool selected)
{
    if(m_bSelected != selected) // to avoid recursion
    {
        m_bSelected = selected;
        update();
        emit selectionChanged(m_bSelected);
    }
};

