rm ../bin/Line ../bin/Base

cd Base
make clean
make
cd ..
cp ./Base/Build/* ../bin/RamdiskFolder/

cd Line
make clean
make
cd ..
cp ./Line/Build/* ../bin/RamdiskFolder/

cd GameOfLife
make clean
make
cd ..
cp ./GameOfLife/Build/* ../bin/RamdiskFolder/

cd lproc
make clean
make
cd ..
cp ./lproc/Build/* ../bin/RamdiskFolder/

cd free
make clean
make
cd ..
cp ./free/Build/* ../bin/RamdiskFolder/
