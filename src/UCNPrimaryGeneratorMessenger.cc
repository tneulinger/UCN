#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"

#include "UCNPrimaryGeneratorMessenger.hh"
#include "UCNPrimaryGeneratorAction.hh"

UCNPrimaryGeneratorMessenger::UCNPrimaryGeneratorMessenger(UCNPrimaryGeneratorAction* UCNGun)
  : fPrimaryAction(UCNGun)
{
  // G4cout << "create messenger for the UCN Primary Generator class " << G4endl;
  gunDirectory = new G4UIdirectory("/gun/");
  gunDirectory->SetGuidance("Particle Gun control commands.");

  setGunEnergy_neVCmd = new G4UIcmdWithADouble("/gun/gunEnergyneV",this);
  setGunEnergy_neVCmd->SetGuidance(" Set energy of the gun in neV");
  setGunEnergy_neVCmd->SetParameterName("e",true);
  setGunEnergy_neVCmd->SetDefaultValue(0.0) ;

  setGunPositionCmd = new G4UIcmdWith3VectorAndUnit("/gun/gunPosition",this);
  setGunPositionCmd->SetGuidance(" Set coord. of the gun position in mm.");
  setGunPositionCmd->SetParameterName("X","Y","Z",true,true);
  setGunPositionCmd->SetDefaultUnit("mm");
  //setGunPositionCmd->SetDefaultValue(0.0*mm, 0.0*mm, 0.0*mm) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

UCNPrimaryGeneratorMessenger::~UCNPrimaryGeneratorMessenger()
{
  delete setGunEnergy_neVCmd;
  delete setGunPositionCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void UCNPrimaryGeneratorMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{
  if( command == setGunEnergy_neVCmd)
   { fPrimaryAction->SetGunEnergy_neV(setGunEnergy_neVCmd->GetNewDoubleValue(newValue));}

  if( command == setGunPositionCmd)
   { fPrimaryAction->SetGunPosition(setGunPositionCmd->GetNew3VectorValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
