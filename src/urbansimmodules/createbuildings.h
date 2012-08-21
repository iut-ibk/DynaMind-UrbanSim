#ifndef CREATEBUILDINGS_H
#define CREATEBUILDINGS_H

#include <dmmodule.h>
#include <dm.h>



using namespace DM;
class DM_HELPER_DLL_EXPORT CreateBuildings : public  Module {
DM_DECLARE_NODE(  CreateBuildings )

private:
    int YearsToRun;
    DM::View grids;
    DM::View buildings;
    DM::System * city;


public:
    CreateBuildings();

void run();
};


#endif // CREATEBUILDINGS_H
