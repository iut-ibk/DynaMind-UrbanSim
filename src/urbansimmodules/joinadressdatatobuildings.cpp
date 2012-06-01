/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
 * This file is part of DynaMind
 *
 * Copyright (C) 2012  Christian Urich

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

#include "joinadressdatatobuildings.h"
#include <QPolygonF>
#include <map>
DM_DECLARE_NODE_NAME(JoinAdressDataToBuildings, UrbanSim)
JoinAdressDataToBuildings::JoinAdressDataToBuildings()
{
    addresses = DM::View("Address", DM::NODE, DM::READ);
    addresses.getAttribute("baujahr");
    buildings = DM::View("Building", DM::FACE, DM::READ);
    buildings.addAttribute("ConstructionYear");
    std::vector<DM::View> views;

    views.push_back(addresses);
    views.push_back(buildings);


    this->addData("city", views);
}

void JoinAdressDataToBuildings::run() {
    city = this->getData("city");

    std::vector<std::string> buildings_names = this->city->getUUIDsOfComponentsInView(buildings);
    std::vector<std::string> addresses_names =city->getUUIDsOfComponentsInView(addresses);
    std::vector<std::string> addresses_uuid;
    std::vector<QPointF> addresses_points;

    foreach (std::string name, addresses_names) {
        Node * n = city->getNode(name);
        addresses_points.push_back(QPointF(n->getX(), n->getY()));
        addresses_uuid.push_back(name);
    }

    foreach (std::string name, buildings_names) {
        std::vector<std::string> points = city->getFace(name)->getNodes();
        QPolygonF poly;
        foreach (std::string p, points) {
            poly.push_back(QPointF(city->getNode(p)->getX(), city->getNode(p)->getY()));
        }

        double year = -1;
        for (int i = 0; i <addresses_points.size(); i++ ) {
            if (poly.containsPoint(addresses_points[i], Qt::WindingFill)) {
                std::string uuid = addresses_uuid[i];
                Component * cmp = this->city->getComponent(uuid);
                year = QString::fromStdString(cmp->getAttribute("baujahr")->getString()).toDouble();
                break;
            }
        }

        Component * building = this->city->getComponent(name);
        building->addAttribute("ConstructionYear", year);
    }
}

