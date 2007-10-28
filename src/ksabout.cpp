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

#include "ksabout.h"
#include "ksplattformspec.h"

#include <QPushButton>
#include <QApplication>
#include <QLabel>
#include <QFrame>
#include <QTextEdit>
#include <QGroupBox>
#include <QTabWidget>

// layouts
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QString>
#include <QPixmap>
#include <QDir>
#include <QSizePolicy>
#include <QPalette>
#include <QFont>

ksAbout::ksAbout(QWidget *parent)
 : QDialog(parent)
{
    setModal(FALSE);
    createGui();
    initGui();
    retranslateUi();
    
    if(parent)
    {
        setWindowIcon(parent->windowIcon());
    }
}


ksAbout::~ksAbout()
{
    
}


void ksAbout::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if(event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
}

void ksAbout::createGui()
{
    // widgets
    lblHeader = new QLabel;
    lblVersion = new QLabel;
    lblIcon = new QLabel;
    tabContainer = new QTabWidget;
    lblInfoAbout = new QLabel;
    lblInfoAuthor = new QLabel;
    txtInfoLicense = new QTextEdit;
    btnClose = new QPushButton;
    wdgInfoAbout = new QWidget;
    
    
    //layout for tabs
    layoutInfoAbout = new QHBoxLayout;
    wdgInfoAbout->setLayout(layoutInfoAbout);
    
    layoutInfoAbout->addWidget(lblIcon);
    layoutInfoAbout->addWidget(lblInfoAbout);
    
    // layouts
    layoutTop = new QGridLayout;
    layoutTop->setMargin(0);
    //layoutTop->addWidget(lblIcon, 0, 0, 2, 1);
    layoutTop->addWidget(lblHeader, 0, 0);
    layoutTop->addWidget(lblVersion, 0, 1);
    
    layoutBottom = new QHBoxLayout;
    layoutBottom->addStretch(1);
    layoutBottom->addWidget(btnClose);
    layoutBottom->setMargin(0);
    
    tabContainer->addTab(wdgInfoAbout, "About");
    tabContainer->addTab(lblInfoAuthor, "Author");
    tabContainer->addTab(txtInfoLicense, "License");
    
    layoutParent = new QVBoxLayout;
    layoutParent->addLayout(layoutTop);
    layoutParent->addWidget(tabContainer);
    layoutParent->addLayout(layoutBottom);
    
    setLayout(layoutParent);
    
    //connect slots
    connect(btnClose, SIGNAL(clicked()), this, SLOT(accept()));
}


void ksAbout::initGui()
{
    txtInfoLicense->setLineWrapMode(QTextEdit::WidgetWidth);
    txtInfoLicense->setWordWrapMode(QTextOption::WordWrap);
    QPalette pal = palette();
    pal.setBrush(QPalette::Base, pal.brush(QPalette::Window));
    txtInfoLicense->setPalette(pal);
    txtInfoLicense->setReadOnly(TRUE);
    
    txtInfoLicense->setFrameStyle(QFrame::NoFrame);
    txtInfoLicense->setAutoFillBackground(FALSE);
    
    lblInfoAuthor->setContentsMargins(10, 10, 10, 10);
    
    lblInfoAbout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    
    lblIcon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // Header
    lblHeader->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    lblHeader->setFont(QFont("sdfsdf", 20, QFont::Bold));
    lblHeader->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    // Version
    lblVersion->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    lblVersion->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    
    // get application path
    QString szApplicationPath = QApplication::applicationDirPath();
    if( !szApplicationPath.endsWith(QDir::separator()))
    {
        szApplicationPath += QDir::separator();
    }
    
    QDir iconDir(szApplicationPath);
    iconDir.cdUp();
    iconDir.cd("pic");
    lblIcon->setPixmap(QPixmap(iconDir.filePath("kollegstufe.png")));
    
    
    resize(400, 250);
    update();
    
    resize((int)(width() * 1.1), height());
}



void ksAbout::retranslateUi()
{
    lblHeader->setText(tr("Kollegstufe"));
    lblVersion->setText(tr("version") + " " + ksPlattformSpec::versionAsString());
    btnClose->setText(tr("close"));
    //rename tabs
    tabContainer->setTabText(0, tr("About"));
    tabContainer->setTabText(1, tr("Author"));
    tabContainer->setTabText(2, tr("License"));
    
    QString infoText;
    infoText += "<br>" + tr("The manager for your marks during the <i>Kollegstufe</i>.") + "<br>";
    infoText += "<p>(c) Thorsten Wi&szlig;mann<br></p>";
    lblInfoAbout->setText(infoText);
    
    infoText = "";
    infoText += "Thorsten Wi&szlig;mann";
    infoText += "<br>&nbsp;&nbsp;&nbsp;";
    infoText += "<a href=\"kollegstufe@thorsten-wissmann.de\">kollegstufe@thorsten-wissmann.de</a>";
    lblInfoAuthor->setText(infoText);
    
    infoText = "<font face=\"Courier New\" size=\"3\"><br>" + tr("This software was released under the GPL.") + "<br></font>";
    //infoText = tr("This software was released under the GPL.");
    txtInfoLicense->setText(infoText);
    
    setWindowTitle(tr("About Kollegstufe"));
}




