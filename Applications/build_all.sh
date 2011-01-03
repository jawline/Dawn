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

cd uptime
make clean
make
cd ..
cp ./uptime/Build/* ../bin/RamdiskFolder/

cd armageddon
make clean
make
cd ..
cp ./armageddon/Build/* ../bin/RamdiskFolder/


cd reboot
make clean
make
cd ..
cp ./reboot/Build/* ../bin/RamdiskFolder/
