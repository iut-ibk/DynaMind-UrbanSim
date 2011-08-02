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

#include "urbansimconfig.h"



VIBe_DECLARE_NODE_NAME(UrbanSimConfig, UrbanSim)
UrbanSimConfig::UrbanSimConfig()
{
    this->DBName = "urbansim_export_test";
    this->TableName = "urbansim_constants";
    this->DeleteExistingDB = false;
    this->isGridCell = true;
    this->YearsToRun = 20;
    this->addParameter("DBName", VIBe2::STRING,& this->DBName);
    this->addParameter("TableName", VIBe2::STRING, & this->TableName);
    this->addParameter("GridCell", VIBe2::BOOL, &this->isGridCell);
    this->addParameter("DeleteExistingDB", VIBe2::BOOL, &this->DeleteExistingDB);
    this->addParameter("YearToRun", VIBe2::INT, & this->YearsToRun);

}

void UrbanSimConfig::run() {

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
    QString dbname = QString::fromStdString(this->DBName);
    QString tablename = QString::fromStdString(this->TableName);

    QSqlQuery query(db);
    bool sr;
    sr = query.exec("USE "+dbname);
    if (sr  && this->DeleteExistingDB) {
        sr = query.exec("DROP DATABASE "+dbname);
    }
    for (int i = 1900; i < 2100; i++) {
        std::stringstream ss;
        ss << "ress_" << i;
        query.exec("DROP DATABASE "+ QString::fromStdString(ss.str()));
    }
    sr = query.exec("CREATE DATABASE IF NOT EXISTS "+dbname);
    sr = query.exec("USE "+dbname);

    if (isGridCell) {
        this->create_urbansim_constants();
        this->create_base_year();
        this->create_development_types();
        this->create_development_type_groups();
        this->create_development_constraints();
        this->create_development_type_group_definitions();
        this->create_buidling_types();
        this->create_zones();
        this->create_target_vacancies();
        this->create_job_building_types();
        this->create_household_characteristic_for_ht();
        this->create_household_location_choice_model_specification();
        this->create_household_location_choice_model_coefficients();
        this->create_annual_relocation_rates_for_households();
        this->create_race_names();
        this->create_residential_development_location_choice_model_coefficients();
        this->create_residential_development_location_choice_model_specification();
        this->create_plan_types();
        this->create_plan_type_groups();
        this->create_plan_type_groups_definitions();
        this->create_travle_data();
        this->create_development_templates();
        this->create_development_template_components();
        this->create_land_use_types();
        this->create_demolition_cost_per_sqft();
        this->create_employment_sectors();
        this->create_generic_land_use_type_id();
        this->create_real_estate_price_model_coefficients();
        this->create_real_estate_price_model_specification();
        this->create_buidling_building_sqft_per_job();
    }
    if (!isGridCell) {
        this->create_urbansim_constants();
        this->create_base_year();
        //this->create_development_types();
        //this->create_development_type_groups();
        this->create_development_constraints_parcel();
        //this->create_development_type_group_definitions();
        this->create_buidling_types_parcel();
        this->create_zones();
        this->create_target_vacancies_parcel();
        this->create_job_building_types_parcel();
        this->create_household_characteristic_for_ht();
        this->create_household_location_choice_model_specification_parcel();
        this->create_household_location_choice_model_coefficients_parcel();
        this->create_annual_relocation_rates_for_households();
        this->create_race_names();
        //this->create_residential_development_location_choice_model_coefficients();
        //this->create_residential_development_location_choice_model_specification();
        this->create_plan_types();
        this->create_plan_type_groups();
        this->create_plan_type_groups_definitions();
        this->create_travle_data();
        this->create_development_templates();
        this->create_development_template_components();
        this->create_land_use_types();
        this->create_demolition_cost_per_sqft();
        this->create_employment_sectors();
        this->create_generic_land_use_type_id();
        this->create_real_estate_price_model_coefficients();
        this->create_real_estate_price_model_specification();
        this->create_buidling_building_sqft_per_job();
    }
    db.close();
}
void UrbanSimConfig::create_development_type_groups() {
    std::string TableName = "development_type_groups";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "group_id" << " "  << "INT NOT NULL AUTO_INCREMENT PRIMARY KEY";
    ss << ", ";
    ss << "non_overlapping_groups" << " "  << "TEXT";
    ss << ", ";
    ss << "name" << " "  << "TEXT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "group_id";
    insertstream << ", ";
    insertstream << "non_overlapping_groups";
    insertstream << ", ";
    insertstream << "name" ;
    insertstream  << ") " << " VALUES (";

    insertstream << ":group_id";
    insertstream << ", ";
    insertstream << ":non_overlapping_groups";
    insertstream << ", ";
    insertstream << ":name" ;
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":name", "industrial");
    query.bindValue(":non_overlapping_groups", "dynamic_land_use_variables");
    sr = query.exec();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":name", "governmental");
    query.bindValue(":non_overlapping_groups",  "dynamic_land_use_variables");

    sr = query.exec();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":name", "commercial");
    query.bindValue(":non_overlapping_groups",  "dynamic_land_use_variables");
    sr = query.exec();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":name", "mixed_use");
    query.bindValue(":non_overlapping_groups",  "dynamic_land_use_variables");
    sr = query.exec();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":name", "residential");
    query.bindValue(":non_overlapping_groups",  "dynamic_land_use_variables");
    sr = query.exec();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":name", "high_density_residential");
    query.bindValue(":non_overlapping_groups",  " ");
    sr = query.exec();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":name", "developed");
    query.bindValue(":non_overlapping_groups",  "");
    sr = query.exec();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":name", "residential_vacancy_adjustment");
    query.bindValue(":non_overlapping_groups",  "developer_model_with_vacancy_adjustment");
    sr = query.exec();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":name", "non_residential_vacancy_adjustment");
    query.bindValue(":non_overlapping_groups",  "developer_model_with_vacancy_adjustment");
    sr = query.exec();


    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":name", "vacant_developable");
    query.bindValue(":non_overlapping_groups",  "dynamic_land_use_variables");
    sr = query.exec();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":name", "vacant_undevelopable");
    query.bindValue(":non_overlapping_groups",  "dynamic_land_use_variables");
    sr = query.exec();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":name", "vacant");
    query.bindValue(":non_overlapping_groups",  "developer_model_with_vacancy_adjustment");
    sr = query.exec();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":name", "developable");
    query.bindValue(":non_overlapping_groups",  "");
    sr = query.exec();


    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}

