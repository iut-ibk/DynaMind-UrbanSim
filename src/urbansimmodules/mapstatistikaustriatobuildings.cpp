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

#include "mapstatistikaustriatobuildings.h"
#include <QPolygonF>
#include <map>
DM_DECLARE_NODE_NAME(MapStatistikAustriaToBuildings, UrbanSim)
MapStatistikAustriaToBuildings::MapStatistikAustriaToBuildings()
{
    this->IdentifierBuildings = "BUILDING_";
    this->IdentifierGrid = "GRID_";
    this->IdentifierAttribute = "";
    this->toAttribute = "";
    this->asInteger = true;

    this->addParameter("Attribute", DM::STRING, &this->IdentifierAttribute);
    this->addParameter("toAttribute", DM::STRING, &this->toAttribute);
    this->addParameter("asInteger", DM::BOOL, &this->asInteger);


    grids = DM::View("Grid", DM::FACE, DM::READ);
    grids.addAttribute("Buildings");
    buildings = DM::View("Building", DM::FACE, DM::READ);
    buildings.getAttribute("centroid_x");
    buildings.getAttribute("centroid_y");
    buildings.addAttribute("Persons");
    buildings.addAttribute("Grid_UUID");

    std::vector<DM::View> views;

    views.push_back(grids);
    views.push_back(buildings);


    this->addData("city", views);

}

void MapStatistikAustriaToBuildings::run() {

    city = this->getData("city");

    //CreateVectorToFindData

    std::vector<QPointF> building_centeroids;
    std::vector<std::string> building_uuid;
    std::vector<std::string> buildings_names =city->getUUIDsOfComponentsInView(buildings);


    foreach (std::string name, buildings_names) {
        Component * attr = city->getComponent(name);
        building_centeroids.push_back(QPointF(attr->getAttribute("centroid_x")->getDouble(), attr->getAttribute("centroid_y")->getDouble()));
        building_uuid.push_back(name);

    }

    std::vector<std::string> grid_names = this->city->getUUIDsOfComponentsInView(grids);

    foreach (std::string name, grid_names) {
        std::vector<std::string> points = city->getFace(name)->getNodes();
        QPolygonF poly;
        foreach (std::string p, points) {
                poly.push_back(QPointF(city->getNode(p)->getX(), city->getNode(p)->getY()));

        }

        //Find all Elements in GridCell
        std::vector<std::string> buildingIdsInGridCell;

        for (int i = 0; i <building_centeroids.size(); i++ ) {

            if (poly.containsPoint(building_centeroids[i], Qt::WindingFill)) {
                buildingIdsInGridCell.push_back(building_uuid[i]);
            }
        }

        Component * grid_attr = city->getComponent(name);


        //Persons in Cell
        double Persons = 0;
        Persons+= grid_attr->getAttribute("ledig")->getDouble();
        Persons+= grid_attr->getAttribute("verwitwet")->getDouble();
        Persons+= grid_attr->getAttribute("verheirate")->getDouble();


        double ResidentialUnits = grid_attr->getAttribute("Whg01")->getDouble();


        double Household1 = grid_attr->getAttribute("PHH_1Pers")->getDouble();
        double Household2 = grid_attr->getAttribute("PHH_2Pers")->getDouble();
        double Household3 = grid_attr->getAttribute("PHH_3Pers")->getDouble();

        double PersonsHouseholds = Household1+ Household2*2+Household3*3;


        std::map<std::string, double> AttributesOut;

        AttributesOut["Persons"] = Persons;
        AttributesOut["RUnits"] = ResidentialUnits;
        AttributesOut["PHouseholds"] = PersonsHouseholds;

        double areaTot = 0;
        foreach (std::string id, buildingIdsInGridCell) {
            areaTot+=this->city->getComponent(id)->getAttribute("area")->getDouble();
        }


        //Create Households
        foreach (std::string id, buildingIdsInGridCell) {
            Component * building_attr = this->city->getComponent(id);
            Attribute grid_uuid("Grid_UUID");
            grid_uuid.setString(grid_attr->getUUID());
            building_attr->addAttribute(grid_uuid);

            for (std::map<std::string, double>::iterator it = AttributesOut.begin(); it != AttributesOut.end(); ++it ) {
                double val = it->second/areaTot*building_attr->getAttribute("area")->getDouble();
                if (asInteger)
                    val = (int) val;
                building_attr->addAttribute(it->first, val );
                AttributesOut[it->first] = AttributesOut[it->first]- val;
            }

            areaTot-= building_attr->getAttribute("area")->getDouble();
        }
        Logger(Debug) << "Buildings in Cell" << buildingIdsInGridCell.size();

        grid_attr->addAttribute("Buildings", buildingIdsInGridCell.size());
    }


}
