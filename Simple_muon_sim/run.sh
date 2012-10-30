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

exe="../build/Release/Simple_muon_sim"
log="> /dev/null"
n_particles=500000
separation=10
st_x=1 
st_mat=( "Galactic" "AIR" "Cu" ) # will prepend the required 'G4_'
file_name="out.root" # will prepend the other info

for mat in "${st_mat[@]}";
do
    file="sep_"$separation"mm_st-x_"$st_x"mm_st-mat_"$mat"_n-particles_"$n_particles"_"$file_name
    echo $file
    cmd="$exe $separation $st_x G4_$mat $n_particles $file $log"
    echo $cmd
    eval $cmd
done;