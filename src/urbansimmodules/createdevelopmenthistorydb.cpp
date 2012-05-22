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
DM_DECLARE_NODE_NAME(CreateDevelopmentHistoryDB, UrbanSim)


CreateDevelopmentHistoryDB::CreateDevelopmentHistoryDB()
{

}

void CreateDevelopmentHistoryDB::run() {
    QSqlDatabase db;
    //   db = QSqlDatabase::addDatabase("QSQLITE");
    db = QSqlDatabase::addDatabase("QMYSQL", QUuid::createUuid().toString());
    db.setHostName("127.0.0.1");
    db.setUserName("urbansim");
    db.setPassword("urbansim");
    // db.setDatabaseName("/tmp/urbansim.txt");

    bool ok = db.open();
    if( ok == false) {
        Logger(Error) << "Database failed";
        return;
    }

    // Setup the db and start using it somewhere after successfully connecting to the server..
    QString dbname = QString::fromStdString("urbansim_export_test");
    QString tablename = QString::fromStdString("development_event_history");

    QSqlQuery query(db);
    bool sr;
    sr = query.exec("USE "+dbname);

    /*  commercial_improvement_value 	int(11) 			Yes 	NULL 		Browse distinct values 	Change 	Drop 	Primary 	Unique 	Index 	Fulltext
    commercial_sqft 	int(11) 			Yes 	0 		Browse distinct values 	Change 	Drop 	Primary 	Unique 	Index 	Fulltext
    governmental_improvement_value 	int(11) 			Yes 	NULL 		Browse distinct values 	Change 	Drop 	Primary 	Unique 	Index 	Fulltext
    governmental_sqft 	int(11) 			Yes 	NULL 		Browse distinct values 	Change 	Drop 	Primary 	Unique 	Index 	Fulltext
    grid_id 	int(11) 			Yes 	NULL 		Browse distinct values 	Change 	Drop 	Primary 	Unique 	Index 	Fulltext
    industrial_improvement_value 	int(11) 			Yes 	NULL 		Browse distinct values 	Change 	Drop 	Primary 	Unique 	Index 	Fulltext
    industrial_sqft 	int(11) 			Yes 	NULL 		Browse distinct values 	Change 	Drop 	Primary 	Unique 	Index 	Fulltext
    industrial_sqft_per_job 	int(11) 			No 	1000 		Browse distinct values 	Change 	Drop 	Primary 	Unique 	Index 	Fulltext
    residential_improvement_value 	int(11) 			Yes 	NULL 		Browse distinct values 	Change 	Drop 	Primary 	Unique 	Index 	Fulltext
    residential_improvement_value_change_type 	text 	latin1_swedish_ci 		Yes 	NULL 		Browse distinct values 	Change 	Drop 	Primary 	Unique 	Index 	Fulltext
    residential_units 	int(11) 			No 	0 		Browse distinct values 	Change 	Drop 	Primary 	Unique 	Index 	Fulltext
    residential_units_change_type 	text 	latin1_swedish_ci 		Yes 	NULL 		Browse distinct values 	Change 	Drop 	Primary 	Unique 	Index 	Fulltext
    scheduled_year 	int(11) 			Yes 	NULL 		Browse distinct values 	Change 	Drop 	Primary 	Unique 	Index 	Fulltext
    starting_development_type_id 	int(11) 	*/

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

    insertstream  << ") " << " VALUES (";

    insertstream << ":commercial_improvement_value";
    insertstream << ", ";
    insertstream << ":commercial_sqft";
    insertstream << ", ";
    insertstream << ":governmental_improvement_value";
    insertstream << ", ";
    insertstream << ":governmental_sqft";
    insertstream << ", ";
    insertstream << ":grid_id" ;
    insertstream << ", ";
    insertstream << ":industrial_improvement_value" ;
    insertstream << ", ";
    insertstream << ":industrial_sqft" ;
    insertstream << ", ";
    insertstream << ":industrial_sqft_per_job" ;
    insertstream << ", ";
    insertstream << ":residential_improvement_value" ;
    insertstream << ", ";
    insertstream << ":residential_improvement_value_change_type" ;
    insertstream << ", ";
    insertstream << ":residential_units" ;
    insertstream << ", ";
    insertstream << ":residential_units_change_type" ;
    insertstream << ", ";
    insertstream << ":scheduled_year" ;
    insertstream << ", ";
    insertstream << ":starting_development_type_id" ;

    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare(QString::fromStdString(insertstream.str()));
    query.bindValue(":commercial_improvement_value", "0");
    query.bindValue(":commercial_sqft", "0");
    query.bindValue(":governmental_improvement_value", "0");
    query.bindValue(":governmental_sqft", "0");
    query.bindValue(":grid_id", "1");
    query.bindValue(":industrial_sqft", "0");
    query.bindValue(":industrial_sqft_per_job", "0");
    query.bindValue(":industrial_improvement_value", "0");
    query.bindValue(":residential_improvement_value", "1000");
    query.bindValue(":residential_improvement_value_change_type", "A");
    query.bindValue(":residential_units", "1");
    query.bindValue(":residential_units_change_type", "A");
    query.bindValue(":scheduled_year", "1980");
    query.bindValue(":starting_development_type_id", "8");

    sr = query.exec();


    /*query.prepare(QString::fromStdString(insertstream.str()));
    query.bindValue(":commercial_improvement_value", "0");
    query.bindValue(":commercial_sqft", "0");
    query.bindValue(":governmental_improvement_value", "0");
    query.bindValue(":governmental_sqft", "0");
    query.bindValue(":grid_id", "2");
    query.bindValue(":industrial_sqft", "0");
    query.bindValue(":industrial_sqft_per_job", "0");
    query.bindValue(":industrial_improvement_value", "0");
    query.bindValue(":residential_improvement_value", "2000");
    query.bindValue(":residential_improvement_value_change_type", "A");
    query.bindValue(":residential_units", "2");
    query.bindValue(":residential_units_change_type", "A");
    query.bindValue(":scheduled_year", "1980");
    query.bindValue(":starting_development_type_id", "8");

    sr = query.exec();*/

}
