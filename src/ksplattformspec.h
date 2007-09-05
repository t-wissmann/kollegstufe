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
#ifndef KSPLATTFORMSPEC_H
#define KSPLATTFORMSPEC_H

#include <QString>

/**
	@author Thorsten Wissmann <towi89@web.de>
*/
class xmlObject;

class ksPlattformSpec{
public:
    ksPlattformSpec();

    ~ksPlattformSpec();
    static bool createKsDir(); // creates a kollegstufe-directory
                        //returns true on success, false on error
    static QString getKsDir(); // allways ends with QDir::separator()
    static bool createConfigFile(); // creates a kollegstufe-config-file
                                    //returns true on success, false on error
    
    // xmlObject - Functions
    static void   addMissingExamAttributes(xmlObject*  ExamToComplete);
    static void   addMissingConfigAttributes(xmlObject* configFileToComplete);
    enum kasus
    {
        kasusNominativ,
        kasusGenitiv,
        kasusDativ,
        kasusAkkusativ
    };
    static QString  getArticleForNoun(QString noun, kasus kasusOfNoun);
};

#endif
