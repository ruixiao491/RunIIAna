#!/bin/sh
#echo `hostname`

inputdatafile=$1
inputmcfile=$2
iptstart=$3
iptend=$4
isPbPb=$5
centlow=$6
centhigh=$7

root -l -b -q 'MassFit.C++("'$inputdatafile'", "'$inputmcfile'", '$iptstart', '$iptend', '$isPbPb', '$centlow', '$centhigh')'

./cleanup.sh
