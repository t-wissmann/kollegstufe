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
#include "ksplattformspec.h"
#include <QDir>
#include <QStringList>
#include "xmlparser.h"
#include "dateConverter.h"

ksPlattformSpec::ksPlattformSpec()
{
}


ksPlattformSpec::~ksPlattformSpec()
{
}


bool ksPlattformSpec::createKsDir()
{
    QString directoryName = ".kollegstufe";
#if defined(Q_WS_WIN)
    directoryName = "kollegstufe";
#endif
    QDir ksDir = QDir::home();
    if(ksDir.exists(directoryName))
    {
        return TRUE;
    }
    else
    {
        return ksDir.mkdir(directoryName);
    }
    return TRUE;
}

QString ksPlattformSpec::getKsDir()
{
    
    QString directoryName = "";
#if defined(Q_WS_WIN)
    directoryName = "kollegstufe";
#else
    directoryName = ".kollegstufe";
#endif
    QString ksDir = QDir::homePath();
    
    if(!ksDir.endsWith(QDir::separator()))
    {
        ksDir += QDir::separator();
    }
    ksDir += directoryName;
    ksDir += QDir::separator();
    return ksDir;
}

void ksPlattformSpec::catchFileList(QStringList*    targetList)
{
    if (!targetList)
    {
        return;
    }
    // clear old list:
    targetList->clear();
    
    QString ksPath = getKsDir();
    QDir ksDir(ksPath);
    
    (*targetList) = ksDir.entryList( QStringList("*.xml"), QDir::Files);
    for (int i = 0; i < targetList->size(); i++)
    {
        if(targetList->at(i) == "config.xml")
        {
            targetList->removeAt(i);
            i--;
            continue;
        }
        (*targetList)[i] = ksPath + (*targetList)[i];
    }
    
}

bool ksPlattformSpec::createConfigFile()
{
    QDir configDir(getKsDir());
    if(configDir.exists("config.xml"))
    {
        return TRUE;
    }
    else
    {
        xmlObject newConfigFile;
        addMissingConfigAttributes(&newConfigFile);
        if ( 0 != WriteClassToFile(QString(getKsDir() + "config.xml").toAscii().data() ,&newConfigFile))
        {
            return FALSE;
        }
    }
    
    return TRUE;
}

QString ksPlattformSpec::getUserName()
{
    return "thorsten";
}

QString  ksPlattformSpec::getArticleForNoun(QString noun, kasus kasusOfNoun)
{
    QString article = "der/die/das";
    switch(kasusOfNoun)
    {
        case kasusNominativ:
            article = "der/die/das";
            break;
        case kasusGenitiv:
            article = "des/der/des";
            break;
        case kasusDativ:
            article = "dem/die/dem";
            break;
        case kasusAkkusativ:
            article = "der/die/das";
            if(noun == "Schulaufgabe")
                article = "die ";
            if(noun == "Stegreifaufgabe")
                article = "die ";
            if(noun == "Ex")
                article = "die ";
            if(noun == "Mitarbeit")
                article = "die ";
            if(noun == "Referat")
                article = "das ";
            if(noun == "Abfrage")
                article = "die ";
            if(noun == "Arbeit")
                article = "die ";
            if(noun == "Unterrichtsbeitrag")
                article = "den ";
            if(noun == "Abfrage")
                article = "die ";
            if(noun == "Facharbeit")
                article = "die ";
            if(noun == "Kurzarbeit")
                article = "die ";
            if(noun == "Jahrgangsstufentest")
                article = "den ";
            if(noun == "Arbeit")
                article = "die ";
            break;
    }
    return article;
}



/*  THE WAY, THE CONFIG FILE SHOULD LOOK LIKE:

<config>
 <window-settings width="739" height="376">
  <splitterMain coord="198"></splitterMain>
 </window-settings>
 <session>
  <file>/home/thorsten/.kde/share/apps/kollegstufe/archiv_zwei.xml</file>
 </session>
</config>



*/

void ksPlattformSpec::addMissingConfigAttributes(xmlObject* configFileToComplete)
{
    configFileToComplete->setName("config");
    if (!configFileToComplete->cGetObjectByName("window-settings"))
    {
        configFileToComplete->nAddObject("window-settings");
    }
    
    xmlObject* windowSettings = configFileToComplete->cGetObjectByName("window-settings");
    if (!windowSettings->cGetAttributeByName("width"))
    {
        windowSettings->nAddAttribute("width", "730");
    }
    if (!windowSettings->cGetAttributeByName("height"))
    {
        windowSettings->nAddAttribute("height", "376");
    }
    if (!windowSettings->cGetObjectByName("splitterMain"))
    {
        windowSettings->nAddObject("splitterMain");
    }
    if (!windowSettings->cGetObjectByName("splitterMain")->cGetAttributeByName("coord"))
    {
        windowSettings->cGetObjectByName("splitterMain")->nAddAttribute("coord", "198");
    }
    if (!configFileToComplete->cGetObjectByName("session"))
    {
        configFileToComplete->nAddObject("session");
    }
    if (!configFileToComplete->cGetObjectByName("session")->cGetObjectByName("file"))
    {
        configFileToComplete->cGetObjectByName("session")->nAddObject("file");
    }
}


