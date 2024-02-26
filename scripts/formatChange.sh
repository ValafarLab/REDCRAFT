shopt -s nullglob
for file in "$@"
do
	declare -a comments1=("temp" "C" "N" "C" "CA" "HA" "HA")
	declare -a comments2=("temp" "N" "H" "H" "HA" "H" "H")
	declare -a shift1=(8 -1 0 -1 0 0 -1)
	linecount=1
	atomcount=0
	chain=" "
	while read -r q line
	do
		linecount=$(($linecount+1))
	done <"$file"
		for i in $(seq 1 $linecount);
		do
			y=$(((i-1) % 7))
			if [ "$y" -eq 0 ]
			then
				string=`sed "${i}q;d" $file`
				echo "$string" >> "nef$file"
				chain="$( echo "$string" | cut -d ' ' -f1) "
				atomcount=$((atomcount+1))
				
			elif [ "$y" -ne 0 ]
			then
				a=$(sed "${i}q;d" $file)
				resNum=$(($atomcount+${shift1[$y]}))
				res1="$atomcount "
				res2="$resNum "
				atom1="${comments1[$y]} "
				atom2=" ${comments2[$y]} " 
				c=$res2$chain$atom1$res1$chain$atom2$a
				echo "$c" >> "nef$file"
				
			fi
		done
done
		
