#!/bin/sh

HEADER_TMPL=$1
OUTPUT_FILENAME=$2

CKSUM=`cksum $1 | awk ' { print $1 } '`

sed 's/%%INTERFACE_ID%%/'"${CKSUM}"'/g' $1 > $2

echo "Checksum for file $1 is ${CKSUM}"

