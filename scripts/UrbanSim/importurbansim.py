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
from pydynamind import *	
from opus_core.database_management.configurations.database_server_configuration import DatabaseServerConfiguration
from opus_core.storage_factory import StorageFactory
from opus_core.database_management.database_server import DatabaseServer

from opus_core.export_storage import ExportStorage
from opus_core.store.flt_storage import flt_storage
from opus_core.store.sql_storage import sql_storage
from opus_core.database_management.database_server import DatabaseServer
from opus_core.database_management.configurations.estimation_database_configuration import EstimationDatabaseConfiguration

class ImportUrbanSim(Module):
	"""
	@ingroup UrbanSim
	@author Christian Urich
	"""
        def __init__(self):
            Module.__init__(self)
	    self.createParameter("YearsToRun", DOUBLE, "How many years to import")
            self.createParameter("isParcel", BOOL, "If parcel model")
            self.start = 0
            self.stop = 0
            self.YearsToRun = 20
            self.isParcel = False
         
		
  
	def run (self):
		time = -1
		latest = ""
		directoryname = 'data/vibe_gridcell/runs/'
		if self.isParcel is True:
			directoryname = 'data/vibe_parcel/'
		for filename in os.listdir(os.path.join(os.environ['OPUS_HOME'], directoryname)):
			print filename
			if time == -1:
				time = os.path.getmtime(os.path.join(os.environ['OPUS_HOME'], directoryname, filename))
				latest = filename
			if os.path.getmtime(os.path.join(os.environ['OPUS_HOME'], directoryname, filename)) > time:
				time = os.path.getmtime(os.path.join(os.environ['OPUS_HOME'], directoryname, filename))
				latest = filename

		config = DatabaseServerConfiguration(host_name = 'localhost',
                                           user_name = 'urbansim',
					   password = 'urbansim',
                                           protocol = 'mysql')
		db_server = DatabaseServer(config)
		for i in range(1981, 1980+int(self.YearsToRun)):
			newdir = latest + '/' + str(i)
			flt_directory_in = os.path.join(os.environ['OPUS_HOME'], directoryname ,newdir)
			input_storage = flt_storage(storage_location = flt_directory_in)	
			db = db_server.get_database('ress_'+str(i))
			output_storage = StorageFactory().get_storage('sql_storage', storage_location = db)
			ExportStorage().export(in_storage=input_storage, out_storage=output_storage)
