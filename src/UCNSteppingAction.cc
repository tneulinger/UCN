#include "G4Event.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4TrackStatus.hh"

#include "UCNAnalysis.hh"
#include "UCNEventAction.hh"
#include "UCNSnapshot.hh"
#include "UCNSteppingAction.hh"

#include <fstream>
std::ofstream trajectoryFile("trajectory.out");

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

  // // ===== TRAJECTORY DEBUGGING =====
  // const G4Event* event = G4RunManager::GetRunManager()->GetCurrentEvent();
  // G4int eventID = event->GetEventID();
  // G4int trackID = step->GetTrack()->GetTrackID();
  // if (step->GetTrack()->GetTrackStatus() != fStopAndKill)
  // {
  //   if (eventID==0 && trackID==1)
  //   {
  //     // pre step point
  //     G4StepPoint* prePoint = step->GetPreStepPoint();
  //     G4double preTime = prePoint->GetGlobalTime()*1e-9;                // seconds
  //     G4double preEne  = prePoint->GetKineticEnergy()*1e6*1e9;;         // neV
  //     G4ThreeVector prePos = prePoint->GetPosition();                   // mm
  //     G4ThreeVector preVel = prePoint->GetMomentum()*319075.6614201027; // m/s
  //     G4String preMat = prePoint->GetMaterial()->GetName();
  //     // post step point
  //     G4StepPoint* posPoint = step->GetPostStepPoint();
  //     G4double posTime = posPoint->GetGlobalTime()*1e-9;                // seconds
  //     G4double posEne  = posPoint->GetKineticEnergy()*1e6*1e9;;         // neV
  //     G4ThreeVector posPos = posPoint->GetPosition();                   // mm
  //     G4ThreeVector posVel = posPoint->GetMomentum()*319075.6614201027; // m/s
  //     G4String posMat = posPoint->GetMaterial()->GetName();
  //     G4String procName = posPoint->GetProcessDefinedStep()->GetProcessName();
  //     // write to file
  //     trajectoryFile << preTime << ", "  << preMat << ", " << preEne << ", " << prePos << ", " << preVel << " || "
  //                    << procName << " || "
  //                    << posTime << ", "  << posMat << ", " << posEne << ", " << posPos << ", " << posVel << '\n';
  //   }
  // }

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