void UrbanSimConfig::create_base_year() {
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << "base_year";
    ss << " (";
    ss << "year" << " "  << "INT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << "base_year" << "(";
    insertstream << "year";


    insertstream  << ") " << " VALUES (";

    insertstream << ":year";
    insertstream <<")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":year" ,1980);


    sr = query.exec();

    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}

void UrbanSimConfig::create_urbansim_constants() {
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << this->TableName;
    ss << " (";
    ss << "cell_size" << " "  << "FLOAT";
    ss << ", ";
    ss << "units" << " "  << "TEXT";
    ss << ", ";
    ss << "walking_distance_circle_radius" << " "  << "FLOAT";
    ss << ", ";
    ss << "young_age" << " "  << "INT";
    ss << ", ";
    ss << "property_value_to_annual_cost_ratio" << " "  << "FLOAT";
    ss << ", ";
    ss << "low_income_fraction" << " "  << "FLOAT";
    ss << ", ";
    ss << "mid_income_fraction" << " "  << "FLOAT";
    ss << ", ";
    /*ss << "high_income_fraction" << " "  << "FLOAT";
    ss << ", ";*/
    ss << "near_arterial_threshold" << " "  << "FLOAT";
    ss << ", ";
    ss << "near_highway_threshold" << " "  << "FLOAT";
    ss << ", ";
    ss << "percent_coverage_threshold" << " "  << "INT";
    ss << ", ";
    ss << "recent_years" << " "  << "INT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << this->TableName << "(";
    insertstream << "cell_size";
    insertstream << ", ";
    insertstream << "units";
    insertstream << ", ";
    insertstream << "walking_distance_circle_radius" ;
    insertstream << ", ";
    insertstream << "young_age" ;
    insertstream << ", ";
    insertstream << "property_value_to_annual_cost_ratio";
    insertstream << ", ";
    insertstream << "low_income_fraction";
    insertstream << ", ";
    insertstream << "mid_income_fraction";
    insertstream << ", ";
    /*insertstream << "high_income_fraction" ;
    insertstream << ", ";*/
    insertstream << "near_arterial_threshold";
    insertstream << ", ";
    insertstream << "near_highway_threshold" ;
    insertstream << ", ";
    insertstream << "percent_coverage_threshold";
    insertstream << ", ";
    insertstream << "recent_years";


    insertstream  << ") " << " VALUES (";

    insertstream << ":cell_size";
    insertstream << ", ";
    insertstream << ":units";
    insertstream << ", ";
    insertstream << ":walking_distance_circle_radius" ;
    insertstream << ", ";
    insertstream << ":young_age" ;
    insertstream << ", ";
    insertstream << ":property_value_to_annual_cost_ratio";
    insertstream << ", ";
    insertstream << ":low_income_fraction";
    insertstream << ", ";
    insertstream << ":mid_income_fraction";
    insertstream << ", ";
    /*insertstream << ":high_income_fraction" ;
    insertstream << ", ";*/
    insertstream << ":near_arterial_threshold";
    insertstream << ", ";
    insertstream << ":near_highway_threshold" ;
    insertstream << ", ";
    insertstream << ":percent_coverage_threshold";
    insertstream << ", ";
    insertstream << ":recent_years";
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":cell_size" , (float) 200.);
    query.bindValue(":units", "meters");
    query.bindValue(":walking_distance_circle_radius", 600);
    query.bindValue(":young_age", 30);
    query.bindValue(":property_value_to_annual_cost_ratio", 10);
    query.bindValue(":low_income_fraction", 0.1);
    query.bindValue(":mid_income_fraction", 0.5);
    //query.bindValue(":high_income_fraction", 0.5);
    query.bindValue(":near_arterial_threshold", 300);
    query.bindValue(":near_highway_threshold", 300);
    query.bindValue(":percent_coverage_threshold", 50);;
    query.bindValue(":recent_years", 3);

    sr = query.exec();

    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}
void UrbanSimConfig::create_development_types() {
    std::string TableName = "development_types";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "development_type_id" << " "  << "INT NOT NULL AUTO_INCREMENT PRIMARY KEY";
    ss << ", ";
    ss << "name" << " "  << "TEXT";
    ss << ", ";
    ss << "min_units" << " "  << "INT";
    ss << ", ";
    ss << "max_units" << " "  << "INT";
    ss << ", ";
    ss << "min_sqft" << " "  << "INT";
    ss << ", ";
    ss << "max_sqft" << " "  << "INT";

    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "development_type_id";
    insertstream << ", ";
    insertstream << "name";
    insertstream << ", ";
    insertstream << "min_units" ;
    insertstream << ", ";
    insertstream << "max_units" ;
    insertstream << ", ";
    insertstream << "min_sqft";
    insertstream << ", ";
    insertstream << "max_sqft";
    insertstream  << ") " << " VALUES (";

    insertstream << ":development_type_id";
    insertstream << ", ";
    insertstream << ":name";
    insertstream << ", ";
    insertstream << ":min_units" ;
    insertstream << ", ";
    insertstream << ":max_units" ;
    insertstream << ", ";
    insertstream << ":min_sqft";
    insertstream << ", ";
    insertstream << ":max_sqft";
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":name", "R1");
    query.bindValue(":min_units", 1);
    query.bindValue(":max_units", 1);
    query.bindValue(":min_sqft", 0);
    query.bindValue(":max_sqft", 99);
    sr = query.exec();

    query.bindValue(":name", "R2");
    query.bindValue(":min_units", 2);
    query.bindValue(":max_units", 4);
    query.bindValue(":min_sqft", 0);
    query.bindValue(":max_sqft", 99);
    sr = query.exec();

    query.bindValue(":name", "R3");
    query.bindValue(":min_units", 5);
    query.bindValue(":max_units", 9);
    query.bindValue(":min_sqft", 0);
    query.bindValue(":max_sqft", 99);
    sr = query.exec();

    query.bindValue(":name", "R4");
    query.bindValue(":min_units", 10);
    query.bindValue(":max_units", 14);
    query.bindValue(":min_sqft", 0);
    query.bindValue(":max_sqft", 99);
    sr = query.exec();

    query.bindValue(":name", "R5");
    query.bindValue(":min_units", 15);
    query.bindValue(":max_units", 21);
    query.bindValue(":min_sqft", 0);
    query.bindValue(":max_sqft", 99);
    sr = query.exec();

    query.bindValue(":name", "R6");
    query.bindValue(":min_units", 22);
    query.bindValue(":max_units", 30);
    query.bindValue(":min_sqft", 0);
    query.bindValue(":max_sqft", 99);
    sr = query.exec();

    query.bindValue(":name", "R7");
    query.bindValue(":min_units", 31);
    query.bindValue(":max_units", 75);
    query.bindValue(":min_sqft", 0);
    query.bindValue(":max_sqft", 99);
    sr = query.exec();

    query.bindValue(":name", "R8");
    query.bindValue(":min_units", 76);
    query.bindValue(":max_units", 65000);
    query.bindValue(":min_sqft", 0);
    query.bindValue(":max_sqft", 99);
    sr = query.exec();



    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}
void UrbanSimConfig::create_development_constraints() {
    std::string TableName = "development_constraints";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "constraint_id" << " "  << "INT NOT NULL AUTO_INCREMENT PRIMARY KEY";
    ss << ", ";
    ss << "min_units" << " "  << "INT";
    ss << ", ";
    ss << "max_units" << " "  << "INT";
    ss << ", ";
    ss << "min_commercial_sqft" << " "  << "INT";
    ss << ", ";
    ss << "max_commercial_sqft" << " "  << "INT";
    ss << ", ";
    ss << "min_industrial_sqft" << " "  << "INT";
    ss << ", ";
    ss << "max_industrial_sqft" << " "  << "INT";
    ss << ", ";
    ss << "plan_type_id" << " "  << "INT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "min_units";
    insertstream << ", ";
    insertstream << "max_units" ;
    insertstream << ", ";
    insertstream << "min_commercial_sqft" ;
    insertstream << ", ";
    insertstream << "max_commercial_sqft";
    insertstream << ", ";
    insertstream << "min_industrial_sqft";
    insertstream << ", ";
    insertstream << "max_industrial_sqft" ;
    insertstream << ", ";
    insertstream << "plan_type_id";
    insertstream  << ") " << " VALUES (";

    insertstream << ":min_units";
    insertstream << ", ";
    insertstream << ":max_units";
    insertstream << ", ";
    insertstream << ":min_commercial_sqft" ;
    insertstream << ", ";
    insertstream << ":max_commercial_sqft" ;
    insertstream << ", ";
    insertstream << ":min_industrial_sqft";
    insertstream << ", ";
    insertstream << ":max_industrial_sqft";
    insertstream << ", ";
    insertstream << ":plan_type_id";
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":min_units", 0);
    query.bindValue(":max_units", 200);
    query.bindValue(":min_commercial_sqft", 0);
    query.bindValue(":max_commercial_sqft", 9999999);
    query.bindValue(":min_industrial_sqft", 0);
    query.bindValue(":max_industrial_sqft", 9999999);
    query.bindValue(":plan_type_id", 1);

    sr = query.exec();

    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}
