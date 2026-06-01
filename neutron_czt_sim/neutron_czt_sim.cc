#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#include "QGSP_BIC_HP.hh"

int main(int argc, char** argv)
{
auto runManager =
    G4RunManagerFactory::CreateRunManager(G4RunManagerType::SerialOnly);

    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new QGSP_BIC_HP);

    runManager->SetUserAction(new PrimaryGeneratorAction());

    auto runAction = new RunAction();
    runManager->SetUserAction(runAction);
    runManager->SetUserAction(new SteppingAction(runAction));

    runManager->Initialize();

    auto uiManager = G4UImanager::GetUIpointer();

    if(argc > 1)
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        uiManager->ApplyCommand(command + fileName);

        delete runManager;
        return 0;
    }

    auto visManager = new G4VisExecutive;
    visManager->Initialize();

    auto ui = new G4UIExecutive(argc, argv);
    uiManager->ApplyCommand("/control/execute macros/vis.mac");
    ui->SessionStart();

    delete ui;
    delete visManager;
    delete runManager;

    return 0;
}
