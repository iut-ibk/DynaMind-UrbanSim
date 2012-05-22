"""
@file
@author  Chrisitan Urich <christian.urich@gmail.com>
@version 1.0
@section LICENSE

This file is part of DynaMind
Copyright (C) 2012  Christian Urich

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
"""

import os
import sys

from pydynamind import *
import start_run_vibe
import start_run_vibe_parcel

class UrbanSimExport(Module):
	"""
	@ingroup UrbanSim
       @brief starts UrbanSim
	@author Christian Urich
	"""
        def __init__(self):
            Module.__init__(self) 
            self.createParameter("Parcel", BOOL, "Parcel or Gridbased UrbanSim simulation")
            self.Parcel = False
            self.createParameter("YearsToRun", DOUBLE, "UrbanSim simulation years")
	    self.YearsToRun = 20   
            
        def run(self):
	    if self.Parcel == False:
		    u = start_run_vibe.UrbanSim_VIBe(int(self.YearsToRun))
		    u.run()
	    if self.Parcel == True:
		    u = start_run_vibe_parcel.UrbanSim_VIBe(int(self.YearsToRun))
		    u.run()
 
