#include "G4Isotope.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4UCNMaterialPropertiesTable.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UserLimits.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4UniformGravityField.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

#include "G4RepleteEofM.hh"
//#include "G4EqGravityField.hh"

#include "G4ClassicalRK4.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"
#include "G4PropagatorInField.hh"

#include "UCNDetectorConstruction.hh"
#include "UCNDetectorMessenger.hh"
#include "UCNMaterialDataHelper.hh"
#include "UCNTrackerSD.hh"
#include "G4SDManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNDetectorConstruction::UCNDetectorConstruction(G4VPhysicalVolume *setWorld)
 : fWorldPhysVol(0), fMaxStep_mm(0.5), fMaxTime_s(100.0)
{

  fDetectorMessenger = new UCNDetectorMessenger(this);

  fWorldPhysVol = setWorld;
  DefineMaterials();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNDetectorConstruction::~UCNDetectorConstruction()
{
  if (fField) delete fField;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Relies on GDML files populating materials table with basic materials information.
// Loops through all materials and applies UCN materials properties, listed in
// materials/ directory in text files. See UCNMaterialDataHelper.
void UCNDetectorConstruction::DefineMaterials()
{
  // Get handle to materials table
  G4MaterialTable* mt = (G4Material::GetMaterialTable());
  // Get the number of materials in the materials table
  G4int numOfMaterials = G4Material::GetNumberOfMaterials();

  // Apply UCN materials properties to materials
  // G4double density, scatcs, scatal, abscs, absal, vel;
  G4String matName;
  G4String matFileName;
  G4Material* theMat;
  UCNMaterialDataHelper* mdh;
  G4UCNMaterialPropertiesTable* mpt;
  for (G4int matNum = 0; matNum < numOfMaterials; matNum++)
  {
    // Get the material name from the material table
    matName = mt[0][matNum]->GetName();
    // Get the file name in the materials/ directory
    matFileName = "../materials/" + matName + ".txt";
    // Import the UCN materials properties
    mdh = new UCNMaterialDataHelper(matFileName);
    // Add the UCN properties to a properties table
    mpt = new G4UCNMaterialPropertiesTable();
    mpt->AddConstProperty("DIFFUSION", mdh->GetDiffusion());
    mpt->AddConstProperty("FERMIPOT",  mdh->GetFermiPot());
    mpt->AddConstProperty("SPINFLIP",  mdh->GetSpinFlip());
    mpt->AddConstProperty("LOSS",      mdh->GetLoss());
    mpt->AddConstProperty("LOSSCS",    mdh->GetLossCS());
    mpt->AddConstProperty("ABSCS",     mdh->GetAbsCS());
    mpt->AddConstProperty("SCATCS",    mdh->GetScatCS());
    // Get the handle to the material
    theMat = G4Material::GetMaterial(matName);
    // Apply the UCN properties to this material
    theMat->SetMaterialPropertiesTable(mpt);

    // // print some information
    // G4cout << " ============= " << matName << " ============= " << G4endl;
    // density = theMat->GetTotNbOfAtomsPerVolume();
    // G4cout << "density: " << density << G4endl;
    // // G4UCNMultiScattering
    // scatcs = mpt->GetConstProperty("SCATCS");
    // scatcs *= barn;
    // G4cout << "SCATCS: " << scatcs << G4endl;
    // scatal = 1./density/scatcs;
    // G4cout << "SCATCS attenuation length: " << scatal << G4endl;
    // // G4UCNAbsorpton
    // abscs = mpt->GetConstProperty("ABSCS");
    // abscs *= barn;
    // G4cout << "ABSCS: " << abscs << G4endl;
    // vel = 4*meter/second;
    // abscs *= 2200.*meter/second/vel;
    // G4cout << "ABSCS (scaled): " << abscs << G4endl;
    // absal = 1./density/abscs;
    // G4cout << "ABSCS attenuation length: " << absal << G4endl;


  }

  // G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  // OLD
  // #include "UCNDetectorMaterials.icc"
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* UCNDetectorConstruction::Construct()
{

  // get handle to world logical volume
  G4LogicalVolume* worldLogVol  = fWorldPhysVol->GetLogicalVolume();

  // ========== USER LIMITS ==========
  G4double maxStep = fMaxStep_mm*mm;
  G4double maxTime = fMaxTime_s*s;
  G4UserLimits* stepLimit = new G4UserLimits(maxStep,DBL_MAX,maxTime);
  // G4UserLimits* stepLimit = new G4UserLimits(maxStep,DBL_MAX,DBL_MAX);
  worldLogVol->SetUserLimits(stepLimit);
  // =================================

  // ========== VISUALIZATION ==========
  worldLogVol-> SetVisAttributes(G4VisAttributes::Invisible);

  // G4VisAttributes* Red        = new G4VisAttributes( G4Colour(255/255. ,0/255.   ,0/255.   ));
  // G4VisAttributes* Yellow     = new G4VisAttributes( G4Colour(255/255. ,255/255. ,0/255.   ));
  // G4VisAttributes* LightBlue  = new G4VisAttributes( G4Colour(0/255.   ,204/255. ,204/255. ));
  // G4VisAttributes* LightGreen = new G4VisAttributes( G4Colour(153/255. ,255/255. ,153/255. ));
  // G4VisAttributes* White      = new G4VisAttributes( G4Colour(255/255. ,355/255. ,255/255. ));

  // G4LogicalVolumeStore* MyGDML = G4LogicalVolumeStore::GetInstance();
  // MyGDML->GetVolume("thing1")->SetVisAttributes(LightGreen);
  // MyGDML->GetVolume("thing2")->SetVisAttributes(LightGreen);

  return fWorldPhysVol;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal G4UniformGravityField* UCNDetectorConstruction::fField = 0;

void UCNDetectorConstruction::ConstructSDandField()
{

  // register the detectors, do this by searching for all logical volumes which
  // have the "Detector" material applied to them and registering these as
  // sensitive detectors
  G4String detectorSDname  = "UCN/TrackerDetectorSD";
  UCNTrackerSD* aTrackerSD = new UCNTrackerSD(detectorSDname,
                                            "TrackerHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);
  // find the logical volumes which have "Detector" material assigned to them
  G4LogicalVolume* worldLogVol  = fWorldPhysVol->GetLogicalVolume();
  G4int numberOfDaughters = worldLogVol->GetNoDaughters();
  G4VPhysicalVolume* daughterPhysVol;
  G4LogicalVolume*   daughterLogVol;
  G4String           daughterMatName;
  G4String           daughterLogVolName;
  G4double maxStep = fMaxStep_mm*mm;
  G4double maxTime = fMaxTime_s*s;
  // G4UserLimits* stepLimit = new G4UserLimits(maxStep,DBL_MAX,maxTime);
  G4UserLimits* stepLimit = new G4UserLimits(maxStep,DBL_MAX,DBL_MAX);
  for (G4int i=0; i<numberOfDaughters; i++)
  {
    daughterPhysVol = worldLogVol->GetDaughter(i);
    daughterLogVol  = daughterPhysVol->GetLogicalVolume();
    daughterLogVol->SetUserLimits(stepLimit);
    daughterMatName = daughterLogVol->GetMaterial()->GetName();
    if (daughterMatName == "Detector")
    {
      daughterLogVolName = daughterLogVol->GetName();
      SetSensitiveDetector(daughterLogVolName, aTrackerSD, true);
      G4cout << " ===== " << daughterLogVolName <<
        " has been registered as a detector" << " ===== " << G4endl;
    }
  }



  // gravity field
  if (!fField) {

     fField = new G4UniformGravityField();

     G4RepleteEofM* equation = new G4RepleteEofM(fField);
//     G4RepleteEofM* equation = new G4RepleteEofM(fField,12);
//     G4EqGravityField* equation = new G4EqGravityField(fField);

     G4FieldManager* fieldManager
      = G4TransportationManager::GetTransportationManager()->GetFieldManager();
     fieldManager->SetDetectorField(fField);

     G4MagIntegratorStepper* stepper = new G4ClassicalRK4(equation,8);
//     G4MagIntegratorStepper* stepper = new G4ClassicalRK4(equation,12);

     G4double minStep           = 0.1*mm;

     G4ChordFinder* chordFinder =
                   new G4ChordFinder((G4MagneticField*)fField,minStep,stepper);

     // Set accuracy parameters
     G4double deltaChord     = 3.0*mm;
     // G4double deltaChord        = 1.0*nm;
     chordFinder->SetDeltaChord( deltaChord );

     G4double deltaOneStep      = 0.01*mm;
     fieldManager->SetAccuraciesWithDeltaOneStep(deltaOneStep);

     G4double deltaIntersection = 0.1*mm;
     fieldManager->SetDeltaIntersection(deltaIntersection);

     G4TransportationManager* transportManager =
                           G4TransportationManager::GetTransportationManager();

     G4PropagatorInField* fieldPropagator =
                                      transportManager->GetPropagatorInField();

     G4double epsMin            = 2.5e-7*mm;
     G4double epsMax            = 0.05*mm;

     fieldPropagator->SetMinimumEpsilonStep(epsMin);
     fieldPropagator->SetMaximumEpsilonStep(epsMax);

     fieldManager->SetChordFinder(chordFinder);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNDetectorConstruction::SetMaxTime_s(G4double t)
{
  fMaxTime_s = t;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNDetectorConstruction::SetMaxStep_mm(G4double dx)
{
  fMaxStep_mm = dx ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
