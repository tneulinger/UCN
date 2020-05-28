#include "globals.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

#include "UCNSnapshot.hh"

#include <fstream>
#include <vector>

int main()
{
  G4double time;
  G4double energy;
  G4ThreeVector position;
  G4ThreeVector momentum;

  // generate fake track times in nanoseconds
  std::vector<G4double> trackTimes;
  int N = 200;
  for (int i=0; i<N; i++)
  {
    trackTimes.push_back( (i*0.5+0.1)*1e9);
  }


  // initialize the snapshot object
  G4String timesToSnapshotFile = "timesToSnapshot.txt";
  G4String outputFile = "Snapshots.txt";
  UCNSnapshot* snapshot = new UCNSnapshot(timesToSnapshotFile, outputFile);


  // go through the track times and take snapshots
  G4double theTimeNowInNanoSeconds;
  G4bool shouldWe = false;
  for (int i=0; i<N; i++)
  {
    theTimeNowInNanoSeconds = trackTimes[i];

    shouldWe = snapshot->ShouldWeTakeASnapshotNow(theTimeNowInNanoSeconds);

    if (shouldWe)
    {
      time = theTimeNowInNanoSeconds*1e-9;
      energy = time;
      position[0] = time;
      position[1] = time;
      position[2] = time;
      momentum[0] = time;
      momentum[1] = time;
      momentum[2] = time;

      snapshot->SetTime(time);
      snapshot->SetEnergy(energy);
      snapshot->SetPosition(position);
      snapshot->SetMomentum(momentum);

      snapshot->WriteTimeEnergyPositionMomentum();

    }

  }

  delete snapshot;


  return 0;
}
