#!/bin/bash
# David Hadaller

current_hash="$(shasum -a 512 "$1" | cut -d" " -f1)"


for argument in "$@"

do
	echo "image: ${argument} | sha512 hash: $(shasum -a 512 $argument | cut -d" " -f1)"
	previous_hash="${current_hash}"
	current_hash="$(shasum -a 512 "$argument" | cut -d" " -f1)"
	cum_xor="$(( 0x${current_hash} ^ 0x${previous_hash} ))"
done


if [ "${cum_xor}" -eq 0 ]
then
	echo "All image hashes are the same."
else
	echo "The image hashes are different."
fi
