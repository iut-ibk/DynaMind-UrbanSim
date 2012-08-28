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

#include "createdevelopmenthistorydb.h"
#include <QtSql>
#include <sstream>
#include <dm.h>
DM_DECLARE_NODE_NAME(CreateDevelopmentHistoryDB, UrbanSim)


CreateDevelopmentHistoryDB::CreateDevelopmentHistoryDB()
{

    std::vector<DM::View> data;
    data.push_back(  DM::View ("Building", DM::COMPONENT, DM::MODIFY) );
    this->addData("City", data);
}

void CreateDevelopmentHistoryDB::run() {

    std::vector<std::map<int, int> * > events;
    for (int i = 0; i < 30; i++) {
        events.push_back(new std::map<int, int>());
    }
    DM::System * sys = this->getData("City");
    std::vector<std::string> uuids = sys->getUUIDsOfComponentsInView(DM::View ("Building", DM::SUBSYSTEM, DM::MODIFY) );
    foreach (std::string uuid, uuids) {
        DM::Component * cmp = sys->getComponent(uuid);
        int y = (int)cmp->getAttribute("year_built")->getDouble();
        std::map<int, int> * m = events[y-1950];
        int id = cmp->getAttribute("grid_id")->getDouble();
        int r =  (*m)[id];
        (*m)[id] = r+(int)cmp->getAttribute("residential_units")->getDouble();

    }

    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QMYSQL", QUuid::createUuid().toString());
    db.setHostName("127.0.0.1");
    db.setUserName("urbansim");
    db.setPassword("urbansim");

    bool ok = db.open();
    if( ok == false) {
        Logger(Error) << "Database failed";
        return;
    }

    // Setup the db and start using it somewhere after successfully connecting to the server..
    QString dbname = QString::fromStdString("urbansim");
    QString tablename = QString::fromStdString("development_event_history");

    QSqlQuery query(db);
    bool sr;
    sr = query.exec("USE "+dbname);

    stringstream ss;

    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << tablename.toStdString();
    ss << " (";
    ss << "commercial_improvement_value" << " "  << "INT";
    ss << ", ";
    ss << "commercial_sqft" << " "  << "INT";
    ss << ", ";
    ss << "governmental_improvement_value" << " "  << "INT";
    ss << ", ";
    ss << "governmental_sqft" << " "  << "INT";
    ss << ", ";
    ss << "grid_id" << " "  << "INT";
    ss << ", ";
    ss << "industrial_improvement_value" << " "  << "INT";
    ss << ", ";
    ss << "industrial_sqft" << " "  << "INT";
    ss << ", ";
    ss << "industrial_sqft_per_job" << " "  << "INT";
    ss << ", ";
    ss << "residential_improvement_value" << " "  << "INT";
    ss << ", ";
    ss << "residential_improvement_value_change_type" << " "  << "TEXT";
    ss << ", ";
    ss << "residential_units" << " "  << "INT";
    ss << ", ";
    ss << "residential_units_change_type" << " "  << "TEXT";
    ss << ", ";
    ss << "scheduled_year" << " "  << "INT";
    ss << ", ";
    ss << "starting_development_type_id" << " "  << "INT";
    ss << ", ";
    ss << "governmental_sqft_change_type"<< " "  << "TEXT";
    ss << ", ";
    ss << "governmental_improvement_value_change_type" << " "  << "TEXT";
    ss << ", ";
    ss << "commercial_sqft_change_type" << " "  << "TEXT";
    ss << ", ";
    ss << "commercial_improvement_value_change_type" << " "  << "TEXT";
    ss << ", ";
    ss << "industrial_sqft_change_type" << " "  << "TEXT";
    ss << ", ";
    ss << "industrial_improvement_value_change_type" << " "  << "TEXT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << tablename.toStdString() << "(";
    insertstream << "commercial_improvement_value";
    insertstream << ", ";
    insertstream << "commercial_sqft";
    insertstream << ", ";
    insertstream << "governmental_improvement_value";
    insertstream << ", ";
    insertstream << "governmental_sqft";
    insertstream << ", ";
    insertstream << "grid_id" ;
    insertstream << ", ";
    insertstream << "industrial_improvement_value" ;
    insertstream << ", ";
    insertstream << "industrial_sqft" ;
    insertstream << ", ";
    insertstream << "industrial_sqft_per_job" ;
    insertstream << ", ";
    insertstream << "residential_improvement_value" ;
    insertstream << ", ";
    insertstream << "residential_improvement_value_change_type" ;
    insertstream << ", ";
    insertstream << "residential_units" ;
    insertstream << ", ";
    insertstream << "residential_units_change_type" ;
    insertstream << ", ";
    insertstream << "scheduled_year" ;
    insertstream << ", ";
    insertstream << "starting_development_type_id" ;
    insertstream << ", ";
    insertstream << "governmental_sqft_change_type" ;
    insertstream << ", ";
    insertstream << "governmental_improvement_value_change_type" ;
    insertstream << ", ";
    insertstream << "commercial_sqft_change_type" ;
    insertstream << ", ";
    insertstream << "commercial_improvement_value_change_type" ;
    insertstream << ", ";
    insertstream << "industrial_sqft_change_type" ;
    insertstream << ", ";
    insertstream << "industrial_improvement_value_change_type" ;


    insertstream  << ") " << " VALUES ";

    QString elements = "";


    elements += "(";
    for (int i = 0; i < 20; i++) {
        if (i > 0)
            elements += ",";
        elements += "?";
    }
    elements += ")";



    int c = 0;
    for (int i = 0; i < 30; i++) {
        std::map<int, int> * m = events[i];
        for (std::map<int, int>::const_iterator it = m->begin(); it != m->end(); ++it) {
            if (c > 0)
                insertstream << ",";
            insertstream << elements.toStdString();
            c++;
        }
    }

    insertstream  << ";";



    query.prepare(QString::fromStdString(insertstream.str()));
    for (int i = 0; i < 30; i++) {
        std::map<int, int> * m = events[i];
        for (std::map<int, int>::const_iterator it = m->begin(); it != m->end(); ++it) {
            query.addBindValue(0);
            query.addBindValue(0);
            query.addBindValue(0);
            query.addBindValue(0);
            query.addBindValue(it->first);
            query.addBindValue(0);
            query.addBindValue(0);
            query.addBindValue(0);
            query.addBindValue(0);
            query.addBindValue("A");
            query.addBindValue(it->second);
            query.addBindValue("A");
            query.addBindValue(i+1950);
            query.addBindValue( "1");
            query.addBindValue("A");
            query.addBindValue("A");
            query.addBindValue("A");
            query.addBindValue("A");
            query.addBindValue("A");
            query.addBindValue("A");
        }
    }

    if ( !query.exec() )
        Logger(Error) << query.lastError().text().toStdString();

    for (int i = 0; i < 30; i++) {
        delete events[i];
    }
    events.clear();
}
