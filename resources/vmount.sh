
#!/bin/bash
sshfs -o local -o volname=sisopmedia vagrant@sisopenv:/home/vagrant /Volumes/sisopmedia
case $? in
	0) #Ok
		echo "Volume mounted"
		;;
	1) #Pattern not found
		echo "Already mounted"
		;;
	*) #Error
		echo "brew install sshfs"
		;;
esac
