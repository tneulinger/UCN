#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

#include "UCNPrimaryGeneratorMessenger.hh"
#include "UCNPrimaryGeneratorAction.hh"

UCNPrimaryGeneratorMessenger::UCNPrimaryGeneratorMessenger(UCNPrimaryGeneratorAction* UCNGun)
  : fPrimaryAction(UCNGun)
{
  // G4cout << "create messenger for the UCN Primary Generator class " << G4endl;
  fGunDirectory = new G4UIdirectory("/gun/");
  fGunDirectory->SetGuidance("Particle Gun control commands.");

  fSetGunToRandomDirectionCmd = new G4UIcmdWithABool("/gun/gunRandomDirection", this);
  fSetGunToRandomDirectionCmd->SetGuidance(" Set direction of the gun to random");
  fSetGunToRandomDirectionCmd->SetParameterName("b",true);
  fSetGunToRandomDirectionCmd->SetDefaultValue(true) ;

  fSetGunEnergy_neVCmd = new G4UIcmdWithADouble("/gun/gunEnergyneV",this);
  fSetGunEnergy_neVCmd->SetGuidance(" Set energy of the gun in neV");
  fSetGunEnergy_neVCmd->SetParameterName("e",true);
  fSetGunEnergy_neVCmd->SetDefaultValue(0.0) ;

  fSetGunDirectionCmd = new G4UIcmdWith3Vector("/gun/gunDirection",this);
  fSetGunDirectionCmd->SetGuidance(" Set direction of gun vector");
  fSetGunDirectionCmd->SetParameterName("Vx","Vy","Vz",true,true);
  G4ThreeVector defaultDirection;
  defaultDirection[0] = 1.0;
  defaultDirection[1] = 0.0;
  defaultDirection[2] = 0.0;
  fSetGunDirectionCmd->SetDefaultValue( defaultDirection ) ;

  fSetGunPositionCmd = new G4UIcmdWith3VectorAndUnit("/gun/gunPosition",this);
  fSetGunPositionCmd->SetGuidance(" Set coord. of the gun position in mm.");
  fSetGunPositionCmd->SetParameterName("X","Y","Z",true,true);
  fSetGunPositionCmd->SetDefaultUnit("mm");
  G4ThreeVector defaultPosition;
  defaultPosition[0] = 1.0*mm;
  defaultPosition[1] = 1.0*mm;
  defaultPosition[2] = 1.0*mm;
  fSetGunPositionCmd->SetDefaultValue( defaultPosition ) ;
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
  if( command == fSetGunToRandomDirectionCmd)
   { fPrimaryAction->SetGunToRandomDirection(fSetGunToRandomDirectionCmd->GetNewBoolValue(newValue));}

  if( command == fSetGunDirectionCmd)
   { fPrimaryAction->SetGunDirection(fSetGunDirectionCmd->GetNew3VectorValue(newValue));}

  if( command == fSetGunEnergy_neVCmd)
   { fPrimaryAction->SetGunEnergy_neV(fSetGunEnergy_neVCmd->GetNewDoubleValue(newValue));}

  if( command == fSetGunPositionCmd)
   { fPrimaryAction->SetGunPosition(fSetGunPositionCmd->GetNew3VectorValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
