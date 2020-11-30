%if "%{?_tis_build_type}" == "rt"
%define bt_ext -rt
%else
%undefine bt_ext
%endif

# Define the kmod package name here.
%define kmod_name igb_uio

Name:    %{kmod_name}-kmod%{?bt_ext}
Version: 20.05
Release: 0%{?_tis_dist}.%{tis_patch_ver}
Group:   System Environment/Kernel
License: GPLv2
Summary: %{kmod_name} kernel module(s)
URL:     http://www.intel.com/

BuildRequires: kernel%{?bt_ext}-devel, redhat-rpm-config, perl, openssl
BuildRequires: elfutils-libelf-devel
BuildRequires: gcc
BuildRequires: glibc-headers
BuildRequires: numactl-devel
BuildRequires: libmnl-devel
ExclusiveArch: x86_64

# Get the kernel headers version installed, not based on uname.
%define KERNEL_VERSION %(rpm -q kernel%{?bt_ext}-devel | sort --version-sort | tail -1 | sed 's/kernel%{?bt_ext}-devel-//')
%define DPDK_TARGET_MACHINE default
%define STAGING_KERNEL_DIR /usr/src/kernels/%{KERNEL_VERSION}/
%define DPDK_EXTRA_CFLAGS "-fPIC -g -msse4.2"
%define DPDK_TARGET x86_64-native-linuxapp-gcc

%define EXTRA_OEMAKE prefix= \\\
TARGET_LDFLAGS= \\\
TARGET_CFLAGS= \\\
RTE_KERNELDIR=%{STAGING_KERNEL_DIR} \\\
EXTRA_CFLAGS=%{DPDK_EXTRA_CFLAGS} \\\
RTE_TARGET=%{DPDK_TARGET} \\\
GCC_MAJOR_VERSION="%(gcc -dumpversion | cut -f1 -d.)" \\\
GCC_MINOR_VERSION="%(gcc -dumpversion | cut -f2 -d.)"

# Sources.
Source0:  dpdk-%{version}.tar.gz
Patch01: 0001-Use-python3-instead-of-python.patch

%define kversion %(rpm -q kernel%{?bt_ext}-devel | sort --version-sort | tail -1 | sed 's/kernel%{?bt_ext}-devel-//')

%package       -n kmod-igb_uio%{?bt_ext}
Summary:          igb_uio kernel module(s)
Group:            System Environment/Kernel
%global _use_internal_dependency_generator 0
Provides:         kernel-modules >= %{kversion}
Provides:         igb_uio-kmod = %{?epoch:%{epoch}:}%{version}-%{release}
Requires(post):   /usr/sbin/depmod
Requires(postun): /usr/sbin/depmod

%description   -n kmod-igb_uio%{?bt_ext}
This package provides the igb_uio kernel module(s) built
for the Linux kernel using the %{_target_cpu} family of processors.

%post          -n kmod-igb_uio%{?bt_ext}
echo "Working. This may take some time ..."
if [ -e "/boot/System.map-%{kversion}" ]; then
    /usr/sbin/depmod -aeF "/boot/System.map-%{kversion}" "%{kversion}" > /dev/null || :
fi
echo "Done."

%preun         -n kmod-igb_uio%{?bt_ext}
rpm -ql kmod-igb_uio%{?bt_ext}-%{version}-%{release}.x86_64 | grep '\.ko$' > /var/run/rpm-kmod-igb_uio%{?bt_ext}-modules

%postun        -n kmod-igb_uio%{?bt_ext}
echo "Working. This may take some time ..."
if [ -e "/boot/System.map-%{kversion}" ]; then
    /usr/sbin/depmod -aeF "/boot/System.map-%{kversion}" "%{kversion}" > /dev/null || :
fi
rm /var/run/rpm-kmod-igb_uio%{?bt_ext}-modules
echo "Done."

%files         -n kmod-igb_uio%{?bt_ext}
%defattr(644,root,root,755)
/lib/modules/%{kversion}/
%doc /usr/share/doc/kmod-igb_uio-%{version}/
%defattr(755,root,root,755)
%{_datadir}/starlingx/scripts/dpdk-devbind.py
%exclude %{_datadir}/starlingx/scripts/*.py[oc]

# Disable the building of the debug package(s).
%define debug_package %{nil}

%description
This package provides the %{kmod_name} kernel module(s).
It is built to depend upon the specific ABI provided by a range of releases
of the same variant of the Linux kernel and not on any one specific build.

%prep
%autosetup -p 1 -n dpdk-%{version}

%build
make T=%{DPDK_TARGET} config

# This line changes default compiler flags from native to proper value
sed -i 's/"native"/"%{DPDK_TARGET_MACHINE}"/' build/.config
sed -i 's/CONFIG_RTE_EAL_IGB_UIO=n/CONFIG_RTE_EAL_IGB_UIO=y/' build/.config

make %{?_smp_mflags} %{EXTRA_OEMAKE}
ls build/kmod/

%install
find . -name *.ko
%{__install} -d %{buildroot}/lib/modules/%{kversion}/extra/%{kmod_name}/
%{__install} build/kmod/%{kmod_name}.ko %{buildroot}/lib/modules/%{kversion}/extra/%{kmod_name}/
%{__install} -d %{buildroot}%{_defaultdocdir}/kmod-%{kmod_name}-%{version}/
%{__install} license/gpl-2.0.txt %{buildroot}%{_defaultdocdir}/kmod-%{kmod_name}-%{version}/
%{__install} -d %{buildroot}%{_datadir}/starlingx/scripts
%{__install} -m755 usertools/dpdk-devbind.py %{buildroot}%{_datadir}/starlingx/scripts/dpdk-devbind.py

# Strip the modules(s).
find %{buildroot} -type f -name \*.ko -exec %{__strip} --strip-debug \{\} \;

# Always Sign the modules(s).
# If the module signing keys are not defined, define them here.
%{!?privkey: %define privkey /usr/src/kernels/%{kversion}/signing_key.pem}
%{!?pubkey: %define pubkey /usr/src/kernels/%{kversion}/signing_key.x509}
for module in $(find %{buildroot} -type f -name \*.ko);
do /usr/src/kernels/%{kversion}/scripts/sign-file \
    sha256 %{privkey} %{pubkey} $module;
done

%clean
%{__rm} -rf %{buildroot}

%changelog
* Wed Jun 03 2020 Steven Webster <steven.webster@windriver.com> - 20.05-1
- Initial RPM package, based on Starlingx iavf-kmod.
