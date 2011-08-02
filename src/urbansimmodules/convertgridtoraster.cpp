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


#include "convertgridtoraster.h"
#include "vectordatahelper.h"

VIBe_DECLARE_NODE_NAME(ConvertGridToRaster, UrbanSim)


ConvertGridToRaster::ConvertGridToRaster()
{
    VectorData * input;
    identifierGrid = "GRID_";
    identifierValues = "persons";
    identifierTable = "household";
    height = 40;
    width = 40;
    cellsize = 200;
    this->addParameter("IdentifierGrid", VIBe2::STRING, &this->identifierGrid);
    this->addParameter("IdentifierValues", VIBe2::STRING, &this->identifierValues);
    this->addParameter("IdentifierTable", VIBe2::STRING, &this->identifierTable);
    this->addParameter("Width", VIBe2::LONG, &this->width);
    this->addParameter("Height", VIBe2::LONG, &this->height);
    this->addParameter("CellSize", VIBe2::DOUBLE, &this->cellsize);
    this->addParameter("Grid", VIBe2::VECTORDATA_IN, & this->input);
    this->addParameter("OutPut", VIBe2::RASTERDATA_OUT, & this->outPut);

}

void ConvertGridToRaster::run() {

    this->outPut->setSize(this->width, this->height, this->cellsize);
    this->outPut->clear();

    int counter = 1;
    std::vector<std::string> names = VectorDataHelper::findElementsWithIdentifier(this->identifierGrid, this->input->getAttributeNames());
    std::vector<std::string> attrnames = VectorDataHelper::findElementsWithIdentifier(this->identifierTable, this->input->getAttributeNames());
    std::vector<double> vals = std::vector<double>(names.size()+1);
    for (int i = 0; i < vals.size(); i++) {
        vals[i] = 0;
    }

    foreach (std::string name, attrnames) {
        Attribute attr = this->input->getAttributes(name);
        int Grid_ID = attr.getAttribute("GRID_ID");
        vals[Grid_ID] = vals[Grid_ID] +  attr.getAttribute(identifierValues);
    }

    foreach (std::string name, names) {
        int pos = name.find("_");
        std::string id =name.substr(pos+1, name.size()-1);
        Attribute attr = this->input->getAttributes(name);
        int x = attr.getAttribute("relative_x");
        int y= attr.getAttribute("relative_y");

        this->outPut->setValue(x,y,vals[QString::fromStdString(id).toInt()] );

    }

  /*  for (int i = 0; i < this->width; i++) {
        for (int j = 0; j < this->height; j++) {
            stringstream id;
            id << identifierGrid << counter;

            Attribute attr = this->input->getAttributes(id.str());
            double val = attr.getAttribute(identifierValues);

            this->outPut->setValue(i,j,val);



            counter++;
        }

    }*/

}
