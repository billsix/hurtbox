mkdir build
mkdir buildInstall
cd build
cmake -DCMAKE_INSTALL_PREFIX=../buildInstall ../
cmake --build  . --target all --config Debug
cmake --build  . --target install --config Debug
cd ../buildInstall
./bin/hurtbox
