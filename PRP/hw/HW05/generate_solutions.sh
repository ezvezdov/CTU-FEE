#!/bin/sh

HW=05
PROGRAM=./b0b36prp-hw$HW-genref

if [ "$1" = "-osx" ]
then
	echo "Build for osx has been selected."
	PROGRAM=./b0b36prp-hw$HW-genref-osx
fi

mkdir -p files
for i in `seq 1 4`
do
   PROBLEM=files/hw$HW-$i
   echo "Generate random input '$PROBLEM.in'"
   $PROGRAM -generate > $PROBLEM.in 2>/dev/null
   echo "Solve '$PROBLEM.in' and store the reference solution to '$PROBLEM.out'"
   $PROGRAM < $PROBLEM.in > $PROBLEM.out 2>$PROBLEM.err
done

echo ""
echo "Generate and solve optional assignments"

for i in `seq 1 4`
do
   PROBLEM=`printf "files/hw%sopt-$i" $HW`
   echo "Generate random input '$PROBLEM.in'"
   $PROGRAM -generate -optional > $PROBLEM.in 2>/dev/null
   echo "Solve '$PROBLEM.in' and store the reference solution to '$PROBLEM.out'"
   $PROGRAM -prp-optional < $PROBLEM.in > $PROBLEM.out 2>$PROBLEM.err
done

return 0
