#include "UCNMaterialDataHelper.hh"

#include <iostream>
#include <fstream>
#include <string>


UCNMaterialDataHelper::UCNMaterialDataHelper(G4String fileName) :
fMatFileName(fileName)
{
  fDiffusion = 0.0;
  fFermiPot  = 0.0;
  fSpinFlip  = 0.0;
  fLoss      = 0.0;
  fLossCS    = 0.0;
  fAbsCS     = 0.0;
  fScatCS    = 0.0;

  ImportMaterialData();
}

UCNMaterialDataHelper::~UCNMaterialDataHelper(){}

void UCNMaterialDataHelper::ImportMaterialData()
{

  std::string   line;
  std::ifstream matFile(fMatFileName);

  if (matFile.is_open())
  {
    // DIFFUSION
    std::getline (matFile, line);
    fDiffusion = std::stod(line);

    // FERMIPOT
    std::getline (matFile, line);
    fFermiPot = std::stod(line);

    // SPINFLIP
    std::getline (matFile, line);
    fSpinFlip = std::stod(line);

    // LOSS
    std::getline (matFile, line);
    fLoss = std::stod(line);

    // LOSSCS
    std::getline (matFile, line);
    fLossCS = std::stod(line);

    // ABSCS
    std::getline (matFile, line);
    fAbsCS = std::stod(line);

    // SCATCS
    std::getline (matFile, line);
    fScatCS = std::stod(line);

    matFile.close();
  }

  else G4cout << "Material file not found!" << G4endl;

}
