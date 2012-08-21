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

#include <iostream>
#include "dmnodefactory.h"
#include "dmmoduleregistry.h"
#include "createhouseholds.h"
#include "exporttodatabase.h"
#include "urbansimconfig.h"
#include "createjobs.h"
#include "convertsqltovibe.h"
#include "createdevelopmenthistorydb.h"
#include "importsqlparcel.h"
#include "removesmallelements.h"
#include "mapstatistikaustriatobuildings.h"
#include "joinadressdatatobuildings.h"
#include "createbuildings.h"
/**
  * @addtogroup UrbanSim
  * @brief The UrbanSim modules provide the interfaces to run UrbanSim simulations in the DynaMind environment
  * @author Christian Urich
  */
extern "C" void DM_HELPER_DLL_EXPORT  registerModules(ModuleRegistry *registry) {
    registry->addNodeFactory(new NodeFactory<CreateHouseHolds>());
    registry->addNodeFactory(new NodeFactory<ExportToDataBase>());
    registry->addNodeFactory(new NodeFactory<UrbanSimConfig>());
    registry->addNodeFactory(new NodeFactory<CreateJobs>());
    registry->addNodeFactory(new NodeFactory<ConvertSQLtoVIBe>());
    registry->addNodeFactory(new NodeFactory<CreateDevelopmentHistoryDB>());
    registry->addNodeFactory(new NodeFactory<ImportSQLParcel>());
    registry->addNodeFactory(new NodeFactory<RemoveSmallElements>());
    registry->addNodeFactory(new NodeFactory<MapStatistikAustriaToBuildings>());
    registry->addNodeFactory(new NodeFactory<JoinAdressDataToBuildings>());
    registry->addNodeFactory(new NodeFactory<CreateBuildings>());
}
