#!/bin/bash


choice=4
# Print to stdout
 echo "1. HTML"
 echo "2. PDF"
 echo "Other =  Exit"
 echo -n "Please choose [1,2 or Other]? "
# read user input
read choice
if [ $choice -eq 1 ] ; then
 makeinfo --html ooc.texinfo
 cp *.png ooc/
else
 if [ $choice -eq 2 ] ; then
   texi2pdf --clean ooc.texinfo
 fi
fi
echo "Finished, press any key to continue"
read choice
