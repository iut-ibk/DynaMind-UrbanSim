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
#include "vectordatahelper.h"
#include <QtSql>
VIBe_DECLARE_NODE_NAME(ImportSQLParcel, UrbanSim)
ImportSQLParcel::ImportSQLParcel()
{
    this->Year = 1980;
    this->addParameter("UrbanEnvironment_In", VIBe2::VECTORDATA_IN, & this->UrbanEnvironment_In);
    this->addParameter("UrbanEnvironment_Out", VIBe2::VECTORDATA_OUT, & this->UrbanEnvironment_Out);
    this->addParameter("Year", VIBe2::INT,  & this->Year);
}

void ImportSQLParcel::run() {

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", QUuid::createUuid().toString());
    db.setUserName("urbansim");
    db.setPassword("urbansim");
    bool ok = db.open();
    if( ok == false) {
        Logger(Error) << "Database failed";
        return;
    }

    *(this->UrbanEnvironment_Out) = *(this->UrbanEnvironment_In);

    std::map<std::string, std::string> tablesToImport;
    tablesToImport["parcels"] = "parcel_";
    tablesToImport["buildings"] = "building_";
    tablesToImport["households"] = "household_";
    tablesToImport["persons"] = "person_";
    // Setup the db and start using it somewhere after successfully connecting to the server.
    QString dbname = "ress_"+QString::number(this->Year);
    bool sr;

    QSqlQuery query(db);
    sr = query.exec("USE "+dbname);
    if (!sr) {
        Logger(Error) << query.lastError().text().toStdString();
        return;
    }

    for (std::map<std::string, std::string>::const_iterator it = tablesToImport.begin();
         it != tablesToImport.end();
         ++it) {
        int numberOfImportedEntries = 0;
        int numberOfRemovedEntries = 0;

        //
        std::vector<std::string> names = VectorDataHelper::findElementsWithIdentifier(it->second, this->UrbanEnvironment_Out->getAttributeNames());
        foreach (std::string name, names) {
            Attribute attr = this->UrbanEnvironment_Out->getAttributes(name);
            attr.setAttribute("exists", false);
            this->UrbanEnvironment_Out->setAttributes(name, attr);
        }




        std::stringstream squery;
        squery<< "SELECT * FROM " << it->first;
        sr = query.exec(QString::fromStdString(squery.str()));
        if (!sr) {
            Logger(Error) << query.lastError().text().toStdString();
            return;
        }

        int numberOfFields = query.record().count();
        std::stringstream sID;
        sID << it->second << "id";
        int index_grid_id = query.record().indexOf(QString::fromStdString(sID.str()));
        while (query.next()) {
            numberOfImportedEntries++;
            std::stringstream ss;
            ss << it->second<<  query.value(index_grid_id).toInt();
            Attribute attr = this->UrbanEnvironment_Out->getAttributes(ss.str());
            for (int i = 0; i < numberOfFields; i++) {
                if (i == index_grid_id )
                    continue;
                QString name = query.record().fieldName(i);
                attr.setAttribute(name.toStdString(), query.value(i).toDouble());


            }
            attr.setAttribute("exists", true);
            this->UrbanEnvironment_Out->setAttributes(ss.str(), attr);
        }
        names = VectorDataHelper::findElementsWithIdentifier(it->second, this->UrbanEnvironment_Out->getAttributeNames());
        foreach (std::string name, names) {
            Attribute attr = this->UrbanEnvironment_Out->getAttributes(name);
            if (! (bool) attr.getAttribute("exists")) {
                this->UrbanEnvironment_Out->setAttributes(name, Attribute());
                numberOfRemovedEntries++;
            }
        }

        Logger(Debug) << "Imported " << it->first << " " << numberOfImportedEntries;
        Logger(Debug) << "Removed " << it->first << " " << numberOfRemovedEntries;
    }

}
