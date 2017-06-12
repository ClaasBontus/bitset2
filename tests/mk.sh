cc=g++-7
#cc=clang++



p=test_array2array
echo "Compiling $p"
$cc -O2 -Wall -Wextra -std=c++1y -I.. -I../detail -fdiagnostics-color=auto $p.cpp -o $p

p=tests01
echo "Compiling $p"
$cc -O2 -Wall -Wextra -std=c++1y -I.. -fdiagnostics-color=auto $p.cpp -o $p

p=test_bitset2_01
echo "Compiling $p"
$cc -O2 -Wall -Wextra -Wno-unused-but-set-variable -std=c++1y -I.. -fdiagnostics-color=auto $p.cpp -o $p

p=test_bitset2_02
echo "Compiling $p"
$cc -O2 -Wall -Wextra -std=c++1y -I.. -fdiagnostics-color=auto $p.cpp -o $p

p=counter128
echo "Compiling $p"
$cc -O2 -Wall -Wextra -std=c++1y -I.. -fdiagnostics-color=auto $p.cpp -o $p

p=bench01
echo "Compiling $p"
$cc -O2 -Wall -Wextra -std=c++1y -I.. -fdiagnostics-color=auto -D_GLIBCXX_USE_NANOSLEEP $p.cpp -o $p

p=example01
echo "Compiling $p"
$cc -O2 -Wall -Wextra -std=c++1y -I.. -fdiagnostics-color=auto $p.cpp -o $p

p=test_ullong2array
echo "Compiling $p"
$cc -O2 -Wall -Wextra -std=c++1y -I.. -I../detail -fdiagnostics-color=auto $p.cpp -o $p
