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

#ifndef EXPORTTODATABASE_H
#define EXPORTTODATABASE_H

#include <dmcompilersettings.h>
#include <dmmodule.h>
#include <dm.h>

using namespace DM;
/** @ingroup UrbanSim
  * @brief Exports Attributes to a Database
  *
  */
class DM_HELPER_DLL_EXPORT ExportToDataBase : public  DM::Module {
DM_DECLARE_NODE(ExportToDataBase)
private:
    std::string DBName;
    std::string TableName;
    std::string Identifier;
    bool DeleteExistingDB;
    std::map<std::string, std::string> Dataypes;
    std::map<std::string, std::string> Export;
    DM::System * Input;

public:
    ExportToDataBase();
    void run();
};

#endif // EXPORTTODATABASE_H
