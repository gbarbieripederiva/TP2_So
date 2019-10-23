
all:  bootloader kernel userland image

debugger: bootloader debugging_kernel debugging_userland image 

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

debugging_kernel:
	cd Kernel; make debugger

debugging_userland:
	cd Userland; make debugger

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean

.PHONY: bootloader image collections kernel userland all clean
