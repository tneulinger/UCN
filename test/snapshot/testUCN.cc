#include "globals.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

#include "UCNSnapshot.hh"

#include <fstream>

int main()
{

  G4String timesToSnapshotFile = "timesToSnapshot.txt";
  G4String outputFile = "outputFile.txt";

  UCNSnapshot* snapshot = new UCNSnapshot(timesToSnapshotFile, outputFile);

  snapshot->PrintSnapshotTimes();

  G4double time = 10.0*s;

  G4double energy = 13.0*MeV;

  G4ThreeVector position;
  position[0] = 1.0*mm;
  position[1] = 1.5*mm;
  position[2] = 5.0*cm;

  G4ThreeVector momentum;
  momentum[0] = 14.0*MeV/c_light;
  momentum[1] = 1.0*MeV/c_light;
  momentum[2] = 4.0*MeV/c_light;

  snapshot->SetTime(time);
  snapshot->SetEnergy(energy);
  snapshot->SetPosition(position);
  snapshot->SetMomentum(momentum);

  snapshot->WriteTimeEnergyPositionMomentum();
  snapshot->WriteTimeEnergyPositionMomentum();
  snapshot->WriteTimeEnergyPositionMomentum();
  snapshot->WriteTimeEnergyPositionMomentum();

  delete snapshot;


  return 0;
}
