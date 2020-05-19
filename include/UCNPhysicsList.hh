#ifndef UCNPhysicsList_hh
#define UCNPhysicsList_hh

#include "G4VModularPhysicsList.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class UCNPhysicsList: public G4VModularPhysicsList
{
public:

    UCNPhysicsList();
    virtual ~UCNPhysicsList();

    virtual void ConstructParticle();
    virtual void ConstructProcess();

    void SetCuts();

private:

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif
