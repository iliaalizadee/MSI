BUILD_PATH := /lib/modules/$(shell uname -r)/build
VERSION := 1.00
DKMS_PATH := /usr/src/msi-$(VERSION)

obj-m += msi.o

module:
	make -C $(BUILD_PATH) M=$(CURDIR) modules

clean:
	make -C $(BUILD_PATH) M=$(CURDIR) clean


install:
	dkms --version >> /dev/null
	mkdir -p $(DKMS_PATH)
	cp $(CURDIR)/dkms.conf $(DKMS_PATH)
	cp $(CURDIR)/Makefile $(DKMS_PATH)
	cp $(CURDIR)/msi.c $(DKMS_PATH)
	cp $(CURDIR)/msi.h $(DKMS_PATH)

	sed -e "s/@VERSION@/${VERSION}/" -i $(DKMS_PATH)/dkms.conf

	dkms add msi/$(VERSION)
	dkms build msi/$(VERSION)
	dkms install msi/$(VERSION)

uninstall:
	dkms remove msi/$(VERSION) --all
	rm -rf $(DKMS_PATH)