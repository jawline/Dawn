echo "Rebuild components script"

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
