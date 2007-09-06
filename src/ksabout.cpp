/***************************************************************************
 *   Copyright (C) 2007 by Thorsten W.	                                   *
 *   towi89@web.de towi16.piranho.de                                       *
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
    setModal(TRUE);
    createGui();
    initGui();
}


ksAbout::~ksAbout()
{
    
}

void ksAbout::createGui()
{
    // widgets
    lblHeader = new QLabel("Kollegstufe");
    lblIcon = new QLabel;
    frmContainer = new QGroupBox;
    txtInfoText = new QTextEdit;
    btnClose = new QPushButton(QString::fromLocal8Bit("Schließen"));
    
    // layouts
    layoutBottom = new QHBoxLayout;
    layoutBottom->addStretch(1);
    layoutBottom->addWidget(btnClose);
    layoutBottom->setMargin(0);
    
    layoutFrame = new QHBoxLayout;
    layoutFrame->addWidget(lblIcon);
    layoutFrame->addWidget(txtInfoText);
    frmContainer->setLayout(layoutFrame);
    
    layoutParent = new QVBoxLayout;
    layoutParent->addWidget(lblHeader);
    layoutParent->addWidget(frmContainer);
    layoutParent->addLayout(layoutBottom);
    
    setLayout(layoutParent);
    
    //connect slots
    connect(btnClose, SIGNAL(clicked()), this, SLOT(accept()));
}


void ksAbout::initGui()
{
    // info text
    txtInfoText->setReadOnly(TRUE);
    txtInfoText->setFrameStyle(QFrame::NoFrame);
    QString infoText;
    infoText  = "Kollegstufe is a small Programe, that allows you to manage your marks in the bavarian Kollegstufe. ";
    infoText += "It is released under the GPL - GNU General Public License. ";
    infoText += "So it's free Software. You are allowed to copy, modify and distribute it for non-comercial use. ";
    infoText += "\n You should have recieved a copy of the GPL with this. ";
    infoText += "If not, see <i>http://www.gnu.org/licenses/gpl-3.0.html</i> for more details";
    txtInfoText->setContentsMargins(0, 2, 22, 22);
    txtInfoText->setHtml(infoText);
    // Header
    lblHeader->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    lblHeader->setFont(QFont("sdfsdf", 50, QFont::Bold));
    lblHeader->setAlignment(Qt::AlignHCenter);
    // Frame
    frmContainer->setBackgroundRole(QPalette::Base);
    frmContainer->setAutoFillBackground(TRUE);
    layoutFrame->setAlignment(lblIcon, Qt::AlignBottom);
    
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
    setWindowTitle(QString::fromLocal8Bit("Über Kollegstufe"));
    
    resize(400, 250);
    
    update();
}





