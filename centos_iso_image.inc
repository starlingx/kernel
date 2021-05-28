# List of packages to be included/installed in the ISO image.
# If these have dependencies, they will be pulled in automatically.
#

# kernel
kernel
kernel-modules-extra
kernel-tools
kernel-tools-libs
kernel-headers
kernel-devel
perf
python3-perf

# kernel-rt
kernel-rt
kernel-rt-kvm
kernel-rt-modules-extra
kernel-rt-tools
kernel-rt-headers
kernel-rt-devel

# e1000e-kmod
kmod-e1000e
kmod-e1000e-rt

# i40e-kmod
kmod-i40e
kmod-i40e-rt

# iavf-kmod
kmod-iavf
kmod-iavf-rt

# ice-kmod
kmod-ice
kmod-ice-rt

# ixgbevf-kmod
kmod-ixgbevf
kmod-ixgbevf-rt

# ixgbe-kmod
kmod-ixgbe
kmod-ixgbe-rt

# igb_uio-kmod
kmod-igb_uio
kmod-igb_uio-rt

# drbd-kernel
kmod-drbd
kmod-drbd-rt

# qat17
qat17
qat17-rt

# opae-intel-fpga-driver
kmod-opae-fpga-driver
kmod-opae-fpga-driver-rt

# mellanox drivers
mlnx-ofa_kernel-modules
mlnx-ofa_kernel-rt-modules
rdma-core
