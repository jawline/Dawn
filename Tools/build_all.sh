Dirlist=$(find . -maxdepth 1 -type d)

for direc in $Dirlist ; do
	    
	    if [ "$direc" = "." ]; then
               echo Not evaluating .
	    else
		echo Compiing $direc
		cd $direc

		make clean
		make

		cd ..
		cp $direc/Build/* ../bin/Tools

            fi
            
done
