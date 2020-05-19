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

UCNDetectorConstruction::UCNDetectorConstruction()
 : fVacuum(0), fGuideMaterial(0)
{
  // materials
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

  // --- Ni diffuse 10%

  G4UCNMaterialPropertiesTable* MPT = new G4UCNMaterialPropertiesTable();

  //  MPT->AddConstProperty("REFLECTIVITY",1.);
  //  Commented out above line as REFLECTIVITY=1 by default in
  //  G4OpBoundaryProcess.  Also use AddProperty to set REFLECTIVITY if needed
  MPT->AddConstProperty("DIFFUSION",0.1);
  MPT->AddConstProperty("FERMIPOT",252.0); // Gollub, Table 2.1 in neV
  MPT->AddConstProperty("SPINFLIP",0.);
  // MPT->AddConstProperty("LOSS", 12.5e-5); //  Gollub, Table 2.1, f = W/V
  MPT->AddConstProperty("LOSS", 0);
  MPT->AddConstProperty("LOSSCS",0.);

  // this number is the absorption cross section for 2200 m/s neutrons
  MPT->AddConstProperty("ABSCS",4.49); // 1/v loss cross-section  at room temp.

  // this number is the total bound scattering cross section for natural nickel
  // in barns
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
  //
  // World
  //

  G4double worldSizeX =  1.*m;
  G4double worldSizeY =  1.*m;
  G4double worldSizeZ =  1.*m;

  G4Box* solidWorld = new G4Box("World",
                                worldSizeX/2.,worldSizeY/2.,worldSizeZ/2.);

  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,
                                                    fVacuum,
                                                    "World");

  G4VPhysicalVolume* physiWorld = new G4PVPlacement(0,
                                                    G4ThreeVector(),
                                                    "World",
                                                    logicWorld,
                                                    0,
                                                    false,
                                                    0);

// --------------------------------- Guide -------------------------------------

  G4double GuideR =  45.*mm;
  G4double GuideW =   2.*mm;
  G4double GuideL = 500.*mm;

  G4Tubs* solidGuide = new G4Tubs("SolidGuide",
                                  GuideR,GuideR+GuideW,GuideL/2.,0.,twopi);

  G4LogicalVolume* logicGuide = new G4LogicalVolume(solidGuide,
                                                    fGuideMaterial,
                                                    "Guide");

  new G4PVPlacement(0,G4ThreeVector(),"Guide",logicGuide,physiWorld,false,0);

// ------------------------------ End Plate 1  ---------------------------------

  G4Tubs* solidEndPlate1 = new G4Tubs("EndPlate1",0.,GuideR, GuideW/2., 0.,twopi);

  G4LogicalVolume* logicEndPlate1 = new G4LogicalVolume(solidEndPlate1,
                                                       fGuideMaterial,
                                                       "EndPlate1");

  G4ThreeVector endPlatePos1 = G4ThreeVector(0.,0.,GuideL/2.+GuideW/2.);

  new G4PVPlacement(0,endPlatePos1,"EndPlate1",logicEndPlate1,physiWorld,false,0);

// ------------------------------ End Plate 2  ---------------------------------

  G4Tubs* solidEndPlate2 = new G4Tubs("EndPlate2",0.,GuideR, GuideW/2., 0.,twopi);

  G4LogicalVolume* logicEndPlate2 = new G4LogicalVolume(solidEndPlate2,
                                                       fGuideMaterial,
                                                       "EndPlate2");

  G4ThreeVector endPlatePos2 = G4ThreeVector(0.,0.,-GuideL/2.-GuideW/2.);

  new G4PVPlacement(0,endPlatePos2,"EndPlate2",logicEndPlate2,physiWorld,false,0);

// -----------------------------------------------------------------------------

  G4double maxStep = 1*mm;
  G4double maxTime = 5.01*s;

  G4UserLimits* stepLimit = new G4UserLimits(maxStep,DBL_MAX,maxTime);
  // G4UserLimits* stepLimit = new G4UserLimits(maxStep,DBL_MAX,DBL_MAX);

  logicWorld->SetUserLimits(stepLimit);

  //
  // Visualization attributes
  //

  G4VisAttributes* guideColor = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  guideColor->SetVisibility(true);
  guideColor->SetForceWireframe(true);

  G4VisAttributes* endPlateColor1 = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  endPlateColor1->SetVisibility(true);
  endPlateColor1->SetForceWireframe(true);

  G4VisAttributes* endPlateColor2 = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  endPlateColor2->SetVisibility(true);
  endPlateColor2->SetForceWireframe(true);

  logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
  logicGuide->SetVisAttributes(guideColor);
  logicEndPlate1->SetVisAttributes(endPlateColor1);
  logicEndPlate2->SetVisAttributes(endPlateColor2);

  //
  //always return the physical World
  //
  return physiWorld;
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
