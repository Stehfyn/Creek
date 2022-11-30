@echo off

pushd ..
SET ASSIMP_SRC="vendor\assimp"
SET ASSIMP_BIN="vendor\assimp"
mkdir %ASSIMP_BIN%
cmake %ASSIMP_SRC% -G "Visual Studio 17 2022" -DBUILD_SHARED_LIBS=OFF -A x64 -S %ASSIMP_SRC% -B %ASSIMP_BIN% 
vendor\bin\premake5.exe vs2022
popd
pause