#ifndef UCNPrimaryGeneratorAction_hh
#define UCNPrimaryGeneratorAction_hh

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"

class G4Event;
class G4ParticleGun;
class UCNPrimaryGeneratorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class UCNPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:

  UCNPrimaryGeneratorAction(void);
  virtual ~UCNPrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event*);

  void SetGunEnergy_neV(G4double e);
  void SetGunPosition(G4ThreeVector xyz);

private:

  G4ParticleGun*                 fParticleGun;
  UCNPrimaryGeneratorMessenger*  fGunMessenger;
  G4double                       fGunEnergy_neV;
  G4ThreeVector                  fGunPosition;   // mm
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
