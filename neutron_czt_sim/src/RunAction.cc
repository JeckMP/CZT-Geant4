#include "RunAction.hh"

#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

RunAction::RunAction()
: fCaptureCount(0),
  fTotalEdep(0.0),
  fPixelHits(fNPixels, std::vector<int>(fNPixels, 0))
{}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*)
{
    fCaptureCount = 0;
    fTotalEdep = 0.0;

    for(auto& row : fPixelHits)
    {
        std::fill(row.begin(), row.end(), 0);
    }

   // fSpectrumFile.open("pure_cd_spectrum.csv");
   fSpectrumFile.open("czt_spectrum.csv");
   fSpectrumFile << "eventID,edep_keV,x_cm,y_cm,z_cm,particle,process,parentID\n";

   // fPhotonFile.open("pure_cd_exit_photons.csv");
   fPhotonFile.open("czt_exit_photons.csv");
   fPhotonFile << "eventID,photonEnergy_keV,x_cm,y_cm,z_cm,creatorProcess\n";
}

void RunAction::EndOfRunAction(const G4Run*)
{
    if(fSpectrumFile.is_open())
    {
        fSpectrumFile.close();
    }

    if(fPhotonFile.is_open())
{
    fPhotonFile.close();
}


   // std::ofstream pixelFile("pure_cd_pixel_hits.csv");
    std::ofstream pixelFile("czt_pixel_hits.csv");
    pixelFile << "ix,iy,hits\n";

    for(int ix = 0; ix < fNPixels; ix++)
    {
        for(int iy = 0; iy < fNPixels; iy++)
        {
            pixelFile << ix << ","
                      << iy << ","
                      << fPixelHits[ix][iy] << "\n";
        }
    }

    pixelFile.close();

    G4cout << G4endl;
    G4cout << "==================================" << G4endl;
    G4cout << "Neutron capture events in CZT: "
           << fCaptureCount << G4endl;
    G4cout << "Total deposited energy in CZT: "
           << fTotalEdep/keV << " keV" << G4endl;
    //G4cout << "Saved: pure_cd_spectrum.csv" << G4endl;
    //G4cout << "Saved: pure_cd_exit_photons.csv" << G4endl;
    //G4cout << "Saved: pure_cd_pixel_hits.csv" << G4endl; 
   G4cout << "Saved: czt_spectrum.csv" << G4endl;
G4cout << "Saved: czt_exit_photons.csv" << G4endl;
G4cout << "Saved: czt_pixel_hits.csv" << G4endl;  

 G4cout << "==================================" << G4endl;
    G4cout << G4endl;
}

void RunAction::AddCaptureHit(G4double x, G4double y)
{
    fCaptureCount++;
    AddPixelHit(x, y);
}
void RunAction::AddEnergyDeposit(G4int eventID,
                                 G4double edep,
                                 G4ThreeVector pos,
                                 G4String particleName,
                                 G4String processName,
                                 G4int parentID)
{
    if(edep <= 0.0) return;

    fTotalEdep += edep;

    AddPixelHit(pos.x(), pos.y());

    if(fSpectrumFile.is_open())
    {
        fSpectrumFile << eventID << ","
                      << edep/keV << ","
                      << pos.x()/cm << ","
                      << pos.y()/cm << ","
                      << pos.z()/cm << ","
                      << particleName << ","
                      << processName << ","
                      << parentID << "\n";
    }
}

void RunAction::AddPixelHit(G4double x, G4double y)
{
G4double detHalfSize = 3.936*cm;
    G4double pixelSize = (2.0*detHalfSize)/fNPixels;

    int ix = static_cast<int>((x + detHalfSize)/pixelSize);
    int iy = static_cast<int>((y + detHalfSize)/pixelSize);

    if(ix >= 0 && ix < fNPixels &&
       iy >= 0 && iy < fNPixels)
    {
        fPixelHits[ix][iy]++;
    }
}

void RunAction::AddExitPhoton(G4int eventID,
                              G4double energy,
                              G4ThreeVector pos,
                              G4String creatorProcess)
{
    if(fPhotonFile.is_open())
    {
        fPhotonFile << eventID << ","
                    << energy/keV << ","
                    << pos.x()/cm << ","
                    << pos.y()/cm << ","
                    << pos.z()/cm << ","
                    << creatorProcess << "\n";
    }
}
