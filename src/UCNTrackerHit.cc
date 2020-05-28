
#include "UCNTrackerHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>
#include <fstream>
std::ofstream detectorFile("detector.out");

G4ThreadLocal G4Allocator<UCNTrackerHit>* UCNTrackerHitAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNTrackerHit::UCNTrackerHit()
 : G4VHit(),
   fTime(0.),
   fPos(G4ThreeVector())
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNTrackerHit::~UCNTrackerHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNTrackerHit::UCNTrackerHit(const UCNTrackerHit& right)
  : G4VHit()
{
  fTime      = right.fTime;
  fPos       = right.fPos;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const UCNTrackerHit& UCNTrackerHit::operator=(const UCNTrackerHit& right)
{
  fTime      = right.fTime;
  fPos       = right.fPos;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool UCNTrackerHit::operator==(const UCNTrackerHit& right) const
{
  return ( this == &right ) ? true : false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNTrackerHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(fPos);
    circle.SetScreenSize(4.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNTrackerHit::Print()
{

  G4double timeInSeconds = fTime*1e-9;
  detectorFile << timeInSeconds << ", " << fPos << std::endl;

  //
  // G4cout
  //    << "Time: "
  //    << std::setw(7) << G4BestUnit(fTime,"Time")
  //    << " Position: "
  //    << std::setw(7) << G4BestUnit( fPos,"Length")
  //    << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
