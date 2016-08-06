cc=g++-5
#cc=clang++

$cc -O2 -Wall -std=c++1y -I.. -fdiagnostics-color=auto test_bitset2_01.cpp -o test_bitset2_01

$cc -O2 -Wall -std=c++1y -I.. -fdiagnostics-color=auto test_bitset2_02.cpp -o test_bitset2_02

$cc -O2 -Wall -std=c++1y -I.. -fdiagnostics-color=auto counter128.cpp -o counter128

$cc -O2 -Wall -std=c++1y -I.. -fdiagnostics-color=auto tests01.cpp -o tests01

$cc -O2 -Wall -std=c++1y -I.. -fdiagnostics-color=auto example01.cpp -o example01
