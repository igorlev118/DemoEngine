@echo off

rem Create an empty directory.
mkdir "CMake-VS2017" >nul 2>&1
cd "CMake-VS2017"

rem Generate solution files.
cmake -G "Visual Studio 15" ./../..

rem Open the solution file.
echo -- Opening the solution file..
start "" "Game Engine.sln"
