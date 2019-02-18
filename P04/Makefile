obj-m	:= kernModTimer.o
LINUXDIR := /home/madhumitha/Softwares/BuildrootInstallation/buildroot/output/build/linux-4796173fc58688055a99a1cef19a839174067220/
PWD       := $(shell pwd)
export ARCH := arm
export COMPILER := arm-linux-

 
all: default
 
default:
	$(MAKE) -C $(LINUXDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(COMPILER) modules

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions 
