#! /bin/bash
# Running all test cases as a test suite with memory leak check

MYNAME=`basename $0`
RETURN=0
ARGS="$@"

command -v valgrind >/dev/null 2>&1 || { echo "Valgrind is required for memory leak check, but it's not installed! Skipping memory leak check." >&2; exit 0; }

for i in *; do

 [[ $i == $MYNAME ]] && continue
 [[ $i == "linux-check-all.sh" ]] && continue
 [[ -x ./$i ]] || continue
 [[ -d ./$i ]] && continue

 ./$i $ARGS ; R=$?
 [[ $R != 0 ]] && RETURN=1
 [[ $R != 0 ]] && continue

 # Display the current test name
 echo $i

 # Run the testcase
 ./$i $ARGS ; R=$?
 [[ $R != 0 ]] && RETURN=1

 # Filter out those executables that should not run in Valgrind
 [[ $i == "testcasetest" ]] && continue

 # Run the testcase in valgrind
 valgrind --leak-check=yes --quiet ./$i $ARGS ; R=$?
 [[ $R != 0 ]] && RETURN=1
 
done

exit $RETURN
