#include "globals.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "UCNAnalysis.hh"
#include "UCNEventAction.hh"
#include "UCNRunAction.hh"
#include "UCNSnapshot.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNEventAction::UCNEventAction() : G4UserEventAction()
{
  // create snapshot object for the run
  G4String timesToSnapshotFile = "snapshots.in";
  G4String outputFile = "snapshots.out";
  fSnapshot = new UCNSnapshot(timesToSnapshotFile, outputFile);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNEventAction::~UCNEventAction()
{
  // delete snapshot object
  delete fSnapshot;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNEventAction::BeginOfEventAction(const G4Event*)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNEventAction::EndOfEventAction(const G4Event* event)
{
  // reset snapshot
  fSnapshot->FlushOutputStream();   // flush data to file so we can see it
  fSnapshot->ResetSnapshotIndex();  // reset index
  fSnapshot->ResetSnapshotStatus(); // reset flags

  // get information from hits collection
  G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0);
  G4cout << event->GetEventID() << ":    "
    << hc->GetSize() << " UCNs detected in this event." << G4endl;
}
