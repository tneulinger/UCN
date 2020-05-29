#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"

#include "UCNDetectorMessenger.hh"
#include "UCNDetectorConstruction.hh"

UCNDetectorMessenger::UCNDetectorMessenger(UCNDetectorConstruction* detectorConstruction)
  : fDetectorConstruction(detectorConstruction)
{

  fDetectorDirectory = new G4UIdirectory("/detector/");
  fDetectorDirectory->SetGuidance("Detector Construction control commands.");

  fSetMaxTime_sCmd = new G4UIcmdWithADouble("/detector/setMaxTime_s", this);
  fSetMaxTime_sCmd->SetGuidance(" Set simulation time in seconds. (Default 100.)");
  fSetMaxTime_sCmd->SetParameterName("t",true);
  fSetMaxTime_sCmd->SetDefaultValue(100.0) ;

  fSetMaxStep_mmCmd = new G4UIcmdWithADouble("/detector/setMaxStep_mm", this);
  fSetMaxStep_mmCmd->SetGuidance(" Set maximum step size in mm. (Default 0.5)");
  fSetMaxStep_mmCmd->SetParameterName("dx",true);
  fSetMaxStep_mmCmd->SetDefaultValue(0.5) ;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

UCNDetectorMessenger::~UCNDetectorMessenger()
{
  delete fSetMaxTime_sCmd;
  delete fSetMaxStep_mmCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void UCNDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if( command == fSetMaxTime_sCmd)
   { fDetectorConstruction->SetMaxTime_s(fSetMaxTime_sCmd->GetNewDoubleValue(newValue));}

  if( command == fSetMaxStep_mmCmd)
   { fDetectorConstruction->SetMaxStep_mm(fSetMaxStep_mmCmd->GetNewDoubleValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
