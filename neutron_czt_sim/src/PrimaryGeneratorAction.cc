#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include <cmath>

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    fParticleGun = new G4ParticleGun(1);

    auto particle =
        G4ParticleTable::GetParticleTable()->FindParticle("neutron");

    fParticleGun->SetParticleDefinition(particle);

    // -------------------------------------------------------------------------
    // Neutron energy selection
    // -------------------------------------------------------------------------
    // Reference thermal neutron energy:
    // 25.3 meV
    // fParticleGun->SetParticleEnergy(25.3e-9 * MeV);

    // MAPD/MAD beamline-relevant neutron energy:
    // 17.1 meV
    // fParticleGun->SetParticleEnergy(17.1e-9 * MeV);

    // MacSANS 2.6 A neutron energy:
    // 12.1 meV
    fParticleGun->SetParticleEnergy(12.1e-9 * MeV);

    // Cold neutron beam (~4.0 A):
    // 5.1 meV
    // fParticleGun->SetParticleEnergy(5.1e-9 * MeV);

    // Beam travels along +z.
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    // Circular beam:
    // diameter = 7.3 cm
    // radius   = 3.65 cm
    G4double beamRadius = 3.65 * cm;

    G4double r =
        beamRadius * std::sqrt(G4UniformRand());

    G4double phi =
        2.0 * CLHEP::pi * G4UniformRand();

    G4double x = r * std::cos(phi);
    G4double y = r * std::sin(phi);

    // Source plane upstream of object and detector.
    fParticleGun->SetParticlePosition(
        G4ThreeVector(x, y, -20 * cm)
    );

    fParticleGun->GeneratePrimaryVertex(event);
}
