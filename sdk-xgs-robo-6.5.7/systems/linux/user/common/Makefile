# -*- Makefile -*-
# $Id: Makefile,v 1.83 Broadcom SDK $
# $Copyright: (c) 2016 Broadcom.
# Broadcom Proprietary and Confidential. All rights reserved.$
#

# Select a random backoff time (1-20 secs) for retrying 'ln -sf' operations
BACKOFF := $$$$(( (RANDOM % 20) + 1 ))

SDK :=$(shell if [ -n "$$SDK" ] ; then\
	echo $$SDK;\
	else\
	cd $(dir $(lastword $(MAKEFILE_LIST))); while /usr/bin/test ! -e RELEASE ; do \
	dir=`cd ../;pwd`;			\
	if [ "$$dir" = "/" ] ; then		\
	   echo Cannot find SDK in $(lastword $(MAKEFILE_LIST)) 1>&2;		\
	   exit 1;				\
	fi ;					\
	cd $$dir;				\
	done ;					\
	pwd;					\
	fi)

ifeq ($(SDK),)
$(error Please run this in a tree)
endif

export SDK

override-target=unix-user

ifeq ($(LINUX_MAKE_SHARED_LIB),1)
libext = so.$(SHAREDLIBVER)
else
libext = a
endif

ifeq (,$(platform))
$(error Internal error: platform variable is not set)
endif

include $(SDK)/make/Make.config
LOCALDIR=systems/linux/user/common
include $(SDK)/make/Make.depend
kernel-override=linux-$(platform)
ifdef BLDCONFIG
KERN_BLDROOT=${SDK}/${SDKBUILD}/${BLDCONFIG}/$(kernel-override)${all_suffix}$(bldroot_suffix)
else
KERN_BLDROOT=${SDK}/${SDKBUILD}/$(kernel-override)${all_suffix}$(bldroot_suffix)
endif

ifeq (,$(kernel_version))
kernel_version=2_4
endif

ifneq ($(kernel_version),2_4)
KOBJ=ko
else
KOBJ=o
endif

ifneq (,$(TOOLS_DIR))
override PATH := $(TOOLS_DIR):$(PATH)
endif

ifeq ($(DEST_DIR),)
DEST_DIR=${BLDDIR}
endif

BCM_LOCAL := bcm.$(targetplat)$(all_suffix)
BCM := $(DEST_DIR)/$(BCM_LOCAL)

KERNEL_BDE_LOCAL := linux-kernel-bde.$(KOBJ)
KERNEL_BDE := $(DEST_DIR)/$(KERNEL_BDE_LOCAL)

USER_BDE_LOCAL := linux-user-bde.$(KOBJ)
USER_BDE := $(DEST_DIR)/$(USER_BDE_LOCAL)

UK_PROXY_LOCAL := linux-uk-proxy.$(KOBJ)
UK_PROXY := $(DEST_DIR)/$(UK_PROXY_LOCAL)

KNET_CB_LOCAL := linux-knet-cb.$(KOBJ)
KNET_CB := $(DEST_DIR)/$(KNET_CB_LOCAL)

BCM_KNET_LOCAL := linux-bcm-knet.$(KOBJ)
BCM_KNET := $(DEST_DIR)/$(BCM_KNET_LOCAL)

BCM_ROBO_KNET_LOCAL := linux-bcm-robo-knet.$(KOBJ)
BCM_ROBO_KNET := $(DEST_DIR)/$(BCM_ROBO_KNET_LOCAL)

NETSERVE_LOCAL := netserve
NETSERVE := $(DEST_DIR)/$(NETSERVE_LOCAL)

# Kernel modules
KERNEL_TARGETS := $(KERNEL_BDE) $(USER_BDE)
LOCAL_KERNEL_TARGETS := $(patsubst %,$(realpath ..)/$(platform)/%,$(KERNEL_BDE_LOCAL) $(USER_BDE_LOCAL))

# Main BCM application
BCM_TARGETS := $(BCM)
LOCAL_BCM_TARGETS := $(patsubst %,$(realpath ..)/$(platform)/%,$(BCM_LOCAL) $(BCM_LOCAL).dbg)

