###########################################################################
# Intel QV Linux kernel driver
# Copyright (c) 1999, Intel Corporation.
#
# This program is free software; you can redistribute it and/or modify it
# under the terms and conditions of the GNU General Public License,
# version 2, as published by the Free Software Foundation.
#
# This program is distributed in the hope it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along with
# this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
#
###########################################################################


SHELL=/bin/bash

ifeq ($(MAKELEVEL),0)
  ifeq ($(filter-out all, $(MAKECMDGOALS)),)
    export KERNEL_CONFIG_INCLUDED=1
  endif
endif

ifneq ($(KERNELRELEASE),)

obj-$(CONFIG_IQVLINUX) += iqvlinux.o

iqvlinux-y := nalioctldrv.o \
	      linuxnaldriver.o \
	      linuxdriveros_i.o \
	      linuxdriverpci_i.o \
	      linuxdriverdevice_i.o \
	      linuxdrivermemory_i.o

else    # ifneq($(KERNELRELEASE),)

DRIVER := iqvlinux

DISABLE_MODULE_SIGNING := Yes
INSTALL_MOD_DIR ?= kernel/drivers/net

COMMON_MK ?= $(wildcard common.mk)
ifeq (${COMMON_MK},)
  $(error Cannot find common.mk build rules)
else
  include ${COMMON_MK}
endif

INSTALL_MOD_PATH ?= /lib/modules/$(KVER)
EXTRA_CFLAGS += -I$(NALDIR)/inc -I$(NALDIR)/inc/linux
EXTRA_CFLAGS += -DNAL_LINUX -DNAL_DRIVER -DNAL_DRIVER_UNSECURE -DLINUX -D__KERNEL__ -DMODULE -O2 -pipe -Wall

all:
	+$(call kernelbuild,modules)

clean:
	+$(call kernelbuild,clean)

modules_install: all
	+$(call kernelbuild,modules_install)

modules_uninstall:
	rm -f ${INSTALL_MOD_PATH}/${INSTALL_MOD_DIR}/${DRIVER}.ko

install: modules_install
	$(call cmd_depmod)

uninstall: modules_uninstall
	$(call cmd_depmod)

help:
	@echo 'Building external (out-of-tree) modules:'
	@echo '  all               - default target, build the module(s) and manpage'
	@echo '  clean             - remove generated files'
	@echo '  modules_install   - install the module(s) only'
	@echo '  install           - install the module(s) and manpage, and update initramfs'
	@echo '  modules_uninstall - uninstall the module(s) only'
	@echo '  uninstall         - uninstall the module(s) and manpage, and update initramfs'
	@echo ''
	@echo 'Command-line options:'
	@echo '  KSRC=<path>     - Path to kernel source (defaults to running kernel)'
	@echo '  LINUX_VERSION=<x.y.z> - Debug tool to force kernel LINUX_VERSION_CODE for'
	@echo '                          external module(s). *** Use at your own risk! ***'
	@echo '  INSTALL_MOD_PATH=<path> - Prefix added to default module(s) installation path'
	@echo '                            (/lib/modules/$(KVER))'
	@echo '  INSTALL_MOD_DIR=<path> - Install module(s) in subdirectory other than default'
	@echo '                           (kernel/drivers/net)'
	@echo ''

.PHONY: all clean modules_install install modules_uninstall uninstall help

installrpm:
	mkdir -p $(DESTDIR)/inc/linux/
	mkdir -p $(DESTDIR)/src/linux/driver/
	cp -r $(NALDIR)/inc/* $(DESTDIR)/inc
	cp $(NALDIR)/src/linux/driver/* $(DESTDIR)/src/linux/driver/
	cp $(NALDIR)/COPYING $(DESTDIR)/COPYING

endif   # ifneq($(KERNELRELEASE),)
