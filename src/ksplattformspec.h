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
#ifndef KSPLATTFORMSPEC_H
#define KSPLATTFORMSPEC_H

#include <QString>
#include <QObject>
#include <QIcon>
#include <QPixmap>

/**
	@author Thorsten Wissmann <towi89@web.de>
*/
class xmlObject;
class QStringList;
class QTranslator;


class ksPlattformSpec : public QObject {
            
    Q_OBJECT
            
public:
    ksPlattformSpec(): QObject()  {};

    ~ksPlattformSpec(){};
    static bool createKsDir(); // creates a kollegstufe-directory
                        //returns true on success, false on error
    static QString getKsDir(); // allways ends with QDir::separator()
    static bool createConfigFile(); // creates a kollegstufe-config-file
                                    //returns true on success, false on error
    
    // language - Functions
    static bool  setLanguage(QString language, QTranslator* translator);
    static QString getLanguageString();
    static QStringList getAvailibleLanguages();
    // database tamplates
    static void  catchKsDatabaseTemplates(QString language);
    static void  addDatabaseTemplates(QString targetdir, QString sourcedir, QString language);
    
    // average computing functions // return -1 if average is invalid
    static double computeEntireAverageOfSubject(xmlObject* subject, QString semester, xmlObject* pSemesterList);
    static double computeEntireAverageFromPartAverages(double averageOral, double averageWritten, int weightingOral, int weightingWritten);
    static double computeAverageOfSubject(xmlObject* subject, QString weightingType);
    static double computeAverageOfSubject(xmlObject* subject, QString weightingType, QString semester, xmlObject* pSemesterList);
    
    // icon catcher
    static QIcon  getIcon(QString name, QString extension = "png");
    static QPixmap  getIconPixmap(QString name, QString extension = "png");
    static QPixmap  getIconPixmapFromApplicationTheme(QString name, QString extension = "png");
    
    // xmlObject - Functions
    static void   addMissingCathegoryAttributes(xmlObject*  CathegoryToComplete);
    static void   addMissingExamAttributes(xmlObject*  ExamToComplete);
    static void   addMissingSubjectAttributes(xmlObject*  SubjectToComplete);
    static void   addMissingConfigAttributes(xmlObject* configFileToComplete);
    static void   addMissingDatabaseAttributes(xmlObject* databaseToComplete);
    static void   addMissingPropertiesAttributes(xmlObject* propertiesToComplete);
    static void   catchFileList(QStringList*    targetList);
    static QString getSemesterContainigDate(xmlObject* pSemesterList, QString  timestamp);
    static QString getUserName();
    static QString getNewFilename(QString prefix, QString suffix = ".xml");
    static bool    fileExists(QString filename);
    
    static QString szToUmlauts(char* string);
    static char*   qstringToSz(QString string);
    
    static QString versionAsString();
    
    enum kasus
    {
        kasusNominativ,
        kasusGenitiv,
        kasusDativ,
        kasusAkkusativ
    };
    static QString  getArticleForNoun(QString noun, kasus kasusOfNoun);
    static bool     deleteFile(QString fileToDelete);
};

#endif
