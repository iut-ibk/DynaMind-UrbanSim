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
#include "vectordatahelper.h"
VIBe_DECLARE_NODE_NAME(CreateJobs, UrbanSim)
CreateJobs::CreateJobs()
{
    this->IdentifierGrid = "GRID_";
    this->addParameter("Jobs", VIBe2::VECTORDATA_IN, &Jobs);
    this->addParameter("IdentifierGrid", VIBe2::STRING, &IdentifierGrid);
    this->addParameter("Jobs_out", VIBe2::VECTORDATA_OUT, &Jobs_out);
}
void CreateJobs::run() {

    *(Jobs_out) = *(Jobs);
    std::vector<std::string> ids = VectorDataHelper::findElementsWithIdentifier(this->IdentifierGrid, this->Jobs->getAttributeNames());
    unsigned long JobCounter = 1;
    foreach (std::string id, ids) {
        Attribute attr = this->Jobs->getAttributes(id);


        long population = (long) attr.getAttribute("Population");
        long grid_id = (int) attr.getAttribute("GRID_ID");

        do {
            int size = rand() % 10 + 1;

            Attribute job;
            job.setAttribute("GRID_ID", grid_id);
            job.setAttribute("job_id", JobCounter);
            job.setAttribute("home_based", 0 );
            job.setAttribute("sector_id", 1);
            job.setAttribute("building_type", 1);
            stringstream ss;
            ss << "Job_";
            ss << JobCounter;

            this->Jobs_out->setAttributes(ss.str(), job);
            population = population-size;
            JobCounter++;

        } while (population > 0);



    }
    Logger(Debug) <<(long) JobCounter << "Households created";
}
