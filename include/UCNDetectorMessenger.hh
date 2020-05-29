#ifndef UCNDetectorMessenger_hh
#define UCNDetectorMessenger_hh

#include "globals.hh"
#include "G4UImessenger.hh"

class UCNDetectorConstruction;
class G4UIcmdWithADouble;
class G4UIdirectory;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class UCNDetectorMessenger: public G4UImessenger
{
  public:
    UCNDetectorMessenger(UCNDetectorConstruction*);
    virtual ~UCNDetectorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    UCNDetectorConstruction*   fDetectorConstruction;
    G4UIdirectory*             fDetectorDirectory;
    G4UIcmdWithADouble*        fSetMaxTime_sCmd;
    G4UIcmdWithADouble*        fSetMaxStep_mmCmd;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif
