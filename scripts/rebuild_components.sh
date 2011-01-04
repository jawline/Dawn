echo "Rebuild components script"

rm ./bin/RamdiskFolder/*
rm ./bin/*
rm ./bin/Tools/*

#Rebuild all tools
echo "Rebuilding tools"
cd Tools
sh build_all.sh
cd ..

#Rebuild all drivers
echo "Rebuilding drivers"
cd Drivers
sh build_all.sh
cd ..

#Rebuilding applications
echo "Rebuilding applications"
cd Applications
sh build_all.sh
cd ..

#Exit out
echo "Done rebuilding components"