void    ksPlattformSpec::addMissingSubjectAttributes(xmlObject*  SubjectToComplete)
{
    if(SubjectToComplete == NULL)
    {
        return;
    }
    if(SubjectToComplete->szGetName() != QString("subject"))
    {
        SubjectToComplete->setName("subject");
    }
    if(! SubjectToComplete->cGetAttributeByName("name"))
    {
        SubjectToComplete->nAddAttribute("name", "neues Fach");
    }
    if(! SubjectToComplete->cGetAttributeByName("teacher"))
    {
        SubjectToComplete->nAddAttribute("teacher", "");
    }
    
}


void    ksPlattformSpec::addMissingExamAttributes(xmlObject*  ExamToComplete)
{
    if(!ExamToComplete)
    {
        return;
    }
    
    if(ExamToComplete->szGetName() != QString("exam"))
    {
        ExamToComplete->setName("exam");
    }
    
    xmlObject* newObject;
    
    if(!ExamToComplete->cGetObjectByAttributeValue("name", "type"))
    {
        newObject = ExamToComplete->cGetObjectByIdentifier(ExamToComplete->nAddObject("attribute"));
        newObject->nAddAttribute("name", "type");
    }
    if(!ExamToComplete->cGetObjectByAttributeValue("name", "type")->cGetAttributeByName("value"))
        ExamToComplete->cGetObjectByAttributeValue("name", "type")->nAddAttribute("value", "Arbeit");
    
    if(!ExamToComplete->cGetObjectByAttributeValue("name", "points"))
    {
        newObject = ExamToComplete->cGetObjectByIdentifier(ExamToComplete->nAddObject("attribute"));
        newObject->nAddAttribute("name", "points");
    }
    if(!ExamToComplete->cGetObjectByAttributeValue("name", "points")->cGetAttributeByName("value"))
        ExamToComplete->cGetObjectByAttributeValue("name", "points")->nAddAttribute("value", "0");
    
    if(!ExamToComplete->cGetObjectByAttributeValue("name", "number"))
    {
        newObject = ExamToComplete->cGetObjectByIdentifier(ExamToComplete->nAddObject("attribute"));
        newObject->nAddAttribute("name", "number");
    }
    if(!ExamToComplete->cGetObjectByAttributeValue("name", "number")->cGetAttributeByName("value"))
        ExamToComplete->cGetObjectByAttributeValue("name", "number")->nAddAttribute("value", "0");
    
    if(!ExamToComplete->cGetObjectByAttributeValue("name", "date"))
    {
        newObject = ExamToComplete->cGetObjectByIdentifier(ExamToComplete->nAddObject("attribute"));
        newObject->nAddAttribute("name", "date");
    }
    if(!ExamToComplete->cGetObjectByAttributeValue("name", "date")->cGetAttributeByName("value"))
    {
        cDateConverter Date;
        ExamToComplete->cGetObjectByAttributeValue("name", "date")->nAddAttribute("value", Date.getDateString());
    }
    
    if(!ExamToComplete->cGetObjectByAttributeValue("name", "semester"))
    {
        newObject = ExamToComplete->cGetObjectByIdentifier(ExamToComplete->nAddObject("attribute"));
        newObject->nAddAttribute("name", "semester");
    }
    if(!ExamToComplete->cGetObjectByAttributeValue("name", "semester")->cGetAttributeByName("value"))
        ExamToComplete->cGetObjectByAttributeValue("name", "semester")->nAddAttribute("value", "auto");
    
    if(!ExamToComplete->cGetObjectByAttributeValue("name", "weighting"))
    {
        newObject = ExamToComplete->cGetObjectByIdentifier(ExamToComplete->nAddObject("attribute"));
        newObject->nAddAttribute("name", "weighting");
    }
    if(!ExamToComplete->cGetObjectByAttributeValue("name", "weighting")->cGetAttributeByName("value"))
        ExamToComplete->cGetObjectByAttributeValue("name", "weighting")->nAddAttribute("value", "oral");
}



void ksPlattformSpec::addMissingDatabaseAttributes(xmlObject* databaseToComplete)
{
    if(!databaseToComplete)
    {
        return;
    }
    if(databaseToComplete->szGetName() != QString("archiv"))
    {
        databaseToComplete->setName("archiv");
    }
    if(!databaseToComplete->cGetObjectByName("properties"))
    {
        databaseToComplete->nAddObject("properties");
    }
    if(!databaseToComplete->cGetObjectByName("data"))
    {
        databaseToComplete->nAddObject("data");
    }
}


void ksPlattformSpec::addMissingPropertiesAttributes(xmlObject* propertiesToComplete)
{
    xmlObject* currentObject;
    if(!propertiesToComplete)
    {
        return;
    }
    if(propertiesToComplete->szGetName() != QString("properties"))
    {
        propertiesToComplete->setName("properties");
    }
    if(!propertiesToComplete->cGetObjectByName("author"))
    {
        propertiesToComplete->nAddObject("author");
    }
    if(propertiesToComplete->cGetObjectByName("author")->nGetContentLength() < 1)
    {
        propertiesToComplete->cGetObjectByName("author")->nSetContent(getUserName().toAscii().data());
    }
    if(propertiesToComplete->cGetObjectByName("time"))
    {
        propertiesToComplete->nAddObject("time");
    }
    
}




