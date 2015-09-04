dirlist=$(find . -maxdepth 1 -type d)

for direc in $dirlist ; do
	    if [ "$direc" = "." ]; then
               echo Not evaluating .
	    else
		echo Compiing $direc
		cd $direc
		make clean
		make
		cd ..
		mkdir ../bin/Tools/
		cp $direc/bin/* ../bin/Tools/
            fi
done
