#!/bin/bash
# David Hadaller

current_hash="$(shasum -a 512 "$1" | cut -d" " -f1)"


for argument in "$@"

do

	printf "\nimage: ${argument} \nsha512 hash: $(shasum -a 512 $argument | cut -d" " -f1)"
	previous_hash="${current_hash}"
	current_hash="$(shasum -a 512 "$argument" | cut -d" " -f1)"
	cum_xor="$(( 0x${current_hash} ^ 0x${previous_hash} ))"
	if [ "${cum_xor}" -eq 0 ]
	then
		printf "\n"
	else
		python3 -c 'print ("\nPercent hash difference is {0:0.2f} %\n".format(abs((int("'${previous_hash}'", 16) - int("'${current_hash}'", 16))/int("'${previous_hash}'", 16) * 100)))'
	fi
done


# if [ "${cum_xor}" -eq 0 ]
# then
# 	echo "All image hashes are the same."
# else
# 	echo "The image hashes are different."
# fi
