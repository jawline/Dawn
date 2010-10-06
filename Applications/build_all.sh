rm ../bin/Line ../bin/Base

cd Base
make clean
make
cd ..
cp ./Base/Build/* ../bin/

cd Line
make clean
make
cd ..
cp ./Line/Build/* ../bin/