void UrbanSimConfig::create_development_constraints_parcel() {
    std::string TableName = "development_constraints";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "constraint_id" << " "  << "INT";
    ss << ", ";
    ss << "maximum" << " "  << "INT";
    ss << ", ";
    ss << "constraint_type" << " "  << "TEXT";
    ss << ", ";
    ss << "minimum" << " "  << "INT";
    ss << ", ";
    ss << "generic_land_use_type_id" << " "  << "INT";
    ss << ", ";
    ss << "plan_type_id" << " "  << "INT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "constraint_id";
    insertstream << ", ";
    insertstream << "maximum" ;
    insertstream << ", ";
    insertstream << "constraint_type" ;
    insertstream << ", ";
    insertstream << "minimum" ;
    insertstream << ", ";
    insertstream << "generic_land_use_type_id";
    insertstream << ", ";
    insertstream << "plan_type_id";
    insertstream  << ") " << " VALUES (";

    insertstream << ":constraint_id";
    insertstream << ", ";
    insertstream << ":maximum";
    insertstream << ", ";
    insertstream << ":constraint_type" ;
    insertstream << ", ";
    insertstream << ":minimum" ;
    insertstream << ", ";
    insertstream << ":generic_land_use_type_id";
    insertstream << ", ";
    insertstream << ":plan_type_id";
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":constraint_id", 1);
    query.bindValue(":maximum", 1);
    query.bindValue(":constraint_type", "far");
    query.bindValue(":minimum", 0);
    query.bindValue(":generic_land_use_type_id", 1);
    query.bindValue(":plan_type_id", 1);

    sr = query.exec();

    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}

void UrbanSimConfig::create_development_type_group_definitions() {
    std::string TableName = "development_type_group_definitions";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "group_id" << " "  << "INT";
    ss << ", ";
    ss << "development_type_id" << " "  << "INT";

    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "group_id";
    insertstream << ", ";
    insertstream << "development_type_id";



    insertstream  << ") " << " VALUES (";

    insertstream << ":group_id";
    insertstream << ", ";
    insertstream << ":development_type_id";

    insertstream  << ")";
    Logger(Debug) << insertstream.str();



    std::vector<int> group_id;
    std::vector<int> development_type_id;



    group_id.push_back( 		5 );  development_type_id.push_back( 	1    );
    group_id.push_back( 		13  );  development_type_id.push_back(	1);
    group_id.push_back( 		7  );  development_type_id.push_back(	1);
    group_id.push_back( 	 		8  );  development_type_id.push_back(	1);
    group_id.push_back( 	 		5  );  development_type_id.push_back(	2    );
    group_id.push_back( 	 		7  );  development_type_id.push_back(	2    );
    group_id.push_back( 	 		13  );  development_type_id.push_back(	2    );
    group_id.push_back( 	 		8  );  development_type_id.push_back(	2    );
    group_id.push_back( 	 		8  );  development_type_id.push_back(	3    );
    group_id.push_back( 	 		5  );  development_type_id.push_back(	3    );
    group_id.push_back( 	 		13  );  development_type_id.push_back(	3    );
    group_id.push_back( 	 		7  );  development_type_id.push_back(	3    );
    group_id.push_back( 	 		8  );  development_type_id.push_back(	4    );
    group_id.push_back( 	 		5  );  development_type_id.push_back(	4    );
    group_id.push_back( 	 		13  );  development_type_id.push_back(	4    );
    group_id.push_back( 	 		7  );  development_type_id.push_back(	4    );
    group_id.push_back( 	 		6  );  development_type_id.push_back(	5    );
    group_id.push_back( 	 		7  );  development_type_id.push_back(	5    );
    group_id.push_back( 	 		5  );  development_type_id.push_back(	5    );
    group_id.push_back( 	 		8  );  development_type_id.push_back(	5    );
    group_id.push_back( 	 		13  );  development_type_id.push_back(	    5);
    group_id.push_back( 	 		7  );  development_type_id.push_back(	6    );
    group_id.push_back( 	 		6  );  development_type_id.push_back(	6);
    group_id.push_back( 	 		13  );  development_type_id.push_back(	6);
    group_id.push_back( 	 		5  );  development_type_id.push_back(	6);
    group_id.push_back( 	 		8  );  development_type_id.push_back(	6);
    group_id.push_back( 	 		7  );  development_type_id.push_back(	7);
    group_id.push_back( 	 		8  );  development_type_id.push_back(	7);
    group_id.push_back( 	 		6  );  development_type_id.push_back(	7);
    group_id.push_back( 	 		13  );  development_type_id.push_back(	7);
    group_id.push_back( 	 		5  );  development_type_id.push_back(	7);
    group_id.push_back( 	 		6  );  development_type_id.push_back(	8);
    group_id.push_back( 	 		13  );  development_type_id.push_back(	8);
    group_id.push_back( 	 		7  );  development_type_id.push_back(	8);
    group_id.push_back( 	 		5  );  development_type_id.push_back(	8 );
    group_id.push_back( 	 		8  );  development_type_id.push_back(	8 );

    for (int i = 0;i < group_id.size(); i++ ) {
        query.prepare( QString::fromStdString(insertstream.str()));
        query.bindValue(":group_id" ,  group_id[i]);
        query.bindValue(":development_type_id", development_type_id[i]);


        sr = query.exec();
    }

    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}

void UrbanSimConfig::create_buidling_types() {
    std::string TableName = "building_types";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "building_type_id" << " "  << "INT NOT NULL AUTO_INCREMENT PRIMARY KEY";
    ss << ", ";
    ss << "units" << " "  << "TEXT";
    ss << ", ";
    ss << "is_residential" << " "  << "INT";
    ss << ", ";
    ss << "name" << " "  << "TEXT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "units";
    insertstream << ", ";
    insertstream << "is_residential" ;
    insertstream << ", ";
    insertstream << "name" ;
    insertstream  << ") " << " VALUES (";

    insertstream << ":units";
    insertstream << ", ";
    insertstream << ":is_residential";
    insertstream << ", ";
    insertstream << ":name" ;
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":units", "commercial_sqft");
    query.bindValue(":is_residential", 0);
    query.bindValue(":name", "commercial");
    sr = query.exec();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":units", "governmental_sqft");
    query.bindValue(":is_residential", 0);
    query.bindValue(":name", "governmental");
    sr = query.exec();


    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":units", "industrial_sqft");
    query.bindValue(":is_residential", 0);
    query.bindValue(":name", "industrial");
    sr = query.exec();


    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":units", "residential_units");
    query.bindValue(":is_residential", 1);
    query.bindValue(":name", "residential");
    sr = query.exec();

    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}

