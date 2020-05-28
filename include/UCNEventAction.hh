#ifndef UCNEventAction_hh
#define UCNEventAction_hh

#include "globals.hh"
#include "G4UserEventAction.hh"

class UCNSnapshot;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class UCNEventAction : public G4UserEventAction
{

  public:
    UCNEventAction();
    virtual ~UCNEventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void   EndOfEventAction(const G4Event* event);

    UCNSnapshot* GetSnapshotPointer() { return fSnapshot; }

  private:

    UCNSnapshot* fSnapshot;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
