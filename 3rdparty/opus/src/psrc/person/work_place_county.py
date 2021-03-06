# Opus/UrbanSim urban simulation software.
# Copyright (C) 2005-2009 University of Washington
# See opus_core/LICENSE 

from opus_core.variables.variable import Variable
from variable_functions import my_attribute_label
from urbansim.functions import attribute_label

class work_place_county(Variable):
    """The county_id of a person's work place."""

    def dependencies(self):
        return [my_attribute_label("work_place_parcel_id"), 
                attribute_label("parcel","county")]
        
    def compute(self, dataset_pool):
        parcels = dataset_pool.get_dataset('parcel')
        return self.get_dataset().get_join_data(parcels, name="county", 
                                                join_attribute="work_place_parcel_id")
    

from opus_core.tests import opus_unittest
from urbansim.variable_test_toolbox import VariableTestToolbox
from numpy import array
from psrc.datasets.person_dataset import PersonDataset
from psrc.datasets.parcel_dataset import ParcelDataset
from opus_core.storage_factory import StorageFactory


class Tests(opus_unittest.OpusTestCase):
    variable_name = "psrc.person.work_place_county"
    
    def test_my_inputs(self):
        storage = StorageFactory().get_storage('dict_storage')
        
        persons_table_name = 'persons'
        storage.write_table(
                table_name=persons_table_name,
                table_data={
                    'person_id':array([1, 2, 3, 4, 5]),
                    'household_id':array([1, 1, 3, 3, 3]),
                    'member_id':array([1,2,1,2,3]),
                    'work_place_parcel_id':array([1,1,3,2,4]),
                    },
            )

        parcels_table_name = 'jobs'
        storage.write_table(
                table_name=parcels_table_name,
                table_data={
                    'parcel_id':array([1,2,3,4]),
                    'county':array(['033','035','033','053']),
                    },
            )

        persons = PersonDataset(in_storage=storage, in_table_name=persons_table_name)
        parcels = ParcelDataset(in_storage=storage, in_table_name=parcels_table_name)
        
        values = VariableTestToolbox().compute_variable(self.variable_name, \
            data_dictionary = {
                'parcel':parcels,
                'person':persons
                },
            dataset = 'person'
            )
            
        should_be = array(['033', '033', '033', '035', '053'])
        
        for i in range(values.size):
            self.assert_(values[i] == should_be[i], 
                'Error in ' + self.variable_name)


if __name__=='__main__':
    opus_unittest.main()