void UrbanSimConfig::create_buidling_types_parcel() {
    std::string TableName = "building_types";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "building_type_id" << " "  << "INT NOT NULL AUTO_INCREMENT PRIMARY KEY";
    ss << ", ";
    ss << "unit_name" << " "  << "TEXT";
    ss << ", ";
    ss << "is_residential" << " "  << "INT";
    ss << ", ";
    ss << "building_type_name " << " "  << "TEXT";
    ss << ", ";
    ss << "generic_building_type_id " << " "  << "INT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "unit_name";
    insertstream << ", ";
    insertstream << "is_residential" ;
    insertstream << ", ";
    insertstream << "building_type_name " ;
    insertstream << ", ";
    insertstream << "generic_building_type_id" ;
    insertstream  << ") " << " VALUES (";

    insertstream << ":unit_name";
    insertstream << ", ";
    insertstream << ":is_residential";
    insertstream << ", ";
    insertstream << ":building_type_name " ;
    insertstream << ", ";
    insertstream << ":generic_building_type_id" ;
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":unit_name", "building_sqft");
    query.bindValue(":is_residential", 1);
    query.bindValue(":building_type_name ", "residential");
    query.bindValue(":generic_building_type_id ", 1);
    sr = query.exec();


    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}

void UrbanSimConfig::create_zones() {
    std::string TableName = "zones";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "zone_id" << " "  << "INT NOT NULL AUTO_INCREMENT PRIMARY KEY";
    ss << ", ";
    ss << "travel_time_to_airport" << " "  << "INT";
    ss << ", ";
    ss << "travel_tom_to_cbd" << " "  << "INT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "travel_time_to_airport";
    insertstream << ", ";
    insertstream << "travel_tom_to_cbd" ;

    insertstream  << ") " << " VALUES (";

    insertstream << ":travel_time_to_airport";
    insertstream << ", ";
    insertstream << ":travel_tom_to_cbd";

    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":travel_tom_to_cbd", 20);
    query.bindValue(":travel_time_to_airport", 20);
    sr = query.exec();


    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}
void UrbanSimConfig::create_target_vacancies() {
    std::string TableName = "target_vacancies";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "target_total_non_residential_vacancy" << " "  << "FLOAT";
    ss << ", ";
    ss << "year" << " "  << "INT";
    ss << ", ";
    ss << "target_total_residential_vacancy" << " "  << "FLOAT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "target_total_non_residential_vacancy";
    insertstream << ", ";
    insertstream << "year" ;
    insertstream << ", ";
    insertstream << "target_total_residential_vacancy" ;
    insertstream  << ") " << " VALUES (";
    insertstream << ":target_total_non_residential_vacancy";
    insertstream << ", ";
    insertstream << ":year";
    insertstream << ", ";
    insertstream << ":target_total_residential_vacancy";
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    for (int i = 1980; i < 1980+ this->YearsToRun; i++) {
        query.prepare( QString::fromStdString(insertstream.str()));
        query.bindValue(":target_total_non_residential_vacancy", 0.1);
        query.bindValue(":target_total_residential_vacancy", 0.05);
        query.bindValue(":year", i);
        sr = query.exec();
    }



    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}
void UrbanSimConfig::create_target_vacancies_parcel() {
    std::string TableName = "target_vacancies";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "target_vacancy_rate" << " "  << "FLOAT";
    ss << ", ";
    ss << "year" << " "  << "INT";
    ss << ", ";
    ss << "building_type_id" << " "  << "INT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "target_vacancy_rate";
    insertstream << ", ";
    insertstream << "year" ;
    insertstream << ", ";
    insertstream << "building_type_id" ;
    insertstream  << ") " << " VALUES (";
    insertstream << ":target_vacancy_rate";
    insertstream << ", ";
    insertstream << ":year";
    insertstream << ", ";
    insertstream << ":building_type_id";
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    for (int i = 1980; i < 1980+ this->YearsToRun; i++) {
        query.prepare( QString::fromStdString(insertstream.str()));
        query.bindValue(":target_vacancy_rate", 0.05);
        query.bindValue(":building_type_id",1);
        query.bindValue(":year", i);
        sr = query.exec();
    }



    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}

void UrbanSimConfig::create_job_building_types() {
    std::string TableName = "job_building_types";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "id" << " "  << "INT NOT NULL AUTO_INCREMENT PRIMARY KEY";
    ss << ", ";
    ss << "home_based" << " "  << "INT";
    ss << ", ";
    ss << "name" << " "  << "TEXT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "home_based" ;
    insertstream << ", ";
    insertstream << "name" ;
    insertstream  << ") " << " VALUES (";

    insertstream << ":home_based";
    insertstream << ", ";
    insertstream << ":name" ;
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":home_based", 0);
    query.bindValue(":name", "commercial");
    sr = query.exec();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":home_based", 0);
    query.bindValue(":name", "governmental");
    sr = query.exec();


    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":home_based", 0);
    query.bindValue(":name", "industrial");
    sr = query.exec();


    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":home_based", 1);
    query.bindValue(":name", "home_based");
    sr = query.exec();

    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}
void UrbanSimConfig::create_job_building_types_parcel() {
    std::string TableName = "job_building_types";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "id" << " "  << "INT NOT NULL AUTO_INCREMENT PRIMARY KEY";
    ss << ", ";
    ss << "home_based" << " "  << "INT";
    ss << ", ";
    ss << "name" << " "  << "TEXT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "home_based" ;
    insertstream << ", ";
    insertstream << "name" ;
    insertstream  << ") " << " VALUES (";

    insertstream << ":home_based";
    insertstream << ", ";
    insertstream << ":name" ;
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":home_based", 0);
    query.bindValue(":name", "non_home_based");
    sr = query.exec();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":home_based", 1);
    query.bindValue(":name", "home_based");
    sr = query.exec();


    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}
void UrbanSimConfig::create_household_characteristic_for_ht() {
    std::string TableName = "household_characteristics_for_ht";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "characteristic" << " "  << "TEXT";
    ss << ", ";
    ss << "max" << " "  << "INT";
    ss << ", ";
    ss << "min" << " "  << "INT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "characteristic" ;
    insertstream << ", ";
    insertstream << "max" ;
    insertstream << ", ";
    insertstream << "min" ;
    insertstream  << ") " << " VALUES (";

    insertstream << ":characteristic";
    insertstream << ", ";
    insertstream << ":max" ;
    insertstream << ", ";
    insertstream << ":min" ;
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":characteristic", "Income");
    query.bindValue(":max", "14999");
    query.bindValue(":min", "0");
    sr = query.exec();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":characteristic", "Workers");
    query.bindValue(":max", "1");
    query.bindValue(":min", "1");
    sr = query.exec();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":characteristic", "age_of_head");
    query.bindValue(":max", "75");
    query.bindValue(":min", "0");
    sr = query.exec();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":characteristic", "Cars");
    query.bindValue(":max", "3");
    query.bindValue(":min", "0");
    sr = query.exec();



    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}
