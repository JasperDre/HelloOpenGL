echo 'Initializing submodules and dependencies'
git submodule init
git submodule update
echo 'Generating solution'
call Generate.bat
pause