# Additional user mode applications
USER_TARGETS := $(NETSERVE)
LOCAL_USER_TARGETS := $(patsubst %,$(realpath ..)/$(platform)/%,$(NETSERVE_LOCAL))

ifeq (,$(findstring DELIVER,$(MAKECMDGOALS)))

# Allow override of default make target
ifeq (,$(DEFAULT_MAKE_TARGET))
.DEFAULT_GOAL := all
else
.DEFAULT_GOAL := $(DEFAULT_MAKE_TARGET)
endif

# Kernel network support
ifdef BUILD_KNET

KERNEL_TARGETS += $(UK_PROXY)
LOCAL_KERNEL_TARGETS += $(patsubst %,$(realpath ..)/$(platform)/%,$(UK_PROXY_LOCAL))

ifdef BUILD_ROBO_KNET
KERNEL_TARGETS += $(BCM_ROBO_KNET)
LOCAL_KERNEL_TARGETS += $(patsubst %,$(realpath ..)/$(platform)/%,$(BCM_ROBO_KNET_LOCAL))
endif

ifdef ESW_CHIPS
KERNEL_TARGETS += $(BCM_KNET)
LOCAL_KERNEL_TARGETS += $(patsubst %,$(realpath ..)/$(platform)/%,$(BCM_KNET_LOCAL))
endif

ifdef DPP_CHIPS
KERNEL_TARGETS += $(BCM_KNET)
LOCAL_KERNEL_TARGETS += $(patsubst %,$(realpath ..)/$(platform)/%,$(BCM_KNET_LOCAL))
endif

ifdef BUILD_KNET_CB
KERNEL_TARGETS += $(KNET_CB)
LOCAL_KERNEL_TARGETS += $(patsubst %,$(realpath ..)/$(platform)/%,$(KNET_CB_LOCAL))
endif

ADD_TO_CFLAGS += -I$(SDK)/systems/linux/kernel/modules/include
COND_KNET_LIBS = libuser.$(libext)

endif # BUILD_KNET

ifeq ($(NO_LOCAL_TARGETS),)
LOCAL_TARGETS := $(LOCAL_KERNEL_TARGETS) $(LOCAL_BCM_TARGETS) $(LOCAL_USER_TARGETS)
all_targets := $(LOCAL_TARGETS)
else
LOCAL_TARGETS :=
all_targets := $(KERNEL_TARGETS) $(BCM_TARGETS) $(USER_TARGETS)
endif

export BCM_LIB_PETRA

# Build all targets
all: $(BLDDIR)/.tree $(all_targets)

ifeq ($(NO_LOCAL_TARGETS),)
# Build kernel modules only
mod: $(LOCAL_KERNEL_TARGETS)

# Build main BCM application only
bcm: $(LOCAL_BCM_TARGETS)

# Build additional user mode applications only
user: $(LOCAL_USER_TARGETS)

define LOCAL_TARGET_DEF
$(1) : $(DEST_DIR)/$(notdir $(1))
	@for i in {1..10}; do echo "Symlinking $1..." ; ln -sf $$? $$@ && break || sleep ${BACKOFF} ; done
endef
endif

# Target specific
ADD_TO_CFLAGS += -DLINUX_SAL_DMA_ALLOC_OVERRIDE
#ADD_TO_CFLAGS += -DLINUX_SAL_USLEEP_OVERRIDE
#ADD_TO_CFLAGS += -DLINUX_SAL_SEM_OVERRIDE
#ADD_TO_CFLAGS += -DLINUX_SAL_UDELAY_OVERRIDE

# User BDE libraries
ADD_TO_CFLAGS += -I$(SDK)/systems/bde/linux/include

CFLAGS += $(ADD_TO_CFLAGS)