void UrbanSimConfig::create_household_location_choice_model_specification() {
    std::string TableName = "household_location_choice_model_specification";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "sub_model_id" << " "  << "INT";
    ss << ", ";
    ss << "equation_id" << " "  << "INT";
    ss << ", ";
    ss << "coefficient_name" << " "  << "TEXT";
    ss << ", ";
    ss << "variable_name" << " "  << "TEXT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "sub_model_id" ;
    insertstream << ", ";
    insertstream << "equation_id" ;
    insertstream << ", ";
    insertstream << "coefficient_name" ;
    insertstream << ", ";
    insertstream << "variable_name" ;
    insertstream  << ") " << " VALUES (";

    insertstream << ":sub_model_id";
    insertstream << ", ";
    insertstream << ":equation_id" ;
    insertstream << ", ";
    insertstream << ":coefficient_name" ;
    insertstream << ", ";
    insertstream << ":variable_name" ;
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", -2);
    query.bindValue(":equation_id", -2);
    query.bindValue(":coefficient_name", "percent_high_income_households_within_walking_distance_if_low_income");
    query.bindValue(":variable_name", "urbansim.household_x_gridcell.percent_high_income_households_within_walking_distance_if_low_income");
    sr = query.exec();




    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}

void UrbanSimConfig::create_household_location_choice_model_specification_parcel() {
    std::string TableName = "household_location_choice_model_specification";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "sub_model_id" << " "  << "INT";
    ss << ", ";
    ss << "fixed_value" << " "  << "INT";
    ss << ", ";
    ss << "equation_id" << " "  << "INT";
    ss << ", ";
    ss << "coefficient_name" << " "  << "TEXT";
    ss << ", ";
    ss << "variable_name" << " "  << "TEXT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "sub_model_id" ;
    insertstream << ", ";
    insertstream << "equation_id" ;
    insertstream << ", ";
    insertstream << "fixed_value" ;
    insertstream << ", ";
    insertstream << "coefficient_name" ;
    insertstream << ", ";
    insertstream << "variable_name" ;
    insertstream  << ") " << " VALUES (";

    insertstream << ":sub_model_id";
    insertstream << ", ";
    insertstream << ":equation_id" ;
    insertstream << ", ";
    insertstream << ":fixed_value" ;
    insertstream << ", ";
    insertstream << ":coefficient_name" ;
    insertstream << ", ";
    insertstream << ":variable_name" ;
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", -2);
    query.bindValue(":equation_id", -2);
    query.bindValue(":fixed_value", 0);
    query.bindValue(":coefficient_name", "is_low_income_x_is_residential");
    query.bindValue(":variable_name", "is_low_income_x_is_residential = urbansim.household.is_low_income * urbansim.building.is_residential");
    sr = query.exec();

    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", -2);
    query.bindValue(":equation_id", -2);
    query.bindValue(":fixed_value", 1);
    query.bindValue(":coefficient_name", "bias");
    query.bindValue(":variable_name", "urbansim_parcel.household_x_building.ln_sampling_probability_for_bias_correction_mnl_vacant_residential_units");
    sr = query.exec();

    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", -2);
    query.bindValue(":equation_id", -2);
    query.bindValue(":fixed_value", 0);
    query.bindValue(":coefficient_name", "ln_parcel_sf_per_unit");
    query.bindValue(":variable_name", "ln_parcel_sf_per_unit = ln(urbansim_parcel.building.parcel_sqft_per_unit)");
    sr = query.exec();

    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", -2);
    query.bindValue(":equation_id", -2);
    query.bindValue(":fixed_value", 0);
    query.bindValue(":coefficient_name", "lnpopden");
    query.bindValue(":variable_name", "lnpopden=(ln(building.disaggregate(urbansim_parcel.zone.population_per_acre))).astype(float32)");
    sr = query.exec();

    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", -2);
    query.bindValue(":equation_id", -2);
    query.bindValue(":fixed_value", 0);
    query.bindValue(":coefficient_name", "lnempden");
    query.bindValue(":variable_name", "lnempden=(ln(building.disaggregate(urbansim_parcel.zone.number_of_jobs_per_acre))).astype(float32)");
    sr = query.exec();

    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", -2);
    query.bindValue(":equation_id", -2);
    query.bindValue(":fixed_value", 0);
    query.bindValue(":coefficient_name", "ln_income_x_ln_avg_value_per_unit");
    query.bindValue(":variable_name", "ln_income_x_ln_avg_value_per_unit = ln(household.income) * ln(urbansim_parcel.building.unit_price)");
    sr = query.exec();

    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }


}
void UrbanSimConfig::create_household_location_choice_model_coefficients() {
    std::string TableName = "household_location_choice_model_coefficients";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "sub_model_id" << " "  << "INT";
    ss << ", ";
    ss << "t_statistic" << " "  << "FLOAT";
    ss << ", ";
    ss << "estimate" << " "  << "FLOAT";
    ss << ", ";
    ss << "coefficient_name" << " "  << "TEXT";
    ss << ", ";
    ss << "standard_error" << " "  << "FLOAT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "sub_model_id" ;
    insertstream << ", ";
    insertstream << "t_statistic" ;
    insertstream << ", ";
    insertstream << "coefficient_name" ;
    insertstream << ", ";
    insertstream << "estimate" ;
    insertstream << ", ";
    insertstream << "standard_error" ;
    insertstream  << ") " << " VALUES (";

    insertstream << ":sub_model_id";
    insertstream << ", ";
    insertstream << ":t_statistic" ;
    insertstream << ", ";
    insertstream << ":coefficient_name" ;
    insertstream << ", ";
    insertstream << ":estimate" ;
    insertstream << ", ";
    insertstream << ":standard_error" ;
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", -2);
    query.bindValue(":t_statistic", -8.21031);
    query.bindValue(":coefficient_name", "percent_high_income_households_within_walking_distance_if_low_income");
    query.bindValue(":estimate", -0.0316443);
    query.bindValue(":standard_error", 0.00385421);
    sr = query.exec();
    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }


}

void UrbanSimConfig::create_household_location_choice_model_coefficients_parcel() {
    std::string TableName = "household_location_choice_model_coefficients";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "sub_model_id" << " "  << "INT";
    ss << ", ";
    ss << "t_statistic" << " "  << "FLOAT";
    ss << ", ";
    ss << "estimate" << " "  << "FLOAT";
    ss << ", ";
    ss << "coefficient_name" << " "  << "TEXT";
    ss << ", ";
    ss << "standard_error" << " "  << "FLOAT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "sub_model_id" ;
    insertstream << ", ";
    insertstream << "t_statistic" ;
    insertstream << ", ";
    insertstream << "coefficient_name" ;
    insertstream << ", ";
    insertstream << "estimate" ;
    insertstream << ", ";
    insertstream << "standard_error" ;
    insertstream  << ") " << " VALUES (";

    insertstream << ":sub_model_id";
    insertstream << ", ";
    insertstream << ":t_statistic" ;
    insertstream << ", ";
    insertstream << ":coefficient_name" ;
    insertstream << ", ";
    insertstream << ":estimate" ;
    insertstream << ", ";
    insertstream << ":standard_error" ;
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", -2);
    query.bindValue(":t_statistic", 16.0609);
    query.bindValue(":coefficient_name", "is_low_income_x_is_residential");
    query.bindValue(":estimate", 1.34305);
    query.bindValue(":standard_error",0.0836223);
    sr = query.exec();
    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", -2);
    query.bindValue(":t_statistic",0);
    query.bindValue(":coefficient_name", "bias");
    query.bindValue(":estimate",1);
    query.bindValue(":standard_error", 0);
    sr = query.exec();
    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", -2);
    query.bindValue(":t_statistic",89.6086);
    query.bindValue(":coefficient_name", "ln_parcel_sf_per_unit");
    query.bindValue(":estimate",0.887979);
    query.bindValue(":standard_error", 0.00990954);
    sr = query.exec();
    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", -2);
    query.bindValue(":t_statistic",50.2347);
    query.bindValue(":coefficient_name", "ln_income_x_ln_avg_value_per_unit");
    query.bindValue(":estimate",0.0699578);
    query.bindValue(":standard_error", 0.00477604);
    sr = query.exec();
    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", -2);
    query.bindValue(":t_statistic",11.5181);
    query.bindValue(":coefficient_name", "lnempden");
    query.bindValue(":estimate",0.228174);
    query.bindValue(":standard_error", 0.00990954);
    sr = query.exec();
    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", -2);
    query.bindValue(":t_statistic",22.9397);
    query.bindValue(":coefficient_name", "lnpopden");
    query.bindValue(":estimate",0.531983);
    query.bindValue(":standard_error", 0.0231905);
    sr = query.exec();
    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }

}

