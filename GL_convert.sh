#!/bin/bash
FILENAME=$1
#######for default DAQ
#NOHEADERFILENAME=`echo ${FILENAME} | cut -d '_' -f 1,3,4`
#NOHEADERFILENAME=`echo ${FILENAME} | sed -e '_' -f 3-4`
#ONLYDATAFILENAME=`echo ${NOHEADERFILENAME/csv/txt}`
#sed -e '1,25d' ${FILENAME} > ${NOHEADERFILENAME}
#cut -d "," -f 4-23 ${NOHEADERFILENAME} > ${ONLYDATAFILENAME}
#######
#######for output from FiberQC.py
ONLYDATAFILENAME=`echo ${FILENAME/csv/txt}`
cut -d "," -f 2-65 ${FILENAME} | sed 's/ //g' | sed 's/-//g' | sed 's/+/-/g' | sed 's/,/ /g' > ${ONLYDATAFILENAME}
