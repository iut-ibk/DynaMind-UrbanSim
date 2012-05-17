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


#include "convertsqltovibe.h"


#include <QtSql>
DM_DECLARE_NODE_NAME(ConvertSQLtoVIBe, UrbanSim)
ConvertSQLtoVIBe::ConvertSQLtoVIBe()
{
    this->identifier = "GRID_";
    this->identifier_households = "HOUSEHOLD_";
    this->Year = 1981;
    this->start = 0;
    this->addParameter("Identifier", DM::STRING, &identifier);
    this->addParameter("IdentifierHouseholds", DM::STRING, &this->identifier_households);

    this->addParameter("Start", DM::DOUBLE, &this->start);
    this->addParameter("Year", DM::INT, &this->Year);

}

void ConvertSQLtoVIBe::run() {
    Logger(Standard) << "Year: "<< this->Year;


    Attribute y = GridData_out->getAttributes("Globals");
    Logger(Debug) << y.getAttribute("Year");
    y.setAttribute("Year", this->Year);
    y.setAttribute("Datatype", "GridData");
    GridData_out->setAttributes("Globals", y);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", QUuid::createUuid().toString());
    db.setUserName("urbansim");
    db.setPassword("urbansim");
    bool ok = db.open();
    if( ok == false) {
        Logger(Error) << "Database failed";
        return;
    }

    //Set Population to 0;


    std::vector<std::string> names = VectorDataHelper::findElementsWithIdentifier(this->identifier, this->GridData->getAttributeNames());
    std::vector<std::string> names_existing_Households = VectorDataHelper::findElementsWithIdentifier(this->identifier_households, this->GridData->getAttributeNames());


    // Setup the db and start using it somewhere after successfully connecting to the server.
    QString dbname = "ress_"+QString::number(this->Year);
    bool sr;

    QSqlQuery query(db);
    sr = query.exec("USE "+dbname);
    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();
        return;
    }

    //Get Grid Informations
    sr = query.exec("SELECT * FROM gridcells");
    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();
        return;
    }

    int numberOfFields = query.record().count();

    int index_grid_id = query.record().indexOf("grid_id");
    while (query.next()) {
        std::stringstream ss;
        ss << this->identifier <<  query.value(index_grid_id).toInt();
        Attribute attr = this->GridData_out->getAttributes(ss.str());
        for (int i = 0; i < numberOfFields; i++) {
            if (i == index_grid_id )
                continue;
            QString name = query.record().fieldName(i);
            attr.setAttribute(name.toStdString(), query.value(i).toDouble());


        }
        this->GridData_out->setAttributes(ss.str(), attr);
    }
    //Set status of all households dead. The status is set alive when household still exists.
    foreach (std::string name, names_existing_Households) {
        Attribute attr = this->GridData_out->getAttributes(name);
        attr.setAttribute("exists", false);
        this->GridData_out->setAttributes(name, attr);
    }





    sr = query.exec("SELECT grid_id, persons FROM households");
    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();
        return;
    }
    int id_grid = query.record().indexOf("grid_id");
    int id_persons = query.record().indexOf("persons");
    std::vector<double> pop = std::vector<double>(names.size()+1);
    for (int i = 0; i < pop.size(); i++) {
        pop[i] = 0;
    }
    double pop_total = 0;
    while (query.next()) {

        int id = query.value(id_grid).toInt();
        int p = query.value(id_persons).toInt();
        if (id < 0 || id >= pop.size())
            continue;
        pop[id] = pop[id]+p;
        pop_total = pop_total+p;
    }

    foreach (std::string name, names) {
        int pos = name.find("_");
        std::string id =name.substr(pos+1, name.size()-1);
        Attribute attr = this->GridData_out->getAttributes(name);
        attr.setAttribute("Population",pop[QString::fromStdString(id).toInt()] );
        this->GridData_out->setAttributes(name, attr);

    }

    sr = query.exec("SELECT * FROM households");
    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();
        return;
    }
    while (query.next()) {
        int household_id = query.record().indexOf("household_id");
        int cars = query.record().indexOf("cars");
        int workers = query.record().indexOf("workers");
        int GRID_ID = query.record().indexOf("grid_id");
        int persons = query.record().indexOf("persons");
        int race_id = query.record().indexOf("race_id");
        int income = query.record().indexOf("income");
        int age_of_head = query.record().indexOf("age_of_head");
        int children = query.record().indexOf("children");

        std::stringstream ss;
        ss << this->identifier_households << query.value(household_id).toInt();
        Attribute attr = this->GridData_out->getAttributes(ss.str());
        attr.setAttribute("exists", true);
        attr.setAttribute("cars", query.value(cars).toDouble());
        attr.setAttribute("workers", query.value(workers).toDouble());
        attr.setAttribute("GRID_ID", query.value(GRID_ID).toDouble());
        attr.setAttribute("persons", query.value(persons).toDouble());
        attr.setAttribute("race_id", query.value(race_id).toDouble());
        attr.setAttribute("income", query.value(income).toDouble());
        attr.setAttribute("age_of_head", query.value(age_of_head).toDouble());
        attr.setAttribute("children", query.value(children).toDouble());
        this->GridData_out->setAttributes(ss.str(), attr);
    }

    Logger(Standard) << "Total Population" << pop_total;

    foreach (std::string name, names_existing_Households) {
        Attribute attr = this->GridData_out->getAttributes(name);
        if (! (bool) attr.getAttribute("exists"))
            this->GridData_out->setAttributes(name, Attribute());
    }

    db.close();
}
