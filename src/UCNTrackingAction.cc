#include "G4ThreeVector.hh"
#include "G4Track.hh"

#include "UCNTrackingAction.hh"

#include <fstream>
std::ofstream aliveTimeFile("alive.out");

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
  // get the time and place of birth of the track
  fTimeOfBirth  = track->GetGlobalTime();
  fPlaceOfBirth = track->GetPosition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNTrackingAction::PostUserTrackingAction(const G4Track* track)
{
  // get the time and place of death of the track
  fTimeOfDeath  = track->GetGlobalTime();
  fPlaceOfDeath = track->GetPosition();
  // calculate time interval that particle existed
  G4double aliveTime = (fTimeOfDeath - fTimeOfBirth)*1e-9; // in seconds
  // check to make sure it's a primary (UCN) before writing to file
  G4int ID = track->GetTrackID();
  if (ID == 1)
  {
    G4ThreeVector position = track->GetPosition();
    aliveTimeFile << aliveTime << ", " << fPlaceOfBirth << ", " << fPlaceOfDeath << std::endl;
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
