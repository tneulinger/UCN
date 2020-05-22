#ifndef UCNPrimaryGeneratorMessenger_hh
#define UCNPrimaryGeneratorMessenger_hh

#include "globals.hh"
#include "G4UImessenger.hh"

class UCNPrimaryGeneratorAction;
class G4UIcmdWithADouble;
class G4UIcmdWith3VectorAndUnit;
class G4UIdirectory;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class UCNPrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    UCNPrimaryGeneratorMessenger(UCNPrimaryGeneratorAction*);
    virtual ~UCNPrimaryGeneratorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    UCNPrimaryGeneratorAction*   fPrimaryAction;
    G4UIdirectory*               gunDirectory;
    G4UIcmdWithADouble*          setGunEnergy_neVCmd;
    G4UIcmdWith3VectorAndUnit*   setGunPositionCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif
