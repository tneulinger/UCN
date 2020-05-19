#include "globals.hh"

#include "G4ProcessTable.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4UCNBoundaryProcess.hh"

#include "UCNAnalysis.hh"
#include "UCNRunAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNRunAction::UCNRunAction() : G4UserRunAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNRunAction::~UCNRunAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNRunAction::BeginOfRunAction(const G4Run*)
{
  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // create histogram
  analysisManager->CreateH1("0","Y-pos", 90, -45.0*CLHEP::mm, +45.0*CLHEP::mm);

  // open file
  G4String fileName = "Y-POS";
  analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNRunAction::EndOfRunAction(const G4Run*)
{
  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // save histogram and write file
  analysisManager->Write();
  analysisManager->CloseFile();

  // G4ProcessTable* processTable = G4ProcessTable::GetProcessTable();
  //
  // G4UCNBoundaryProcess* ucnBoundaryProcess =
  //            (G4UCNBoundaryProcess*) processTable->
  //            FindProcess("UCNBoundaryProcess",G4Neutron::NeutronDefinition());

  // ucnBoundaryProcess->BoundaryProcessSummary();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
