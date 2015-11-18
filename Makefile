obj-m += fake_battery.o

KERN_VER=$(shell uname -r)

all:
	make -C /lib/modules/$(KERN_VER)/build M=$(shell pwd) modules
