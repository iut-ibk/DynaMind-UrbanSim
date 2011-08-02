"""
@file
@author  Chrisitan Urich <christian.urich@gmail.com>
@version 1.0
@section LICENSE

This file is part of VIBe2
Copyright (C) 2011  Christian Urich

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

from pyvibe import *
import start_run_vibe
import start_run_vibe_parcel

class UrbanSimExport(Module):
	"""
	@ingroup UrbanSim
	@author Christian Urich
	"""
        def __init__(self):
            Module.__init__(self)  
	    self.start = 0.
	    self.end = 0.
	    self.YearsToRun = 20
            self.Parcel = False
	    self.addParameter(self, "start" , VIBe2.DOUBLEDATA_IN)
	    self.addParameter(self, "YearsToRun" , VIBe2.DOUBLE)
	    self.addParameter(self, "end", VIBe2.DOUBLEDATA_OUT)
	    self.addParameter(self, "Parcel", VIBe2.BOOL)
            
        def run(self):
	    if self.Parcel == False:
		    u = start_run_vibe.UrbanSim_VIBe(int(self.YearsToRun))
		    u.run()
	    if self.Parcel == True:
		    u = start_run_vibe_parcel.UrbanSim_VIBe(int(self.YearsToRun))
		    u.run()
 
