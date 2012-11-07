//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: exampleN01.cc,v 1.6 2006-06-29 17:47:10 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// --------------------------------------------------------------
//      GEANT 4 - exampleN01
// --------------------------------------------------------------

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "TString.h"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "Root.hh"
#include "SteppingAction.hh"
#include "EventAction.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

// if args are incorrect (or the help message is requested) check_args exits
void check_args(int argc,char** argv);

int main(int argc,char** argv) {
    
    check_args(argc,argv);
    
    G4double separation = atof(argv[1]);
    G4double st_x = atof(argv[2]);
    G4String st_mat = G4String(argv[3]);
    G4int n_particles = atoi(argv[4]);
    G4String particle_type = G4String(argv[5]);
    G4String out_root_name = G4String(argv[6]);
    
    // Construct the default run manager
    //
    G4RunManager* runManager = new G4RunManager;
    
    // set mandatory initialization classes
    //
    G4VUserDetectorConstruction* detector
                = new DetectorConstruction(separation, st_x,st_mat);
    runManager->SetUserInitialization(detector);
    //
    PhysicsList* physics = new PhysicsList;
    
    runManager->SetUserInitialization(physics);
    
    // set mandatory user action class
    //
    PrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction(particle_type,separation+st_x);
    runManager->SetUserAction(gen_action);
    
    
    // Make the root object and set the stepping & event actions
    Root* root = new Root(out_root_name);
    EventAction* event = new EventAction(root);
    runManager->SetUserAction(event);
    
    SteppingAction* step = new SteppingAction(root);
    runManager->SetUserAction(step);
    
    // Initialize G4 kernel
    //
    runManager->Initialize();
    runManager->BeamOn(n_particles);
    
    delete root;
    delete runManager;
    
    return 0;
}


void check_args(int argc,char** argv) {
    if (argc > 1 && G4String(argv[1]) == G4String("-h")) {
        G4cout << "usage: Simple_muon_sim <counter separation> "
            << "<target_thickness> <target_material> "
            << "<n_initial_particles> <initial_particle_type> <out_root_file> "
            <<G4endl<<G4endl;
        G4cout <<"<counter separation>: distance between counters in mm; in "
            << "addition to target thickness"
            <<G4endl;
        G4cout <<"<target_thickness> in mm"
            <<G4endl;
        G4cout <<"<target_material> from the Geant4 NIST database (e.g. G4_AIR)"
            <<G4endl;
        G4cout <<"<n_initial_particles> number of events to generate"
            <<G4endl;
        G4cout <<"<initial_particle_type> including charge (e.g. mu+)"
            <<G4endl;
        G4cout <<"<out_root_file> where to save the output "
            <<G4endl;
        exit(0);
    } else if (argc!=7) {
        G4cout << "usage: Simple_muon_sim "
            << "<counter separation> "
            << "<target_thickness> "
            << "<target_material> "
            << "<n_intitial_particles> "
            << "<initial_particle_type> "
            << "<out file> "
        <<G4endl;
        G4cout << "For more information use `Simple_muon_sim -h`"<<G4endl;
        exit(1);
    }  
}