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

#include "mapstatistikaustriatobuildings.h"
#include <QPolygonF>
#include "vectordatahelper.h"
#include <map>
VIBe_DECLARE_NODE_NAME(MapStatistikAustriaToBuildings, UrbanSim)
MapStatistikAustriaToBuildings::MapStatistikAustriaToBuildings()
{
    this->IdentifierBuildings = "BUILDING_";
    this->IdentifierGrid = "GRID_";
    this->IdentifierAttribute = "";
    this->toAttribute = "";
    this->asInteger = true;
    this->addParameter("IdentifierBuildings", VIBe2::STRING, &this->IdentifierBuildings);
    this->addParameter("IdentifierGrid", VIBe2::STRING, &this->IdentifierGrid);
    this->addParameter("Attribute", VIBe2::STRING, &this->IdentifierAttribute);
    this->addParameter("toAttribute", VIBe2::STRING, &this->toAttribute);
    this->addParameter("asInteger", VIBe2::BOOL, &this->asInteger);
    this->addParameter("BuildingsIn", VIBe2::VECTORDATA_IN, &this->buildings_in);
    this->addParameter("GRID", VIBe2::VECTORDATA_IN, &this->grid_in);
    this->addParameter("BuildingsOut", VIBe2::VECTORDATA_OUT, &this->buildings_out);
}

void MapStatistikAustriaToBuildings::run() {

    *(buildings_out) = *(buildings_in);

    //CreateVectorToFindData

    std::vector<QPointF> building_centeroids;
    std::vector<int> building_id;
    std::vector<std::string> buildings_names = VectorDataHelper::findElementsWithIdentifier(this->IdentifierBuildings, buildings_in->getAttributeNames());
    std::stringstream buildings_id_string;
    buildings_id_string << this->IdentifierBuildings << "ID";

    foreach (std::string name, buildings_names) {
        Attribute attr;
        attr = this->buildings_in->getAttributes(name);
        building_centeroids.push_back(QPointF(attr.getAttribute("centroid_x"), attr.getAttribute("centroid_y")));
        building_id.push_back(attr.getAttribute(buildings_id_string.str()));

    }

    std::vector<std::string> grid_names = VectorDataHelper::findElementsWithIdentifier(this->IdentifierGrid, grid_in->getAttributeNames());

    foreach (std::string name, grid_names) {


        std::vector<Face> faces = this->grid_in->getFaces(name);
        std::vector<Point> points = this->grid_in->getPoints(name);
        QPolygonF poly;
        foreach (Face f, faces) {
            std::vector<long> ids = f.getIDs();
            for (int i = 0; i < ids.size(); i++) {
                poly.push_back(QPointF(points[ids[i]].x, points[ids[i]].y));
            }
        }

        //Find all Elements in GridCell
        std::vector<int> buildingIdsInGridCell;

        for (int i = 0; i <building_centeroids.size(); i++ ) {

            if (poly.containsPoint(building_centeroids[i], Qt::WindingFill)) {
                buildingIdsInGridCell.push_back(building_id[i]);
            }
        }

        //Append Attributes to Buildings

        Attribute grid_attribute = this->grid_in->getAttributes(name);

        double totalValue = grid_attribute.getAttribute(this->IdentifierAttribute);

        double Val = totalValue / buildingIdsInGridCell.size();


        //Persons in Cell
        double Persons = 0;
        Persons+= grid_attribute.getAttribute("ledig");
        Persons+= grid_attribute.getAttribute("verwitwet");
        Persons+= grid_attribute.getAttribute("verheirate");


        double ResidentialUnits = grid_attribute.getAttribute("Whg01");


        double Household1 = grid_attribute.getAttribute("PHH_1Pers");
        double Household2 = grid_attribute.getAttribute("PHH_2Pers");
        double Household3 = grid_attribute.getAttribute("PHH_3Pers");

        double PersonsHouseholds = Household1+ Household2*2+Household3*3;


        std::map<std::string, double> AttributesOut;

        AttributesOut["Persons"] = Persons;
        AttributesOut["RUnits"] = ResidentialUnits;
        AttributesOut["PHouseholds"] = PersonsHouseholds;

        double areaTot = 0;
        foreach (int id, buildingIdsInGridCell) {
            std::stringstream buildingid;
            buildingid << this->IdentifierBuildings << id;
            Attribute building_attr = this->buildings_out->getAttributes(buildingid.str());
            areaTot+=building_attr.getAttribute("area");
        }


        //Create Households
        foreach (int id, buildingIdsInGridCell) {
            std::stringstream buildingid;
            buildingid << this->IdentifierBuildings << id;
            Attribute building_attr = this->buildings_out->getAttributes(buildingid.str());

            for (std::map<std::string, double>::iterator it = AttributesOut.begin(); it != AttributesOut.end(); ++it ) {

                double val = it->second/areaTot*building_attr.getAttribute("area");
                if (asInteger)
                    val = (int) val;
                building_attr.setAttribute(it->first, val );

                AttributesOut[it->first] = AttributesOut[it->first]- val;
            }
            areaTot-= building_attr.getAttribute("area");

            this->buildings_out->setAttributes(buildingid.str(), building_attr);

        }

        Logger(Debug) << "Buildings in Cell" << buildingIdsInGridCell.size();


    }


}
