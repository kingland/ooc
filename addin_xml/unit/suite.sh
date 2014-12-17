#! /bin/bash
# Running all test cases as a test suite with memory leak check

MYNAME=`basename $0`
RETURN=0
ARGS="$@"

for i in *; do
 [[ $i == $MYNAME ]] && continue
 [[ -x ./$i ]] || continue
 [[ -d ./$i ]] && continue

 # Filter out those executables that should not run in Valgrind
 [[ $i == "my...test" ]] && continue

 # Run the testcase in valgrind
 valgrind --leak-check=yes --quiet ./$i $ARGS ; R=$?

 [[ $R != 0 ]] && RETURN=1
done

exit $RETURN

