#include "G4Step.hh"

#include "UCNAnalysis.hh"
#include "UCNEventAction.hh"
#include "UCNSteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
UCNSteppingAction::UCNSteppingAction(UCNEventAction* eventAction) :
 G4UserSteppingAction(),
 fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNSteppingAction::~UCNSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNSteppingAction::UserSteppingAction(const G4Step* step)
{

  // (what time are we interested in?)
  G4double timeOfInterest = 5.0e9;                     // 1e9 ns = 1 s
  G4double timeWindow     = 5.0e6;                       // 1e6 ns = 1 ms

  // have we added the y-position of the current run to the hisogram?
  G4bool haveBinnedIt = fEventAction->GetBinnedStatus();

  // if we haven't...
  if (!haveBinnedIt)
  {
    G4StepPoint* postStepPoint = step->GetPostStepPoint();
    G4double globalTime = postStepPoint->GetGlobalTime();  // get time in ns

    // ... and if we are in the time of interest...
    if (    (globalTime>timeOfInterest)
         && (globalTime<(timeOfInterest+timeWindow))  )
    {
           // ... then get the y-position
           G4double ypos = postStepPoint->GetPosition()[1];

           // get analysis manager and fill histogram
           auto analysisManager = G4AnalysisManager::Instance();
           analysisManager->FillH1(0, ypos);

           // update the binned status so we don't count it twice
           fEventAction->ChangeBinnedStatus();
    }

  }







}
