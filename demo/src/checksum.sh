#!/bin/bash
# David Hadaller

first_hash=$(shasum -a 512 "$1" | cut -d" " -f1)


for argument in "$@"

do

	printf "\nimage: ${argument} \nsha512 hash: $(shasum -a 512 $argument | cut -d" " -f1)"
	current_hash=$(shasum -a 512 "$argument" | cut -d" " -f1)
	xor=$(( ${current_hash} ^ ${first_hash} ))
	nxor=$(( ~ ${xor} ))
	printf "\nbitwise xnor: ${nxor}"

	if [ "${nxor}" -eq 0 ]
	then
		printf "\n"
	else
		#python3 -c 'print ("\nPercent hash difference is {0:0.2f} %\n".format(abs((int("'${previous_hash}'", 16) - int("'${current_hash}'", 16))/int("'${previous_hash}'", 16) * 100)))'
		difference=$(echo ${xor} | tr -d 0 | wc -c)
		abs_diff=${difference#-}
		per_diff=$(( 100 * ${abs_diff} / 512 ))
		printf "\nThe percentage difference is ${per_diff}%%\n"
	fi
done

printf " "
