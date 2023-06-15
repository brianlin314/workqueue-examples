obj-$(CONFIG_SAMPLE_TRACE_PRINTK) += workqueue-example.o

KERNELDIR ?= /lib/modules/`uname -r`/build
PWD       := $(shell pwd)

TARGET_MODULE := workqueue-example

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

check: all
	sudo insmod $(TARGET_MODULE).ko
	sleep 7
	sudo rmmod $(TARGET_MODULE)
