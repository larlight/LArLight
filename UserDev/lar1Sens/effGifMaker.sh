#!/bin/bash

# let x=0
# points=""
# for (( i = 0; i < 10; i++ )); do
#   x=$(echo "$x + 0.005" | bc)
#   points="$points $x"
#   echo $x
# done

files_norm=$(ls events_norm*)
files_eff=$(ls eff_*)

num=$(ls events_norm* | grep e -c)

list=""
for (( i = 0; i < $num; i++ )); do
  fnorm=$(echo $files_norm | tr ' ' '\n' |  awk -v n=$i 'NR==n')
  eff=$(echo $files_eff | tr ' ' '\n' |  awk -v n=$i 'NR==n')

  montage -geometry 700 -tile 2x1 $eff $fnorm combo_${i}.png
  list="$list combo_${i}.png"
  echo "Finished $i."
  #statements
done 

convert -delay 15 $list eff.gif

# montage -geometry 400 -tile 1x2 events_norm_0.015.png events_unnorm_0.015.png