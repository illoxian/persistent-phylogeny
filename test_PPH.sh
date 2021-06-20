#!/bin/bash

echo "PPH"
bin/ppp.exe -g  ../data/soto-matrix/PPPH/4x3.txt.d/out_n_10 | grep RES
bin/ppp.exe   ../data/soto-matrix/PPPH/4x3.txt.d/out_n_10 | grep RES
echo ""

echo "PPH"
bin/ppp.exe -g  ../data/soto-matrix/PPPH/4x5.txt.d/out_n_11 | grep RES
bin/ppp.exe   ../data/soto-matrix/PPPH/4x5.txt.d/out_n_11 | grep RES
echo ""
 
echo "PPH"
bin/ppp.exe -g  ../data/soto-matrix/PPPH/4x5.txt.d/out_n_18 | grep RES
bin/ppp.exe   ../data/soto-matrix/PPPH/4x5.txt.d/out_n_18 | grep RES
echo ""
 
echo "PPH"
bin/ppp.exe -g  ../data/soto-matrix/PPPH/4x5.txt.d/out_n_12 | grep RES
bin/ppp.exe   ../data/soto-matrix/PPPH/4x5.txt.d/out_n_12 | grep RES
echo ""
 
echo "PPH"
bin/ppp.exe -g  ../data/soto-matrix/PPPH/4x5.txt.d/out_n_13 | grep RES
bin/ppp.exe   ../data/soto-matrix/PPPH/4x5.txt.d/out_n_13 | grep RES
echo ""

echo "PPH"
bin/ppp.exe -g  ../data/soto-matrix/PPPH/4x5.txt.d/out_n_14 | grep RES
bin/ppp.exe   ../data/soto-matrix/PPPH/4x5.txt.d/out_n_14 | grep RES
echo ""

echo "PPH"
bin/ppp.exe -g  ../data/soto-matrix/PPPH/4x5.txt.d/out_n_15 | grep RES
bin/ppp.exe   ../data/soto-matrix/PPPH/4x5.txt.d/out_n_15 | grep RES
echo ""

echo "PPH"
bin/ppp.exe -g  ../data/soto-matrix/PPPH/4x5.txt.d/out_n_16 | grep RES
bin/ppp.exe   ../data/soto-matrix/PPPH/4x5.txt.d/out_n_16 | grep RES
echo ""

echo "PPH"
bin/ppp.exe -g  ../data/soto-matrix/PPPH/4x5.txt.d/out_n_17 | grep RES
bin/ppp.exe   ../data/soto-matrix/PPPH/4x5.txt.d/out_n_17 | grep RES
echo ""

echo "noPPH"
bin/ppp.exe -g  ../data/soto-matrix/noPPPH/7x3.txt | grep RES
bin/ppp.exe   ../data/soto-matrix/noPPPH/7x3.txt | grep RES
echo ""

echo "noPPH"
bin/ppp.exe -g  ../data/soto-matrix/noPPPH/4x4.txt | grep RES
bin/ppp.exe   ../data/soto-matrix/noPPPH/4x4.txt | grep RES
echo ""



