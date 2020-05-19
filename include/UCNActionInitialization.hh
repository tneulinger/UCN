#ifndef UCNActionInitialization_hh
#define UCNActionInitialization_hh

#include "G4VUserActionInitialization.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class UCNActionInitialization : public G4VUserActionInitialization
{
  public:
    UCNActionInitialization();
    virtual ~UCNActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
