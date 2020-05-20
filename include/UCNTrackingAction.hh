#ifndef UCNTrackingAction_hh
#define UCNTrackingAction_hh

#include "G4UserTrackingAction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class UCNTrackingAction : public G4UserTrackingAction {

  public:
    UCNTrackingAction();
   ~UCNTrackingAction();

    virtual void  PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

    void PrintTimeAlive();

  private:
    G4double fTime_birth,  fTime_end, fTimeAlive;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
