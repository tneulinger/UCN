#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"

#include "UCNPrimaryGeneratorMessenger.hh"
#include "UCNPrimaryGeneratorAction.hh"

UCNPrimaryGeneratorMessenger::UCNPrimaryGeneratorMessenger(UCNPrimaryGeneratorAction* UCNGun)
  : fPrimaryAction(UCNGun)
{
  // G4cout << "create messenger for the UCN Primary Generator class " << G4endl;
  fGunDirectory = new G4UIdirectory("/gun/");
  fGunDirectory->SetGuidance("Particle Gun control commands.");

  fSetGunEnergy_neVCmd = new G4UIcmdWithADouble("/gun/gunEnergyneV",this);
  fSetGunEnergy_neVCmd->SetGuidance(" Set energy of the gun in neV");
  fSetGunEnergy_neVCmd->SetParameterName("e",true);
  fSetGunEnergy_neVCmd->SetDefaultValue(0.0) ;

  fSetGunPositionCmd = new G4UIcmdWith3VectorAndUnit("/gun/gunPosition",this);
  fSetGunPositionCmd->SetGuidance(" Set coord. of the gun position in mm.");
  fSetGunPositionCmd->SetParameterName("X","Y","Z",true,true);
  fSetGunPositionCmd->SetDefaultUnit("mm");
  //fSetGunPositionCmd->SetDefaultValue(0.0*mm, 0.0*mm, 0.0*mm) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

UCNPrimaryGeneratorMessenger::~UCNPrimaryGeneratorMessenger()
{
  delete fSetGunEnergy_neVCmd;
  delete fSetGunPositionCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void UCNPrimaryGeneratorMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{
  if( command == fSetGunEnergy_neVCmd)
   { fPrimaryAction->SetGunEnergy_neV(fSetGunEnergy_neVCmd->GetNewDoubleValue(newValue));}

  if( command == fSetGunPositionCmd)
   { fPrimaryAction->SetGunPosition(fSetGunPositionCmd->GetNew3VectorValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
