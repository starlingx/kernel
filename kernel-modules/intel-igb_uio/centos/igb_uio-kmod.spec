%if "%{?_tis_build_type}" == "rt"
%define bt_ext -rt
%else
%undefine bt_ext
%endif

# dpdk-devbind.py uses Python 3.
%define __python python3

# Define the kmod package name here.
%define kmod_name igb_uio

Name:    %{kmod_name}-kmod%{?bt_ext}
Version: 21.02
Release: 0%{?_tis_dist}.%{tis_patch_ver}
Group:   System Environment/Kernel
License: GPLv2
Summary: %{kmod_name} kernel module(s)
URL:     http://www.intel.com/


BuildRequires: kernel%{?bt_ext}-devel, openssl, redhat-rpm-config
%if 0%{?rhel} == 7
BuildRequires:  devtoolset-8-build
BuildRequires:  devtoolset-8-binutils
BuildRequires:  devtoolset-8-gcc
BuildRequires:  devtoolset-8-make
%endif
 
ExclusiveArch: x86_64 i686 aarch64 ppc64le

# Sources.
Source0: dpdk-kmods-2a9f0f72a2d926382634cf8f1de10e1acf57542b.tar.gz
Source1: dpdk-devbind.py

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
%autosetup -p 1 -n dpdk-kmods

%build
%if 0%{?rhel} == 7
source scl_source enable devtoolset-8 || :
source scl_source enable llvm-toolset-7.0 || :
%endif

cd linux/igb_uio
%{__make} KSRC=%{_usrsrc}/kernels/%{kversion}

%install
find . -name *.ko
%{__install} -d %{buildroot}/lib/modules/%{kversion}/extra/%{kmod_name}/
%{__install} linux/igb_uio/%{kmod_name}.ko %{buildroot}/lib/modules/%{kversion}/extra/%{kmod_name}/
%{__install} -d %{buildroot}%{_datadir}/starlingx/scripts
%{__install} -m755 %{SOURCE1} %{buildroot}%{_datadir}/starlingx/scripts/dpdk-devbind.py

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
* Mon Apr 26 2021 Jiping Ma <jiping.ma2@windriver.com> - 21.02
- Up to version 21.02, based on Linux kernel 5.10.
* Wed Jun 03 2020 Steven Webster <steven.webster@windriver.com> - 20.05-1
- Initial RPM package, based on Starlingx iavf-kmod.
