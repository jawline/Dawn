[33mcommit 22f7661ab5ef44d8f27b956fd5e42ff6fa2b2dd2[m
Author: Blake Lo0ring <blake@blake-desktop.(none)>
Date:   Tue Feb 23 18:56:04 2010 +0000

    Added itoa function to common, Converts integers to character arrays
    Added a function in main.c to output the memory limits (Upper and lower) passed by GRUB during the boot.

[33mcommit 8365de111951b0ff091ab2fe3cbbc8b1d312868b[m
Author: Blake Lo0ring <blake@blake-desktop.(none)>
Date:   Tue Feb 23 18:05:51 2010 +0000

    Added common files to the source.
    Added simple text mode screen manipulation files to the source
    
    Created scripts to aid the building of the Kernel and generation of a disk image

[33mcommit e964179477e052a15294e9a49720acd27f40d930[m
Author: Blake Lo0ring <blake@blake-desktop.(none)>
Date:   Tue Feb 23 16:49:36 2010 +0000

    Added boot code.
    Added entry point.
    Created a sh script used to mount the disk image and copy the kernel over to it.
    Created a bochsrc that will run the disk image.
