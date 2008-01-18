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
#include <QPainter>
#include <QPixmap>
#include <QTimer>

class ksStatisticsConfigWidget;

class QResizeEvent;
class QMouseEvent;
class QPaintEvent;
class QEvent;

class xmlObject;

/**
	@author Thorsten Wissmann <towi89@web.de>
*/
class ksStatisticsWidget : public QFrame
{
Q_OBJECT
signals:
    void currentItemChanged(int index);
    void currentItemChanged(xmlObject* item);
public slots:
    void refreshConfigButtonColor();
    void startConfigButtonAnimation();
    void animationEvent();
    
    void showConfigBox();
    void hideConfigBox();
    void toggleConfigBoxVisibility();
    void adjustConfigBoxSize();
    void adjustConfigBoxPosition();
    
    void resetAnimationTimerState();
    
public:
    ksStatisticsWidget(QWidget *parent = 0);

    ~ksStatisticsWidget();
    
    enum LabelVisibility
    {
        LabelAlwaysVisible,
        LabelSelectedVisible,
        LabelNeverVisible,
    };
    
    void reloadIcons();
    void resetDefaultColors();
    
    void clearItemList();
    void loadItemListFromSubject(xmlObject* subject);
    void addItem(ksStatisticsItem newItem);
    int indexOfItem(xmlObject* item);
    
    
    int minimumY() const;
    int maximumY() const;
    
    void setMinMaxY(int newMinimumY, int newMinimumY);
    
    int pointDiameter() const;
    void setPointDiameter(int newPointDiameter);
    
    void sortItemList();
    bool isItemListSorted() const { return bItemListSorted; };
    
    int  itemListSize() const { return itemList.size(); };
    
    static int rangeValue(int value, int minValue, int maxValue);
    
    int selectedItemIndex() const { return nSelectedItemIndex; };
    void setSelectedItem(int index);
    
    xmlObject* selectedXmlSource() const;
    
    void setItemLabelVisibility(LabelVisibility visible) { nItemLabelVisibility = visible; update(); };
    LabelVisibility itemLabelVisibility() const { return nItemLabelVisibility; };
    
    int itemIndexAt(int x, int y);
    
    bool isPositionInConfigButton(int x, int y);
    
protected: // events
    void mousePressEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void changeEvent ( QEvent * event );
    virtual void resizeEvent( QResizeEvent* event);
    
    void drawAxis(QPainter* painter);
    void drawYScaleLinesAndLabels(QPainter* painter);
    void drawGrid();
    void drawGraph();
    void drawItems();
    void drawCircleAt(int circleX, int circleY) { drawCircleAt(circleX, circleY, nPointDiameter); };
    void drawCircleAt(int circleX, int circleY, int diameter);
    void drawCaptionAt(int captionX, int captionY, QString caption, Qt::AlignmentFlag alignment = Qt::AlignHCenter, bool above = FALSE, bool selected = FALSE);
    void updateGridAndGraphProperties();
    void drawSelectionRectangle(int nX, int nY, int nWidth, int nHeight);
    
    // about config button
    void drawConfigButton();
    void drawConfigBox();
    void moveConfigBox();
    
    
    int  getScreenYForWorldY(int worldY) const;
    
    void setConfigBtnTargetAlpha(int cursorx, int cursory);
    
    virtual void mouseMoveEvent(QMouseEvent* event);
    
    
private:
    void initMembers();
    
    void allocateWidgets();
    
    // widgets
    ksStatisticsConfigWidget* wdgConfigBox;
    
    //members
    
    int  nPointDiameter; // in pixels; german: durchmesser
    
    
    QList<ksStatisticsItem>  itemList;
    bool                     bItemListSorted;
    QDate nXMinimum;
    QDate nXMaximum;
    int nMinimumY;
    int nMaximumY;
    int nSelectedItemIndex;
    int nHoveredItemIndex;
    
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
    
    LabelVisibility nItemLabelVisibility;
    
    QTimer          tmrAnimations;
// config button
    Qt::Alignment   configBtnAlignment;
    QPixmap         configBtnIcon;
    QColor          configBtnCurrentColor;
    QColor          configBtnTargetColor;
    QColor          configBtnNormalColor;
    QColor          configBtnHoveredColor;
    double          configBtnRadius;
    bool            configBtnIsAnimated;
//config box
    char            configBoxYState; // between 0(hidden) and 100 (shown)
    char            configBoxYTarget;
    char            configBoxYSpeed; // pixels in 40 ms = tmrAnimations time
    bool            configBoxIsAnimated;
    int             configBoxMarginLeft;
    int             configBoxMarginRight;
    
    
};

#endif
