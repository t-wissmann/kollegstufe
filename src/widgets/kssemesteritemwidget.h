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
#ifndef KSSEMESTERITEMWIDGET_H
#define KSSEMESTERITEMWIDGET_H

#include <QWidget>

/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/

#include <QDate>
#include <QString>
class QDateEdit;
class QLineEdit;
class QLabel;
class QClickableLabel;
class QPaintEvent;

class QHBoxLayout;
class xmlObject;

class ksSemesterItemWidget : public QWidget
{
Q_OBJECT
signals:
    void getsSelected();
    void getsSelected(ksSemesterItemWidget* selectedWidget);
public:
    ksSemesterItemWidget(QWidget *parent = 0);

    ~ksSemesterItemWidget();
    
    void setSemester(xmlObject* semester);
    xmlObject* semester() { return m_pSourceSemester; };
    
    void retranslateUi();
    
    bool isSelected() const { return m_bSelected; };
    bool isAboutToDelete() const { return m_bAboutToDelete; }; // says if this semester should be deleted when changes are applied
    void setAboutToDelete(bool deleteThis);
    
    void applyChanges();
    void writeDataTo(xmlObject* semester) const;
    
    // some getter & setter
    void setStartDate(QDate start);
    QDate startDate() const;
    void setEndDate(QDate end);
    QDate endDate() const;
    void setName(QString name);
    QString name()const;
    
    
public slots:
    void setSelected(bool selected = TRUE);
    
protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void focusInEvent(QFocusEvent* event);
    virtual void focusOutEvent(QFocusEvent* event);
private:
    void createGui();
    
    QLabel*     lblName;
    
    QDateEdit*  dteStart;
    QDateEdit*  dteEnd;
    QLineEdit*  txtName;
    
    QLabel* lblFrom;
    QLabel* lblTo;
    
    
    QHBoxLayout* layoutParent;
    
    bool        m_bSelected;
    bool        m_bAboutToDelete;// says if this semester should be deleted when changes are applied
    xmlObject*  m_pSourceSemester;
};

#endif
