/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
 * This file is part of DynaMind
 *
 * Copyright (C) 2011-2012  Christian Urich

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "exporttodatabase.h"

#include <QtSql>
#include <sstream>

DM_DECLARE_NODE_NAME(ExportToDataBase, UrbanSim)
ExportToDataBase::ExportToDataBase()
{
    this->DBName = "urbansim";
    this->TableName = "Households";
    this->Identifier = "Household_";
    this->DeleteExistingDB = false;

    this->addParameter("DBName", DM::STRING,& this->DBName);
    this->addParameter("TableName", DM::STRING, & this->TableName);
    this->addParameter("Identifier", DM::STRING, & this->Identifier);


    this->addParameter("DeleteExistingDB", DM::BOOL, &this->DeleteExistingDB);
    this->addParameter("Export", DM::STRING_MAP, &this->Export);
    this->addParameter("Datatypes", DM::STRING_MAP, &this->Dataypes);

    std::vector<DM::View> data;
    data.push_back(  DM::View ("dummy", DM::SUBSYSTEM, DM::MODIFY) );

    this->addData("Data", data);



}

void ExportToDataBase::run() {
    Input = this->getData("Data");
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", QUuid::createUuid().toString());
    db.setHostName("127.0.0.1");
    db.setUserName("urbansim");
    db.setPassword("urbansim");
    bool ok = db.open();
    if( ok == false) {
        Logger(Error) << "Database failed";
        return;
    }

    // Setup the db and start using it somewhere after successfully connecting to the server..
    QString dbname = QString::fromStdString(this->DBName);

    QSqlQuery query(db);
    bool sr;
    sr = query.exec("USE "+dbname);
    if (sr  && this->DeleteExistingDB) {
        sr = query.exec("DROP DATABASE "+dbname);
    }

    sr = query.exec("CREATE DATABASE IF NOT EXISTS "+dbname);
    sr = query.exec("USE "+dbname);

    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << this->TableName;
    ss << " (";

    int counter = 0;
    for (std::map<std::string, std::string>::const_iterator it = this->Dataypes.begin(); it !=  this->Dataypes.end(); ++it) {
        if (counter > 0)
            ss << ",";
        ss << it->first;
        ss << " ";
        ss << it->second;
        counter++;
    }
    ss << ")";
    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));


    //get Component Type
    std::vector<DM::View> ExistingViews = Input->getViews();
    DM::View ViewID;
    foreach (DM::View v, ExistingViews) {
        if (this->Identifier.compare(v.getName()) == 0) {
            ViewID = v;
            break;
        }
    }
    if (ViewID.getName().empty()) {
        DM::Logger(DM::Error) << "Identifier doesn't exist " << this->Identifier;
        return;
    }

    std::vector<std::string> names;
    names = Input->getUUIDsOfComponentsInView(ViewID);

    stringstream insertstream;

    //query.exec("BEGIN TRANSACTION");

    insertstream << "INSERT INTO " << this->TableName << "(";
    counter = 0;
    for (std::map<std::string, std::string>::const_iterator it = this->Export.begin(); it !=  this->Export.end(); ++it) {
        if (counter > 0)
            insertstream << ",";
        insertstream << it->second;
        counter++;
    }
    insertstream  << ")" << " VALUES ";
    counter = 0;

    QString elements = "";


    elements += "(";
    for (int i = 0; i < Export.size(); i++) {
        if (i > 0)
            elements += ",";
        elements += "?";

    }
    elements += ")";

    for (int i = 0; i < names.size(); i++) {
        if (i > 0)
            insertstream << ",";
        insertstream << elements.toStdString();
/*        if (i == 5000)
            break;*/
    }
    insertstream << ";";
    int c = 0;
    query.prepare(QString::fromStdString(insertstream.str()));
    foreach(std::string name, names) {
        Component * attr;
        attr = Input->getComponent(name);
        for (std::map<std::string, std::string>::const_iterator it = this->Export.begin(); it !=  this->Export.end(); ++it) {
            stringstream insert;
            insert << ":"<< it->second;
            std::string n = it->first;
            //Atttribute to String
            std::stringstream ss;

            if (attr->getAttribute(n)->hasString()) {
                ss <<attr->getAttribute(n)->getString();
            }
            else {
                ss << (int) attr->getAttribute(n)->getDouble();
            }
            query.addBindValue(QString::fromStdString(ss.str()) );
        }

        /*if (c == 5000) {
            c = -1;
            query.exec();
            query.prepare(QString::fromStdString(insertstream.str()));
        }*/

        c++;




    }
    //query.exec();
    if ( !query.exec() )
        Logger(Error) << query.lastError().text().toStdString();
    /*if ( !query.exec("END TRANSACTION") )
        Logger(Error) << query.lastError().text().toStdString();*/

    db.close();
}
