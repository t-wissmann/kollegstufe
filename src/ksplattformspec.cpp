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
#include "ksplattformspec.h"
#include <QDir>
#include <QApplication>
#include <QFileInfo>
#include <QStringList>
#include <QTranslator>
#include <QLocale>
#include "xmlparser.h"
#include "dateConverter.h"


#if defined(Q_WS_WIN)
#include <windows.h>
#endif



QString ksPlattformSpec::versionAsString()
{
    return "1.0 Beta 1";
}

bool ksPlattformSpec::createKsDir()
{
    QString directoryName = ".kollegstufe";
    QDir ksDir = QDir::home();
#if defined(Q_WS_WIN)

    directoryName =  "kollegstufe";
#endif
    if(ksDir.exists(directoryName))
    {
        return TRUE;
    }
    else
    {
        return ksDir.mkdir(directoryName);
    }
    return FALSE;
}

QString ksPlattformSpec::getKsDir()
{
    
    QString directoryName = "";
    QString pathToReturn;
    QDir myHome;
    myHome.cd(QDir::homePath());
    pathToReturn = myHome.path();
    QString slash = QDir::separator();
    
    if(!pathToReturn.endsWith(slash))
    {
        pathToReturn += slash;
    }
#ifdef Q_WS_WIN
    pathToReturn += slash + "kollegstufe";
#else
    pathToReturn += slash + ".kollegstufe";
#endif
    pathToReturn += slash;
    return pathToReturn;
}

QString ksPlattformSpec::szToUmlauts(char* string)
{
    QString stringToReturn;
    stringToReturn = QString::fromLocal8Bit(string);
    //stringToReturn = string;
    //return QString::fromLocal8Bit(string);
    return stringToReturn;
}

char*   ksPlattformSpec::qstringToSz(QString string)
{
    return string.toLocal8Bit().data();
}

// language functions
QString ksPlattformSpec::getLanguageString()
{
    QString language;
    language = QLocale::languageToString(QLocale::system ().language());
    language = language.toLower();
    language = language.toLatin1();
    return language;
}

bool ksPlattformSpec::setLanguage(QString language, QTranslator* translator)
{
    if( ! translator)
    {
        return FALSE;
    }
    
    bool    returnValue = TRUE;
    QString tsFileName;
    QDir  translationDir(QApplication::applicationDirPath());
    translationDir.cdUp();
    translationDir.cd("translations");
    tsFileName  = "ks_";
    tsFileName += language;
    tsFileName += ".qm";
    if(!translator->load(translationDir.filePath(tsFileName)) && 0)
    {
        // if it wasn't able to load file; i.e.
        // if file doesn't exists, choose system standart
        tsFileName  = "ks_";
        tsFileName += getLanguageString();
        tsFileName += ".qm";
        if(!translator->load(translationDir.filePath(tsFileName)))
        {
            //if system-standart doesn't exists, choose english
            tsFileName  = "ks_";
            tsFileName += "english";
            tsFileName += ".qm";
            if(!translator->load(translationDir.filePath(tsFileName)))
            {
                //if english translation doesn't exists, do nothing
                returnValue = FALSE;
            }
        }
    }
    // install translator to application
    qApp->installTranslator(translator);
    return returnValue;
}

QStringList ksPlattformSpec::getAvailibleLanguages()
{
    QStringList list;
    
    QDir  translationDir(QApplication::applicationDirPath());
    translationDir.cdUp();
    translationDir.cd("translations");
    list = translationDir.entryList( QStringList("*.qm"), QDir::Files);
    for (int i = 0; i < list.count(); i++)
    {
        if(!list[i].startsWith("ks_"))
        {
            list.removeAt(i);
            i--;
            continue;
        }
        list[i] = list[i].remove("ks_");
        list[i] = list[i].remove(".qm");
    }
    return list;
}



void ksPlattformSpec::catchFileList(QStringList* targetList)
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

QString ksPlattformSpec::getNewFilename(QString prefix, QString suffix)
{
    QDir dir(getKsDir());
    int number = 1;
    while(dir.exists(prefix + QString::number(number) + suffix))
    {
        number++;
    }
    
    return getKsDir() + prefix + QString::number(number) + suffix;
}

