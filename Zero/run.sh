#!/bin/bash

make splint
make
echo

expected=(
# Input 1
"n=0, s=[]
n=10, s=[no-brainer]
n=3, s=[one]
n=0, s=[]
n=0, s=[]
n=-1, s=[]"

# Input 2
"n=8, s=[trickier]
n=2, s=[to]
n=6, s=[master]
n=0, s=[]
n=7, s=[Yee Haw]
n=0, s=[]
n=-1, s=[]"

# Input 3
"n=9, s=[different]
n=5, s=[words]
n=0, s=[]
n=6, s=[Hel lo]
n=3, s=[hel]
n=2, s=[lo]
n=-1, s=[]"
)

echo "----------------"
echo "| Test Results |"
echo "----------------"

for i in {1..3}; do
	filename="input${i}"
	actual=$(./p0 < $filename)
	expected=${expected[i-1]}
	if [ "${expected}" = "${actual}" ]; then
		echo "Test ${i} succeeded."
	else
		tee<<EOF
Test ${i} failed!
Actual:
${actual}

Expected:
${expected}

Diff:
$(diff <(echo "${expected}") <(echo "${actual}"))
EOF
		exit 1
	fi
done
