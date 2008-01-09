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
#ifndef KSCONFIGOPTIONWIDGET_H
#define KSCONFIGOPTIONWIDGET_H

#include <QWidget>

class QPushButton;
class ColorButton;
class QVBoxLayout;
class QSpinBox;
class QDoubleSpinBox;
class QLineEdit;
class QLabel;
class QCheckBox;

class QHBoxLayout;

class ksConfigOption;

/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/
class ksConfigOptionWidget : public QWidget
{
Q_OBJECT
public:
    ksConfigOptionWidget(QWidget *parent = 0);

    ~ksConfigOptionWidget();
    
    void setConfigOption(ksConfigOption* configOption);
    void retranslateUi();
public slots:
    void setToDefault();
    void applyChanges();
private:
    void allocateWidgets();
    void createLayouts();
    void connectSlots();
    
    // widgets
    QLabel*      lblDescription;
    QPushButton* btnToDefault;
    QSpinBox*    spinValueInt;
    QDoubleSpinBox* spinValueDouble;
    ColorButton* btnValueColor;
    QLineEdit*   txtValueString;
    QCheckBox*   chkValueBool;
    // layouts
    QHBoxLayout* layoutParent;
    
    // members    
    ksConfigOption* m_pConfigOption;
};

#endif
