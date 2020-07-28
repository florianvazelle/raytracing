cd %~f0
mkdir build
cd build
:: cmake .. -G "Visual Studio 16 2019" -A Win64
:: cmake --build . --config Debug
cd ..