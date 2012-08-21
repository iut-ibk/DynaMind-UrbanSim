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

#ifndef URBANSIMCONFIG_H
#define URBANSIMCONFIG_H
#include <dmcompilersettings.h>
#include "dmmodule.h"
#include <QtSql>

using namespace DM;
/** @ingroup UrbanSim
  * @author Christian Urich
  */
class DM_HELPER_DLL_EXPORT UrbanSimConfig : public  Module {

    DM_DECLARE_NODE (UrbanSimConfig)
    private:
        std::string DBName;
    std::string TableName;
    int YearsToRun;
    bool DeleteExistingDB;
    bool isGridCell;
    QSqlDatabase db;
public:
    UrbanSimConfig();
    void run();
    void create_urbansim_constants();
    void create_development_types();
    void create_base_year();
    void create_development_type_groups();
    void create_development_constraints();
    void create_development_constraints_parcel();
    void create_development_type_group_definitions();
    void create_buidling_types();
    void create_buidling_types_parcel();
    void create_buidling_building_sqft_per_job();
    void create_zones();
    void create_target_vacancies();
    void create_target_vacancies_parcel();
    void create_job_building_types();
    void create_job_building_types_parcel();
    void create_household_characteristic_for_ht();
    void create_household_location_choice_model_specification();
    void create_household_location_choice_model_coefficients();
    void create_household_location_choice_model_specification_parcel();
    void create_household_location_choice_model_coefficients_parcel();
    void create_annual_relocation_rates_for_households();
    void create_plan_types();
    void create_plan_type_groups();
    void create_plan_type_groups_definitions();
    void create_travle_data();
    void create_development_template_components();
    void create_development_templates();
    void create_land_use_types ();
    void create_demolition_cost_per_sqft();
    void create_employment_sectors();
    void create_generic_land_use_type_id();
    void create_real_estate_price_model_coefficients();
    void create_real_estate_price_model_specification();
    void create_residential_development_location_choice_model_specification();
    void create_residential_development_location_choice_model_coefficients();
    void create_land_price_model_specification();
    void create_land_price_model_coefficients();
    void create_residential_land_share_model_specification();
    void create_residential_land_share_model_coefficients();

    void create_race_names();
};

#endif // URBANSIMCONFIG_H
