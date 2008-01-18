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
#ifndef SELECTABLELABEL_H
#define SELECTABLELABEL_H

#include <QWidget>
#include <QString>

class QPaintEvent;
class QFocusEvent;
class QMouseEvent;

/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/
class SelectableLabel : public QWidget
{
Q_OBJECT
signals:
    void selectionChanged(bool selected);
public:
    SelectableLabel(QWidget *parent = 0);

    ~SelectableLabel();
    
    QString text() const;
    void setText(QString text);
    
    bool isSelected() const { return m_bSelected; };
    void setSelected(bool selected);
    
    void drawLabelAt(int x, int y, int width, int height, QString text, bool selected = FALSE);
    
protected:
    virtual void focusInEvent(QFocusEvent* event);
    virtual void focusOutEvent(QFocusEvent* event);
    //virtual void mousePressEvent(QMouseEvent* event);
    virtual void paintEvent(QPaintEvent* event);
    
private:
    QString  m_szText;
    bool     m_bSelected;
};

#endif
