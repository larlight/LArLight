#!/bin/bash

x=1


while [ $x -le 9 ]
do

  while read line
    do
    iFS="\t" read -a array <<< $line
    break
  done < compression_parameters.txt
  
  outfile="/uboone/data/users/davidc1/electrons/WF_compressed_RMS_1_var_"${array[0]}"_n_"${array[1]}"_Na_"${array[2]}"_Np_"${array[3]}".root"

  python mac/compressor.py /uboone/data/users/davidc1/electrons/straight_electrons_RMS1p0_light.root $outfile

  sed -i -e "1d" "compression_parameters.txt"

  x=$(( $x + 1 ))
done