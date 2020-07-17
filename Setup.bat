echo 'Initializing submodules and dependencies'
git submodule init
git submodule update
echo 'Generating solution'
mkdir build
cd build
cmake -G "Visual Studio 16 2019" -Ax64 ..
pause
