#ifndef UCNExtraPhysics_hh
#define UCNExtraPhysics_hh

#include "G4VPhysicsConstructor.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class UCNExtraPhysics : public G4VPhysicsConstructor
{
public:

    UCNExtraPhysics();
    virtual ~UCNExtraPhysics();

    virtual void ConstructParticle();
    virtual void ConstructProcess();

private:

    void ConstructUCN();

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
