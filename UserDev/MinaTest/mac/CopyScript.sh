#!/bin/bash                                                                                                                                                                          

echo "Script to secure copy histograms from uboonegpvm05"

for (( i=1; i<=76; i++ ))
do
    echo "Copying file number $i"
    scp ehimwich@uboonegpvm05.fnal.gov:/uboone/app/users/ehimwich/MCTruthStudy4/srcs/larexamples/DarkPhotonAna/DarkPhotonCosmicHist__${i}.root .
    
done

