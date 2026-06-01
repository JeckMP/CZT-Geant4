#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

DetectorConstruction::DetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    auto nist = G4NistManager::Instance();

    // -------------------------------------------------------------------------
    // Materials
    // -------------------------------------------------------------------------
    auto worldMat = nist->FindOrBuildMaterial("G4_AIR");

    // Pure Cd detector option.
    // Uncomment this if the detector should be pure cadmium instead of CZT.
    // auto cdMat = nist->FindOrBuildMaterial("G4_Cd");

    // CZT detector material: Cd0.45 Zn0.05 Te0.50 by fraction.
    auto Cd = nist->FindOrBuildElement("Cd");
    auto Zn = nist->FindOrBuildElement("Zn");
    auto Te = nist->FindOrBuildElement("Te");

    auto cztMat = new G4Material(
        "CdZnTe",
        5.78 * g / cm3,
        3
    );

    cztMat->AddElement(Cd, 0.45);
    cztMat->AddElement(Zn, 0.05);
    cztMat->AddElement(Te, 0.50);

    // LiF converter material.
    auto Li = nist->FindOrBuildElement("Li");
    auto F  = nist->FindOrBuildElement("F");

    auto lifMat = new G4Material(
        "LiF",
        2.64 * g / cm3,
        2
    );

    lifMat->AddElement(Li, 1);
    lifMat->AddElement(F, 1);

    // B4C converter material.
    // Uncomment this block when using B4C as the active converter.
    // auto B = nist->FindOrBuildElement("B");
    // auto C = nist->FindOrBuildElement("C");
    //
    // auto b4cMat = new G4Material(
    //     "B4C",
    //     2.52 * g / cm3,
    //     2
    // );
    //
    // b4cMat->AddElement(B, 4);
    // b4cMat->AddElement(C, 1);

    // Gd converter material.
    // Uncomment this block when using Gd as the active converter.
    // auto Gd = nist->FindOrBuildElement("Gd");
    //
    // auto gdMat = new G4Material(
    //     "Gd",
    //     7.90 * g / cm3,
    //     1
    // );
    //
    // gdMat->AddElement(Gd, 1.0);

    // -------------------------------------------------------------------------
    // World
    // -------------------------------------------------------------------------
    auto solidWorld = new G4Box(
        "World",
        50 * cm,
        50 * cm,
        50 * cm
    );

    auto logicWorld = new G4LogicalVolume(
        solidWorld,
        worldMat,
        "World"
    );

    auto physWorld = new G4PVPlacement(
        nullptr,
        G4ThreeVector(),
        logicWorld,
        "World",
        nullptr,
        false,
        0,
        true
    );

    logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

    // -------------------------------------------------------------------------
    // Detector
    // -------------------------------------------------------------------------
    // Detector options:
    //   CdZnTe_Detector = CZT detector
    //   Cd detector     = pure Cd validation detector, if cdMat is used
    //
    // Area = 7.872 cm x 7.872 cm
    // Current full thickness = 5 mm
    auto solidDetector = new G4Box(
        "CdZnTe_Detector",
        3.936 * cm,
        3.936 * cm,
        // 4.0 * mm
        // 1.0 * mm
        // 5.0 * mm
        // 0.5 * mm
        2.5 * mm
    );

    auto logicDetector = new G4LogicalVolume(
        solidDetector,
        cztMat,
        "CdZnTe_Detector"
    );

    // -------------------------------------------------------------------------
    // Active converter option: LiF
    // -------------------------------------------------------------------------
    // LiF neutron converter layer.
    G4double converterHalfThickness = 8.5 * um;  // 17 um total LiF

    auto solidConverter = new G4Box(
        "LiF_Converter",
        3.936 * cm,
        3.936 * cm,
        converterHalfThickness
    );

    auto logicConverter = new G4LogicalVolume(
        solidConverter,
        lifMat,
        "LiF_Converter"
    );

    new G4PVPlacement(
        nullptr,
        G4ThreeVector(0, 0, -2.5 * mm - converterHalfThickness),
        logicConverter,
        "LiF_Converter",
        logicWorld,
        false,
        0,
        true
    );

    // -------------------------------------------------------------------------
    // Alternative converter option: B4C
    // -------------------------------------------------------------------------
    // B4C neutron converter layer.
    // Uncomment this block and comment out the active LiF block above
    // when using B4C as the active converter.
    //
    // G4double converterHalfThickness = 0.5 * um;  // 1 um total B4C
    //
    // auto solidConverter = new G4Box(
    //     "B4C_Converter",
    //     3.936 * cm,
    //     3.936 * cm,
    //     converterHalfThickness
    // );
    //
    // auto logicConverter = new G4LogicalVolume(
    //     solidConverter,
    //     b4cMat,
    //     "B4C_Converter"
    // );
    //
    // new G4PVPlacement(
    //     nullptr,
    //     G4ThreeVector(0, 0, -2.5 * mm - converterHalfThickness),
    //     logicConverter,
    //     "B4C_Converter",
    //     logicWorld,
    //     false,
    //     0,
    //     true
    // );

    // -------------------------------------------------------------------------
    // Alternative converter option: Gd
    // -------------------------------------------------------------------------
    // Gd neutron converter layer.
    // Uncomment this block and comment out the active LiF/B4C block above
    // when using Gd as the active converter.
    //
    // G4double gdHalfThickness = 0.025 * um;
    //
    // auto solidGdConverter = new G4Box(
    //     "Gd_Converter",
    //     3.936 * cm,
    //     3.936 * cm,
    //     gdHalfThickness
    // );
    //
    // auto logicGdConverter = new G4LogicalVolume(
    //     solidGdConverter,
    //     gdMat,
    //     "Gd_Converter"
    // );
    //
    // new G4PVPlacement(
    //     nullptr,
    //     G4ThreeVector(0, 0, -2.5 * mm - gdHalfThickness),
    //     logicGdConverter,
    //     "Gd_Converter",
    //     logicWorld,
    //     false,
    //     0,
    //     true
    // );

    // -------------------------------------------------------------------------
    // Visualization
    // -------------------------------------------------------------------------
    auto detectorVis = new G4VisAttributes(G4Colour(0.6, 0.6, 0.6));
    detectorVis->SetVisibility(true);
    detectorVis->SetForceSolid(true);
    logicDetector->SetVisAttributes(detectorVis);

    // -------------------------------------------------------------------------
    // Detector placement
    // -------------------------------------------------------------------------
    new G4PVPlacement(
        nullptr,
        G4ThreeVector(0, 0, 20 * cm),
        logicDetector,
        "CdZnTe_Detector",
        logicWorld,
        false,
        0,
        true
    );

    return physWorld;
}
