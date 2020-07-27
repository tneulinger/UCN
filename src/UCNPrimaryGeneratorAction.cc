#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "UCNPrimaryGeneratorAction.hh"
#include "UCNDetectorConstruction.hh"
#include "UCNPrimaryGeneratorMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNPrimaryGeneratorAction::UCNPrimaryGeneratorAction(void) :
 fGunEnergy_neV(100.),
 fGunPosition(0.000*mm,0.000*mm,0.000*mm),
 fGunRandomDirection(true),
 fGunDirection(1.0, 0.0, 0.0)

{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  fGunMessenger = new UCNPrimaryGeneratorMessenger(this);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

  G4ParticleDefinition* particle = particleTable->FindParticle("neutron");
  fParticleGun->SetParticleDefinition(particle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNPrimaryGeneratorAction::~UCNPrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fGunMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of event

  // position
  fParticleGun->SetParticlePosition(fGunPosition);

  // polarization
  fParticleGun->SetParticlePolarization(G4ThreeVector(0,1,0));

  // energy
  G4double particleEnergy = fGunEnergy_neV*1e-9*eV;
  fParticleGun->SetParticleEnergy(particleEnergy);
  // uniform random energy
  // G4double particleEnergy = 1e-9*eV + G4UniformRand()*(1e-7*eV-1e-9*eV);

  // direction
  if (fGunRandomDirection){
    // * random direction
    G4double theta = 2*pi*G4UniformRand();         // azimuthal, normally phi
    G4double phi = std::acos(1-2*G4UniformRand()); // polar, usually theta
    // if (phi > pi/2 && phi < pi) phi = pi-phi;   // would map to 2 pi steradians
    G4double z = std::sin(phi)*std::cos(theta);    // z, normally x
    G4double x = std::sin(phi)*std::sin(theta);    // x, normally y
    G4double y = std::cos(phi);                    // y, normally z
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x,y,z));
  } else {
    // * constant direction
    fParticleGun->SetParticleMomentumDirection(fGunDirection);
  }

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNPrimaryGeneratorAction::SetGunToRandomDirection(G4bool b)
{
  fGunRandomDirection = b ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNPrimaryGeneratorAction::SetGunDirection(G4ThreeVector uvw)
{
  fGunDirection = uvw ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNPrimaryGeneratorAction::SetGunEnergy_neV(G4double e)
{
  fGunEnergy_neV = e ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNPrimaryGeneratorAction::SetGunPosition(G4ThreeVector xyz)
{
  fGunPosition = xyz ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
