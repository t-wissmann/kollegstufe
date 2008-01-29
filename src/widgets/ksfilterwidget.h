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
#ifndef KSFILTERWIDGET_H
#define KSFILTERWIDGET_H

#include <QWidget>

class QLineEdit;
class QToolButton;
class QHBoxLayout;
class QLabel;
class QEvent;

class QKeyEvent;

/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/

class ksFilterWidget : public QWidget
{
Q_OBJECT
signals:
    void filterChanged(QString);
    void returnPressed();
public:
    ksFilterWidget(QWidget *parent = 0);

    ~ksFilterWidget();
    
    void retranslateUi();
    void reloadIcons();
    
    void changeEvent(QEvent* event);
    
    void setCaption(QString caption); // commit "" for default caption text
    QString caption() const;
    
    void setCaptionVisible(bool visible);
    bool isLabelVisible() const;
    
    QString filter() const;
    void setFilter(QString filter);
    
    void focusTextInputField();
    
public slots:
    void filterTextChanged();
    void clear();
    
protected:
    virtual void keyPressEvent(QKeyEvent* event);
private:
    void createGui();
    
    QToolButton*    btnClear;
    QLineEdit*      txtFilter;
    QLabel*         lblCaption;
    
    QHBoxLayout*    layoutParent;
    
    bool            m_bIsCustomLabel;
    QString         m_szDefaultCaption;
};

#endif
