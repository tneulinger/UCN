#ifndef UCNSteppingAction_hh
#define UCNSteppingAction_hh

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"

class UCNEventAction;
class UCNSnapshot;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class UCNSteppingAction : public G4UserSteppingAction
{

  public:
    UCNSteppingAction(UCNEventAction* eventAction);
    virtual ~UCNSteppingAction();

    virtual void UserSteppingAction(const G4Step*);

  private:
    UCNEventAction* fEventAction;
    UCNSnapshot*    fSnapshot;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
