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

#include "createbuildings.h"

DM_DECLARE_NODE_NAME(CreateBuildings, UrbanSim)
CreateBuildings::CreateBuildings()
{
    grids = DM::View("GRID", DM::FACE, DM::READ);
    grids.getAttribute("grid_id");
    grids.getAttribute("Persons");
    grids.getAttribute("residential_units");
    grids.addAttribute("residential_units");
    grids.addAttribute("year_built");

    buildings = DM::View("Building", DM::COMPONENT, DM::WRITE);
    buildings.addAttribute("building_id");
    buildings.addAttribute("residential_units");
    buildings.addAttribute("year_built");
    buildings.addAttribute("sqft");
    buildings.addAttribute("improvement_value");
    buildings.addAttribute("building_type");
    buildings.addAttribute("grid_id");

    std::vector<DM::View> city_data;
    city_data.push_back(grids);
    city_data.push_back(buildings);

    this->addData("city", city_data);

}
void CreateBuildings::run() {
    this->city = this->getData("city");
    std::vector<std::string> ids = city->getUUIDsOfComponentsInView(grids);
    unsigned long bid = 1;
    foreach (std::string id, ids) {
        Component * grid = this->city->getComponent(id);
        grid->addAttribute("year_built",1900);
        long resunits = (long) grid->getAttribute("residential_units")->getDouble();
        long grid_id = (int) grid->getAttribute("grid_id")->getDouble();

        do {
            int size = rand() % 60 + 1;

            Component * b = this->city->addComponent(new Component(), buildings);
            b->addAttribute("building_id", bid++);
            b->addAttribute("residential_units", size);
            double year_built = rand() % 30 + 1950;
            b->addAttribute("year_built", rand() % 30 + 1950);
            b->addAttribute("sqft", 0);
            b->addAttribute("improvement_value", rand() % 1000);
            b->addAttribute("building_type", 4);
            b->addAttribute("grid_id", grid_id);
            if (grid->getAttribute("year_built")->getDouble() < year_built) {
                grid->addAttribute("year_built",year_built);
            }
            resunits = resunits-size;

        } while (resunits > 0);

    }
    Logger(Debug) <<(long) bid << "Jobs created";
}
