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


#include "importsqlparcel.h"
#include <sstream>
#include <QtSql>
DM_DECLARE_NODE_NAME(ImportSQLParcel, UrbanSim)
ImportSQLParcel::ImportSQLParcel()
{
    city = DM::View("CITY", DM::FACE, DM::READ);
    city.getAttribute("Year");


    parcels = DM::View("PARCEL", DM::FACE, DM::READ);
    parcels.getAttribute("id");

    households = DM::View("HOUSEHOLD", DM::NODE, DM::MODIFY);
    households.modifyAttribute("id");

    buildings = DM::View("BUILDING", DM::FACE, DM::MODIFY);
    buildings.modifyAttribute("id");

    persons = DM::View("PERSON", DM::SUBSYSTEM, DM::MODIFY);
    persons.modifyAttribute("id");

    std::vector<DM::View> city_data;
    city_data.push_back(parcels);
    //city_data.push_back(households);
    city_data.push_back(buildings);
    //city_data.push_back(persons);
    city_data.push_back(city);

    this->addData("City", city_data);



}

void ImportSQLParcel::run() {
    this->sys = this->getData("City");


    std::vector<std::string> city_uuids = this->sys->getUUIDsOfComponentsInView(city);
    if (city_uuids.size() != 1) {
        Logger(Error) << "More than one city object";
        return;
    }


    DM::Component * cmp_city = sys->getComponent(city_uuids[0]);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", QUuid::createUuid().toString());
    db.setUserName("urbansim");
    db.setPassword("urbansim");
    bool ok = db.open();
    if( ok == false) {
        Logger(Error) << "Database failed";
        return;
    }


    std::map<std::string, DM::View> tablesToImport;
    tablesToImport["parcels"] = parcels;
    tablesToImport["buildings"] = buildings;
    tablesToImport["households"] = households;
    tablesToImport["persons"] = persons;

    std::map<std::string, std::string> UrbanSim_IDs;
    UrbanSim_IDs["parcels"] = "grid_";
    UrbanSim_IDs["buildings"] = "building_";
    UrbanSim_IDs["households"] = "household_";
    UrbanSim_IDs["persons"] = "person_";

    Logger(Debug) << "Year: "<< (int)cmp_city->getAttribute("Year")->getDouble();

    // Setup the db and start using it somewhere after successfully connecting to the server.
    QString dbname = "ress_"+QString::number((int)cmp_city->getAttribute("Year")->getDouble());
    bool sr;

    QSqlQuery query(db);
    sr = query.exec("USE "+dbname);
    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();
        return;
    }



    std::map<int, std::string> parcel_ids;
    std::map<int, std::string> building_ids;

    std::vector<std::string> parcel_uuids = this->sys->getUUIDsOfComponentsInView(parcels);
    std::vector<std::string> building_uuids = this->sys->getUUIDsOfComponentsInView(buildings);

    foreach (std::string p_uuid, parcel_uuids) {
        Component * p = sys->getComponent(p_uuid);
        parcel_ids[(int)   p->getAttribute("id")->getDouble()] = p_uuid;
    }

    foreach (std::string p_uuid, building_uuids) {
        Component * p = sys->getComponent(p_uuid);
        building_ids[(int)   p->getAttribute("id")->getDouble()] = p_uuid;
    }




    std::stringstream squery;
    squery<< "SELECT * FROM " << "buildings";
    sr = query.exec(QString::fromStdString(squery.str()));
    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();
        return;
    }
    int parcel_id_No = query.record().indexOf("parcel_id");
    int building_id_No = query.record().indexOf("building_id");
    int residentail_units_No = query.record().indexOf("residentail_units");
    int year_built_No = query.record().indexOf("year_built");

    int counter = 0;
   int create_counter = 0;
    while (query.next()) {

        counter++;
        int parcel_id =query.value(parcel_id_No).toInt();
        int building_id = query.value(building_id_No).toInt();
        if (building_ids.find(building_id) == building_ids.end()) {
            //Create new building
            DM::Node * n1 =this->sys->addNode(0,0,0);
            std::vector<Node*> ve;
            ve.push_back(n1);
            DM::Component * cmp = this->sys->addFace(ve, buildings);
            cmp->addAttribute("id", query.value(building_id).toInt());
            cmp->addAttribute("residentail_units", query.value(residentail_units_No).toInt());
            cmp->addAttribute("year_built", query.value(year_built_No).toInt());
            cmp->getAttribute("PARCEL")->setLink(parcels.getName(), parcel_ids[parcel_id]);

            DM::Component * cmp_p = this->sys->getComponent(parcel_ids[parcel_id]);
            cmp_p->getAttribute("BUILDING")->setLink(buildings.getName(), cmp->getUUID());

            create_counter++;
        }
    }
    Logger(Debug) << "Elements imported " << counter;
    Logger(Debug) << "Elements created " << create_counter;

}
