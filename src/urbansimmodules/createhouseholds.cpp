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

#include "createhouseholds.h"

DM_DECLARE_NODE_NAME(CreateHouseHolds, UrbanSim)
CreateHouseHolds::CreateHouseHolds()
{
    this->YearsToRun = 20;

    grids = DM::View("GRID", DM::FACE, DM::READ);
    grids.getAttribute("Population");
    prognsis = DM::View("HOUSEHOLD_PROGNOSIS", DM::COMPONENT, DM::WRITE);
    prognsis.addAttribute("year");
    prognsis.addAttribute("race_id");
    prognsis.addAttribute("persons");
    prognsis.addAttribute("total_number_of_households");

    households = DM::View("HOUSEHOLD", DM::NODE, DM::WRITE);
    households.addAttribute("persons");
    households.addAttribute("race_id");
    households.addAttribute("GRID_id");
    households.addAttribute("income");
    households.addAttribute("cars");
    households.addAttribute("workers");
    households.addAttribute("age_of_head");
    households.addAttribute("children");

    std::vector<DM::View> data;
    data.push_back(grids);
    data.push_back(households);
    this->addParameter("YearsToRun", DM::INT, &YearsToRun);
    this->addData("City", data);



}
void CreateHouseHolds::run() {
    double h1[] = {
        1.0165260793,
        1.0162574081,
        1.0159973329,
        1.0157454478,
        1.0155013717,
        1.0153718064,
        1.0151390913,
        1.0149133172,
        1.0146941783,
        1.0144813862,
        1.0093325523,
        1.0092462611,
        1.009161551,
        1.009078379,
        1.0089967035,
        1.006265818,
        1.0062268019,
        1.0061882688,
        1.0061502097,
        1.0061126158
    };
    double h2[] = {
        1.0087662088,
        1.0086900301,
        1.0086151641,
        1.008541577,
        1.0084692364,
        1.0050274912,
        1.005002342,
        1.0049774431,
        1.0049527909,
        1.0049283817,
        1.0072860311,
        1.0072333289,
        1.0071813836,
        1.007130179,
        1.0070796995,
        1.0040482613,
        1.0040319389,
        1.0040157477,
        1.003999686,
        1.0039837522

    };
    double h3[] = {
        1.0052293359,
        1.0052021322,
        1.0051752101,
        1.0051485652,
        1.0051221933,
        1.0035950325,
        1.0035821545,
        1.0035693685,
        1.0035566734,
        1.0035440683,
        1.004519541,
        1.0044992066,
        1.0044790545,
        1.004459082,
        1.0044392868,
        1.0026339408,
        1.0026270214,
        1.0026201383,
        1.0026132911,
        1.0026064796

    };
    double h4[] = {
        0.9997245594,
        0.9997244835,
        0.9997244076,
        0.9997243316,
        0.9997242556,
        1.0011297748,
        1.0011284998,
        1.0011272277,
        1.0011259585,
        1.0011246922,
        1.0010005404,
        1.0009995403,
        1.0009985423,
        1.0009975462,
        1.0009965521,
        1.0006102461,
        1.0006098739,
        1.0006095022,
        1.0006091309,
        1.0006087601

    };
    double h5[] = {
        0.9959879727,
        0.9959718115,
        0.9959555196,
        0.9959390954,
        0.9959225372,
        0.9996832383,
        0.9996831379,
        0.9996830375,
        0.999682937,
        0.9996828364,
        0.9987791477,
        0.9987776554,
        0.9987761595,
        0.9987746598,
        0.9987731565,
        0.9994446084,
        0.9994442998,
        0.9994439908,
        0.9994436815,
        0.9994433719

    };
    double h6[] = {
        0.9916639898,
        0.9915939166,
        0.9915226553,
        0.9914501755,
        0.9913764456,
        0.9976653006,
        0.997659837,
        0.9976543478,
        0.9976488328,
        0.9976432918,
        0.9964488633,
        0.9964362078,
        0.9964234617,
        0.9964106242,
        0.9963976942,
        0.9974432309,
        0.9974366771,
        0.9974300896,
        0.9974234681,
        0.9974168125

    };

    city = this->getData("City");

    std::vector<std::string> ids = city->getUUIDsOfComponentsInView(grids);
    unsigned long HouseholdCounter = 0;
    foreach (std::string id, ids) {

        long population = (long) city->getComponent(id)->getAttribute("Population")->getDouble();

        do {

            //Demographics for Innsbruck
            int r = rand() % 100+1;
            int size = 0;
            if (r < 36) {
                size = 1;
            } else if ( r < 64.5) {
                size = 2;
            }else if ( r < 80.46) {
                size = 3;
            }else if ( r < 93.33) {
                size = 4;
            }else if ( r < 97.83) {
                size = 5;
            } else {
                size = 6;
            }


            DM::Node * household = new DM::Node();

            Attribute grid_id("GRID_ID");
            grid_id.setString(id);
            household->addAttribute(grid_id);
            household->addAttribute("persons", size );
            household->addAttribute("race_id", 1 );
            household->addAttribute("income",   rand() % 50000 + 1);
            household->addAttribute("cars",   rand() % 3 );
            household->addAttribute("workers",   rand() % 3 );
            household->addAttribute("age_of_head",   rand() % 40+20 );
            household->addAttribute("children",   rand() % 2);

        } while (population > 0);



    }
    Logger(Debug) <<(long) HouseholdCounter << "Households created";

    //Create Annual Household Controlls

    std::vector<std::string> househildIDs = this->city->getUUIDsOfComponentsInView(households);

    std::vector<double> persons_counter;

    for (int i = 0; i < 6; i++) {
        persons_counter.push_back(0);
    }


    foreach(std::string id, househildIDs) {
        int persons = (int) city->getComponent(id)->getAttribute("persons")->getDouble();
        persons_counter[persons-1] = persons_counter[persons-1]+1;

    }
    std::vector<double> househodsizes;
    for (int i = 0; i < 6; i++)
        househodsizes.push_back(i+1);


    for (int year = 1980; year < 1980+YearsToRun; year++) {
        Component * cmp = new Component();
        cmp->setName(QString::number(year).toStdString());
        this->city->addComponent(cmp, prognsis);
        cmp->addAttribute("year",year);
        cmp->addAttribute("race_id", 1);

        for (int i = 0; i < 6; i++) {
            cmp->addAttribute("persons", i+1);

            //Calculate new placed households
            if (i < 1) {
                persons_counter[i] = persons_counter[i]*h1[year-1980];
            } else if ( i < 2) {
                persons_counter[i] = persons_counter[i]*h2[year-1980];
            }else if ( i < 3) {
                persons_counter[i] = persons_counter[i]*h3[year-1980];
            }else if ( i < 4) {
                persons_counter[i] = persons_counter[i]*h4[year-1980];
            }else if ( i < 5) {
                persons_counter[i] = persons_counter[i]*h5[year-1980];
            } else {
                persons_counter[i] = persons_counter[i]*h6[year-1980];
            }


        }
        Attribute th ("total_number_of_households");
        th.setDoubleVector(persons_counter);
        cmp->addAttribute(th);

        Attribute ps ("persons");
        ps.setDoubleVector(househodsizes);
        cmp->addAttribute(ps);
    }
}
