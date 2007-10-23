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
#ifndef KSSTATISTICSWIDGET_H
#define KSSTATISTICSWIDGET_H

#include <QFrame>

#include "ksstatisticsitem.h"
#include <QList>

class QMouseEvent;
class QPaintEvent;

/**
	@author Thorsten Wissmann <towi89@web.de>
*/
class ksStatisticsWidget : public QFrame
{
Q_OBJECT
public:
    ksStatisticsWidget(QWidget *parent = 0);

    ~ksStatisticsWidget();
    
    void clearItemList();
    void addItem(ksStatisticsItem newItem);
    
    int minimumY() const;
    int maximumY() const;
    
    void setMinMaxY(int newMinimumY, int newMinimumY);
    
    int pointDiameter() const;
    void setPointDiameter(int newPointDiameter);
    
    void sortItemList();
    bool isItemListSorted() const { return bItemListSorted; };
    
    int  itemListSize() const { return itemList.size(); };
    
    static int rangeValue(int value, int minValue, int maxValue);
    
protected: // events
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    
    void drawAxis(QPainter* painter);
    void drawYScaleLinesAndLabels(QPainter* painter);
    void drawGrid();
    void drawGraph();
    void drawItems();
    void drawCircleAt(int circleX, int circleY);
    void drawCaptionAt(int captionX, int captionY, QString caption, Qt::AlignmentFlag alignment = Qt::AlignHCenter, bool above = FALSE);
    void updateGridAndGraphProperties();
    
    int  getScreenYForWorldY(int item) const;
    
private:
    void initMembers();
    
    int  nPointDiameter; // in pixels; german: durchmesser
    
    
    QList<ksStatisticsItem>  itemList;
    bool                     bItemListSorted;
    QDate nXMinimum;
    QDate nXMaximum;
    int nMinimumY;
    int nMaximumY;
    
// grid and graph properities
    
    int marginLeft;
    int marginTop;
    int marginRight;
    int marginBottom;
    int YAxisLeft;
    int YAxisTop;
    int XAxisBottom;
    int XAxisRight;
    
    int scaleLineLength;
    int arrowLength; // parallel to Axis
    int arrowWidth;
};

#endif
