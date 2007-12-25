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
#include "ksstatisticswidget.h"
#include <QWidget>

#include <QPalette>
#include <QApplication>
#include <QSize>
#include <QSizePolicy>

#include <QRect>
#include <QPoint>
#include <QPointF>
#include <QBrush>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QGradient>
#include <QColor>
#include <QLinearGradient>
#include <QTextOption>
#include <QMessageBox>
#include <stdio.h>

ksStatisticsWidget::ksStatisticsWidget(QWidget *parent)
 : QFrame(parent)
{
    setFrameStyle(QFrame::Box);
    setMinimumWidth(100);
    setMinimumHeight(140);
    updateGridAndGraphProperties();
    nPointDiameter = 16;
    
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // set to 'white' background brush
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(TRUE);
    bItemListSorted = FALSE;
    nSelectedItemIndex = 0;
    
}

ksStatisticsWidget::~ksStatisticsWidget()
{
}


void ksStatisticsWidget::initMembers()
{
    nPointDiameter = 10;
    nMinimumY = 0;
    nMaximumY = 0;
}

void ksStatisticsWidget::mousePressEvent(QMouseEvent *event)
{
    
}

void ksStatisticsWidget::mouseMoveEvent(QMouseEvent *event)
{
    
}

void ksStatisticsWidget::paintEvent(QPaintEvent *event)
{
    
    updateGridAndGraphProperties();
    drawGrid();
    drawGraph();
    drawItems();
    //drawSelectionRectangle(10, 10, 160, 90);
    QFrame::paintEvent(event);
}

void ksStatisticsWidget::drawAxis(QPainter* painter)
{
    if(!painter)
    {
        return;
    }
    //draw Y-Axis
    painter->drawLine(YAxisLeft, YAxisTop,              YAxisLeft,             height() - XAxisBottom);
    //draw X-Axis
    painter->drawLine(YAxisLeft, height() - XAxisBottom, width() - XAxisRight, height() - XAxisBottom);
    //draw arrows
    //arrow at Y-Axis
    painter->drawLine(YAxisLeft, YAxisTop, YAxisLeft-arrowWidth, YAxisTop + arrowLength);
    painter->drawLine(YAxisLeft+arrowWidth, YAxisTop+arrowLength, YAxisLeft, YAxisTop);
    //arrow at X-Axis
    painter->drawLine(width() - XAxisRight, height() - XAxisBottom, width() - XAxisRight - arrowLength, height() - XAxisBottom - arrowWidth);
    painter->drawLine(width() - XAxisRight, height() - XAxisBottom, width() - XAxisRight - arrowLength, height() - XAxisBottom + arrowWidth);
    
}

void ksStatisticsWidget::drawYScaleLinesAndLabels(QPainter* painter)
{
    if(!painter)
    {
        return;
    }
    // draw scale lines on Y axis
    
    int sign = 1;
    // sign indecates if y values are listet ascending or descending
    if(nMinimumY < nMaximumY)
    {
        sign = 1;
    }
    else
    {
        sign = -1;
    }
    
    for (int i = nMinimumY; i != nMaximumY + sign; i+= sign)
    {
        int currentY = getScreenYForWorldY(i);
        int yBefore = getScreenYForWorldY(i-sign);
        int yAfter = getScreenYForWorldY(i+sign);
        //draw scale line
        
        /*printf("%d: currentY = %d\n", i, currentY);
        printf("%d: yBefore = %d\n", i, yBefore);
        printf("%d: yAfter = %d\n", i, yAfter);*/
        
        painter->drawLine( YAxisLeft - scaleLineLength, currentY, YAxisLeft + scaleLineLength, currentY);
        
        QString label = QString::number(i);
        QRect labelSpace(marginLeft, yAfter, YAxisLeft - scaleLineLength - marginLeft, yBefore-yAfter);
        // TODO if there is not enough space
        if(yBefore-yAfter > 25 || i%2)
        {
            painter->drawText(labelSpace, Qt::AlignHCenter|Qt::AlignVCenter, label);
        }
    }
}

