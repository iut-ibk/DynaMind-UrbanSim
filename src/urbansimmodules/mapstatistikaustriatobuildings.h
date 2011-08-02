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

#ifndef MAPSTATISTIKAUSTRIATOBUILDINGS_H
#define MAPSTATISTIKAUSTRIATOBUILDINGS_H

#include "module.h"
using namespace vibens;
/**
  * @ingroup UrbanSim
  * Based the Dataset A and C of the Statistik Austria
  * @author Christian Urich
  */
class VIBE_HELPER_DLL_EXPORT MapStatistikAustriaToBuildings : public Module
{
    VIBe_DECLARE_NODE(MapStatistikAustriaToBuildings)
    private:
        VectorData * buildings_in;
        VectorData * grid_in;
        VectorData * buildings_out;
        std::string IdentifierBuildings;
        std::string IdentifierGrid;
        std::string IdentifierAttribute;
        std::string toAttribute;
        bool asInteger;

public:
    MapStatistikAustriaToBuildings();
    void run();

};

#endif // MAPSTATISTIKAUSTRIATOBUILDINGS_H
