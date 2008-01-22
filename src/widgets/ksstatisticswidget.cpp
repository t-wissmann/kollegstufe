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
#include <io/ksiconcatcher.h>
#include "ksstatisticsconfigwidget.h"
#include <io/xmlparser.h>
#include <core/dateConverter.h>
#include <core/ksplattformspec.h>
#include <QWidget>

#include <QPalette>
#include <QApplication>
#include <QSize>
#include <QSizePolicy>

#include <QRect>
#include <QLineF>
#include <QPoint>
#include <QPointF>
#include <QBrush>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QEvent>
#include <QPaintEvent>
#include <QGradient>
#include <QColor>
#include <QLinearGradient>
#include <QTextOption>
#include <QMessageBox>

ksStatisticsWidget::ksStatisticsWidget(QWidget *parent)
 : QFrame(parent)
{
    nItemLabelVisibility = LabelAlwaysVisible;
    setFrameStyle(QFrame::Box);
    setMinimumWidth(100);
    setMinimumHeight(140);
    updateGridAndGraphProperties();
    
    
    
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // set to 'white' background brush
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(TRUE);
    bItemListSorted = FALSE;
    nSelectedItemIndex = -1;
    nHoveredItemIndex = -1;
    setMouseTracking(TRUE);
    resetDefaultColors();
    initMembers();
    allocateWidgets();
    reloadIcons();
    
}

ksStatisticsWidget::~ksStatisticsWidget()
{
}


void ksStatisticsWidget::initMembers()
{
    nPointDiameter = 14;
    nMinimumY = 0;
    nMaximumY = 0;
    
    //init config button
    configBtnAlignment = Qt::AlignBottom | Qt::AlignRight;
    configBtnRadius = 15;
    configBtnIsAnimated = FALSE;
    configBtnTargetColor = configBtnNormalColor;
    configBtnCurrentColor = configBtnNormalColor;
    // int config button animation
    tmrAnimations.setSingleShot(FALSE);
    tmrAnimations.setInterval(40);
    
    // config box
    configBoxYState = 0;
    configBoxYTarget = 0;
    configBoxYSpeed = 5;
    configBoxIsAnimated = FALSE;
    configBoxMarginLeft  = 5;
    configBoxMarginRight  = 20;
    
    connect(&tmrAnimations, SIGNAL(timeout()), this, SLOT(animationEvent()));
    
}

void ksStatisticsWidget::allocateWidgets()
{
    wdgConfigBox = new ksStatisticsConfigWidget(this, Qt::Widget);
    wdgConfigBox->resize(wdgConfigBox->sizeHint());
    wdgConfigBox->setBackgroundRole(QPalette::Highlight);
    wdgConfigBox->setForegroundRole(QPalette::HighlightedText);
    wdgConfigBox->setStatisticsWidget(this);
    adjustConfigBoxSize();
    adjustConfigBoxPosition();
}

void ksStatisticsWidget::resetDefaultColors()
{
    //configBtnHoveredColor.setNamedColor("green"); // just a test color
    configBtnHoveredColor = palette().highlight().color();
    configBtnHoveredColor.setAlpha(255);
    configBtnNormalColor = configBtnHoveredColor;
    configBtnNormalColor.setHsv(configBtnNormalColor.hue(), 0, configBtnNormalColor.value(), configBtnNormalColor.alpha()/2);
    //configBtnNormalColor.setNamedColor("#ff0000");
}

void ksStatisticsWidget::reloadIcons()
{
    configBtnIcon = ksIconCatcher::getIconPixmap("configure", 16);
    configBoxMarginRight = configBtnIcon.width() + 10;
    
}


