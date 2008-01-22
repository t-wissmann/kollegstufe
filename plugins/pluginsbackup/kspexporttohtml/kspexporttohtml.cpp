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
#include "kspexporttohtml.h"
#include <pluginengine/ksplugin.h>
#include <pluginengine/ksplugininformation.h>
#include <core/ksplattformspec.h>
#include <io/xmlparser.h>
#include <QAction>
#include <QPalette>
#include <QFile>
#include <QFileDialog>

#include <QColor>
#include <QTextStream>

#include <QMessageBox>
#include <QtPlugin>

Q_EXPORT_PLUGIN2(kspexporttohtml, kspExportToHtml)

kspExportToHtml::kspExportToHtml()
 : ksPlugin()
{
    setIdentifier("kspExportToHtml");
    setIsHavingAnAboutDialog(TRUE);
    setIsConfigurable(TRUE);
    retranslateUi();
}


kspExportToHtml::~kspExportToHtml()
{
}


void kspExportToHtml::retranslate()
{
    setName(tr("To Html Exporter"));
    setAuthor("Thorsten Wissmann");
    setDescription(tr("Exports database to an HTML-File"));
}


void kspExportToHtml::load()
{
    qDebug("%s:%s gets loaded", identifier().toAscii().data(), name().toAscii().data());
    mnaExportToHtml = new QAction(this);
    mnaExportToHtml->setText(tr("Export To Html"));
    addMenuAction(mnaExportToHtml);
    connect(mnaExportToHtml, SIGNAL(triggered()), this, SLOT(exportToHtml()));
}

void kspExportToHtml::refresh()
{
    
}

void kspExportToHtml::unload()
{
    qDebug("%s:%s gets unloaded", identifier().toAscii().data(), name().toAscii().data());
    
    ksPlugin::disconnect(mnaExportToHtml, SIGNAL(triggered()), this, SLOT(exportToHtml()));
    removeMenuAction(mnaExportToHtml);
    delete mnaExportToHtml;
}

void kspExportToHtml::createConfiguration(ksConfigContainer* config)
{
    if(!config)
    {
        return;
    }
    QPalette pal;
    config->setInGuiOptionPart(TRUE); // start of gui-options
    ksConfigOption* option;
    option = config->createOption(ksConfigOption("averagecol", TRUE));
    option->setDescription("Print average in last column");
    option->setWhatsThis("If checked, then there is a last column, which shows the subject average");
    option->rangeValue();
    
    
    option = config->createOption(ksConfigOption("showheader", TRUE));
    option->setDescription("Print a header");
    option->setWhatsThis("Prints a header at the top of the table. This header containes the description of each column");
    option->rangeValue();
    
    option = config->createOption(ksConfigOption("customcolors", FALSE));
    option->setDescription("Use custom colors and style options");
    option->setWhatsThis("Check this option to use custom colors and style options instead of system's default colors");
    option->rangeValue();
    
    option = config->createOption(ksConfigOption("bgcolor", pal.base().color()));
    option->setDescription("Custom background color");
    option->setWhatsThis("This is the background color of your HTML-Document");
    
    option = config->createOption(ksConfigOption("borderwidth", (int)1));
    option->setDescription("Custom border width");
    option->setWhatsThis("This is the width of the borders");
    option->setMinMax(0, 99);
    option->rangeValue();
    
    option = config->createOption(ksConfigOption("bordercolor", pal.text().color()));
    option->setDescription("Custom border color");
    option->setWhatsThis("This is the color of the borders");
    
    option = config->createOption(ksConfigOption("alternatecolor", pal.alternateBase().color()));
    option->setDescription("Custom alternate row color");
    option->setWhatsThis("This is the background color of every second row");
    
    option = config->createOption(ksConfigOption("textcolor", pal.text().color()));
    option->setDescription("Custom text color");
    option->setWhatsThis("This is the color of text in your HTML Document");
    
    config->setInGuiOptionPart(FALSE); // end of gui - options
}

