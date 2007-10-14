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


void ksAbout::retranslateUi()
{
    lblHeader->setText(tr("Kollegstufe"));
    lblVersion->setText(tr("version") + " " + ksPlattformSpec::versionAsString());
    btnClose->setText(tr("close"));
    QString infoText;
    infoText  = tr("Kollegstufe is a small Programe, that allows you to manage your marks in the bavarian Kollegstufe.") + " ";
    infoText += tr("It is released under the GPL - GNU General Public License.") + " ";
    infoText += tr("So it's free Software. You are allowed to copy, modify and distribute it for non-comercial use.") + " \n ";
    infoText += tr("You should have recieved a copy of the GPL with this.") + " ";
    infoText += tr("If not, see <i>http://www.gnu.org/licenses/gpl-3.0.html</i> for more details");
    txtInfoText->setHtml(infoText);
    setWindowTitle(tr("About Kollegstufe"));
}

void ksAbout::createGui()
{
    // widgets
    lblHeader = new QLabel;
    lblVersion = new QLabel;
    lblIcon = new QLabel;
    frmContainer = new QGroupBox;
    txtInfoText = new QTextEdit;
    btnClose = new QPushButton;
    
    // layouts
    layoutTop = new QHBoxLayout;
    layoutTop->setMargin(0);
    layoutTop->addWidget(lblHeader);
    layoutTop->addWidget(lblVersion);
    
    layoutBottom = new QHBoxLayout;
    layoutBottom->addStretch(1);
    layoutBottom->addWidget(btnClose);
    layoutBottom->setMargin(0);
    
    layoutFrame = new QHBoxLayout;
    layoutFrame->addWidget(lblIcon);
    layoutFrame->addWidget(txtInfoText);
    frmContainer->setLayout(layoutFrame);
    
    layoutParent = new QVBoxLayout;
    layoutParent->addLayout(layoutTop);
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
    txtInfoText->setContentsMargins(0, 2, 22, 22);
    // Header
    lblHeader->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    lblHeader->setFont(QFont("sdfsdf", 30, QFont::Bold));
    lblHeader->setAlignment(Qt::AlignHCenter);
    // Version
    lblVersion->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    lblVersion->setAlignment(Qt::AlignBottom);
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
    
    
    resize(400, 250);
    
    update();
}