void ksStatisticsWidget::drawGrid()
{
    
    QPainter pen(this);
    pen.setBrush(palette().brush(QPalette::Text));
    pen.setPen(palette().brush(QPalette::Text).color());
    
    
    drawAxis(&pen);
    drawYScaleLinesAndLabels(&pen);
}

void ksStatisticsWidget::drawGraph()
{
    
    sortItemList();
    
    QPainter graph(this);
    graph.setRenderHint(QPainter::Antialiasing);
    graph.setBrush(palette().brush(QPalette::Text));
    graph.setPen(palette().brush(QPalette::Text).color());
    
    int itemNumber = itemListSize();
    if(itemNumber < 0)
    {
        itemNumber = 0;
    }
    float graphWidth =  (float)((width()-XAxisRight-arrowLength) - (YAxisLeft+scaleLineLength));
    float pointXDistance = graphWidth / ((float)itemNumber+1.0); // +1 because there will be a distance between Y-Axis an point
                                                        // and so, it won't be devided by zero
    
    
    
    for (int i = 0; i < itemList.size(); i++)
    {
        if(i >= 1)
        {
            int x1 = YAxisLeft + arrowWidth + (int)((float)(i) * pointXDistance);
            int x2 = YAxisLeft + arrowWidth + (int)((float)(i+1) * pointXDistance);
            int y1 = getScreenYForWorldY(itemList[i-1].y());
            int y2 = getScreenYForWorldY(itemList[i].y());
            graph.drawLine(x1, y1, x2, y2);
        }
    }
}


int  ksStatisticsWidget::getScreenYForWorldY(int worldY) const
{
    //main
    int  scaleLinesAreaHeight =  (height()-XAxisBottom) - YAxisTop - arrowLength;
    int  numberOfSpaces = 1;
    int  sign = 1;
    
    if (nMinimumY < nMaximumY)
    {
        numberOfSpaces = nMaximumY - nMinimumY;
        sign = 1;
    }
    else
    {
        numberOfSpaces = nMinimumY - nMaximumY;
        sign = -1;
    }
    // there should be a space below and above
    numberOfSpaces+=2; 
    
    float scaleLineDistance =((float)scaleLinesAreaHeight) / (float)numberOfSpaces;
    
    
    int returnValue = 0;
    returnValue  = YAxisTop;
    returnValue += arrowLength;
    returnValue += (int)scaleLineDistance;
    returnValue += (int)(0.5 + ((float)nMaximumY - (float)worldY) * scaleLineDistance) * sign;
    
    
    return returnValue;
}

void ksStatisticsWidget::updateGridAndGraphProperties()
{
    getContentsMargins(&marginLeft, &marginTop, &marginRight, &marginBottom);
    
    YAxisLeft   = marginLeft + 20; // 20 for textlabels on the left
    YAxisTop    = marginTop + 15;
    XAxisBottom = marginBottom + 20; // 20 for textlabels on the bottom
    XAxisRight  = marginRight + 15;
    
    scaleLineLength = 3;
    arrowLength = 6;
    arrowWidth = 4;
}

