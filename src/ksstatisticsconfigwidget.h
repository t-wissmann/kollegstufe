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
#ifndef KSSTATISTICSCONFIGWIDGET_H
#define KSSTATISTICSCONFIGWIDGET_H

#include <QWidget>
class ksStatisticsWidget;

class QComboBox;
class QLabel;
class QHBoxLayout;
class QEvent;


/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/
class ksStatisticsConfigWidget : public QWidget
{
Q_OBJECT
public slots:
    void applyChanges();
public:
    ksStatisticsConfigWidget(QWidget *parent = 0, Qt::WindowFlags f = 0 );

    ~ksStatisticsConfigWidget();
    void setStatisticsWidget(ksStatisticsWidget* statistics);
    void retranslateUi();
    
protected:
    void changeEvent(QEvent* event);
private:
    
    void allocateWidgets();
    void createLayouts();
    void connectSlots();
    void connectWidgetValueSlots();
    void disconnectWidgetValueSlots();
    
    // widgets
    QLabel*             lblLabelVisibility;
    QComboBox*          cmbLabelVisibility;
    //layouts
    QHBoxLayout*        layoutParent;
    
    //members
    ksStatisticsWidget* m_pStatisticsWidget;
};

#endif
