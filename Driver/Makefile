DEV_NAME := Driver
TARGET := $(DEV_NAME).ko

MDIR := arch/arm/gpio_driver

CURRENT := $(shell uname -r)
KDIR := /lib/modules/$(CURRENT)/build
PWD := $(shell pwd)
DEST := /lib/modules/$(CURRENT)/kernel/$(MDIR)

obj-m := gpio_driver.o


default: 
	$(MAKE) -I $(KDIR)/arch/arm/include/asm/ -C $(KDIR) M=$(PWD)

install:
	#@if test -f $(DEST)/$(TARGET).orig; then \
	#	echo "Backup of.ko already exists."; \
	#else \
	#	echo "Creating a backup of .ko."; \
	#	mv -v $(DEST)/$(TARGET) $(DEST)/$(TARGET).origl; \
	#fi
	su -c "cp $(TARGET) $(DEST) && /sbin/depmod -a"
	
revert:
	@echo "Reverting to the original .ko."
	@mv -v $(DEST)/$(TARGET).ORIG $(DEST)/$(TARGET)


start:
	sudo rmmod gpio_driver
	sudo rm /dev/Driver
	sudo insmod gpio_driver.ko
	sudo mknod /dev/Driver c 240 0
	sudo chmod 666 /dev/Driver
	
stop:
	if [ `lsmod | grep -c $(DEV_NAME)` != 0 ]; \
	then \
		sudo rmmod $(TARGET:%.ko=%); \
	fi

clean:
	rm -f *.o $(TARGET) .*.cmd .*.flags *.mod.c *. \
		modules.order Module.symvers *.mod

-include $(KDIR)/Rules.make
