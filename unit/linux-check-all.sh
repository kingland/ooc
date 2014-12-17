#! /bin/bash
#
# Checking all library configurations before distribution
#
#############################################################

MYNAME=`basename $0`
RETURN=0
ARGS="$@"

for config in "debug" "release" "debug_st" "release_st"; do

	make --makefile=linuxbuilder.mak clean
	make --makefile=linuxbuilder.mak $config; R=$?
	if [[ $R != 0 ]]; then
		RETURN=1
	else
	
		./suite.sh $ARGS ; R=$?
		[[ $R != 0 ]] && RETURN=1

	fi
	
	[[ $RETURN != 0 ]] && break
done

if [[ $RETURN == 0 ]]; then
	echo	""
	echo	" Everything is ok."
	echo	""
else
	echo	""
	echo	"******************************"
	echo	"*     There were errors!     *"
	echo	"******************************"
fi

exit $RETURN