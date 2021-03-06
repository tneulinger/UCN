#ifndef UCNTrackerHit_h
#define UCNTrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

/// Tracker hit class
///
/// It defines data members to store the trackID, chamberNb, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fChamberNB, fEdep, fPos

class UCNTrackerHit : public G4VHit
{
  public:
    UCNTrackerHit();
    UCNTrackerHit(const UCNTrackerHit&);
    virtual ~UCNTrackerHit();

    // operators
    const UCNTrackerHit& operator=(const UCNTrackerHit&);
    G4bool operator==(const UCNTrackerHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Set methods
    void SetPos      (G4ThreeVector xyz){ fPos = xyz; };
    void SetTime     (G4double time)    { fTime = time; };

    // Get methods
    G4ThreeVector GetPos() const { return fPos; };
    G4double GetTime() const     { return fTime; };

  private:

      G4int         fTrackID;
      G4ThreeVector fPos;
      G4double      fTime;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<UCNTrackerHit> UCNTrackerHitsCollection;

extern G4ThreadLocal G4Allocator<UCNTrackerHit>* UCNTrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* UCNTrackerHit::operator new(size_t)
{
  if(!UCNTrackerHitAllocator)
      UCNTrackerHitAllocator = new G4Allocator<UCNTrackerHit>;
  return (void *) UCNTrackerHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void UCNTrackerHit::operator delete(void *hit)
{
  UCNTrackerHitAllocator->FreeSingle((UCNTrackerHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
