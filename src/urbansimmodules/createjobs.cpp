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

#include "createjobs.h"

DM_DECLARE_NODE_NAME(CreateJobs, UrbanSim)
CreateJobs::CreateJobs()
{
    grids = DM::View("Grid", DM::FACE, DM::READ);
    grids.getAttribute("grid_id");
    grids.getAttribute("population");

    jobs = DM::View("Jobs", DM::COMPONENT, DM::WRITE);
    jobs.addAttribute("job_id");
    jobs.addAttribute("home_based");
    jobs.addAttribute("sector_id");
    jobs.addAttribute("building_type");
    jobs.addAttribute("grid_id");
    jobs.addAttribute("grid_UUID");

    std::vector<DM::View> city_data;
    city_data.push_back(grids);
    city_data.push_back(jobs);

    this->addData("city", city_data);

}
void CreateJobs::run() {    
    this->city = this->getData("city");
    std::vector<std::string> ids = city->getUUIDsOfComponentsInView(grids);
    unsigned long JobCounter = 1;
    foreach (std::string id, ids) {
        Component * grid = this->city->getComponent(id);


        long population = (long) grid->getAttribute("population")->getDouble();
        long grid_id = (int) grid->getAttribute("grid_id")->getDouble();

        do {
            int size = rand() % 10 + 1;

            Component * job = this->city->addComponent(new Component(), jobs);
            job->addAttribute("grid_id", grid_id);
            job->addAttribute("job_id", JobCounter);
            job->addAttribute("home_based", 0 );
            job->addAttribute("sector_id", 1);
            job->addAttribute("building_type", 1);
            Attribute uuid("grid_UUID");
            uuid.setString(id);
            job->addAttribute(uuid);
            population = population-size;
            JobCounter++;
        } while (population > 0);



    }
    Logger(Debug) <<(long) JobCounter << "Jobs created";
}
