#ifndef UCNDetectorConstruction_hh
#define UCNDetectorConstruction_hh

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

class G4Material;
class G4UniformGravityField;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class UCNDetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    UCNDetectorConstruction(G4VPhysicalVolume *setWorld);
    virtual ~UCNDetectorConstruction();

  public:

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

  private:

    G4Material*        fVacuum;
    G4Material*        fGuideMaterial;
    G4VPhysicalVolume* fWorldPhysVol;

    static G4ThreadLocal G4UniformGravityField* fField;

  private:

    void DefineMaterials();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
