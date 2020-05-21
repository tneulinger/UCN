
#include "globals.hh"

#include "G4GDMLParser.hh"
#include "G4RunManager.hh"
#include "G4UIcommand.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "Randomize.hh"

#include "UCNActionInitialization.hh"
#include "UCNDetectorConstruction.hh"
#include "UCNPhysicsList.hh"

int main(int argc, char** argv)
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  // 1. detect interactive or batch mode
  G4String gdmlFile  = "";
  G4String macroFile = "";
  G4UIExecutive* ui = nullptr;
  if (argc==2)
  {
    // interactive mode
    G4cerr << "Running in interactive mode..." << G4endl;
    gdmlFile  = argv[1];
    G4cerr << "GDML file: "  << gdmlFile << G4endl;
    ui = new G4UIExecutive(argc, argv);
  } else {
    // batch mode, load macro file
    G4cerr << "Running in batch mode..." << G4endl;
    gdmlFile  = argv[1];
    macroFile = argv[2];
    G4cerr << "GDML file: "  << gdmlFile << G4endl;
    G4cerr << "Macro file: " << macroFile << G4endl;
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  // 2. create run manager object and register detector, physics, and actions

  // run manager
  // G4cerr << "Creating run manager..." << G4endl;
  G4RunManager * runManager = new G4RunManager;

  // detector
  // G4cerr << "Registering detector..." << G4endl;
  // parse GDML file
  G4GDMLParser parser;
  parser.SetOverlapCheck(true);
  parser.Read(gdmlFile);
  runManager->SetUserInitialization(
    new UCNDetectorConstruction(parser.GetWorldVolume())
  );

  // physics list
  // G4cerr << "Registering physics..." << G4endl;
  runManager->SetUserInitialization(new UCNPhysicsList());

  // action initialization
  // G4cerr << "Registering action initialization..." << G4endl;
  runManager->SetUserInitialization(new UCNActionInitialization());

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  // 3. initialize the run manager
  // G4cerr << "Initializing the run manager..." << G4endl;
  runManager->Initialize();
  // runManager->BeamOn(1);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  // 4. run the application...
  // G4cerr << "Getting pointer to UI manager..." << G4endl;
  G4UImanager* UIManager = G4UImanager::GetUIpointer();
  if ( !ui )
  {
    // ... in batch mode
    G4cerr << "Running the macro file..." << G4endl;
    G4String command = "/control/execute ";
    UIManager->ApplyCommand(command + macroFile);
  } else {
    // ... in interactive mode
    // G4cerr << "Creating visualization manager..." << G4endl;
    G4VisManager* visManager = new G4VisExecutive;
    // G4cerr << "Initializing visualization manager..." << G4endl;
    visManager->Initialize();
    // G4cerr << "Starting interactive session..." << G4endl;
    UIManager->ApplyCommand("/control/execute vis.mac");
    ui->SessionStart();
    delete ui;
    delete visManager;
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  delete runManager;

  return 0;
}
