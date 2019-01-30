COMPILING :
  DAM : "g++ -std=c++11 -pthread PrimesDAM.cpp -o DAM"
  SAM1 : "g++ -std=c++11 -pthread PrimesSAM1.cpp -o SAM1"
  SAM2 : "g++ -std=c++11 -pthread PrimesSAM2.cpp -o SAM2"

RUN :
  DAM : "./DAM < inp-params.txt"
  SAM1 : "./SAM1 < inp-params.txt"
  SAM2 : "./SAM2 < inp-params.txt"
