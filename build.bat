mkdir build
mkdir buildInstall
cd build
REM create the visual studio solution and project files
cmake -DCMAKE_INSTALL_PREFIX=..\buildInstall ..\
cmake --build . --target INSTALL --config Debug
cd ..\buildInstall
.\bin\hurtbox.exe