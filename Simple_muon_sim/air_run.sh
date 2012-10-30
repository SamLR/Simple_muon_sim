#! /bin/bash

#  make sure environment variables are set

export DYLD_LIBRARY_PATH=/Applications/geant4/geant4_build/outputs/library/Darwin-g++:/Applications/ROOT/root/lib:/Applications/ROOT/root/lib
export G4ABLADATA=/usr/local/share/Geant4-9.5.0/data/G4ABLA3.0
export G4LEDATA=/usr/local/share/Geant4-9.5.0/data/G4EMLOW6.23
export G4LEVELGAMMADATA=/usr/local/share/Geant4-9.5.0/data/PhotonEvaporation2.2
export G4NEUTRONHPDATA=/usr/local/share/Geant4-9.5.0/data/G4NDL4.0
export G4NEUTRONXSDATA=/usr/local/share/Geant4-9.5.0/data/G4NEUTRONXS1.1
export G4PIIDATA=/usr/local/share/Geant4-9.5.0/data/G4PII1.3
export G4RADIOACTIVEDATA=/usr/local/share/Geant4-9.5.0/data/RadioactiveDecay3.4
export G4REALSURFACEDATA=/usr/local/share/Geant4-9.5.0/data/RealSurface1.0
# export G4UI_USE=1
# export G4VIS_USE=1

../build/Release/Simple_muon_sim air_out_500k.root air_run.mac > /dev/null