ifdef ESW_CHIPS
ESW_DIAG_LIBS = libdiag_esw.$(libext) \
                libtsl3uc.$(libext) \
                libtsl3mc.$(libext) \
                libtsmpls.$(libext) \
                libtsmmu.$(libext) \
                libtsoam.$(libext) \
                libtsniv.$(libext) \
                libtsriot.$(libext) \
                libtsvlan.$(libext) \
                libtsvxlan.$(libext) \
                libtsbasic.$(libext) \
                libtsifp.$(libext) \
                libtsvfp.$(libext) \
                libtsefp.$(libext) \
                libtscos.$(libext) \
                libtsloadbalance.$(libext) \
                libtsdgm.$(libext) \
                libtstcb.$(libext) \
                libtspstat.$(libext) \
                libtechsupport.$(libext)
endif

ifdef ROBO_CHIPS
ROBO_DIAG_LIBS = libdiagrobo.$(libext)
endif

ifdef SBX_CHIPS
SBX_DIAG_LIBS = \
	libdiag_sbx.$(libext)
endif

ifdef EA_CHIPS
EA_DIAG_LIBS = \
	libdiag_ea.$(libext)
endif

ifndef NO_SAL_APPL
COND_LIB_SAL_APPL=libsal_appl.$(libext)
COND_LIB_SAL_APPL_PLATFORM=libsal_appl_plat.$(libext)
endif

DIAG_LIBS = \
        libdiag.$(libext)       \
        ${ESW_DIAG_LIBS} \
        ${ROBO_DIAG_LIBS} \
        ${SBX_DIAG_LIBS} \
        ${EA_DIAG_LIBS} \
        ${SAND_DIAG_LIBS} \
	$(COND_LIB_EAV_APPL) \
	${COND_LIB_PHYMOD_DIAG} \
	${COND_LIB_PORTMOD_DIAG} \
	${COND_LIB_BCMX_DIAG} \
        ${COND_LIB_TEST} \
	$(COND_LIB_TCL) \
	$(APPL_LIBS) \
	$(COND_LIB_EDITLINE) \
	$(COND_LIB_WBDM) \
	$(COND_LIB_SMB) \
	$(COND_LIB_PHY_DIAGNOSTICS) \
	$(COND_KNET_LIBS) \
	$(COND_LIB_SAL_APPL) \
	$(COND_LIB_SAL_APPL_PLATFORM) \
	liblubde.$(libext)

ACL_LIB = $(COND_LIB_ACL)
BCM_LIBS = $(DIAG_LIBS) $(DRV_LIBS) $(ACL_LIB) $(COND_LIB_ETH_DRV)
BCM_LIBS_BLD = $(addprefix $(BLDDIR)/,$(BCM_LIBS))

BCM_LIBS_BLD += ${EXTERNAL_LIBS}

# Kernel network support
ifdef BUILD_KNET

knet_subdirs = shared uk-proxy

ifdef BUILD_ROBO_KNET
knet_subdirs += bcm-robo-knet
endif

ifdef ESW_CHIPS
knet_subdirs += bcm-knet
endif

ifdef DPP_CHIPS
knet_subdirs += bcm-knet
endif

ifdef BUILD_KNET_CB
knet_subdirs += knet-cb
endif

endif # BUILD_KNET


kernel_modules:
ifdef IPROC_BUILD
ifneq ($(wildcard $(SDK)/tools/diffconfig.pl),)
	$(PERL) $(SDK)/tools/diffconfig.pl \
                $(SDK)/systems/linux/user/$(platform)/linux-$(platform).config $(KERNDIR)/.config
endif
endif
	$(MAKE) -C $(SDK)/systems/linux/kernel/modules kernel_version=$(kernel_version) OPT_CFLAGS="$(ADD_TO_CFLAGS)" \
                subdirs="shared" override-target=linux-$(platform); \
	$(MAKE) -C $(SDK)/systems/linux/kernel/modules kernel_version=$(kernel_version) OPT_CFLAGS="$(ADD_TO_CFLAGS)" \
                subdirs="uk-proxy" override-target=linux-$(platform); \
	$(MAKE) -C $(SDK)/systems/bde/linux/kernel kernel_version=$(kernel_version) OPT_CFLAGS="$(ADD_TO_CFLAGS)"; \
	$(MAKE) -C $(SDK)/systems/bde/linux/user/kernel kernel_version=$(kernel_version) OPT_CFLAGS="$(ADD_TO_CFLAGS)"
