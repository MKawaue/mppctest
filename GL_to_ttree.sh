#!/bin/bash
INPUTFILENAME=$1
source GL_convert.sh ${INPUTFILENAME}
TEXTFILENAME=`echo ${INPUTFILENAME/csv/txt}`
./GL840_make_ttree ${TEXTFILENAME}
#root "GL840_make_ttree.c(\"${TEXTFILENAME}\")"
