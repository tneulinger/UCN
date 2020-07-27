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

  void SetGunToRandomDirection(G4bool b);
  void SetGunDirection(G4ThreeVector uvw);
  void SetGunEnergy_neV(G4double e);
  void SetGunPosition(G4ThreeVector xyz);

private:

  G4ParticleGun*                 fParticleGun;
  UCNPrimaryGeneratorMessenger*  fGunMessenger;

  // energy (neV)
  G4double                       fGunEnergy_neV;

  // position (mm)
  G4ThreeVector                  fGunPosition;

  // direction, is it random?
  G4bool                         fGunRandomDirection;
  G4ThreeVector                  fGunDirection;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