ifdef BUILD_KNET
	$(MAKE) -C $(SDK)/systems/linux/kernel/modules kernel_version=$(kernel_version) OPT_CFLAGS="$(ADD_TO_CFLAGS)" \
		subdirs="$(knet_subdirs)" override-target=linux-$(platform)
endif

user_libs:
ifdef CUSTOMER_SOURCE_DIR
	$(MAKE) -C $(CUSTOMER_SOURCE_DIR) CFLAGS="$(CFLAGS)"
endif
	$(MAKE) -C $(SDK)/systems/bde/linux/user CFLAGS="$(CFLAGS)"
ifdef BUILD_PHYMOD
	$(MAKE) -C $(SDK)/libs/phymod
endif
	$(MAKE) -C $(SDK)/src CFLAGS="$(ADD_TO_CFLAGS)"
	$(MAKE) -C $(SDK)/systems/drv CFLAGS="$(CFLAGS)"
ifdef BUILD_KNET
	$(MAKE) -C $(SDK)/systems/linux/kernel/user/shared CFLAGS="$(CFLAGS)"
endif
ifdef BUILD_MACSEC
	@${ECHO} "Building MACSEC libraries ...."
	$(MAKE) -C ${MACSEC_HOME} MACSEC_CC="${CC}" MACSEC_LD="${LD}" MACSEC_AR="$(AR)" MACSEC_CFLAGS="${MACSEC_BUILD_FLAG}" MACSEC_TARGET="${targetbase}/${targetplat}/${platform}"
endif

# Broadcom Command Monitor

# Build platform_defines.c to compile in config.bcm
#
ifndef NO_SAL_APPL
PLATFORM_DEFINES_OBJ=${BLDDIR}/platform_defines.o
${BLDDIR}/platform_defines.c: ${SDK}/rc/config.bcm
	$(PERL) ${SDK}/tools/bcm2c_linux.pl ${SDK}/rc/config.bcm >$@
${BLDDIR}/platform_defines.o: ${BLDDIR}/platform_defines.c
	${CC} ${CFLAGS} -c $< -o $@
else
PLATFORM_DEFINES_OBJ=
endif


$(BCM): $(BCM).dbg
	mkdir -p $(@D)
	$(OBJCOPY) --strip-debug $< $@

$(BCM).dbg: user_libs $(BLDDIR)/socdiag.o $(BLDDIR)/version.o \
	${PLATFORM_DEFINES_OBJ}
	@rm -f $@	# Prevent core dump if currently running
	@echo Linking $@ # This is needed for error parsing
	$(CC) $(CFLAGS) -o $@ $(BLDDIR)/socdiag.o $(BLDDIR)/version.o \
	-Wl,--start-group ${PLATFORM_DEFINES_OBJ} $(BCM_LIBS_BLD)  -Wl,--end-group $(LDFLAGS)


$(KERNEL_BDE): $(KERN_BLDROOT)/linux-kernel-bde.$(KOBJ)
	mkdir -p $(@D)
	$(OBJCOPY) --strip-debug $< $@

$(USER_BDE): $(KERN_BLDROOT)/linux-user-bde.$(KOBJ)
	$(OBJCOPY) --strip-debug $< $@

$(UK_PROXY): $(KERN_BLDROOT)/linux-uk-proxy.$(KOBJ)
	$(OBJCOPY) --strip-debug $< $@

$(KNET_CB): $(KERN_BLDROOT)/$(KNET_CB_LOCAL)
	$(OBJCOPY) --strip-debug $< $@

$(BCM_KNET): $(KERN_BLDROOT)/$(BCM_KNET_LOCAL)
	$(OBJCOPY) --strip-debug $< $@

$(BCM_ROBO_KNET): $(KERN_BLDROOT)/linux-bcm-robo-knet.$(KOBJ)
	$(OBJCOPY) --strip-debug $< $@

