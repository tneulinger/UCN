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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNDetectorConstruction::UCNDetectorConstruction(G4VPhysicalVolume *setWorld)
 : fWorldPhysVol(0), fVacuum(0), fGuideMaterial(0)
{
  fWorldPhysVol = setWorld;
  DefineMaterials();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNDetectorConstruction::~UCNDetectorConstruction()
{
  if (fField) delete fField;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNDetectorConstruction::DefineMaterials()
{
  G4NistManager* nistMan = G4NistManager::Instance();

  fVacuum = nistMan->FindOrBuildMaterial("G4_Galactic");
  fGuideMaterial = nistMan->FindOrBuildMaterial("G4_Ni");

  G4UCNMaterialPropertiesTable* MPT = new G4UCNMaterialPropertiesTable();

  //  MPT->AddConstProperty("REFLECTIVITY",1.);
  MPT->AddConstProperty("DIFFUSION",0.1);
  MPT->AddConstProperty("FERMIPOT",252.0); // Gollub, Table 2.1 in neV
  MPT->AddConstProperty("SPINFLIP",0.);
  // MPT->AddConstProperty("LOSS", 12.5e-5); //  Gollub, Table 2.1, f = W/V
  MPT->AddConstProperty("LOSS", 0);
  MPT->AddConstProperty("LOSSCS",0.);
  MPT->AddConstProperty("ABSCS",4.49);  // 1/v loss cross-section  at room temp.
  MPT->AddConstProperty("SCATCS",18.5); // (incoherent) "elastic" scattering cs

  // G4double neV = 1.e-9*eV;

  // MPT->SetMicroRoughnessParameters(30*nm, 1*nm,
  //                                  180, 1000,
  //                                  0*degree, 90*degree,
  //                                  1*neV, 1000*neV,
  //                                  15, 15,
  //                                  0.01*degree);
  // arguments:
  //         1) w = correlation length
  //         2) b = RMS roughness
  //         3) number of angles theta_i in the look-up tables
  //         4) number of energies in the look-up tables
  //         5) minimum value of theta_i
  //         6) maximum value of theta_i
  //         7) minimum value of E
  //         8) maximum value of E
  //         9) number of angles theta_o in the look-up table calculation
  //        10) number of angles phi_o   in the look-up table calculation
  //        11) angular cut


  fGuideMaterial->SetMaterialPropertiesTable(MPT);

  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* UCNDetectorConstruction::Construct()
{

  G4double maxStep = 1*mm;
  G4double maxTime = 5.01*s;

  G4UserLimits* stepLimit = new G4UserLimits(maxStep,DBL_MAX,maxTime);
  // G4UserLimits* stepLimit = new G4UserLimits(maxStep,DBL_MAX,DBL_MAX);

  fWorldPhysVol->GetLogicalVolume()->SetUserLimits(stepLimit);

  //-visualization
  // G4VisAttributes* Red        = new G4VisAttributes( G4Colour(255/255. ,0/255.   ,0/255.   ));
  // G4VisAttributes* Yellow     = new G4VisAttributes( G4Colour(255/255. ,255/255. ,0/255.   ));
  // G4VisAttributes* LightBlue  = new G4VisAttributes( G4Colour(0/255.   ,204/255. ,204/255. ));
  // G4VisAttributes* LightGreen = new G4VisAttributes( G4Colour(153/255. ,255/255. ,153/255. ));
  // G4VisAttributes* White      = new G4VisAttributes( G4Colour(255/255. ,355/255. ,255/255. ));

  fWorldPhysVol->GetLogicalVolume()-> SetVisAttributes(G4VisAttributes::Invisible);

  G4LogicalVolumeStore* MyGDML = G4LogicalVolumeStore::GetInstance();
  // MyGDML->GetVolume("example1")->SetVisAttributes(LightGreen);
  // MyGDML->GetVolume("example2")->SetVisAttributes(LightGreen);
  // MyGDML->GetVolume("example3")->SetVisAttributes(LightGreen);
  return fWorldPhysVol;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal G4UniformGravityField* UCNDetectorConstruction::fField = 0;

void UCNDetectorConstruction::ConstructSDandField()
{
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

     G4double minStep           = 0.01*mm;

     G4ChordFinder* chordFinder =
                   new G4ChordFinder((G4MagneticField*)fField,minStep,stepper);

     // Set accuracy parameters
     G4double deltaChord        = 3.0*mm;
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
