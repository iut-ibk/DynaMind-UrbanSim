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
    this->Year = 1980;
    /*this->addParameter("UrbanEnvironment_In", VIBe2::VECTORDATA_IN, & this->UrbanEnvironment_In);
    this->addParameter("UrbanEnvironment_Out", VIBe2::VECTORDATA_OUT, & this->UrbanEnvironment_Out);*/




    this->addParameter("Year", DM::INT,  & this->Year);

    parcels = DM::View("Parcel", DM::FACE, DM::READ);
    parcels.getAttribute("urbansim_id");

    households = DM::View("Household", DM::NODE, DM::MODIFY);
    households.modifyAttribute("urbansim_id");

    buildings = DM::View("Building", DM::SUBSYSTEM, DM::MODIFY);
    buildings.modifyAttribute("urbansim_id");

    persons = DM::View("Persons", DM::SUBSYSTEM, DM::MODIFY);
    persons.modifyAttribute("urbansim_id");

    std::vector<DM::View> city_data;
    city_data.push_back(parcels);
    city_data.push_back(households);
    city_data.push_back(buildings);
    city_data.push_back(persons);

    this->addData("City", city_data);



}

void ImportSQLParcel::run() {
    this->city = this->getData("City");
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
    UrbanSim_IDs["parcels"] = "parcel_";
    UrbanSim_IDs["buildings"] = "building_";
    UrbanSim_IDs["households"] = "household_";
    UrbanSim_IDs["persons"] = "person_";

    // Setup the db and start using it somewhere after successfully connecting to the server.
    QString dbname = "ress_"+QString::number(this->Year);
    bool sr;

    QSqlQuery query(db);
    sr = query.exec("USE "+dbname);
    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();
        return;
    }

    for (std::map<std::string, DM::View>::const_iterator it = tablesToImport.begin();
         it != tablesToImport.end();
         ++it) {
        int numberOfImportedEntries = 0;
        int numberOfRemovedEntries = 0;

        //





        std::stringstream squery;
        squery<< "SELECT * FROM " << it->first;
        sr = query.exec(QString::fromStdString(squery.str()));
        if (!sr) {
            Logger(Error) << query.lastError().text().toStdString();
            return;
        }

        int numberOfFields = query.record().count();
        std::stringstream sID;
        sID << UrbanSim_IDs[it->first] << "id";

        std::map<int, std::string> UrbanSim2DynaMind;
        std::vector<std::string> names = this->city->getUUIDsOfComponentsInView(it->second);
        foreach (std::string name, names) {
            Component * cmp = this->city->getComponent(name);
            cmp->addAttribute("exists", false);
            UrbanSim2DynaMind[ (int)cmp->getAttribute(sID.str())->getDouble()] =name;
        }


        int index_grid_id = query.record().indexOf(QString::fromStdString(sID.str()));
        while (query.next()) {
            numberOfImportedEntries++;
            int id = query.value(index_grid_id).toInt();

            std::string dmID = UrbanSim2DynaMind[id];
            Component * cmp;
            if (dmID.empty()) {
                DM::View v = it->second;
                if (v.getType() == DM::COMPONENT)
                    cmp = this->city->addComponent(new Component(), v);
                if (v.getType() == DM::NODE)
                    cmp = this->city->addNode(Node(), v);
                if (v.getType() == DM::EDGE)
                    cmp = this->city->addEdge(this->city->addNode(0,0,0), this->city->addNode(0,0,0), v);
                if (v.getType() == DM::FACE) {
                    DM::Node * n1 =this->city->addNode(0,0,0);
                    std::vector<Node*> ve;
                    ve.push_back(n1);
                    cmp = this->city->addFace(ve, v);

                }
                if (v.getType() == DM::SUBSYSTEM)
                    cmp = this->city->addSubSystem(new System(), v);

                cmp->addAttribute(sID.str(), id);
            } else {
                this->city->getComponent(dmID);
            }
            for (int i = 0; i < numberOfFields; i++) {
                if (i == index_grid_id )
                    continue;
                QString name = query.record().fieldName(i);
                cmp->addAttribute(name.toStdString(), query.value(i).toDouble());


            }
            cmp->addAttribute("exists", true);
        }
        names =  this->city->getUUIDsOfComponentsInView(it->second);
        foreach (std::string name, names) {
            Component * cmp;
            cmp = this->city->getComponent(name);
            if (! (bool) cmp->getAttribute("exists")) {

                DM::View v = it->second;
                if (v.getType() == DM::COMPONENT)
                    this->city->removeComponent(name);
                if (v.getType() == DM::NODE)
                     this->city->removeNode(name);
                if (v.getType() == DM::EDGE)
                    this->city->removeEdge(name);
                if (v.getType() == DM::FACE)
                    this->city->removeFace(name);
                if (v.getType() == DM::SUBSYSTEM)
                    this->city->removeSubSystem(name);
            }
        }

        Logger(Debug) << "Imported " << it->first << " " << numberOfImportedEntries;
        Logger(Debug) << "Removed " << it->first << " " << numberOfRemovedEntries;
    }

}
