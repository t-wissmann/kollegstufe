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
#ifndef KSPLUGINITEMWIDGET_H
#define KSPLUGINITEMWIDGET_H

#include <QWidget>


class ksPlugin;
class QLabel;
class QPushButton;
class QCheckBox;
class QClickableLabel;
class QComboBox;
class QMouseEvent;

class QEvent;
class QHBoxLayout;
class QVBoxLayout;
class QBoxLayout;
class QGridLayout;

/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/
class ksPluginItemWidget : public QWidget
{
Q_OBJECT
        
signals:
    void clicked();
    void selected(ksPluginItemWidget* source);
public slots:
    void showConfigurationDialog();
    void showAboutDialog();
    void setSelected();
    void setSelected(bool selected);
    
    void setPluginStateFromGui();
    
public:
    ksPluginItemWidget(QWidget *parent = 0);

    ~ksPluginItemWidget();
    
    
    void setPlugin(ksPlugin*    newPlugin);
    bool isSelected() const { return m_bIsSelected; };
    void retranslateUi();
    void reloadIcons();
    
protected:
    virtual void changeEvent(QEvent* event);
    virtual void paintEvent ( QPaintEvent * event );
    virtual void mousePressEvent(QMouseEvent * event);
private:
    
    void allocateWidgets();
    void createLayouts();
    void connectSlots();
    
    // layouts
    QGridLayout* layoutParent;
    QBoxLayout* layoutRightEnd;
    
    // widgets
    QCheckBox*  chkIsLoaded;
    QComboBox*  cmbScope;
    QClickableLabel* lblName;
    QClickableLabel* lblDescription;
    QPushButton* btnAbout;
    QPushButton* btnConfigure;
    
    bool        m_bIsSelected;
    ksPlugin*   m_pCurrentPlugin;
};

#endif
