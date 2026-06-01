#include "SteppingAction.hh"
#include "RunAction.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4Step.hh"
#include <cmath>
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"
#include "G4ios.hh"

SteppingAction::SteppingAction(RunAction* runAction)
: fRunAction(runAction)
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    auto track = step->GetTrack();

    if(!track->GetVolume()) return;

    auto volumeName =
        track->GetVolume()->GetLogicalVolume()->GetName();

    if(volumeName != "CdZnTe_Detector")
        return;

    auto postVolume = step->GetPostStepPoint()->GetPhysicalVolume();

auto particleName =
    track->GetDefinition()->GetParticleName();

auto eventID =
    G4RunManager::GetRunManager()
    ->GetCurrentEvent()
    ->GetEventID();

// Record gamma photons exiting the Cd slab
if(particleName == "gamma" &&
   step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary)
{
    if(!postVolume ||
       postVolume->GetLogicalVolume()->GetName() != "CdZnTe_Detector")
    {
        auto pos = step->GetPostStepPoint()->GetPosition();
	auto energy = std::abs(track->GetKineticEnergy());
        auto creator = track->GetCreatorProcess();

G4String creatorProcess = "primary";

if(creator)
{
    creatorProcess = creator->GetProcessName();
}

fRunAction->AddExitPhoton(eventID, energy, pos, creatorProcess);

    }
}

    auto parentID =
        track->GetParentID();

    auto pos =
        step->GetPostStepPoint()->GetPosition();

    auto edep =
        step->GetTotalEnergyDeposit();

    auto process =
        step->GetPostStepPoint()->GetProcessDefinedStep();

    G4String processName = "Unknown";

    if(process)
    {
        processName = process->GetProcessName();
    }

    // Count true neutron capture events in CZT.
    if(particleName == "neutron" && processName == "nCapture")
    {
        fRunAction->AddCaptureHit(pos.x(), pos.y());

       //    G4cout << "Neutron capture in CZT at "
         //      << "x=" << pos.x()/cm << " cm, "
           //    << "y=" << pos.y()/cm << " cm, "
             //  << "z=" << pos.z()/cm << " cm"
             //  << G4endl;

        // DO NOT kill the neutron here.
        // Let Geant4 generate and transport secondary gammas/electrons.
    }

    // Score actual detector signal: deposited energy from secondaries.
if(edep > 0*keV)
    {
auto eventID =
    G4RunManager::GetRunManager()
    ->GetCurrentEvent()
    ->GetEventID();

fRunAction->AddEnergyDeposit(
    eventID,
    edep,
    pos,
    particleName,
    processName,
    parentID
);
      //  G4cout << "Energy deposit in CZT: "
        //       << "particle=" << particleName
          //     << ", parentID=" << parentID
            //   << ", process=" << processName
             //  << ", edep=" << edep/keV << " keV"
            //   << G4endl;
    }
}
