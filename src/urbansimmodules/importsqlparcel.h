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

#ifndef IMPORTSQLPARCEL_H
#define IMPORTSQLPARCEL_H
#include <dmmodule.h>
#include <dm.h>
using namespace DM;
/**
  * @ingroup UrbanSim
  * @author Christian Urich
  * @brief Imports results from a parcel based UrbanSim simulation
  */
class DM_HELPER_DLL_EXPORT ImportSQLParcel : public Module
{
    DM_DECLARE_NODE(ImportSQLParcel)

    private:
        DM::System * sys;

        DM::View parcels;
        DM::View households;
        DM::View buildings;
        DM::View persons;
        DM::View city;
public:
    ImportSQLParcel();
    void run();
};

#endif // IMPORTSQLPARCEL_H