void kspExportToHtml::exportToHtml()
{
    if(!m_pInformation || !m_pInformation->currentDataPart() || !m_pCurrentConfig)
    {
        qDebug("kspExportToHtml::exportToHtml(): Error: plugin information or m_pCurrentConfig pointer not set");
        return;
    }
    // create color palette
    createConfiguration(m_pCurrentConfig); // ensure that there are enough config options
    QPalette palette;
    int      borderwidth = 1;
    QColor bordercolor = palette.text().color();
    if(m_pCurrentConfig->getOption("customcolors")->valueToBool())
    {
        palette.setColor(QPalette::Base, m_pCurrentConfig->getOption("bgcolor")->valueToColor());
        palette.setColor(QPalette::Text, m_pCurrentConfig->getOption("textcolor")->valueToColor());
        palette.setColor(QPalette::AlternateBase, m_pCurrentConfig->getOption("alternatecolor")->valueToColor());
        bordercolor = m_pCurrentConfig->getOption("bordercolor")->valueToColor();
        borderwidth = m_pCurrentConfig->getOption("borderwidth")->valueToInt();
    }
    if(borderwidth < 0)
    {
        borderwidth = 0;
    }
    
    bool averagecol = m_pCurrentConfig->getOption("averagecol")->valueToBool();
    bool header     = m_pCurrentConfig->getOption("showheader")->valueToBool();
    
    QString filename = "/home/thorsten/out.html";
    filename = QFileDialog::getSaveFileName((QWidget*)m_pInformation->mainWindow(), tr("Export database to")
                                           , QDir::homePath(), tr("Html files (*.html *.htm)"));
    
    if(filename.isEmpty())
    {
        return;
    }
    if(!filename.endsWith(".htm") && !filename.endsWith(".html"))
    {
        filename += ".html";
    }
    
    xmlObject* data = m_pInformation->currentDataPart();
    if(!data)
    {
        return;
    }
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical((QWidget*)(m_pInformation->mainWindow()), tr("Error during saving file"),
                              tr("The file") + " \'" +  filename + "\' " + tr("couldn't be written.") + "\n"
                              + tr("Probably you haven't got enough write rights."));
        return;
    }
    QTextStream out(&file);
    
    out << "<html>\n<head>";
    out << "\n<style type=\"text/css\">";
    out << "\ntable { border-style: solid; border-width: " << borderwidth << "px;\n"
            << "border-bottom-width: 0px; \n"
            << "border-right-width: 0px; \n"
            << "border-color: " << bordercolor.name() << " ;}";
    out << "\ntd { border-style: solid; border-width: " << borderwidth << "px;\nborder-color: "
            << bordercolor.name()
            << "; color: " << palette.text().color().name() << "; \n"
            << "border-top-width: 0px; \n"
            << "border-left-width: 0px; \n"
            << "}\n";
    out << ".normalrow {\n"
            << "background-color: " << palette.base().color().name() << "; \n"
            << " }\n";
    out << ".alternaterow {\n"
            << "background-color: " << palette.alternateBase().color().name() << "; \n"
            << " }\n";
    out << "</style>\n</head>";
    out << "\n<body><table cellspacing=\"0\" cellpadding=\"2\" border=\"0\">";
    
    xmlObject* currentCategory;
    xmlObject* currentSubject;
    xmlObject* currentExam;
    int currentRowCount = 0; // tells you in which row you are
                             // should be increased when the row end tag "</tr>" is print
    
    if(header) // print header
    {
        out << "<tr class=\"normalrow\">";
        out << "<td>" << tr("Subject") << "&nbsp;</td>";
        out << "<td>" << tr("Exams") << "&nbsp;</td>";
        if(averagecol)
        {
            out << "<td>" << tr("Average") << "&nbsp;</td>";
        }
        out << "</tr>";
        currentRowCount++;
    }
    
    
    for(int i = 0; i < data->nGetObjectCounter(); i++)
    {
        currentCategory = data->cGetObjectByIdentifier(i);
        if(!currentCategory)
        {
            continue;
        }
        for(int subject = 0; subject < currentCategory->nGetObjectCounter(); subject++)
        {
            currentSubject = currentCategory->cGetObjectByIdentifier(subject);
            if(!currentSubject)
            {
                continue;
            }
            ksPlattformSpec::addMissingSubjectAttributes(currentSubject);
            out << "\n<tr ";
            if(currentRowCount % 2 == 1) // if is alternate row
            {
                out << "class=\"alternaterow\"";
            }
            else
            { // normal row
                out << "class=\"normalrow\"";
            }
            out << "><td>";
            out << ksPlattformSpec::szToUmlauts(currentSubject->cGetAttributeByName("name")->valueToXmlCode()) << "</td><td>";
            
            for(int exam = 0; exam < currentSubject->nGetObjectCounter(); exam++)
            {
                currentExam = currentSubject->cGetObjectByIdentifier(exam);
                if(!currentExam)
                {
                    continue;
                }
                ksPlattformSpec::addMissingExamAttributes(currentExam);
                
                out << currentExam->cGetObjectByAttributeValue("name", "type")->cGetAttributeByName("value")->valueToXmlCode();
                QString value = ksPlattformSpec::szToUmlauts(currentExam->cGetObjectByAttributeValue("name", "mark")->cGetAttributeByName("value")->valueToXmlCode());
                out << ": ";
                out << value;
                if(exam < currentSubject->nGetObjectCounter()-1) // if this is before the last loop
                {
                   out << ", ";
                }
            }
            out << "&nbsp;</td>\n";
            if(averagecol) // if there is a third col with the average
            {
                double average = ksPlattformSpec::computeEntireAverageOfSubject(currentSubject, "all", NULL);
                out << "<td>&nbsp;";
                if(average != -1)
                {
                    out << QString::number(average);
                }
                out << "</td>";
            }
            out << "</tr>";
            currentRowCount++;
            
        }
        
    }
    
    out << "\n</table></body>\n</html>";
    file.close();
}
