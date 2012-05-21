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


#include "convertsqltovibe.h"
#include <QtSql>

DM_DECLARE_NODE_NAME(ConvertSQLtoVIBe, UrbanSim)
ConvertSQLtoVIBe::ConvertSQLtoVIBe()
{
    this->Year = 1981;
    this->start = 0;

    this->addParameter("Start", DM::DOUBLE, &this->start);
    this->addParameter("Year", DM::INT, &this->Year);



    globals = DM::View("GLOBALS", DM::COMPONENT, DM::MODIFY);

    globals.addAttribute("Year");

    households = DM::View("HOUSEHOLD", DM::FACE, DM::MODIFY);
    households.getAttribute("urbansim_id");
    households.addAttribute("cars");
    households.addAttribute("workers");
    households.addAttribute("grid_id");
    households.addAttribute("persons");
    households.addAttribute("race_id");
    households.addAttribute("income");
    households.addAttribute("age_of_head");
    households.addAttribute("children");


    grids = DM::View("GRID", DM::FACE, DM::READ);
    grids.getAttribute("urbansim_id");



    std::vector<DM::View> data;
    data.push_back(globals);
    data.push_back(households);
    data.push_back(grids);

    this->addData("City", data);



}

void ConvertSQLtoVIBe::run() {
    city = this->getData("City");
    Logger(Standard) << "Year: "<< this->Year;

    DM::Component * global = city->getComponent(city->getUUIDsOfComponentsInView(globals)[0]);
    global->addAttribute("Year", this->Year);


    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", QUuid::createUuid().toString());
    db.setUserName("urbansim");
    db.setPassword("urbansim");
    bool ok = db.open();
    if( ok == false) {
        Logger(Error) << "Database failed";
        return;
    }

    //Set Population to 0;

    std::vector<std::string> names = city->getUUIDsOfComponentsInView(grids);
    std::vector<std::string> names_existing_Households = city->getUUIDsOfComponentsInView(households);


    // Setup the db and start using it somewhere after successfully connecting to the server.
    QString dbname = "ress_"+QString::number(this->Year);
    bool sr;

    QSqlQuery query(db);
    sr = query.exec("USE "+dbname);
    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();
        return;
    }

    //Import Stuff for Gridcells
    sr = query.exec("SELECT * FROM gridcells");
    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();
        return;
    }

    int numberOfFields = query.record().count();
    std::map<int, std::string> UUIDtoUrbanSimID_Grid;
    names = city->getUUIDsOfComponentsInView(grids);
    foreach (std::string uuid, names){
        Component * cmp = this->city->getComponent(uuid);
        UUIDtoUrbanSimID_Grid[(int)cmp->getAttribute("urbansim_id")->getDouble()]=uuid;
    }




    //Update Grid
    int index_grid_id = query.record().indexOf("grid_id");
    while (query.next()) {
        std::string urbansim_id_grid = UUIDtoUrbanSimID_Grid[index_grid_id];
        DM::Component * cmp = this->city->getComponent(urbansim_id_grid);
        for (int i = 0; i < numberOfFields; i++) {
            if (i == index_grid_id )
                continue;
            QString name = query.record().fieldName(i);
            cmp->addAttribute(name.toStdString(), query.value(i).toDouble());
        }
    }

    //Caluclate Population
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
        std::string urbansim_id_grid = UUIDtoUrbanSimID_Grid[QString::fromStdString(id).toInt()];
        Component * cmp = this->city->getComponent(urbansim_id_grid);
        cmp->addAttribute("Population",pop[QString::fromStdString(id).toInt()] );
    }

    std::map< int, std::string> UUIDtoUrbanSimID_Household;
    names = city->getUUIDsOfComponentsInView(grids);
    foreach (std::string uuid, names){
        Component * cmp = this->city->getComponent(uuid);
        UUIDtoUrbanSimID_Household[(int)cmp->getAttribute("urbansim_id")->getDouble()]=uuid;
    }

    //Set status of all households dead. The status is set alive when household still exists.
    foreach (std::string name, names_existing_Households) {
        Component * attr = city->getComponent(name);
        attr->addAttribute("exists", false);

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

        std::string houeshold_dm = UUIDtoUrbanSimID_Household[query.value(household_id).toInt()];
        Component * cmp = this->city->getComponent(houeshold_dm);

        cmp->addAttribute("exists", true);
        cmp->addAttribute("cars", query.value(cars).toDouble());
        cmp->addAttribute("workers", query.value(workers).toDouble());
        cmp->addAttribute("grid_id", query.value(GRID_ID).toDouble());
        cmp->addAttribute("persons", query.value(persons).toDouble());
        cmp->addAttribute("race_id", query.value(race_id).toDouble());
        cmp->addAttribute("income", query.value(income).toDouble());
        cmp->addAttribute("age_of_head", query.value(age_of_head).toDouble());
        cmp->addAttribute("children", query.value(children).toDouble());
    }

    Logger(Standard) << "Total Population" << pop_total;

    foreach (std::string name, names_existing_Households) {
        Component * cmp = this->city->getComponent(name);
        if (! (bool) cmp->getAttribute("exists"))
            this->city->removeComponent(cmp->getUUID());
    }

    db.close();
}