void UrbanSimConfig::create_annual_relocation_rates_for_households() {
    std::string TableName = "annual_relocation_rates_for_households";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "income_max" << " "  << "INT";
    ss << ", ";
    ss << "income_min" << " "  << "INT";
    ss << ", ";
    ss << "age_of_head_min" << " "  << "INT";
    ss << ", ";
    ss << "age_of_head_max" << " "  << "INT";
    ss << ", ";
    ss << "probability_of_relocating" << " "  << "FLOAT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "income_max" ;
    insertstream << ", ";
    insertstream << "income_min" ;
    insertstream << ", ";
    insertstream << "age_of_head_min" ;
    insertstream << ", ";
    insertstream << "age_of_head_max" ;
    insertstream << ", ";
    insertstream << "probability_of_relocating" ;
    insertstream  << ") " << " VALUES (";

    insertstream << ":income_max";
    insertstream << ", ";
    insertstream << ":income_min" ;
    insertstream << ", ";
    insertstream << ":age_of_head_min" ;
    insertstream << ", ";
    insertstream << ":age_of_head_max" ;
    insertstream << ", ";
    insertstream << ":probability_of_relocating" ;
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":income_max", -1);
    query.bindValue(":income_min", 100000);
    query.bindValue(":age_of_head_min", -1);
    query.bindValue(":age_of_head_max", 75);
    query.bindValue(":probability_of_relocating",0.0462312);
    sr = query.exec();




    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}
void UrbanSimConfig::create_race_names() {
    std::string TableName = "race_names";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "race_id" << " "  << "INT";
    ss << ", ";
    ss << "name" << " "  << "TEXT";
    ss << ", ";
    ss << "minority" << " "  << "INT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "race_id" ;
    insertstream << ", ";
    insertstream << "name" ;
    insertstream << ", ";
    insertstream << "minority" ;
    insertstream  << ") " << " VALUES (";

    insertstream << ":race_id";
    insertstream << ", ";
    insertstream << ":name" ;
    insertstream << ", ";
    insertstream << ":minority" ;
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":race_id", 1);
    query.bindValue(":name", "White");
    query.bindValue(":minority",0);
    sr = query.exec();




    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}

void UrbanSimConfig::create_residential_development_location_choice_model_specification() {
    std::string TableName = "residential_development_location_choice_model_specification";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "sub_model_id" << " "  << "INT";
    ss << ", ";
    ss << "equation_id" << " "  << "INT";
    ss << ", ";
    ss << "coefficient_name" << " "  << "TEXT";
    ss << ", ";
    ss << "variable_name" << " "  << "TEXT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "sub_model_id" ;
    insertstream << ", ";
    insertstream << "equation_id" ;
    insertstream << ", ";
    insertstream << "coefficient_name" ;
    insertstream << ", ";
    insertstream << "variable_name" ;
    insertstream  << ") " << " VALUES (";

    insertstream << ":sub_model_id";
    insertstream << ", ";
    insertstream << ":equation_id" ;
    insertstream << ", ";
    insertstream << ":coefficient_name" ;
    insertstream << ", ";
    insertstream << ":variable_name" ;
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", 1);
    query.bindValue(":equation_id", -2);
    query.bindValue(":coefficient_name", "PRW");
    query.bindValue(":variable_name", "urbansim.gridcell.percent_residential_within_walking_distance");
    sr = query.exec();

    /*query.bindValue(":sub_model_id", 1);
    query.bindValue(":equation_id", -2);
    query.bindValue(":coefficient_name", "SFC_0");
    query.bindValue(":variable_name", "urbansim.gridcell.has_0_commercial_sqft");
    sr = query.exec();*/

    query.bindValue(":sub_model_id", 1);
    query.bindValue(":equation_id", -2);
    query.bindValue(":coefficient_name", "BLIMP");
    query.bindValue(":variable_name", "urbansim.gridcell.ln_total_improvement_value");
    sr = query.exec();

    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}

void UrbanSimConfig::create_residential_development_location_choice_model_coefficients() {
    std::string TableName = "residential_development_location_choice_model_coefficients";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "sub_model_id" << " "  << "INT";
    ss << ", ";
    ss << "t_statistic" << " "  << "FLOAT";
    ss << ", ";
    ss << "estimate" << " "  << "FLOAT";
    ss << ", ";
    ss << "coefficient_name" << " "  << "TEXT";
    ss << ", ";
    ss << "standard_error" << " "  << "FLOAT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "sub_model_id" ;
    insertstream << ", ";
    insertstream << "t_statistic" ;
    insertstream << ", ";
    insertstream << "coefficient_name" ;
    insertstream << ", ";
    insertstream << "estimate" ;
    insertstream << ", ";
    insertstream << "standard_error" ;
    insertstream  << ") " << " VALUES (";

    insertstream << ":sub_model_id";
    insertstream << ", ";
    insertstream << ":t_statistic" ;
    insertstream << ", ";
    insertstream << ":coefficient_name" ;
    insertstream << ", ";
    insertstream << ":estimate" ;
    insertstream << ", ";
    insertstream << ":standard_error" ;
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", 1);
    query.bindValue(":t_statistic", 10.6604);
    query.bindValue(":coefficient_name", "PRW");
    query.bindValue(":estimate",0.053373);
    query.bindValue(":standard_error", 0.00500667);
    sr = query.exec();

    /*query.bindValue(":sub_model_id", 1);
    query.bindValue(":t_statistic", 8.06472);
    query.bindValue(":coefficient_name", "SFC_0");
    query.bindValue(":estimate",1.71746);
    query.bindValue(":standard_error", 0.212959);
    sr = query.exec();*/

    query.bindValue(":sub_model_id", 1);
    query.bindValue(":t_statistic",7.07503);
    query.bindValue(":coefficient_name", "BLIMP");
    query.bindValue(":estimate",0.746247);
    query.bindValue(":standard_error", 0.105476);
    sr = query.exec();


    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}

void UrbanSimConfig::create_plan_types() {
    std::string TableName = "plan_types";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "plan_type_id" << " "  << "INT";
    ss << ", ";
    ss << "name" << " "  << "TEXT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "plan_type_id" ;
    insertstream << ", ";
    insertstream << "name" ;

    insertstream  << ") " << " VALUES (";

    insertstream << ":plan_type_id";
    insertstream << ", ";
    insertstream << ":name" ;

    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":plan_type_id", 1);
    query.bindValue(":name", "Mid Density Residential");
    sr = query.exec();




    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}


void UrbanSimConfig::create_plan_type_groups() {
    std::string TableName = "plan_type_groups";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "group_id" << " "  << "INT";
    ss << ", ";
    ss << "name" << " "  << "TEXT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "group_id" ;
    insertstream << ", ";
    insertstream << "name" ;

    insertstream  << ") " << " VALUES (";

    insertstream << ":group_id";
    insertstream << ", ";
    insertstream << ":name" ;

    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":group_id", 1);
    query.bindValue(":name", "undefined");
    sr = query.exec();




    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}


