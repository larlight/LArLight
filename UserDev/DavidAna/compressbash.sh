
#!/bin/bash

x=1


while [ $x -le 10 ]
do

  while read line
    do
    iFS="\t" read -a array <<< $line
    break
  done < compression_parameters.txt

  #number of elements in array:
  ArrLen=${#array[@]}

#  if [[ "{{array[0]}" == "prod" ]]
 #     then
#  outfile="/uboone/data/users/davidc1/electrons/WFCompressed_RMS_1p0_"${array[0]}"_var_"${array[1]}"_n_"${array[2]}"_Na_"${array[3]}"_Np_"${array[4]}".root"
  #fi  

#  if [[ "${array[0]}" == "threshold" ]]
#      then
#      outfile="/uboone/data/users/davidc1/cosmics/WFCompressed_2_RMS1p0_"${array[0]}"_"${array[1]}".root"
#      outhisto="compressed_wfs_RMS1_"${array[0]}"_"${array[1]}".root"
#  fi
  
#  if [[ "{{array[0]}" == "variance" ]]
#      then
  outfile="/uboone/data/users/davidc1/cosmics/WFCompressed_2_RMS_3p0_"${array[0]}"_var_"${array[1]}"_n_"${array[2]}"_Na_"${array[3]}"_Np_"${array[4]}".root"
  outhisto="compressed_wfs_RMS3_"${array[0]}"_var_"${array[1]}"_n_"${array[2]}"_Na_"${array[3]}"_Np_"${array[4]}".root"
#  fi  
  
  python mac/compressor.py /uboone/data/users/davidc1/cosmics/cosmics_rms3ADC_lighted.root $outfile $outhisto

  sed -i -e "1d" "compression_parameters.txt"

  x=$(( $x + 1 ))
  
done