// Tool to output to a file the UCN energy, position, and momentum at desired
// times (within some time window). The desired times are to be listed in a file
// in seconds, the first line being the time window in seconds.
// Input file:  "snapshots.in"
// Output file: "snapshots.out"

#ifndef UCNSnapshot_hh
#define UCNSnapshot_hh

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <vector>
#include <fstream>

class UCNSnapshot
{
  public:
    UCNSnapshot(G4String timesToSnapshotFile, G4String outputFile);
    ~UCNSnapshot();

    // method to test if we should record the current time from the G4Track into
    // our list of snapshots.
    G4bool ShouldWeTakeASnapshotNow(G4double theTimeNowInNanoSeconds);

    // methods to get the temporary time, energy, position, and momentum
    G4double GetTime() { return fTime; }
    G4double GetEnergy() { return fEnergy; }
    G4ThreeVector GetPosition() { return fPosition; }
    G4ThreeVector GetMomentum() { return fMomentum; }

    // methods to set the temporary time, energy, position, and momentum
    void SetTime(G4double time) { fTime = time; }
    void SetEnergy(G4double energy) { fEnergy = energy; }
    void SetPosition(G4ThreeVector position) {fPosition = position; }
    void SetMomentum(G4ThreeVector momentum) {fMomentum = momentum; }

    // method to write time, energy, position, and momentum to next line in output file
    void WriteTimeEnergyPositionMomentum();

    // print the vector of times at which the snapshots are to be taken
    void PrintSnapshotTimes();

    // reset all the snapshot flags and index
    void ResetSnapshotStatus();
    void ResetSnapshotIndex() { fSnapshotIndex = 0; }

    // flush output stream
    void FlushOutputStream() { fOutStream.flush(); }

  private:

    // read time window and snapshot times from the file
    void ImportSnapshotTimes();

    // file things for input and out
    G4String      fTimesToSnapshotFile; // input file name with snapshot times
    G4String      fOutputFile;          // output file name
    std::ofstream fOutStream;           // output file stream

    // snapshot timing information
    G4int                 fSnapshotIndex;   // index to keep track of which snapshot we are on
    G4double              fTimeWindow;      // in seconds
    std::vector<G4double> fTimesToSnapshot; // in seconds
    std::vector<G4bool>   fSnapshotStatus;

    // temporary variables to store time, energy, position, and momentum
    G4double      fTime;
    G4double      fEnergy;
    G4ThreeVector fPosition;
    G4ThreeVector fMomentum;

};

#endif
