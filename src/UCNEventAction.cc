#include "globals.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "UCNAnalysis.hh"
#include "UCNEventAction.hh"
#include "UCNRunAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNEventAction::UCNEventAction() : G4UserEventAction(),
  fHaveBinnedIt(false)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNEventAction::~UCNEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNEventAction::BeginOfEventAction(const G4Event*)
{
  fHaveBinnedIt = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNEventAction::EndOfEventAction(const G4Event*)
{
  fHaveBinnedIt = false;
}
