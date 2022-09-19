%if "%{?_tis_build_type}" == "rt"
%define bt_ext -rt
%else
%undefine bt_ext
%endif

%define kmod_name ice

Name: 		%{kmod_name}-kmod%{?bt_ext}
Version:	1.8.3
Release:	1%{?_tis_dist}.%{tis_patch_ver}
Group:		System Environment/Kernel
License:	GPL-2.0
Summary:	Intel(R) Ethernet Connection E800 Series Linux Driver
URL: http://support.intel.com


BuildRequires:	kernel%{?bt_ext}-devel, kernel%{?bt_ext}-devel-keys, redhat-rpm-config, openssl, elfutils-libelf-devel
Requires:	kernel%{?bt_ext}-devel, findutils, gawk, bash
%if 0%{?rhel} == 7
BuildRequires:  devtoolset-8-build
BuildRequires:  devtoolset-8-binutils
BuildRequires:  devtoolset-8-gcc
BuildRequires:  devtoolset-8-make
%endif

%define kernel_module_package_buildreqs kernel%{?bt_ext}-devel

Source0: %{kmod_name}-%{version}.tar.gz
Source1: ice_comms-1.3.35.0.zip
Source11: modules-load.conf


Patch1: 0001-ice_main-ice_lib-Use-irq_update_affinity_hint.patch
Patch2: 0002-rename-the-ddp-file-to-avoid-conflict.patch

%define kversion %(rpm -q kernel%{?bt_ext}-devel | sort --version-sort | tail -1 | sed 's/kernel%{?bt_ext}-devel-//')
%define find() %(for f in %*; do if [ -e $f ]; then echo $f; break; fi; done)

%package       -n kmod-ice%{?bt_ext}
Summary:          Intel(R) Ethernet Connection E800 Series Linux Driver
Group:            System Environment/Kernel
%global _use_internal_dependency_generator 0
Provides:         kernel-modules >= %{kversion}
Provides:         ice-kmod = %{?epoch:%{epoch}:}%{version}-%{release}
Requires(post):   /usr/sbin/depmod
Requires(postun): /usr/sbin/depmod

%define debug_package %{nil}
%description	-n kmod-ice%{?bt_ext}
This package provides the Intel(R) Ethernet Connection E800 Series Linux Driver,
ice, built for the Linux kernel using the %{_target_cpu} family of processors.

%prep
%autosetup -p 1 -n %{kmod_name}-%{version}
unzip %{SOURCE1} -d ice_comms

%build
%if 0%{?rhel} == 7
source scl_source enable devtoolset-8 || :
%endif
pushd src >/dev/null
%{__make} KSRC=%{_usrsrc}/kernels/%{kversion}
popd >/dev/null

%install
%if 0%{?rhel} == 7
source scl_source enable devtoolset-8 || :
%endif
%{__install} -d %{buildroot}/lib/modules/%{kversion}/extra/%{kmod_name}/
%{__install} src/%{kmod_name}.ko %{buildroot}/lib/modules/%{kversion}/extra/%{kmod_name}/
%{__install} -d %{buildroot}%{_defaultdocdir}/kmod-%{kmod_name}-%{version}/
%{__install} COPYING %{buildroot}%{_defaultdocdir}/kmod-%{kmod_name}-%{version}/
%{__install} pci.updates %{buildroot}%{_defaultdocdir}/kmod-%{kmod_name}-%{version}/
%{__install} README %{buildroot}%{_defaultdocdir}/kmod-%{kmod_name}-%{version}/
%{__install} -d %{buildroot}%{_mandir}/man7/
%{__install} %{kmod_name}.7 %{buildroot}%{_mandir}/man7/
%{__install} -d %{buildroot}%{_sysconfdir}/modules-load.d
%{__install} -m 644 %{SOURCE11} %{buildroot}%{_sysconfdir}/modules-load.d/ice.conf

# Install both the OS default regular DDP fw and the COMMS version
%{__install} -d %{buildroot}/lib/firmware/updates/intel/ice/ddp/
%{__install} ddp/README %{buildroot}/lib/firmware/updates/intel/ice/ddp/README
%{__install} ddp/LICENSE %{buildroot}/lib/firmware/updates/intel/ice/ddp/LICENSE
%{__install} ddp/ice-*.pkg %{buildroot}/lib/firmware/updates/intel/ice/ddp/
%{__install} -m 644 ice_comms/*.txt %{buildroot}/lib/firmware/updates/intel/ice/ddp/
%{__install} -m 644 ice_comms/ice_comms*.pkg %{buildroot}/lib/firmware/updates/intel/ice/ddp/

# Make the regular DDP fw be the default one to load
mkdir -p %{buildroot}//lib/firmware/intel/ice/ddp/
ln -frs %{buildroot}/lib/firmware/updates/intel/ice/ddp/ice-*.pkg %{buildroot}//lib/firmware/intel/ice/ddp/stx-ice.pkg

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
rm -rf %{buildroot}

%files		-n kmod-ice%{?bt_ext}
%defattr(644,root,root,755)
/lib/modules/%{kversion}/
/lib/firmware/updates/intel/ice/ddp/*
/lib/firmware/intel/ice/ddp/stx-ice.pkg
%{_sysconfdir}/modules-load.d/ice.conf
%doc /usr/share/doc/kmod-ice-%{version}/
%doc /usr/share/man/man7/

# Disable the building of the debug package(s).
%define debug_package %{nil}

%post -n kmod-ice%{?bt_ext}

echo "Working. This may take some time ..."
if [ -e "/boot/System.map-%{kversion}" ]; then
    /usr/sbin/depmod -aeF "/boot/System.map-%{kversion}" "%{kversion}" > /dev/null || :
fi
echo "Done."

%preun -n kmod-ice%{?bt_ext}
rm -rf /usr/local/share/%{name}

%postun -n kmod-ice%{?bt_ext}
uname -r | grep BOOT || /sbin/depmod -a > /dev/null 2>&1 || true

%description
This package provides the ice kernel module(s) built
for the Linux kernel using the %{_target_cpu} family of processors.
This package contains the Intel(R) Ethernet Connection E800 Series Linux Driver.
