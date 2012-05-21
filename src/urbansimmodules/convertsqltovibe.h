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

#ifndef CONVERTSQLTOVIBE_H
#define CONVERTSQLTOVIBE_H

#include "dmcompilersettings.h"
#include "dmmodule.h"
#include "dm.h"
using namespace DM;
/**
  * @ingroup UrbanSim
  * @brief This DynaMind module imports the UrbanSim results from the MySQlDatabase
  *
  * As input the grid is required. If a table already exists the module updates, adds and deletes entries in the existing.
  * Therefore all existing elements in the input VectorData set are marked with the attribute exists == false.
  * If the same entry exists in the imported data set exists is set to true. At the end all entries where exists == false are deleted (overwriten with an empty attribute)
  *
  * Following tables are imported:
  * - household_ID
  *     - GRID_ID
  *     - cars
  *     - workersr
  *     - persons
  *     - race_id
  *     - income
  *     - age_of_head
  *     - children
  *
  * @author Christian Urich
  */
class DM_HELPER_DLL_EXPORT ConvertSQLtoVIBe : public  Module {

    DM_DECLARE_NODE(ConvertSQLtoVIBe)

    DM::System * city;



    DM::View globals;
    DM::View households;
    DM::View grids;


    double start;
    int Year;


public:

    ConvertSQLtoVIBe();
    void run();
};

#endif // CONVERTSQLTOVIBE_H
