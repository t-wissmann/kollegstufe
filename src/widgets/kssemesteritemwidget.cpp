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
#include "../widgets/kssemesteritemwidget.h"

#include <core/ksplattformspec.h>
#include <io/xmlparser.h>
#include <core/dateConverter.h>
#include <widgets/qclickablelabel.h>
#include <QLineEdit>
#include <QDateEdit>
#include <QDate>
#include <QLabel>

#include <QHBoxLayout>

#include <QPen>
#include <QPainter>
#include <QLinearGradient>
#include <QColor>
#include <QPalette>
#include <QPaintEvent>
#include <QFocusEvent>

ksSemesterItemWidget::ksSemesterItemWidget(QWidget *parent)
 : QWidget(parent)
{
    m_bSelected = FALSE;
    m_bAboutToDelete = FALSE;
    m_pSourceSemester = NULL;
    createGui();
    retranslateUi();
    setFocusPolicy(Qt::ClickFocus);
}


ksSemesterItemWidget::~ksSemesterItemWidget()
{
}


void ksSemesterItemWidget::createGui()
{
    dteStart = new QDateEdit;
    dteStart->setCalendarPopup(TRUE);
    dteEnd   = new QDateEdit;
    dteEnd->setCalendarPopup(TRUE);
    txtName  = new QLineEdit;
    lblFrom  = new QLabel;
    lblTo    = new QLabel;
    
    layoutParent = new QHBoxLayout;
    //layoutParent->setMargin(0);
    layoutParent->addWidget(txtName);
    layoutParent->addWidget(lblFrom);
    layoutParent->addWidget(dteStart);
    layoutParent->addWidget(lblTo);
    layoutParent->addWidget(dteEnd);
    
    setLayout(layoutParent);
    
    connect(txtName, SIGNAL(textEdited(QString)), this, SLOT(setSelected()));
    connect(dteStart,SIGNAL(dateChanged(QDate)), this, SLOT(setSelected()));
    connect(dteEnd,SIGNAL(dateChanged(QDate)), this, SLOT(setSelected()));
}


void ksSemesterItemWidget::retranslateUi()
{
    lblFrom->setText(tr("from"));
    lblTo->setText(tr("until"));
}

void ksSemesterItemWidget::setSemester(xmlObject* semester)
{
    m_pSourceSemester = semester;
    if(!semester)
    {
        //setEnabled(FALSE);// disable if semster is not valid
        return;
    }
    else
    {
        setEnabled(TRUE);
    }
    setAboutToDelete(FALSE);
    ksPlattformSpec::addMissingSemesterAttributes(semester);
    txtName->setText(ksPlattformSpec::szToUmlauts(semester->cGetAttributeByName("name")->value()));
    cDateConverter start;
    start.setDateString(semester->cGetAttributeByName("start")->value());
    dteStart->setDate(QDate(start.Year(), start.Month(), start.Day()));
    
    cDateConverter end;
    end.setDateString(semester->cGetAttributeByName("end")->value());
    dteEnd->setDate(QDate(end.Year(), end.Month(), end.Day()));
    
}


void ksSemesterItemWidget::setSelected(bool selected)
{
    //qDebug("%d was selected to %d", this, selected);
    if(selected == m_bSelected)
    {
        return; // return if nothing would be changed
    }
    m_bSelected = selected;
    if(m_bSelected)
    {
        lblFrom->setForegroundRole(QPalette::HighlightedText);
        lblTo->setForegroundRole(QPalette::HighlightedText);
        emit(getsSelected());
        emit(getsSelected(this));
    }
    else
    {
        lblFrom->setForegroundRole(QPalette::WindowText);
        lblTo->setForegroundRole(QPalette::WindowText);
    }
    update();
}

void ksSemesterItemWidget::paintEvent(QPaintEvent*)
{
    if(m_bSelected)
    {
        QLinearGradient gradient(0, 0, 0, height());
        QColor main = palette().highlight().color();
        QColor c1 = main;
        QColor c2 = main;
        c1.setHsv(c1.hue(), c1.saturation(), c1.value()+30, main.alpha());
        c2.setHsv(c2.hue(), c2.saturation(), c2.value()-30, main.alpha());
        gradient.setColorAt(0, c1);
        gradient.setColorAt(1, c2);
        QBrush brush(gradient);
        QPen   pen;
        pen.setColor(palette().highlight().color());
        
        QPainter painter(this);
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.drawRect(0, 0, width()-1, height()-1);
    }
}


void ksSemesterItemWidget::focusInEvent(QFocusEvent*)
{
    setSelected(TRUE);
}

void ksSemesterItemWidget::focusOutEvent(QFocusEvent*)
{
    /// now this gets unselected by parent
    //setSelected(FALSE);
}

void ksSemesterItemWidget::setAboutToDelete(bool deleteThis)
{
    m_bAboutToDelete = deleteThis;
    setVisible(!m_bAboutToDelete); // set this invisible if should be deleted
}

void ksSemesterItemWidget::applyChanges()
{
    writeDataTo(m_pSourceSemester);
}


void ksSemesterItemWidget::writeDataTo(xmlObject* semester) const
{
    if(!semester)
    {
        return;
    }
    ksPlattformSpec::addMissingSemesterAttributes(semester);
    
    QString name  = txtName->text();
    QDate   start = dteStart->date();
    QDate   end   = dteEnd->date();
    
    cDateConverter startdate;
    startdate.setDay(start.day());
    startdate.setMonth(start.month());
    startdate.setYear(start.year());
    
    cDateConverter enddate;
    enddate.setDay(end.day());
    enddate.setMonth(end.month());
    enddate.setYear(end.year());
    
    // copy values to semesterobject
    semester->cGetAttributeByName("name")->SetValue(ksPlattformSpec::qstringToSz(name));
    semester->cGetAttributeByName("start")->SetValue(startdate.getDateString());
    semester->cGetAttributeByName("end")->SetValue(enddate.getDateString());
    
}

// some getter & setter
void ksSemesterItemWidget::setStartDate(QDate start)
{
    dteStart->setDate(start);
}

QDate ksSemesterItemWidget::startDate() const
{
    return dteStart->date();
}

void ksSemesterItemWidget::setEndDate(QDate end)
{
    dteEnd->setDate(end);
}

QDate ksSemesterItemWidget::endDate() const
{
    return dteEnd->date();
}

void ksSemesterItemWidget::setName(QString name)
{
    txtName->setText(name);
}

QString ksSemesterItemWidget::name()const
{
    return txtName->text();
}


