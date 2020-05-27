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
 gunPosition(1.000*mm,1.000*mm,1.000*mm), gunEnergy_neV(200.)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  gunMessenger = new UCNPrimaryGeneratorMessenger(this);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

  G4ParticleDefinition* particle = particleTable->FindParticle("neutron");
  fParticleGun->SetParticleDefinition(particle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNPrimaryGeneratorAction::~UCNPrimaryGeneratorAction()
{
  delete fParticleGun;
  delete gunMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of event

  // position
  fParticleGun->SetParticlePosition(gunPosition);

  // polarization
  fParticleGun->SetParticlePolarization(G4ThreeVector(0,1,0));

  // energy
  // * uniform random energy
  // G4double particleEnergy = 1e-9*eV + G4UniformRand()*(1e-7*eV-1e-9*eV);
  // G4double particleEnergy = G4UniformRand()*1e-7*eV;
  // * user entered energy, default 10 neV
  G4double particleEnergy = gunEnergy_neV*1e-9*eV;
  fParticleGun->SetParticleEnergy(particleEnergy);

  // direction
  // * random direction
  G4double theta = 2*pi*G4UniformRand();         // azimuthal, normally phi
  G4double phi = std::acos(1-2*G4UniformRand()); // polar, usually theta
  // if (phi > pi/2 && phi < pi) phi = pi-phi;   // would map to 2 pi steradians
  G4double z = std::sin(phi)*std::cos(theta);    // z, normally x
  G4double x = std::sin(phi)*std::sin(theta);    // x, normally y
  G4double y = std::cos(phi);                    // y, normally z
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x,y,z));
  // * constant direction
  // fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0,0.0,1.0));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNPrimaryGeneratorAction::SetGunEnergy_neV(G4double e)
{
  gunEnergy_neV = e ;
  // G4cout << "=== Gun energy set to " << gunEnergy_neV << " neV ===" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNPrimaryGeneratorAction::SetGunPosition(G4ThreeVector xyz)
{
  gunPosition = xyz ;
  // G4cout << " Coordinates of the gun position = " << gunPosition/mm <<
  //           " mm." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
