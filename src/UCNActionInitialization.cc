#include "UCNActionInitialization.hh"
#include "UCNEventAction.hh"
#include "UCNPrimaryGeneratorAction.hh"
#include "UCNRunAction.hh"
#include "UCNSteppingAction.hh"
#include "UCNTrackingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNActionInitialization::UCNActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNActionInitialization::~UCNActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNActionInitialization::BuildForMaster() const
{
  SetUserAction(new UCNRunAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNActionInitialization::Build() const
{
  SetUserAction(new UCNPrimaryGeneratorAction);

  SetUserAction(new UCNRunAction());

  UCNEventAction* eventAction = new UCNEventAction();
  SetUserAction(eventAction);

  SetUserAction(new UCNSteppingAction(eventAction));

  UCNTrackingAction* trackingAction = new UCNTrackingAction;
  SetUserAction(trackingAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
