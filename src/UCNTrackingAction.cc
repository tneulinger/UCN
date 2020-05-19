#include "G4Track.hh"

#include "UCNTrackingAction.hh"

#include <fstream>
std::ofstream aliveTimeFile("aliveTime.txt");

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNTrackingAction::UCNTrackingAction()
:G4UserTrackingAction()
{
  //
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNTrackingAction::~UCNTrackingAction()
{
  //
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNTrackingAction::PreUserTrackingAction(const G4Track* track)
{
  fTime_birth = track->GetGlobalTime();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNTrackingAction::PostUserTrackingAction(const G4Track* track)
{
  fTime_end = track->GetGlobalTime();

  fTimeAlive = (fTime_end - fTime_birth)*1e-9;  // in seconds

  G4int ID = track->GetTrackID();
  if (ID == 1)
  {
    // G4cout << "=======================================" << G4endl;
    // G4cout << "Alive for " << fTimeAlive << " seconds." << G4endl;
    // G4cout << "=======================================" << G4endl;
    // G4cout << fTimeAlive << G4endl;
    // aliveTimeFile << fTimeAlive << " " << std::endl;
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
