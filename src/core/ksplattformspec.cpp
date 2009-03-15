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
#include <io/xmlparser.h>
#include "dateConverter.h"
#include <QMessageBox>

#if defined(Q_WS_WIN)
#include <windows.h>
#endif



QString ksPlattformSpec::versionAsString()
{
    return "1.0 Beta 2 Dev20080122";
}

bool ksPlattformSpec::createKsDir()
{
    QString directoryName = ".kollegstufe";
    QDir ksDir = QDir::home();
    bool    returnValue = TRUE; 
#if defined(Q_WS_WIN)

    directoryName =  "kollegstufe";
#endif
    if(!ksDir.exists(directoryName))
    {
        if(!ksDir.mkdir(directoryName))
        {
            returnValue = FALSE;
        }
    }
    if(!ksDir.cd(directoryName))
    {
        return FALSE;
    }
    /*if(!ksDir.exists("plugin-config"))
    {
        if(!ksDir.mkdir("plugin-config"))
        {
            returnValue = FALSE;
        }
    }*/
    return returnValue;
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
    pathToReturn += "kollegstufe";
#else
    pathToReturn += ".kollegstufe";
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

// database tamplates
void  ksPlattformSpec::catchKsDatabaseTemplates(QString language)
{
    QDir templateDir(QApplication::applicationDirPath());
    templateDir.cdUp();
    templateDir.cd("templates");
    ksPlattformSpec::addDatabaseTemplates(ksPlattformSpec::getKsDir(), templateDir.path(), language);
    
    // Ensure that there is at least the english example archiv
    QStringList files;
    catchFileList(&files);
    if(files.count() < 1)
    {
        ksPlattformSpec::addDatabaseTemplates(ksPlattformSpec::getKsDir(), templateDir.path(), "english");
    }
}

void  ksPlattformSpec::addDatabaseTemplates(QString targetdir, QString sourcedir, QString language)
{
    if(!createKsDir())
    {
        return;
    }
    if(!targetdir.endsWith(QDir::separator()))
    {
        targetdir += QDir::separator();
    }
    if(!sourcedir.endsWith(QDir::separator()))
    {
        sourcedir += QDir::separator();
    }
    QDir srcdir(sourcedir);
    QStringList filenames = srcdir.entryList(QStringList(language + "*.xml"));
    for(int i = 0; i < filenames.count(); i++)
    {
        QFile file(sourcedir + filenames[i]);
        file.copy(targetdir + filenames[i]);
    }
    
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
#ifdef Q_WS_WIN

    //ON WINDOWS:
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
    QString article = tr("der/die/das");
    switch(kasusOfNoun)
    {
        case kasusNominativ:
            article = tr("der/die/das");
            break;
        case kasusGenitiv:
            article = tr("des/der/des");
            break;
        case kasusDativ:
            article = tr("dem/die/dem");
            break;
        case kasusAkkusativ:
            article = tr("der/die/das");
            if(noun == "Schulaufgabe")
                article = tr("die ");
            if(noun == "Stegreifaufgabe")
                article = tr("die ");
            if(noun == "Ex")
                article = tr("die ");
            if(noun == "Mitarbeit")
                article = tr("die ");
            if(noun == "Referat")
                article = tr("das ");
            if(noun == "Abfrage")
                article = tr("die ");
            if(noun == "Arbeit")
                article = tr("die ");
            if(noun == "Unterrichtsbeitrag")
                article = tr("den ");
            if(noun == "Abfrage")
                article = tr("die ");
            if(noun == "Facharbeit")
                article = tr("die ");
            if(noun == "Kurzarbeit")
                article = tr("die ");
            if(noun == "Jahrgangsstufentest")
                article = tr("den ");
            if(noun == "Arbeit")
                article = tr("die ");
            break;
    }
    return article;
}


// average computing functions // return -1 if average is invalid

double ksPlattformSpec::computeEntireAverageOfSubject(xmlObject* subject, QString semester, xmlObject* pSemesterList)
{
    if(!subject)
    {
        return -1;
    }
    addMissingSubjectAttributes(subject);
    double averageOral = computeAverageOfSubject(subject, "oral", semester, pSemesterList);
    double averageWritten = computeAverageOfSubject(subject, "written", semester, pSemesterList);
    
    int weightingOral = subject->cGetAttributeByName("weightingOral")->nValueToInt();
    int weightingWritten = subject->cGetAttributeByName("weightingWritten")->nValueToInt();
    
    return computeEntireAverageFromPartAverages(averageOral, averageWritten, weightingOral, weightingWritten);
}

double ksPlattformSpec::computeEntireAverageFromPartAverages(double averageOral, double averageWritten,
                                                                            int weightingOral, int weightingWritten)
{
    if(weightingOral + weightingWritten == 0)
    {
        return -1;
    }
    if(averageOral == -1.0)
    {
        return averageWritten;
    }
    if(averageWritten == -1.0)
    {
        return averageOral;
    }
    double average = averageOral * weightingOral;
    average += averageWritten * weightingWritten;
    average /=  weightingWritten + weightingOral;
    return average;
}

double ksPlattformSpec::computeAverageOfSubject(xmlObject* subject, QString weightingType)
{
    return computeAverageOfSubject(subject, weightingType, "all", NULL);
}


double ksPlattformSpec::computeAverageOfSubject(xmlObject* subject, QString weightingType, QString semester, xmlObject* pSemesterList)
{
    double average = -1;
    int    sumn = 0;
    int    examCounter = 0;
    xmlObject* currentExam = NULL;
    xmlObject* currentAttribute = NULL;
    if (subject == NULL)
    {
        return average;
    }
    for (int i = 0; (currentExam = subject->cGetObjectByIdentifier(i)) != NULL; i++)
    {
        addMissingExamAttributes(currentExam);
        // check if is right weighting type
        currentAttribute = currentExam->cGetObjectByAttributeValue("name", "weighting");
        if(currentAttribute->cGetAttributeByName("value")->value() != weightingType)
        {
            continue;
        }
        // check if is right semester
        if((semester != "all") && (pSemesterList != NULL))
        {
            currentAttribute = currentExam->cGetObjectByAttributeValue("name", "semester");
            QString currentSemester = currentAttribute->cGetAttributeByName("value")->value();
            if(currentSemester == "auto")
            {
                if (semester != getSemesterContainigDate(pSemesterList,
                    currentExam->cGetObjectByAttributeValue("name", "date")->cGetAttributeByName("value")->value()))
                {
                    continue;
                }
            }else
            {
                if(currentSemester != semester)
                {
                    continue;
                }
            }
        }
        
        // get points
        currentAttribute = currentExam->cGetObjectByAttributeValue("name", "mark");
        sumn += currentAttribute->cGetAttributeByName("value")->nValueToInt();
        examCounter++;
    }
    // compute average
    if(examCounter <= 0)
    {
        return average;
    }
    average  = sumn;
    average /= examCounter;
    
    return average;
}

/**  THE WAY, THE CONFIG FILE SHOULD LOOK LIKE:


<config>
 <window-settings width="705" height="355">
  <splitterMain coord="144"></splitterMain>
 </window-settings>
 <language>german</language>
 <session>
  <file>/home/thorsten/.kollegstufe/archiv_zwei.xml</file>
 </session>
 <plugins></plugins>
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
    if (!windowSettings->cGetObjectByName("examlist"))
    {
        windowSettings->nAddObject("examlist");
    }
    if (!windowSettings->cGetObjectByName("examlist")->cGetAttributeByName("viewmode"))
    {
        windowSettings->cGetObjectByName("examlist")->nAddAttribute("viewmode", "table");
    }
    
    
    if (!configFileToComplete->cGetObjectByName("language"))
    {
        configFileToComplete->nAddObject("language");
        
        QString language = getLanguageString();
        
        configFileToComplete->cGetObjectByName("language")->nSetContent(qstringToSz(language));
    }
    if (!configFileToComplete->cGetObjectByName("plugins"))
    {
        configFileToComplete->nAddObject("plugins");
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
        SubjectToComplete->nAddAttribute("name", qstringToSz(tr("new Subject")));
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
        ExamToComplete->cGetObjectByAttributeValue("name", "type")->nAddAttribute("value", qstringToSz(tr("Exam")));
    
    if(!ExamToComplete->cGetObjectByAttributeValue("name", "mark"))
    {
        newObject = ExamToComplete->cGetObjectByIdentifier(ExamToComplete->nAddObject("attribute"));
        newObject->nAddAttribute("name", "mark");
    }
    if(!ExamToComplete->cGetObjectByAttributeValue("name", "mark")->cGetAttributeByName("value"))
        ExamToComplete->cGetObjectByAttributeValue("name", "mark")->nAddAttribute("value", "0");
    
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
    if(!ExamToComplete->cGetObjectByAttributeValue("name", "comment"))
    {
        newObject = ExamToComplete->cGetObjectByIdentifier(ExamToComplete->nAddObject("attribute"));
        newObject->nAddAttribute("name", "comment");
    }
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
    if(!databaseToComplete->cGetObjectByName("plugins"))
    {
        databaseToComplete->nAddObject("plugins");
    }
    if(!databaseToComplete->cGetObjectByName("data"))
    {
        databaseToComplete->nAddObject("data");
    }
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
    // ensure that all objectes in "time" are semesters
    xmlObject* semesterList = propertiesToComplete->cGetObjectByName("time");
    xmlObject* currentSemester;
    for(int i = 0; i < semesterList->nGetObjectCounter(); i++)
    {
        currentSemester = semesterList->cGetObjectByIdentifier(i);
        addMissingSemesterAttributes(currentSemester);
    }
}

void ksPlattformSpec::addMissingSemesterAttributes(xmlObject*  semesterToComplete)
{
    if(!semesterToComplete)
    {
        return;
    }
    xmlAttribute* currentAttribute = NULL;
    if(semesterToComplete->name() != QString("semester"))
    {
        semesterToComplete->setName("semester");
    }
    currentAttribute = semesterToComplete->cGetAttributeByName("name");
    if(!currentAttribute)
    {
        semesterToComplete->nAddAttribute("name", "new semester");
    }
    currentAttribute = semesterToComplete->cGetAttributeByName("start");
    if(!currentAttribute)
    {
        semesterToComplete->nAddAttribute("start", "20070101");
    }
    currentAttribute = semesterToComplete->cGetAttributeByName("end");
    if(!currentAttribute)
    {
        semesterToComplete->nAddAttribute("end", "20070101");
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









