// Helper class to import seven UCN materials properties from text file.
// File structure should be as follows:
// LINE1: "DBL   //DIFFUSION" a probability
// LINE2: "DBL   //FERMIPOT"  in neV
// LINE3: "DBL   //SPINFLIP"  a probability
// LINE4: "DBL   //LOSS"      eta (or f) = W/V
// LINE5: "DBL   //LOSSCS"    in barns, zero if ABSC != 0
// LINE6: "DBL   //ABSCS"     in barns, preferred to LOSSCS
// LINE7: "DBL   //SCATCS"    in barns

#ifndef UCNMaterialDataHelper_hh
#define UCNMaterialDataHelper_hh

#include "globals.hh"

class UCNMaterialDataHelper
{
  public:
    UCNMaterialDataHelper(G4String matFileName);
    ~UCNMaterialDataHelper();

    G4double GetDiffusion() { return fDiffusion; }
    G4double GetFermiPot()  { return fFermiPot;  }
    G4double GetSpinFlip()  { return fSpinFlip;  }
    G4double GetLoss()      { return fLoss;      }
    G4double GetLossCS()    { return fLossCS;    }
    G4double GetAbsCS()     { return fAbsCS;     }
    G4double GetScatCS()    { return fScatCS;    }

  private:

    void ImportMaterialData();

    G4String fMatFileName;

    G4double fDiffusion;
    G4double fFermiPot;
    G4double fSpinFlip;
    G4double fLoss;
    G4double fLossCS;
    G4double fAbsCS;
    G4double fScatCS;
};

#endif
