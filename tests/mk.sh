cc=g++-12
#cc=clang++

std=c++20

p=test_ullong2array
echo "Compiling $p"
$cc -O2 -Wall -Wextra -std=$std -I.. -I../detail -fdiagnostics-color=auto $p.cpp -o $p

p=tests01
echo "Compiling $p"
$cc -O2 -Wall -Wextra -std=$std -I.. -fdiagnostics-color=auto $p.cpp -o $p

p=gray_code
echo "Compiling $p"
$cc -O2 -Wall -Wextra -std=$std -I.. -I../detail -fdiagnostics-color=auto $p.cpp -o $p

p=test_nonconst_constexpr01
echo "Compiling $p"
$cc -O2 -Wall -Wextra -std=$std -I.. -I../detail -fdiagnostics-color=auto $p.cpp -o $p

p=test_array2array
echo "Compiling $p"
$cc -O2 -Wall -Wextra -std=$std -I.. -I../detail -fdiagnostics-color=auto $p.cpp -o $p

p=test_bitset2_01
echo "Compiling $p"
$cc -O2 -Wall -Wextra -Wno-unused-but-set-variable -std=$std -I.. -fdiagnostics-color=auto $p.cpp -o $p

p=test_bitset2_02
echo "Compiling $p"
$cc -O2 -Wall -Wextra -std=$std -I.. -fdiagnostics-color=auto $p.cpp -o $p

p=counter128
echo "Compiling $p"
$cc -O2 -Wall -Wextra -std=$std -I.. -fdiagnostics-color=auto $p.cpp -o $p

p=bench01
echo "Compiling $p"
$cc -O2 -Wall -Wextra -std=$std -I.. -fdiagnostics-color=auto -D_GLIBCXX_USE_NANOSLEEP $p.cpp -o $p

p=example01
echo "Compiling $p"
$cc -O2 -Wall -Wextra -std=$std -I.. -fdiagnostics-color=auto $p.cpp -o $p