$(KERN_BLDROOT)/%.$(KOBJ): kernel_modules
	@echo Force execution of kernel_modules target > /dev/null

$(NETSERVE): $(BLDDIR)/netserve.o
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $(BLDDIR)/netserve.o $(LDFLAGS) -lutil
	$(OBJCOPY) --strip-debug $@

ifeq ($(NO_LOCAL_TARGETS),)
$(foreach targ,$(LOCAL_TARGETS),$(eval $(call LOCAL_TARGET_DEF,$(targ))))
endif

# Build Version
$(BLDDIR)/version.c: $(BLDDIR)/socdiag.o user_libs
	$(PERL) $(SDK)/tools/version.pl > $@
$(BLDDIR)/version.o: $(BLDDIR)/version.c
	$(CC) $(CFLAGS) -c $< -o $@

clean::
	$(MAKE) -C $(SDK)/systems/bde/linux/kernel $@
	$(MAKE) -C $(SDK)/systems/bde/linux/user/kernel $@
	$(MAKE) -C $(SDK)/systems/bde/linux/user CFLAGS="$(CFLAGS)" $@
	$(MAKE) -C $(SDK)/systems/drv $@
	$(MAKE) -C $(SDK)/systems/drv/eth $@
	$(MAKE) -C $(SDK)/src CFLAGS="$(CFLAGS)" $@
ifdef BUILD_PHYMOD
	$(MAKE) -C $(SDK)/libs/phymod $@
endif
	$(MAKE) -C $(SDK)/systems/linux/kernel/user/shared $@
	$(MAKE) -C $(SDK)/systems/linux/kernel/modules \
		subdirs="$(knet_subdirs)" \
		override-target=linux-$(platform) $@
	$(RM) $(BLDDIR)/version.c $(BLDDIR)/version.o
	$(RM) $(BLDDIR)/platform_defines.c $(BLDDIR)/platform_defines.o
	$(RM) $(BLDDIR)/socdiag.o
	$(RM) $(KERNEL_BDE) $(USER_BDE) $(BCM) $(BCM).dbg
	$(RM) $(UK_PROXY) $(BCM_KNET) $(BCM_ROBO_KNET)
	$(RM) $(KERN_BLDROOT)/linux-kernel-bde.$(KOBJ)
	$(RM) $(KERN_BLDROOT)/linux-user-bde.$(KOBJ)
	$(RM) $(KERN_BLDROOT)/linux-uk-proxy.$(KOBJ)
	$(RM) $(KERN_BLDROOT)/linux-bcm-knet.$(KOBJ)
	$(RM) $(KERN_BLDROOT)/linux-bcm-robo-knet.$(KOBJ)
	$(RM) $(BLDDIR)/netserve.o
	$(RM) $(NETSERVE) $(LOCAL_TARGETS)

distclean::
	$(RM) $(BLDROOT)
	$(RM) $(KERN_BLDROOT)
	$(RM) $(DEST_DIR)

.PHONY: variable

#
# Echo variable values used for configuration
# usage: make VAR=CC variable
#
variable::
	@echo $($(VAR))

else
ifndef DELIVERY
$(error DELIVERY required)
endif

DELIVERABLES := $(KERNEL_BDE) $(USER_BDE) $(BCM) $(BCM).dbg $(NETSERVE)
# 1= source
define DELIVERY_RULE
_DEL_TARG += $(DELIVERY)/$(notdir $(1))
$(DELIVERY)/$(notdir $(1)) : $(1)
	@mkdir -p $(DELIVERY)
ifeq ($(filter $(notdir $(BCM)) $(notdir $(BCM).dbg),$(notdir $1)),)
	$(OBJCOPY) --strip-debug $$< $$@
else
	$(CP) -p $$< $$@
endif
endef

$(foreach f,$(DELIVERABLES),$(eval $(call DELIVERY_RULE,$(f))))

DELIVER: $(_DEL_TARG)
endif

# Make.config defines remaining phony targets
.PHONY: build kernel_modules user_libs DELIVER mod bcm user