void UrbanSimConfig::create_plan_type_groups_definitions() {
    std::string TableName = "plan_type_groups_definitions";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "plan_type_id" << " "  << "INT";
    ss << ", ";
    ss << "group_id" << " "  << "INT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "group_id" ;
    insertstream << ", ";
    insertstream << "plan_type_id" ;

    insertstream  << ") " << " VALUES (";

    insertstream << ":group_id";
    insertstream << ", ";
    insertstream << ":plan_type_id" ;

    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":group_id", 1);
    query.bindValue(":plan_type_id", "1");
    sr = query.exec();




    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}
void UrbanSimConfig::create_travle_data() {
    std::string TableName = "travel_data";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "from_zone_id" << " "  << "INT";
    ss << ", ";
    ss << "to_zone_id" << " "  << "INT";
    ss << ", ";
    ss << "am_single_vehicle_to_work_travel_time" << " "  << "FLOAT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "from_zone_id" ;
    insertstream << ", ";
    insertstream << "to_zone_id" ;
    insertstream << ", ";
    insertstream << "am_single_vehicle_to_work_travel_time" ;
    insertstream  << ") " << " VALUES (";

    insertstream << ":from_zone_id";
    insertstream << ", ";
    insertstream << ":to_zone_id" ;
    insertstream << ", ";
    insertstream << ":am_single_vehicle_to_work_travel_time" ;
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":from_zone_id", 1);
    query.bindValue(":to_zone_id", 1);
    query.bindValue(":am_single_vehicle_to_work_travel_time", 4.56);
    sr = query.exec();




    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}
void UrbanSimConfig::create_development_template_components() {
    std::string TableName = "development_template_components";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "component_id" << " "  << "INT";
    ss << ", ";
    ss << "template_id" << " "  << "INT";
    ss << ", ";
    ss << "building_type_id" << " "  << "INT";
    ss << ", ";
    ss << "percent_building_sqft" << " "  << "INT";
    ss << ", ";
    ss << "construction_cost_per_unit" << " "  << "INT";
    ss << ", ";
    ss << "building_sqft_per_unit" << " "  << "INT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "component_id" ;
    insertstream << ", ";
    insertstream << "template_id" ;
    insertstream << ", ";
    insertstream << "building_type_id" ;
    insertstream << ", ";
    insertstream << "percent_building_sqft" ;
    insertstream << ", ";
    insertstream << "construction_cost_per_unit" ;
    insertstream << ", ";
    insertstream << "building_sqft_per_unit" ;

    insertstream  << ") " << " VALUES (";

    insertstream << ":component_id";
    insertstream << ", ";
    insertstream << ":template_id" ;
    insertstream << ", ";
    insertstream << ":building_type_id" ;
    insertstream << ", ";
    insertstream << ":percent_building_sqft" ;
    insertstream << ", ";
    insertstream << ":construction_cost_per_unit" ;
    insertstream << ", ";
    insertstream << ":building_sqft_per_unit" ;

    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":component_id", 1);
    query.bindValue(":template_id", 1);
    query.bindValue(":building_type_id", 1);
    query.bindValue(":percent_building_sqft", 100);
    query.bindValue(":construction_cost_per_unit", 100);
    query.bindValue(":building_sqft_per_unit", 100);
    sr = query.exec();


    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}
void UrbanSimConfig::create_development_templates() {
    std::string TableName = "development_templates";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "template_id" << " "  << "INT";
    ss << ", ";
    ss << "percent_open" << " "  << "INT";
    ss << ", ";
    ss << "land_sqft_min" << " "  << "INT";
    ss << ", ";
    ss << "land_sqft_max" << " "  << "INT";
    ss << ", ";
    ss << "percent_land_overhead" << " "  << "INT";
    ss << ", ";
    ss << "density_type" << " "  << "TEXT";
    ss << ", ";
    ss << "density" << " "  << "FLOAT";
    ss << ", ";
    ss << "land_use_type_id" << " "  << "INT";
    ss << ", ";
    ss << "development_type" << " "  << "TEXT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "template_id" ;
    insertstream << ", ";
    insertstream << "percent_open" ;
    insertstream << ", ";
    insertstream << "land_sqft_min" ;
    insertstream << ", ";
    insertstream << "land_sqft_max" ;
    insertstream << ", ";
    insertstream << "density_type" ;
    insertstream << ", ";
    insertstream << "density" ;
    insertstream << ", ";
    insertstream << "land_use_type_id" ;
    insertstream << ", ";
    insertstream << "development_type" ;
    insertstream << ", ";
    insertstream << "percent_land_overhead" ;


    insertstream  << ") " << " VALUES (";

    insertstream << ":template_id";
    insertstream << ", ";
    insertstream << ":percent_open" ;
    insertstream << ", ";
    insertstream << ":land_sqft_min" ;
    insertstream << ", ";
    insertstream << ":land_sqft_max" ;
    insertstream << ", ";
    insertstream << ":density_type" ;
    insertstream << ", ";
    insertstream << ":density" ;
    insertstream << ", ";
    insertstream << ":land_use_type_id" ;
    insertstream << ", ";
    insertstream << ":development_type" ;
    insertstream << ", ";
    insertstream << ":percent_land_overhead" ;

    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":template_id", 1);
    query.bindValue(":percent_open", 1);
    query.bindValue(":land_sqft_min", 100);
    query.bindValue(":land_sqft_max", 200);
    query.bindValue(":density_type", "far");
    query.bindValue(":density", 1);
    query.bindValue(":land_use_type_id", 1);
    query.bindValue(":percent_land_overhead", 0);
    query.bindValue(":development_type", "singel family houses");
    sr = query.exec();


    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}
void UrbanSimConfig::create_land_use_types() {
    std::string TableName = "land_use_types";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "land_use_type_id" << " "  << "INT";
    ss << ", ";
    ss << "description" << " "  << "CHAR(40)";
    ss << ", ";
    ss << "land_use_name " << " "  << "CHAR(25)";
    ss << ", ";
    ss << "unit_name" << " "  << "CHAR(15)";
    ss << ", ";
    ss << "generic_land_use_type_id " << " "  << "INT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "land_use_type_id" ;
    insertstream << ", ";
    insertstream << "description" ;
    insertstream << ", ";
    insertstream << "land_use_name" ;
    insertstream << ", ";
    insertstream << "unit_name" ;
    insertstream << ", ";
    insertstream << "generic_land_use_type_id" ;


    insertstream  << ") " << " VALUES (";

    insertstream << ":land_use_type_id";
    insertstream << ", ";
    insertstream << ":description" ;
    insertstream << ", ";
    insertstream << ":land_use_name" ;
    insertstream << ", ";
    insertstream << ":unit_name" ;
    insertstream << ", ";
    insertstream << ":generic_land_use_type_id" ;

    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":land_use_type_id", 1);
    query.bindValue(":description", "residential");
    query.bindValue(":land_use_name", "residential");
    query.bindValue(":unit_name", "units");
    query.bindValue(":generic_land_use_type_id", 1);

    sr = query.exec();


    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}

