#include "G4Step.hh"

#include "UCNAnalysis.hh"
#include "UCNEventAction.hh"
#include "UCNSnapshot.hh"
#include "UCNSteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
UCNSteppingAction::UCNSteppingAction(UCNEventAction* eventAction) :
 G4UserSteppingAction(),
 fEventAction(eventAction)
{
  fSnapshot = fEventAction->GetSnapshotPointer();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNSteppingAction::~UCNSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNSteppingAction::UserSteppingAction(const G4Step* step)
{

  // what is the time now?
  G4StepPoint* postStepPoint = step->GetPostStepPoint();
  G4double theTimeNowInNanoSeconds = postStepPoint->GetGlobalTime();

  // should we take a snapshot here?
  G4bool shouldWe = false;
  shouldWe = fSnapshot->ShouldWeTakeASnapshotNow(theTimeNowInNanoSeconds);

  // if so, write the snapshot to file
  G4double time;
  G4double energy;
  G4ThreeVector position;
  G4ThreeVector momentum;
  if (shouldWe)
  {
    time = theTimeNowInNanoSeconds*1e-9;
    energy = postStepPoint->GetKineticEnergy();
    position = postStepPoint->GetPosition();
    momentum = postStepPoint->GetMomentum();

    fSnapshot->SetTime(time);
    fSnapshot->SetEnergy(energy);
    fSnapshot->SetPosition(position);
    fSnapshot->SetMomentum(momentum);

    fSnapshot->WriteTimeEnergyPositionMomentum();

  }

  // ===== POSITION BINNING AT TIME =====
  // // (what time are we interested in?)
  // G4double timeOfInterest = 5.0e9;                     // 1e9 ns = 1 s
  // G4double timeWindow     = 5.0e6;                     // 1e6 ns = 1 ms
  // // have we added the y-position of the current run to the hisogram?
  // G4bool haveBinnedIt = fEventAction->GetBinnedStatus();
  // // if we haven't...
  // if (!haveBinnedIt)
  // {
  //   G4StepPoint* postStepPoint = step->GetPostStepPoint();
  //   G4double globalTime = postStepPoint->GetGlobalTime();  // get time in ns
  //   // ... and if we are in the time of interest...
  //   if (    (globalTime>timeOfInterest)
  //        && (globalTime<(timeOfInterest+timeWindow))  )
  //   {
  //          // ... then get the y-position
  //          G4double ypos = postStepPoint->GetPosition()[1];
  //          // get analysis manager and fill histogram
  //          auto analysisManager = G4AnalysisManager::Instance();
  //          analysisManager->FillH1(0, ypos);
  //          // update the binned status so we don't count it twice
  //          fEventAction->ChangeBinnedStatus();
  //   }
  //
  // }

}