QString ksPlattformSpec::getUserName()
{
    QString userName;
    //ON WINDOWS:
#ifdef Q_WS_WIN

    userName = tr("New Pupil");
#else
    //ON UNIX/LINUX:
    userName = getenv("USER");
#endif
    return userName;
}

bool ksPlattformSpec::fileExists(QString filename)
{
    return QFileInfo(filename).exists();
}

bool ksPlattformSpec::deleteFile(QString fileToDelete)
{
    if( !ksPlattformSpec::fileExists(fileToDelete))
    {
        return TRUE;
    }
    
    QFileInfo file (fileToDelete);
    QDir      dir = file.dir();
    return dir.remove(file.fileName());
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
    if (!configFileToComplete->cGetObjectByName("language"))
    {
        configFileToComplete->nAddObject("language");
        
        QString language = getLanguageString();
        
        configFileToComplete->cGetObjectByName("language")->nSetContent(qstringToSz(language));
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


void   ksPlattformSpec::addMissingCathegoryAttributes(xmlObject*  CathegoryToComplete)
{
    if(!CathegoryToComplete)
    {
        return;
    }
    if(CathegoryToComplete->name() != QString("cathegory"))
    {
        CathegoryToComplete->setName("cathegory");
    }
    if(CathegoryToComplete->cGetAttributeByName("name") == NULL)
    {
        CathegoryToComplete->nAddAttribute("name", qstringToSz(tr("New Subject")));
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
    
    if(! SubjectToComplete->cGetAttributeByName("weightingOral"))
    {
        SubjectToComplete->nAddAttribute("weightingOral", "1");
    }
    if(! SubjectToComplete->cGetAttributeByName("weightingWritten"))
    {
        SubjectToComplete->nAddAttribute("weightingWritten", "2");
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
    /*if(!databaseToComplete->cGetObjectByName("data")->cGetObjectByName("cathegory")) // if there is no cathegory
    {
    int newId = databaseToComplete->cGetObjectByName("data")->nAddObject("cathegory");
    databaseToComplete->cGetObjectByName("data")->cGetObjectByIdentifier(newId)->nAddAttribute("name", "Leistungskurse");
        
    newId = databaseToComplete->cGetObjectByName("data")->nAddObject("cathegory");
    databaseToComplete->cGetObjectByName("data")->cGetObjectByIdentifier(newId)->nAddAttribute("name", "Grundkurse");
}*/
}


void ksPlattformSpec::addMissingPropertiesAttributes(xmlObject* propertiesToComplete)
{
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
    // rating
    if(!propertiesToComplete->cGetObjectByName("rating"))
    {
        propertiesToComplete->nAddObject("rating");
    }
    if(!propertiesToComplete->cGetObjectByName("rating")->cGetAttributeByName("best"))
    {
        propertiesToComplete->cGetObjectByName("rating")->nAddAttribute("best", "15");
    }
    
    if(!propertiesToComplete->cGetObjectByName("rating")->cGetAttributeByName("worst"))
    {
        propertiesToComplete->cGetObjectByName("rating")->nAddAttribute("worst", "0");
    }
    
    // ADD TIME
    if(!propertiesToComplete->cGetObjectByName("time"))
    {
        propertiesToComplete->nAddObject("time");
    }
    cDateConverter  dateToWrite;
    cDateConverter  dateToRead;
    
    int nCurrentSemester;
    QStringList Semesters;
    xmlObject*  currentSemester;
    Semesters.append("12/1");
    Semesters.append("12/2");
    Semesters.append("13/1");
    Semesters.append("13/2");
    for(nCurrentSemester = 0; nCurrentSemester < 4; nCurrentSemester++)
    {
        currentSemester = propertiesToComplete->cGetObjectByName("time")->cGetObjectByAttributeValue("name", Semesters[nCurrentSemester].toAscii().data());
        
        if(!currentSemester)
            currentSemester = propertiesToComplete->cGetObjectByName("time")->cGetObjectByIdentifier(propertiesToComplete->cGetObjectByName("time")->nAddObject( "semester"));
        
        if(QString("semester") != currentSemester->szName)
            currentSemester = propertiesToComplete->cGetObjectByName("time")->cGetObjectByIdentifier(propertiesToComplete->cGetObjectByName("time")->nAddObject( "semester"));
        
        if(!currentSemester->cGetAttributeByName("name"))
            currentSemester->nAddAttribute( "name", Semesters[nCurrentSemester].toAscii().data());
        
        if(!currentSemester->cGetAttributeByName( "start"))
        {
            if(nCurrentSemester == 0) //IF IT'S the first semester, then generate a new start of the semester
            {
                dateToWrite.setToCurrentDate();
                if(dateToWrite.Month() >= AUGUST && dateToWrite.Month() <= DECEMBER)
                    dateToWrite.setMonth( SEPTEMBER );
                if(dateToWrite.Month() >= JANUARY && dateToWrite.Month() < FEBRUARY)
                {
                    dateToWrite.setMonth( SEPTEMBER );
                    dateToWrite.addYears( -1);
                }
                if(dateToWrite.Month() >= FEBRUARY && dateToWrite.Month() <= JULY) // not the first semester, so just set start of 
                {                                                                  // this semester one day after on day after end of semester before
                    dateToWrite.setMonth( FEBRUARY );
                    dateToWrite.setDay( 1 );
                }
            }
            else
            {
                dateToWrite.setDateString (propertiesToComplete->cGetObjectByName("time")->cGetObjectByAttributeValue("name", Semesters[nCurrentSemester-1].toAscii().data())->cGetAttributeByName( "end")->value());
                dateToWrite.addDays( 1); //START IS ONE DAY after the end of the semester before
            }
            currentSemester->nAddAttribute( "start", dateToWrite.getDateString());
        }
        
        if(!currentSemester->cGetAttributeByName( "end"))
        {
            dateToRead.setDateString(currentSemester->cGetAttributeByName( "start")->value());
            if(dateToRead.Month() >= AUGUST && dateToRead.Month() <= DECEMBER)
            {
                dateToWrite.setMonth( FEBRUARY);
                dateToWrite.addYears( 1);
            }
            if(dateToRead.Month() >= JANUARY && dateToRead.Month() < FEBRUARY)
            {
                dateToWrite.setMonth( FEBRUARY);
            }
            if(dateToRead.Month() >= FEBRUARY && dateToRead.Month() <= JULY)
                dateToWrite.setMonth( JULY);
            dateToWrite.setDay(LAST_DAY_OF_MONTH);
            currentSemester->nAddAttribute( "end", dateToWrite.getDateString());
        }
        
    }
}



QString  ksPlattformSpec::getSemesterContainigDate(xmlObject* pSemesterList, QString  timestamp)
{
    xmlObject*      currentSemester;
    cDateConverter  currentDate;
    cDateConverter  currentSemStart;
    cDateConverter  currentSemEnd;
    currentDate.setDateString( timestamp.toAscii().data());
    if(pSemesterList == NULL)
        return QString::null;
    
    for(int i = 0; i < pSemesterList->nGetObjectCounter(); i++)
    {
        currentSemester = pSemesterList->cGetObjectByIdentifier(i);
        if(currentSemester == NULL)
            return QString::null;
        if(currentSemester->szGetName() != QString("semester"))
            continue;
        if(!currentSemester->cGetAttributeByName( "start"))
            continue;
        if(!currentSemester->cGetAttributeByName( "end"))
            continue;
        if(!currentSemester->cGetAttributeByName( "name"))
            continue;
        currentSemStart.setDateString(currentSemester->cGetAttributeByName( "start")->value());
        currentSemEnd.setDateString(currentSemester->cGetAttributeByName( "end")->value());
        
        if(currentDate.isContainedIn(currentSemStart, currentSemEnd))
        {
            return QString(currentSemester->cGetAttributeByName( "name")->value());
        }
    }
            
    return QString::null;
}









