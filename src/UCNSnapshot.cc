#include "UCNSnapshot.hh"

#include <iostream>
#include <fstream>
#include <string>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNSnapshot::UCNSnapshot(G4String timesToSnapshotFile, G4String outputFile) :
fTimesToSnapshotFile(timesToSnapshotFile),
fOutputFile(outputFile),
fSnapshotIndex{0}
{
  // get the snapshot times from file
  ImportSnapshotTimes();

  // open output file stream
  fOutStream.open(fOutputFile, std::ios_base::app); // append to file

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UCNSnapshot::~UCNSnapshot()
{
  // close the output file stream
  fOutStream.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNSnapshot::ImportSnapshotTimes()
{
  // open file with list of times at which to take snapshots
  std::ifstream in(fTimesToSnapshotFile);

  // loop through the file and get the times, also set snapshot status to false
  // (very first line is the time window)
  std::string line;
  G4double    snapshotTime;
  G4bool      initialSnapshotStatus = false;
  bool flag = true;
  while (std::getline(in, line))
  {
    if (flag)
    {
      fTimeWindow = std::stod(line);
      flag = false;
    } else {
      snapshotTime = std::stod(line);
      fTimesToSnapshot.push_back(snapshotTime);
      fSnapshotStatus.push_back(initialSnapshotStatus);
    }
  }

  // close the file
  in.close();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNSnapshot::PrintSnapshotTimes()
{
  for (std::vector<G4double>::size_type i=0; i<fTimesToSnapshot.size(); i++)
  {
    G4cout << fTimesToSnapshot[i] << " ";
  }
  G4cout << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNSnapshot::ResetSnapshotStatus()
{
  G4bool initialSnapshotStatus = false;
  for (std::vector<G4bool>::size_type i=0; i<fSnapshotStatus.size(); i++)
  {
    fSnapshotStatus[i] = initialSnapshotStatus;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool UCNSnapshot::ShouldWeTakeASnapshotNow(G4double theTimeNowInNanoSeconds)
{

  G4double theTimeNow = theTimeNowInNanoSeconds*1e-9; // convert to time in seconds

  G4double currentSnapshotTime = fTimesToSnapshot[fSnapshotIndex];
  G4bool   alreadyTookSnapshot = fSnapshotStatus[fSnapshotIndex];

  // if we already took a snapshot ...
  if (alreadyTookSnapshot)
  {
    return false;
  }

  // if we haven't taken a snapshot ...
  if (!alreadyTookSnapshot)
  {
    // ...and if the time now is within the time window of the current time
    if ((theTimeNow >= currentSnapshotTime) && (theTimeNow <= currentSnapshotTime + fTimeWindow))
    {
      // ...then we should take a snapshot now.
      return true;
    } else {
      // ...otherwise the time is wrong.
      return false;
    }
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UCNSnapshot::WriteTimeEnergyPositionMomentum()
{
  // get energy in neV
  G4double EneV = fEnergy*1e6*1e9;

  // get velocity (in m/s) from momentum (in MeV/c)
  // first multiply by 1e6*q_e/c to convert to SI, then divide by m_n
  // 1e6 = 1e6 eV per MeV
  // q_e = 1.602176634e-19 J per eV (J = kg*m^2/S^2)
  // c   = 299792458 m/s (not in mm/ns! for some reason...)
  // m_n = 1.67492749804e-27 kg
  G4ThreeVector Vmps = fMomentum*319075.6614201027;

  // write information to file
  fOutStream << fTime << ", " << EneV << ", " << fPosition << ", " << Vmps << "\n";

  // mark the status of this snapshot as taken
  fSnapshotStatus[fSnapshotIndex] = true;

  // move on to the next snapshot time
  fSnapshotIndex++;

}
