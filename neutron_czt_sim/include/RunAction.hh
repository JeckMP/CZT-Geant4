#ifndef RunAction_h
#define RunAction_h

#include "G4UserRunAction.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"

#include <fstream>
#include <vector>

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    void AddCaptureHit(G4double x, G4double y);
    void AddEnergyDeposit(G4int eventID,
                      G4double edep,
                      G4ThreeVector pos,
                      G4String particleName,
                      G4String processName,
                      G4int parentID);

void AddExitPhoton(G4int eventID,
                   G4double energy,
                   G4ThreeVector pos,
                   G4String creatorProcess);
private:
static const int fNPixels = 32;
    int fCaptureCount;
    G4double fTotalEdep;

    std::vector<std::vector<int>> fPixelHits;

    std::ofstream fSpectrumFile;

    std::ofstream fPhotonFile;

    void AddPixelHit(G4double x, G4double y);
};

#endif