void UrbanSimConfig::create_demolition_cost_per_sqft() {
    std::string TableName = "demolition_cost_per_sqft";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "building_type_id" << " "  << "INT";
    ss << ", ";
    ss << "demolition_cost_per_sqft" << " "  << "INT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "building_type_id" ;
    insertstream << ", ";
    insertstream << "demolition_cost_per_sqft" ;


    insertstream  << ") " << " VALUES (";

    insertstream << ":building_type_id";
    insertstream << ", ";
    insertstream << ":demolition_cost_per_sqft" ;

    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":building_type_id", 1);
    query.bindValue(":demolition_cost_per_sqft", 50);

    sr = query.exec();


    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}

void UrbanSimConfig::create_employment_sectors() {
    std::string TableName = "employment_sectors ";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "sector_id " << " "  << "INT";
    ss << ", ";
    ss << "name" << " "  << "CHAR(20)";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "sector_id" ;
    insertstream << ", ";
    insertstream << "name" ;
    insertstream  << ") " << " VALUES (";

    insertstream << ":sector_id";
    insertstream << ", ";
    insertstream << ":name" ;

    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sector_id", 1);
    query.bindValue(":name", "somejob");
    sr = query.exec();


    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}
void UrbanSimConfig::create_generic_land_use_type_id() {
    std::string TableName = "generic_land_use_types";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "generic_land_use_type_id" << " "  << "INT";
    ss << ", ";
    ss << "generic_description" << " "  << "CHAR(50)";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "generic_land_use_type_id" ;
    insertstream << ", ";
    insertstream << "generic_description" ;

    insertstream  << ") " << " VALUES (";

    insertstream << ":generic_land_use_type_id";
    insertstream << ", ";
    insertstream << ":generic_description" ;

    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":generic_land_use_type_id", 1);
    query.bindValue(":generic_description", "residential");
    sr = query.exec();


    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}
void UrbanSimConfig::create_real_estate_price_model_coefficients() {
    std::string TableName = "real_estate_price_model_coefficients";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "sub_model_id" << " "  << "INT";
    ss << ", ";
    ss << "t_statistic" << " "  << "INT";
    ss << ", ";
    ss << "estimate" << " "  << "INT";
    ss << ", ";
    ss << "coefficient_name" << " "  << "CHAR(50)";
    ss << ", ";
    ss << "standard_error" << " "  << "INT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "sub_model_id" ;
    insertstream << ", ";
    insertstream << "t_statistic" ;
    insertstream << ", ";
    insertstream << "estimate" ;
    insertstream << ", ";
    insertstream << "coefficient_name" ;
    insertstream << ", ";
    insertstream << "standard_error" ;
    insertstream  << ") " << " VALUES (";

    insertstream << ":sub_model_id";
    insertstream << ", ";
    insertstream << ":t_statistic" ;
    insertstream << ", ";
    insertstream << ":estimate" ;
    insertstream << ", ";
    insertstream << ":coefficient_name" ;
    insertstream << ", ";
    insertstream << ":standard_error" ;

    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", 1);
    query.bindValue(":t_statistic", 8.16253);
    query.bindValue(":estimate", 5.24773);
    query.bindValue(":coefficient_name", "constant");
    query.bindValue(":standard_error", 0.642905);
    sr = query.exec();
    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", 1);
    query.bindValue(":t_statistic", 2.57584);
    query.bindValue(":estimate", 0.0642485);
    query.bindValue(":coefficient_name", "lnemp20da");
    query.bindValue(":standard_error", 0.0249427);
    sr = query.exec();
    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", 1);
    query.bindValue(":t_statistic", 7.78067);
    query.bindValue(":estimate", 0.323458);
    query.bindValue(":coefficient_name", "lnlotsqft");
    query.bindValue(":standard_error", 0.041572);
    sr = query.exec();
    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", 1);
    query.bindValue(":t_statistic", -12.2725);
    query.bindValue(":estimate", -0.685232);
    query.bindValue(":coefficient_name", "lnsqft");
    query.bindValue(":standard_error", 0.0558347);
    sr = query.exec();
    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }


}

void UrbanSimConfig::create_real_estate_price_model_specification() {
    std::string TableName = "real_estate_price_model_specification";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "sub_model_id" << " "  << "INT";
    ss << ", ";
    ss << "equation_id" << " "  << "INT";
    ss << ", ";
    ss << "coefficient_name" << " "  << "TEXT";
    ss << ", ";
    ss << "variable_name" << " "  << "TEXT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "sub_model_id" ;
    insertstream << ", ";
    insertstream << "equation_id" ;
    insertstream << ", ";
    insertstream << "coefficient_name" ;
    insertstream << ", ";
    insertstream << "variable_name" ;
    insertstream  << ") " << " VALUES (";

    insertstream << ":sub_model_id";
    insertstream << ", ";
    insertstream << ":equation_id" ;
    insertstream << ", ";
    insertstream << ":coefficient_name" ;
    insertstream << ", ";
    insertstream << ":variable_name" ;
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", 1);
    query.bindValue(":equation_id",-2);
    query.bindValue(":coefficient_name", "constant");
    query.bindValue(":variable_name", "constant");
    sr = query.exec();
    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", 1);
    query.bindValue(":equation_id",-2);
    query.bindValue(":coefficient_name", "lnemp20da");
    query.bindValue(":variable_name", "lnemp20da=(ln(parcel.disaggregate(urbansim_parcel.zone.employment_within_20_minutes_travel_time_hbw_am_drive_alone))).astype(float32)");
    sr = query.exec();
    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", 1);
    query.bindValue(":equation_id",-2);
    query.bindValue(":coefficient_name", "lnlotsqft");
    query.bindValue(":variable_name", "lnlotsqft=(ln(parcel.parcel_sqft)).astype(float32)");
    sr = query.exec();
    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":sub_model_id", 1);
    query.bindValue(":equation_id",-2);
    query.bindValue(":coefficient_name", "lnsqft");
    query.bindValue(":variable_name", "lnsqft=(ln(urbansim_parcel.parcel.building_sqft)).astype(float32)");
    sr = query.exec();
    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}
void UrbanSimConfig::create_buidling_building_sqft_per_job() {
    std::string TableName = "building_sqft_per_job";
    QSqlQuery query(db);
    stringstream ss;
    bool sr;
    ss << "CREATE TABLE IF NOT EXISTS ";
    ss << TableName;
    ss << " (";
    ss << "zone_id" << " "  << "INT";
    ss << ", ";
    ss << "building_type_id" << " "  << "INT";
    ss << ", ";
    ss << "building_sqft_per_job " << " "  << "INT";
    ss << ")";


    Logger(Debug) << ss.str();
    sr = query.exec(QString::fromStdString(ss.str() ));

    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();

    }
    stringstream insertstream;
    insertstream << "INSERT INTO " << TableName << "(";
    insertstream << "zone_id" ;
    insertstream << ", ";
    insertstream << "building_type_id" ;
    insertstream << ", ";
    insertstream << "building_sqft_per_job" ;
    insertstream  << ") " << " VALUES (";

    insertstream << ":zone_id";
    insertstream << ", ";
    insertstream << ":building_type_id" ;
    insertstream << ", ";
    insertstream << ":building_sqft_per_job" ;
    insertstream  << ")";
    Logger(Debug) << insertstream.str();

    query.prepare( QString::fromStdString(insertstream.str()));
    query.bindValue(":zone_id", 1);
    query.bindValue(":building_type_id",1);
    query.bindValue(":building_sqft_per_job", 20);
    sr = query.exec();
    if (!sr) {
        Logger(Debug) << query.lastError().text().toStdString();
    }
}
