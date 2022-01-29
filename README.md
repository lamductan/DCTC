# Implementation of the simulation of the paper "Target Coverage and Connectivity Problem in Directional Wireless Sensor Networks"

## Requirement:
```
g++10 (The code has been tested on Ubuntu 20.04.)
```

## To build and run
```
git clone https://github.com/lamductan/DCTC
unzip third_party.zip
mkdir build
cd build
cmake ..
make
./main
./test/tests
```

## To run all simulation
```
bash test_script.sh
```
Results are saved in `results/fixed_rs.csv` and `results/fixed_rc.csv`.
