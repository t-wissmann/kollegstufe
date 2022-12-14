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
#include "io/ksdebugoutput.h"

#include <QApplication>
#include "core/kollegstufeparent.h"
#include <stdio.h>


#include <QString>
#include "io/xmlloader.h"
#include "io/xmlparser.h"
#include <string.h>

int testNewXmlLoader();
int runStdKs(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    
    
    return runStdKs(argc, argv);
    //return testNewXmlLoader();
}

int testNewXmlLoader()
{
    
    xmlObject* myObject = new xmlObject;
    xmlLoader* loader = new xmlLoader;
    if(!loader->loadFileToClass("/home/thorsten/.kollegstufe/archiv_zwei.xml", myObject))
    {
        printf("Error during parsing at position %d\n", loader->parsingPosition());
    }
    //qDebug("content is: %s", myObject->szGetContent());
    PutObjectToScreen(myObject);
    //printf("strlen of %s is %d\n", "test", strlen("test"));
    delete myObject;
    delete loader;
    return 0;
}

int runStdKs(int argc, char *argv[]) // run standard kollegstufe
{
    int nResult = 0;
    QApplication app(argc, argv);
    kollegstufeParent mainWindow;
    if(mainWindow.wantsToBeShown())
    {
        mainWindow.show();
        nResult = app.exec();
    }
    return nResult;
}










