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
// $Id: DetectorConstruction.cc,v 1.9 2006-06-29 17:47:19 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include <limits>

#include "DetectorConstruction.hh"
//#include "DetectorConstructionMessenger.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"
#include "globals.hh"

DetectorConstruction::DetectorConstruction(G4double separation, G4double st_x, G4String st_mat)
    :m_separation(separation), m_st_x(st_x*mm), m_st_mat(0), m_mat_name(st_mat),
    m_expHall_log(0),  m_scint1_log(0),  m_scint2_log(0), m_st_log(0),
    m_expHall_phys(0), m_scint1_phys(0), m_scint2_phys(0), m_st_phys(0)
{;}

DetectorConstruction::~DetectorConstruction() {
    delete m_st_log;
    delete m_st_phys;
    delete m_scint1_phys;
    delete m_scint2_phys;
    delete m_scint1_log;
    delete m_scint2_log;
    delete m_expHall_phys;
    delete m_expHall_log;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    
    //
    // Use the NIST material manager
    // Not sure if this includes optical properties, need to confirm
    //
    G4NistManager* material_manager = G4NistManager::Instance();
    G4Material* Air = material_manager->FindOrBuildMaterial("G4_AIR");

    //------------------------------------------------------ volumes
    
    //------------------------------ experimental hall (world volume)
    //------------------------------ beam line along x axis
    
    G4double expHall_x = 50*cm;
    G4double expHall_y = 50*cm;
    G4double expHall_z = 50*cm;
    G4Box* expHall_box = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);
    m_expHall_log = new G4LogicalVolume(expHall_box, Air, "expHall_log");
    m_expHall_phys = new G4PVPlacement(0,G4ThreeVector(), m_expHall_log,"expHall",0,false,0);
    
    G4double scint_x = 10*mm;
    G4double scint_y = 100*mm;
    G4double scint_z = 100*mm;
    G4Box* scint_box = new G4Box("scint_box", scint_x/2, scint_y/2, scint_z/2);
    G4double x_pos = scint_x/2 + m_separation/2 + m_st_x/2;

    m_scint1_log = new G4LogicalVolume(scint_box, Air, "scintD");
    G4ThreeVector scint1_pos(x_pos, 0, 0);
    m_scint1_phys = new G4PVPlacement(0, scint1_pos, m_scint1_log, "scintD_p", m_expHall_log, false, 0);

    m_scint2_log = new G4LogicalVolume(scint_box, Air, "scintU");
    G4ThreeVector scint2_pos(-x_pos, 0, 0);
    m_scint2_phys = new G4PVPlacement(0, scint2_pos, m_scint2_log, "scintU_p", m_expHall_log, false, 0);
    

    // Check if the value not is (reasonably) equal to zero
    G4Box* st_box = new G4Box("st_box", m_st_x/2, scint_y/2, scint_z/2);
    m_st_mat = material_manager->FindOrBuildMaterial(m_mat_name);
    if (!m_st_mat) {
        G4cerr << "Material: "<<m_mat_name<<" not found. Check for it's existance in the Geant4 NIST materials database"<<G4endl;
        exit(1);
    }
    G4ThreeVector st_pos(0.0, 0.0, 0.0);
    m_st_log = new G4LogicalVolume(st_box, m_st_mat, "st");
    m_st_phys = new G4PVPlacement(0, st_pos, m_st_log, "st_p", m_expHall_log, false, 0);

    return m_expHall_phys;
}