void ksStatisticsWidget::drawItems()
{
    int itemNumber = itemListSize();
    if(itemNumber < 0)
    {
        itemNumber = 0;
    }
    float graphWidth =  (float)((width()-XAxisRight-arrowLength) - (YAxisLeft+scaleLineLength));
    float pointXDistance = graphWidth / ((float)itemNumber+1.0); // +1 because there will be a distance between Y-Axis an point
                                                        // and so, it won't be devided by zero
    bool lastItemWasAboveGraph = TRUE;
    
    for (int i = 0; i < itemList.size(); i++)
    {
        int x1 = YAxisLeft + arrowWidth + (int)((float)(i+1) * pointXDistance);
        int y1 = getScreenYForWorldY(itemList[i].y());
        int aboveOrBelow = 1; // 1 = below , -1 = above
        Qt::AlignmentFlag alignment = Qt::AlignHCenter;
        
        // find optimal space for placing label
        /*
        if (i == 0) // if is first item
        {
            if (itemList.size() > 1)
            {
                int yAfter = getScreenYForWorldY(itemList[i+1].y());
                if(yAfter > y1)
                {
                    aboveOrBelow = -1;
                    lastItemWasAboveGraph = TRUE;
                }
                else
                {
                    aboveOrBelow = +1;
                    lastItemWasAboveGraph = FALSE;
                }
            }
            // if itemList.size() is 1, defaultValues fit
            
        } else if ( i == itemList.size()-1)
        {
            if (itemList.size() > 1)
            {
                int yBefore = getScreenYForWorldY(itemList[i-1].y());
                if(yBefore > y1)
                {
                    aboveOrBelow = -1;
                    lastItemWasAboveGraph = TRUE;
                }
                else
                {
                    aboveOrBelow = +1;
                    lastItemWasAboveGraph = FALSE;
                }
            }
            // if itemList.size() is 1, defaultValues fit
        }
        else
        {
            int yAfter = getScreenYForWorldY(itemList[i+1].y());
            int yBefore = getScreenYForWorldY(itemList[i-1].y());
            if(y1 == yBefore && yAfter == y1) // if it is flat
            {
                if(lastItemWasAboveGraph)
                {
                    aboveOrBelow = +1;
                    lastItemWasAboveGraph = FALSE;
                }
                else
                {
                    aboveOrBelow = -1;
                    lastItemWasAboveGraph = TRUE;
                }
            }
            else if(yBefore > y1 && y1 < yAfter) // if it is a real hill
            {
                aboveOrBelow = -1;
                lastItemWasAboveGraph = TRUE;
            }
            else if(yBefore < y1 && y1 > yAfter) // if it is a  real valley
            {
                aboveOrBelow = +1;
                lastItemWasAboveGraph = FALSE;
            }
            else if(yBefore <= y1 && y1 <= yAfter) // if it is falling
            {
                if(lastItemWasAboveGraph)
                {
                    aboveOrBelow = +1;
                    lastItemWasAboveGraph = FALSE;
                    if(y1 == yAfter)
                    {
                        alignment = Qt::AlignHCenter;
                    }else
                    {
                        alignment = Qt::AlignLeft;
                    }
                }
                else
                {
                    aboveOrBelow = -1;
                    lastItemWasAboveGraph = TRUE;
                    if(y1 == yBefore)
                    {
                        alignment = Qt::AlignHCenter;
                    }else
                    {
                        alignment = Qt::AlignRight;
                    }
                }
            }
            else if(yBefore >= y1 && y1 >= yAfter) // if it is rising
            {
                if(lastItemWasAboveGraph)
                {
                    aboveOrBelow = +1;
                    lastItemWasAboveGraph = FALSE;
                    if(y1 == yBefore)
                    {
                        alignment = Qt::AlignHCenter;
                    }else
                    {
                        
                        alignment = Qt::AlignRight;
                    }
                }
                else
                {
                    aboveOrBelow = -1;
                    lastItemWasAboveGraph = TRUE;
                    alignment = Qt::AlignLeft;
                    if(y1 == yAfter)
                    {
                        alignment = Qt::AlignHCenter;
                    }else
                    {
                        alignment = Qt::AlignLeft;
                    }
                }
            }
        }
        */
        
        
        // place circle and label
        drawCaptionAt(x1, y1+(1+nPointDiameter/2)*aboveOrBelow, itemList[i].caption(),
                      alignment, aboveOrBelow == -1, i == nSelectedItemIndex);
        
        int circlealpha = 255;
        drawCircleAt(x1, y1, circlealpha);
    }
    
    
}
void ksStatisticsWidget::drawCircleAt(int circleX, int circleY, int alpha)
{
    QLinearGradient bgGradient( circleX, circleY-nPointDiameter/2, circleX, circleY+nPointDiameter/2);
    QColor  bgColorTop = palette().highlight().color();
    QColor  bgColorBottom = palette().highlight().color();
    int newValueTop = rangeValue(bgColorTop.value() < 3 ? 100 : bgColorTop.value(), 0, 255);
    int newValueBottom = rangeValue(bgColorTop.value()/2, 0, 255);
    
    bgColorTop.setHsv( bgColorTop.hue(), bgColorTop.saturation(), newValueTop, alpha );
    bgColorBottom.setHsv( bgColorBottom.hue(), bgColorBottom.saturation(), newValueBottom, alpha);
    bgGradient.setColorAt(0, bgColorTop);
    bgGradient.setColorAt(1, bgColorBottom);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(bgGradient);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(circleX-nPointDiameter/2, circleY-nPointDiameter/2, nPointDiameter, nPointDiameter);
}

