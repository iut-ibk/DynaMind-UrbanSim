/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
 * This file is part of VIBe2
 *
 * Copyright (C) 2011  Christian Urich

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
#include "vectordatahelper.h"
#include <QtSql>

VIBe_DECLARE_NODE_NAME(ExportToDataBase, UrbanSim)
ExportToDataBase::ExportToDataBase()
{
    this->DBName = "urbansim_export_test";
    this->TableName = "Households";
    this->Identifier = "Household_";
    this->DeleteExistingDB = false;

    this->addParameter("DBName", VIBe2::STRING,& this->DBName);
    this->addParameter("TableName", VIBe2::STRING, & this->TableName);
    this->addParameter("Identifier", VIBe2::STRING, & this->Identifier);

    this->addParameter("DeleteExistingDB", VIBe2::BOOL, &this->DeleteExistingDB);
    this->addParameter("Export", VIBe2::STRING_MAP, &this->Export);
    this->addParameter("Datatypes", VIBe2::STRING_MAP, &this->Dataypes);

    this->addParameter("Input", VIBe2::VECTORDATA_IN, &this->Input);
}

void ExportToDataBase::run() {
    //QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", QUuid::createUuid().toString());
    db.setHostName("127.0.0.1");
    db.setUserName("urbansim");
    db.setPassword("urbansim");
    //db.setDatabaseName("/tmp/urbansim.txt");
    bool ok = db.open();
    if( ok == false) {
        Logger(Error) << "Database failed";
        return;
    }

    // Setup the db and start using it somewhere after successfully connecting to the server..
    QString dbname = QString::fromStdString(this->DBName);
    QString tablename = QString::fromStdString(this->TableName);

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

    std::vector<std::string> names;
    names = VectorDataHelper::findElementsWithIdentifier(this->Identifier, this->Input->getAttributeNames());

    stringstream insertstream;

    query.exec("BEGIN TRANSACTION");

    insertstream << "INSERT INTO " << this->TableName << "(";
    counter = 0;
    for (std::map<std::string, std::string>::const_iterator it = this->Export.begin(); it !=  this->Export.end(); ++it) {
        if (counter > 0)
            insertstream << ",";
        insertstream << it->second;
        counter++;
    }
    insertstream  << ")" << " VALUES (";
    counter = 0;
    for (std::map<std::string, std::string>::const_iterator it = this->Export.begin(); it !=  this->Export.end(); ++it) {
        if (counter > 0)
            insertstream << ",";
        insertstream << " :" << it->second;
        counter++;
    }
    insertstream  << ")";
    Logger(Debug) << insertstream.str();
    query.prepare(QString::fromStdString(insertstream.str()));
    foreach(std::string name, names) {
        Attribute attr = this->Input->getAttributes(name);


        for (std::map<std::string, std::string>::const_iterator it = this->Export.begin(); it !=  this->Export.end(); ++it) {
            stringstream insert;
            insert << ":"<< it->second;
            std::string n = it->first;
            //double val  = attr.getAttribute(n);
            query.bindValue(QString::fromStdString(insert.str()) , QString::fromStdString(attr.getStringAttribute(n)) );
        }

        query.exec();

    }
    if ( !query.exec("COMMIT") );
       Logger(Error) << query.lastError().text().toStdString();

    db.close();
}
