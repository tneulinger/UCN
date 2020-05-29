#ifndef UCNDetectorConstruction_hh
#define UCNDetectorConstruction_hh

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

class G4Material;
class G4UniformGravityField;
class UCNDetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class UCNDetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    UCNDetectorConstruction(G4VPhysicalVolume *setWorld);
    virtual ~UCNDetectorConstruction();

  public:

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    void SetMaxTime_s(G4double t);
    void SetMaxStep_mm(G4double dx);

  private:

    G4VPhysicalVolume* fWorldPhysVol;

    static G4ThreadLocal G4UniformGravityField* fField;

  private:

    void DefineMaterials();

    UCNDetectorMessenger*  fDetectorMessenger;
    G4double               fMaxStep_mm;
    G4double               fMaxTime_s;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