int ksStatisticsWidget::rangeValue(int value, int minValue, int maxValue)
{
    int returnValue = value;
    if(value < minValue)
    {
        returnValue = minValue;
    }
    else if(value > maxValue)
    {
        returnValue = maxValue;
    }
    return returnValue;
}

void ksStatisticsWidget::drawCaptionAt(int captionX, int captionY, QString caption, Qt::AlignmentFlag alignment, bool above, bool selected)
{
    int captionLength = caption.length();
    int lineNumber = 1;
    int wrapLength = 10;
    // insert wordwraps
    while (captionLength > wrapLength)
    {
        lineNumber++;
        captionLength -= wrapLength;
        caption.insert(caption.length()- captionLength, "\n");
    }
    int captionWidth  = (int)((float)captionLength* 7.7) +4 ;
    if( lineNumber > 1) // if there's a wordwrap, wrapLength is max line-character-count
    {
        captionWidth = (int)((float)wrapLength* 7.7) + 4;
    }
    int captionHeight = 17 * lineNumber + 3;

    
    captionY -= captionHeight*(above);
    if(alignment == Qt::AlignLeft)
    {
        captionX -= captionWidth;
    }else if (alignment == Qt::AlignRight)
    {
        // nothing to do
    }else
    {
        captionX -= captionWidth/2;
    }
    
    QLinearGradient bgGradient( captionX, captionY, captionX, captionY+captionHeight);
    QColor  bgColorTop = palette().highlight().color();
    QColor  bgColorBottom = palette().highlight().color();
    int newValueTop = rangeValue(bgColorTop.value() < 3 ? 100 : bgColorTop.value(), 0, 255);
    int newValueBottom = rangeValue(bgColorTop.value()/2, 0, 255);
    
    bgColorBottom.setHsv( bgColorBottom.hue(), bgColorBottom.saturation(), newValueBottom, 255);
    bgColorTop.setHsv( bgColorBottom.hue(), bgColorBottom.saturation(), newValueTop, 255);
    bgGradient.setColorAt(0, bgColorTop);
    bgGradient.setColorAt(1, bgColorBottom);
    
    if(selected) // draw selection
    {
        drawSelectionRectangle(captionX-10, captionY-nPointDiameter-2-10, captionWidth+2*10, captionHeight+nPointDiameter+2+2*10);
    }
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    //draw background if is not selected
    painter.setBrush(bgGradient);
    painter.setPen(Qt::NoPen);
    painter.drawRoundRect(captionX, captionY, captionWidth, captionHeight, 10, 60);
    
    
    //draw text
    painter.setBrush(Qt::NoBrush);
    //painter.setPen(Qt::SolidLine);
    painter.setPen(palette().highlightedText().color());
    QRect captionSpace(captionX, captionY, captionWidth, captionHeight);
    painter.drawText(captionSpace, Qt::AlignHCenter|Qt::AlignVCenter, caption);
    
    painter.end();
    
    
}

void ksStatisticsWidget::drawSelectionRectangle(int nX, int nY, int nWidth, int nHeight)
{
    QLinearGradient bgGradient(QPointF(nX, nY), QPointF(nX, nY + nHeight));
    QColor mainColor = palette().highlight().color();
    mainColor.setAlpha(200);
    QColor colortop = mainColor;
    colortop.setHsv(mainColor.hue(), mainColor.saturation(), rangeValue(mainColor.value()+30, 0, 255));
    QColor colorbottom = mainColor;
    colorbottom.setHsv(mainColor.hue(), mainColor.saturation(), rangeValue(mainColor.value()-40, 0, 255));
    bgGradient.setColorAt(0, colortop);
    bgGradient.setColorAt(0.3, mainColor);
    bgGradient.setColorAt(0.85, mainColor);
    bgGradient.setColorAt(1, colorbottom);
    QBrush bgBrush(bgGradient); // background brush
    
    // setting pen
    QPen pen;
    pen.setWidthF(0.5);
    pen.setBrush(palette().text());
    
    // creating glaze
    QPainterPath gloss;
    gloss.moveTo(nX+1, nY+1);
    gloss.lineTo(nX+1, nY+nHeight);
    gloss.quadTo(QPointF(nX + 0.3 * nWidth, nY + 0.3 * nHeight), QPointF(nX + nWidth, nY + 4));
    gloss.lineTo(nX+nWidth-2, nY+1);
    
    QRadialGradient glossGradient(nX+nWidth/2, nY+nHeight/2, nWidth/2, nX+nWidth, nY+nHeight );
    glossGradient.setColorAt(1, QColor(255, 255, 255, 0));
    glossGradient.setColorAt(0, QColor(255, 255, 255, 130));
    glossGradient.setSpread(QGradient::PadSpread);
    
    // creating shadow of gloss
    QPainterPath glossshadow;
    glossshadow.moveTo(nX+1, nY+nHeight);
    glossshadow.quadTo(QPointF(nX + 0.3 * nWidth, nY + 0.3 * nHeight), QPointF(nX + nWidth, nY + 4));
    glossshadow.lineTo(nX-1+nWidth, nY+nHeight);
    glossshadow.lineTo(nX+3, nY+nHeight);
    
    QRadialGradient glossshadowGradient(nX+nWidth/2, nY+nHeight/2, nWidth/2, nX, nY );
    glossshadowGradient.setColorAt(1, QColor(0, 0, 0, 0));
    glossshadowGradient.setColorAt(0, QColor(0, 0, 0, 130));
    glossshadowGradient.setSpread(QGradient::PadSpread);
    
    
    //painting all to widget
    QPainter painter(this);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(bgBrush);
    painter.drawRoundRect(nX, nY, nWidth, nHeight, 8, 13);
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(glossshadowGradient);
    painter.drawPath(glossshadow);
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(glossGradient);
    painter.drawPath(gloss);
    
    
}

void ksStatisticsWidget::sortItemList()
{
    if(isItemListSorted())
    {
        return;
    }
    qSort(itemList);
    bItemListSorted = TRUE;
}

int ksStatisticsWidget::minimumY() const
{
    return nMinimumY;
}


int ksStatisticsWidget::maximumY() const
{
    return nMaximumY;
}


void ksStatisticsWidget::setMinMaxY(int newMinimumY, int newMaximumY)
{
    nMinimumY = newMinimumY;
    nMaximumY = newMaximumY;
    
    int newMinimumHeight;
    newMinimumHeight = nMinimumY - nMaximumY;
    if(newMinimumHeight  < 0)
    {// make newMinimumHeight positive
        newMinimumHeight *= -1;
    }
    newMinimumHeight++; // add one scale line space: n scale lines -> n+1 scale line spaces (on top and on bottom)
    newMinimumHeight *= 10; // adjust height of one scale line
    newMinimumHeight += marginTop;
    newMinimumHeight += marginBottom;
    
    setMinimumHeight(newMinimumHeight);
}

int ksStatisticsWidget::pointDiameter() const
{
    return nPointDiameter;
}

void ksStatisticsWidget::setPointDiameter(int newPointDiameter)
{
    nPointDiameter = newPointDiameter;
}

void ksStatisticsWidget::clearItemList()
{
    itemList.clear();
    bItemListSorted = TRUE;
}

void ksStatisticsWidget::addItem(ksStatisticsItem newItem)
{
    itemList.append(newItem);
    bItemListSorted = FALSE;
    setMinimumWidth(marginLeft + 10 + marginRight + (itemList.count()+1) * 45);
}

void ksStatisticsWidget::setSelectedItem(int index)
{
    nSelectedItemIndex = index;
    if(nSelectedItemIndex >= itemListSize())
    {
        nSelectedItemIndex = -1;
    }
    if(nSelectedItemIndex < -1)
    {
        nSelectedItemIndex = itemListSize()-1;
    }
    update();
}


int ksStatisticsWidget::indexOfItem(xmlObject* item)
{
    if(item == NULL)
    {
        return -1;
    }
    for(int i = 0; i < itemList.size(); i++)
    {
        if(itemList[i].sourceItem() == item)
        {
            return i;
        }
    }
    return -1;
}



