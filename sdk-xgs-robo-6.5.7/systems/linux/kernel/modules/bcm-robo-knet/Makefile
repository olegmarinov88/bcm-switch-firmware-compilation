# -*- Makefile -*-
# $Id: Makefile,v 1.3 Broadcom SDK $
# $Copyright: (c) 2016 Broadcom.
# Broadcom Proprietary and Confidential. All rights reserved.$
#
LOCALDIR = systems/linux/kernel/modules/bcm-robo-knet

include ${SDK}/make/Make.config

LIBS = $(LIBDIR)/libkern.a

ifeq ($(kernel_version),2_4)
MODULE = $(LIBDIR)/linux-bcm-robo-knet.o
else
KERNEL_MODULE_DIR = kernel_module

THIS_MOD_NAME := linux-bcm-robo-knet
MODULE = $(LIBDIR)/$(THIS_MOD_NAME).o
KMODULE = $(LIBDIR)/$(THIS_MOD_NAME).ko

build: $(MODULE) $(KMODULE)
endif

KBUILD_EXTRA_SYMBOLS := ${BLDDIR}/../../../../bde/linux/kernel/kernel_module/Module.symvers
KBUILD_EXTRA_SYMBOLS += ${BLDDIR}/../uk-proxy/kernel_module/Module.symvers

# BCM Network Device

$(MODULE): $(BLDDIR)/.tree $(BOBJS) $(LIBS)
	$(LD) $(MODULE_LDFLAGS) -r -d $(BOBJS) $(LIBS) -o $@
ifneq ($(kernel_version),2_4)
$(KMODULE): $(MODULE)
	rm -fr $(BLDDIR)/$(KERNEL_MODULE_DIR)
	mkdir $(BLDDIR)/$(KERNEL_MODULE_DIR)
	cp ${SDK}/make/Makefile.linux-kmodule $(BLDDIR)/$(KERNEL_MODULE_DIR)/Makefile
	cat ${KBUILD_EXTRA_SYMBOLS} > $(BLDDIR)/$(KERNEL_MODULE_DIR)/Module.symvers
	MOD_NAME=$(THIS_MOD_NAME) $(MAKE) -C $(BLDDIR)/$(KERNEL_MODULE_DIR) $(THIS_MOD_NAME).ko
endif

# Make.depend is before clean:: so that Make.depend's clean:: runs first.

include ${SDK}/make/Make.depend

clean::
	$(RM) $(BLDDIR)/version.c $(BLDDIR)/version.o
	$(RM) $(BOBJS) $(MODULE)

ifneq ($(kernel_version),2_4)
.PHONY: build
endif