void ksStatisticsWidget::paintEvent(QPaintEvent *event)
{
    
    updateGridAndGraphProperties();
    drawGrid();
    drawGraph();
    drawItems();
    //drawSelectionRectangle(10, 10, 160, 90);
    drawConfigButton();
    drawConfigBox();
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
    
    QPainter painter(this);
    QPen     pen;
    pen.setColor(palette().windowText().color());
    pen.setWidthF(1.5);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(palette().brush(QPalette::Text));
    painter.setPen(pen);
    
    
    drawAxis(&painter);
    drawYScaleLinesAndLabels(&painter);
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
    YAxisTop    = marginTop + 40;
    XAxisBottom = marginBottom + 30; // 20 for textlabels on the bottom
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
        
        
        
        // place circle and label
        
        switch(nItemLabelVisibility)
        {
            case LabelAlwaysVisible:
            {
                drawCaptionAt(x1, y1+(1+nPointDiameter/2)*aboveOrBelow, itemList[i].caption(),
                              alignment, aboveOrBelow == -1, i == nSelectedItemIndex);
                break;
            }
            case LabelSelectedVisible:
            {
                if(i == nSelectedItemIndex || i == nHoveredItemIndex)
                {
                    drawCaptionAt(x1, y1+(1+nPointDiameter/2)*aboveOrBelow, itemList[i].caption(),
                              alignment, aboveOrBelow == -1, FALSE);
                }
                break;
            }
            case LabelNeverVisible:
            {
                if(i == nHoveredItemIndex)
                {
                    drawCaptionAt(x1, y1+(1+nPointDiameter/2)*aboveOrBelow, itemList[i].caption(),
                                  alignment, aboveOrBelow == -1, FALSE);
                }
                break;
            }
        }
        drawCircleAt(x1, y1, nPointDiameter+4 * ( i == nHoveredItemIndex)); // draw bigger if is selected
        //drawCircleAt(x1, y1);
    }
    
    
}
void ksStatisticsWidget::drawCircleAt(int circleX, int circleY, int diameter)
{
    QLinearGradient bgGradient( circleX, circleY-diameter/2, circleX, circleY+diameter/2);
    QColor  bgColorTop = palette().highlight().color();
    QColor  bgColorBottom = palette().highlight().color();
    int newValueTop = rangeValue(bgColorTop.value() < 3 ? 100 : bgColorTop.value(), 0, 255);
    int newValueBottom = rangeValue(bgColorTop.value()/2, 0, 255);
    
    bgColorTop.setHsv( bgColorTop.hue(), bgColorTop.saturation(), newValueTop );
    bgColorBottom.setHsv( bgColorBottom.hue(), bgColorBottom.saturation(), newValueBottom);
    bgGradient.setColorAt(0, bgColorTop);
    bgGradient.setColorAt(1, bgColorBottom);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(bgGradient);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(circleX-diameter/2, circleY-diameter/2, diameter, diameter);
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
    
    if(selected) // draw selection if necessary
    {
        int selectionx = captionX - 10;
        int selectiony = captionY - 10 +  ( above ? 0 : (-1)*nPointDiameter-2);
        int selectionwidth = captionWidth+2*10;
        int selectionheight = captionHeight+nPointDiameter+2+2*10;
        drawSelectionRectangle(selectionx, selectiony, selectionwidth, selectionheight);
    }
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    double cornerradius = 15;
    
    //draw background
    painter.setBrush(bgGradient);
    painter.setPen(Qt::NoPen);
    painter.drawRoundRect(captionX, captionY, captionWidth, captionHeight,
                          (int)cornerradius, (int)((cornerradius) * captionWidth / captionHeight));
    
    
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
    
    QRadialGradient glossGradient(nX+nWidth/2, nY+nHeight/2, nWidth, nX+nWidth, nY+nHeight );
    glossGradient.setColorAt(1, QColor(255, 255, 255, 0));
    glossGradient.setColorAt(0, QColor(255, 255, 255, 110));
    glossGradient.setSpread(QGradient::PadSpread);
    
    // creating shadow of gloss
    QPainterPath glossshadow;
    glossshadow.moveTo(nX+1, nY+nHeight);
    glossshadow.quadTo(QPointF(nX + 0.3 * nWidth, nY + 0.3 * nHeight), QPointF(nX + nWidth, nY + 4));
    glossshadow.lineTo(nX-1+nWidth, nY+nHeight);
    glossshadow.lineTo(nX+3, nY+nHeight);
    
    QRadialGradient glossshadowGradient(nX+nWidth/2, nY+nHeight/2, nWidth, nX, nY );
    glossshadowGradient.setColorAt(1, QColor(0, 0, 0, 0));
    glossshadowGradient.setColorAt(0, QColor(0, 0, 0, 110));
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


void ksStatisticsWidget::drawConfigButton()
{
    int size = configBtnIcon.width();
    if(size < 16)
    {
        size = 16;
    }
    configBtnRadius = size;
    configBtnRadius *= 1.41421356;
    
    configBtnRadius++; // create a 1px space
    
    QPainter painter(this);
    QBrush   bgBrush(palette().highlight());
    bgBrush.setColor(configBtnCurrentColor);
    
    painter.setRenderHint(QPainter::Antialiasing);
    
    // draw pie
    QPainterPath pie;
    pie.moveTo(width()-1, height());
    pie.lineTo(width() - configBtnRadius, height());
    pie.arcTo(width() - configBtnRadius, height() - configBtnRadius, configBtnRadius*2, configBtnRadius*2, 180*16, (-90)*16);
    pie.lineTo(width(), height()-1);
    pie.lineTo(width()-1, height()-1);
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(bgBrush);
    painter.drawPath(pie);
    // draw icon
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawPixmap(width()-size-1, height()-size-2, configBtnIcon);
    
}

void ksStatisticsWidget::drawConfigBox()
{
    
    int x = wdgConfigBox->x(), y = wdgConfigBox->y();
    int width = wdgConfigBox->width(), height = wdgConfigBox->height();
    int cornerradius = 10; // in px
    
    if(y >= this->height() || x >= this->width() || height == 0|| width == 0)
    {
        // we don't need to paint if box can't be seen
        return;
    }
    // create painterpath: rect with round top corners
    QPainterPath path;
    path.moveTo(x, y + height);
    path.lineTo(x, y + cornerradius);
    path.arcTo(x, y, cornerradius*2, cornerradius*2, 180, -90);
    path.lineTo(x - cornerradius+width, y);
    path.arcTo(x + width - cornerradius*2, y, cornerradius*2, cornerradius*2, 90, -90);
    path.lineTo(x + width, y + height);
    
    // create bgBrush
    QColor basicColor = palette().brush(wdgConfigBox->backgroundRole()).color();
    QColor c1 = basicColor, c2 = basicColor;
    c1.setAlpha(200);
    c2.darker(20);
    QLinearGradient bgGradient(x, y, x, y + height);
    bgGradient.setColorAt(0, c1);
    bgGradient.setColorAt(1, c2);
    QBrush  bgBrush(bgGradient);
    
    // init painter
    QPainter painter(this);
    // !! TODO painter.set
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(bgBrush);
    
    
    
    painter.drawPath(path);
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
    update();
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

void ksStatisticsWidget::loadItemListFromSubject(xmlObject* subject)
{
    // clear current list
    clearItemList();
    if(!subject)
    {
        // if subject is invalid, then let list empty
        return;
    }
    xmlObject* currentExam;
    
    for (int i = 0; (currentExam = subject->cGetObjectByIdentifier(i))  != NULL; i++)
    {
        //only use "real" exams
        if(ksPlattformSpec::szToUmlauts(currentExam->name()) != "exam")
        {
            continue;
        }
        // ensure, that there are the required attributes
        ksPlattformSpec::addMissingExamAttributes(currentExam);
        
        ksStatisticsItem itemToAdd;
        QString currentCaption;
        
        int currentNumber = currentExam->cGetObjectByAttributeValue("name", "number")->cGetAttributeByName("value")->nValueToInt();
        if(currentNumber != 0)
        {
            currentCaption += QString::number(currentNumber);
            currentCaption += ". ";
        }
        
        // add exam type
        currentCaption += ksPlattformSpec::szToUmlauts(currentExam->
                cGetObjectByAttributeValue("name", "type")->cGetAttributeByName("value")->value());
        
        // get mark
        int currentPoints = currentExam->cGetObjectByAttributeValue("name", "mark")
                ->cGetAttributeByName("value")->nValueToInt();
        
        //get date
        cDateConverter cDate;
        cDate.setDateString(currentExam->
                cGetObjectByAttributeValue("name", "date")->cGetAttributeByName("value")->value());
        QDate   currentDate;
        currentDate.setYMD(cDate.Year(), cDate.Month(), cDate.Day());
        
        //write variants to ksStatisticItem class
        itemToAdd.setX(currentDate);
        itemToAdd.setY(currentPoints);
        itemToAdd.setCaption(currentCaption);
        itemToAdd.setSourceItem(currentExam);
        
        // add itemToAdd to statistics widget
        addItem(itemToAdd);
        
    }
    
    update();
}

void ksStatisticsWidget::setSelectedItem(int index)
{
    if(nSelectedItemIndex == index)
    {
        // if nothing has changed, then exit
        return;
    }
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

int ksStatisticsWidget::itemIndexAt(int x, int y)
{
    int result = -1;
    
    int itemNumber = itemListSize();
    if(itemNumber < 0)
    {
        itemNumber = 0;
    }
    float graphWidth =  (float)((width()-XAxisRight-arrowLength) - (YAxisLeft+scaleLineLength));
    float pointXDistance = graphWidth / ((float)itemNumber+1.0); // +1 because there will be a distance between Y-Axis an point
                                                        // and so, it won't be devided by zero
    
    for(int i = 0; i < itemNumber; i++)
    {
        int x1 = YAxisLeft + arrowWidth + (int)((float)(i+1) * pointXDistance);
        int y1 = getScreenYForWorldY(itemList[i].y());
        QRect rect(x1 - nPointDiameter/2, y1 - nPointDiameter/2, nPointDiameter, nPointDiameter);
        if(rect.contains(x, y))
        {
            result = i;
            break;
        }
    }
    
    return result;
}

void ksStatisticsWidget::refreshConfigButtonColor()
{
    int newAlpha = configBtnCurrentColor.alpha();
    int newS = configBtnCurrentColor.saturation();
    int newV = configBtnCurrentColor.value();
    int deltaAlpha = 5;
    int dS = 5, dV = 5;
    
    // switch sign of delta values if necessarry
    deltaAlpha *= newAlpha >= configBtnTargetColor.alpha() ? -1 : 1;
    dS *= newS > configBtnTargetColor.saturation() ? -1 : 1;
    dV *= newV > configBtnTargetColor.value() ? -1 : 1;
    
    // apply deltas
    if(abs(newAlpha - configBtnTargetColor.alpha()) < abs(deltaAlpha))
    {
        deltaAlpha = 0;
        newAlpha = configBtnTargetColor.alpha();
    }
    else
    {
        newAlpha += deltaAlpha;
    }
    if(abs(newS - configBtnTargetColor.saturation()) <= abs(dS))
    {
        dS = 0;
        newS = configBtnTargetColor.saturation();
    }
    else
    {
        newS += dS;
    }
    if(abs(newV - configBtnTargetColor.value()) <= abs(dV))
    {
        dV = 0;
        newV = configBtnTargetColor.value();
    }
    else
    {
        newV += dV;
    }
    
    
    // apply new hsv and alpha values
    //configBtnCurrentColor.setAlpha(newAlpha);
    configBtnCurrentColor.setHsv(configBtnCurrentColor.hue(), newS, newV, newAlpha);
    if(deltaAlpha == 0 && dS == 0 && dV == 0)
    {
        //configBtnCurrentColor = configBtnTargetColor;
        configBtnIsAnimated = FALSE;
        resetAnimationTimerState();
    }
}

bool ksStatisticsWidget::isPositionInConfigButton(int x, int y)
{
    int cornerX = width();
    int cornerY = height();
    
    if(abs(cornerX - x) > configBtnRadius+1
       || abs(cornerY - y) > configBtnRadius+1)
    {
        // is cursor isn't in the current rect
        return FALSE;
    }
    else
    {
        QLineF distance(cornerX, cornerY, x, y);
        
        if(distance.length() <= configBtnRadius)
        {
            return TRUE;
            
        }else{
            return FALSE;
        }
    }
}

void ksStatisticsWidget::startConfigButtonAnimation()
{
    configBtnIsAnimated = TRUE;
    resetAnimationTimerState();
}

void ksStatisticsWidget::setConfigBtnTargetAlpha(int cursorx, int cursory)
{
    QColor newTargetColor = configBtnNormalColor;
    
    if(isPositionInConfigButton(cursorx, cursory))
    {
        newTargetColor = configBtnHoveredColor;
    }
    
    if(newTargetColor != configBtnTargetColor)
    {
        configBtnTargetColor = newTargetColor;
        startConfigButtonAnimation();
    }
}

void ksStatisticsWidget::mouseMoveEvent(QMouseEvent* event)
{
    if(!event)
    {
        return;
    }
    int index = itemIndexAt(event->x(), event->y());
    if(index != nHoveredItemIndex)
    {
        nHoveredItemIndex = index;
        update();
    }
    if(index == -1)
    {
        // if a item is hovered, then cursor can't hover config button
        setConfigBtnTargetAlpha(event->x(), event->y());
    }
}


void ksStatisticsWidget::leaveEvent(QEvent *)
{
    // just simulate a mouse cursor somewhere out of the config button
    setConfigBtnTargetAlpha((-1)*width(), 0);
}


void ksStatisticsWidget::mousePressEvent(QMouseEvent *event)
{
    if(!event)
    {
        return;
    }
    int index = itemIndexAt(event->x(), event->y());
    if(index != nSelectedItemIndex)
    {
        nSelectedItemIndex = index;
        update();
        emit currentItemChanged(index);
        if(index != -1)
        {
            emit currentItemChanged(itemList[index].sourceItem());
        }else{
            emit currentItemChanged((xmlObject*)NULL);
        }
    }
    if(index == -1)
    {
        // if a item is hovered, then cursor can't hover config button
        if(isPositionInConfigButton(event->x(), event->y()))
        {
            toggleConfigBoxVisibility();
        }
    }
}


void ksStatisticsWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(nSelectedItemIndex > -1)
    {
        emit itemDoubleClicked(nSelectedItemIndex);
    }
}


void ksStatisticsWidget::changeEvent ( QEvent * event )
{
    if(!event)
    {
        return;
    }
    //if color palette  has changed
    if(event->type() == QEvent::PaletteChange)
    {
        resetDefaultColors();
        if(isVisible())
        {
            // just simulate a mouse cursor somewhere out of the config button
            setConfigBtnTargetAlpha((-1)*width(), 0);
        }
    }
}


void ksStatisticsWidget::showConfigBox()
{
    // set target to "shown":
    configBoxYTarget = 100;
    //  start timer
    configBoxIsAnimated = TRUE;
    resetAnimationTimerState();
    
}

void ksStatisticsWidget::hideConfigBox()
{
    // set target to "hidden":
    configBoxYTarget = 0;
    //  start timer
    configBoxIsAnimated = TRUE;
    resetAnimationTimerState();
}


void ksStatisticsWidget::toggleConfigBoxVisibility()
{
    if(configBoxYTarget == 100) // if configBox currently is shown / showing
    {
        // then hide
        hideConfigBox();
    }
    else  // if configBox  currently is hidden / hiding
    {
        // then show
        showConfigBox();
    }
}

void ksStatisticsWidget::adjustConfigBoxSize()
{
    wdgConfigBox->resize(width()-(configBoxMarginLeft + configBoxMarginRight), wdgConfigBox->height());
}

void ksStatisticsWidget::adjustConfigBoxPosition()
{
    wdgConfigBox->move(configBoxMarginLeft,(int)(height() - ((double)wdgConfigBox->height())*((double)configBoxYState/100)));
}

void ksStatisticsWidget::moveConfigBox()
{
    if(configBoxYTarget > configBoxYState)
    {
        configBoxYState += configBoxYSpeed;
        if(configBoxYState > configBoxYTarget)
        {// if config box was moved to far
            configBoxYState = configBoxYTarget;
        }
    }
    if(configBoxYTarget < configBoxYState)
    {
        configBoxYState -= configBoxYSpeed;
        if(configBoxYState < configBoxYTarget)
        {// if config box was moved to far
            configBoxYState = configBoxYTarget;
        }
    }
    if(configBoxYTarget == configBoxYState)
    {
        // if config box reached the target
        // then stop timer
        configBoxIsAnimated = FALSE;
        resetAnimationTimerState();
    }
    adjustConfigBoxPosition();
}


void ksStatisticsWidget::animationEvent()
{
    refreshConfigButtonColor();
    moveConfigBox();
    update();
}

void ksStatisticsWidget::resetAnimationTimerState()
{
    bool tmrIsActive = configBoxIsAnimated || configBtnIsAnimated;
    if(tmrIsActive && !tmrAnimations.isActive())
    {
        tmrAnimations.start();
    }
    else if(!tmrIsActive && tmrAnimations.isActive())
    {
        tmrAnimations.stop();
    }
}

void ksStatisticsWidget::resizeEvent( QResizeEvent* )
{
    adjustConfigBoxSize();
    adjustConfigBoxPosition();
}


xmlObject* ksStatisticsWidget::selectedXmlSource() const
{
    if(nSelectedItemIndex < 0 || nSelectedItemIndex >= itemList.size())
    {
        return NULL;
    }
    else
    {
        return itemList[nSelectedItemIndex].sourceItem();
    }
}
