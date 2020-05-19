#ifndef UCNPrimaryGeneratorAction_hh
#define UCNPrimaryGeneratorAction_hh

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

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

private:

  G4ParticleGun*                 fParticleGun;
  UCNPrimaryGeneratorMessenger*  gunMessenger;
  G4double                       gunEnergy_neV;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
