%if "%{?_tis_build_type}" == "rt"
%define bt_ext -rt
%else
%undefine bt_ext
%endif

%define kmod_name bnxt_en
%define upstream_release 220.0.13.0

## Condition to check if retpolines are needed.
## Default: retpolines are enabled.
%bcond_without retpoline

%define kernel_module_package_buildreqs kernel%{?bt_ext}-devel

# Disable the building of debug package(s).
%define debug_package %{nil}

Name: kmod-%{kmod_name}%{?bt_ext}
Version: 1.10.2
Release: %{upstream_release}%{?_tis_dist}.%{tis_patch_ver}
Group: System Environment/Base
Packager: StarlingX Community
Vendor: Broadcom Inc
URL: https://www.broadcom.com/support/download-search?pg=&pf=&pn=&pa=&po=&dk=bnxt_en&pl=
License: GPLv2
Summary: Broadcom NetXtreme Gigabit Ethernet Driver

Provides: %{kmod_name}-kmod = %{?epoch:%{epoch}:}%{version}-%{release}
BuildRequires: devtoolset-8-build
BuildRequires: devtoolset-8-binutils
BuildRequires: devtoolset-8-gcc
BuildRequires: devtoolset-8-make
BuildRequires: kernel%{?bt_ext}-devel findutils
BuildRequires: mlnx-ofa_kernel%{?bt_ext}-devel mlnx-ofa_kernel%{?bt_ext}-source
Requires: mlnx-ofa_kernel%{?bt_ext}-modules
Requires(post):   /usr/sbin/depmod
Requires(postun): /usr/sbin/depmod

Source0: netxtreme-%{kmod_name}-%{version}-%{upstream_release}.tar.gz
Source1: modprobe.conf

Patch0001: 0001-bnxt_re-Makefile-Adapt-to-mlnx-ofa_kernel-for-Starli.patch
Patch0002: 0002-bnxt_en-bnxt_compat.h-Fix-up-a-build-failure.patch
Patch0003: 0003-bnxt_en-bnxt_re-Use-irq_update_affinity_hint.patch

#define some build variables required
%define bnxt_en_driver bnxt_en
%define bnxt_re_driver bnxt_re

%define kversion %(rpm -q kernel%{?bt_ext}-devel | sort --version-sort | tail -1 | sed 's/kernel%{?bt_ext}-devel-//')

%description
This package provides the %{bnxt_en_driver} and %{bnxt_re_driver} kernel
modules built for the Linux kernel for the %{_target_cpu} family of processors.

%prep
%autosetup -p 1 -n netxtreme-%{kmod_name}-%{version}-%{upstream_release}
cp %{bnxt_re_driver}/README.TXT %{bnxt_re_driver}/%{bnxt_re_driver}_README.TXT
cp %{bnxt_en_driver}/README.TXT %{bnxt_en_driver}/%{bnxt_en_driver}_README.TXT

%build
%if 0%{?rhel} == 7
source scl_source enable devtoolset-8 || :
%endif

export EXTRA_CFLAGS+='-DVERSION=\"%{version}\"'

%if %{with retpoline}
    find . -name *.c -print0 | xargs -0 sed -i '/MODULE_LICENSE(/a MODULE_INFO(retpoline, "Y");'
%endif

# Regenerate MANIFEST HASH
if [ -e bnxt_en/MANIFEST ]; then
  pushd bnxt_en >/dev/null
  sha512sum $(cat MANIFEST | cut -c 131-) > MANIFEST
  popd >/dev/null
fi

%{__make} %{_smp_mflags} -C bnxt_en \
	KVER=%{kversion} \
	KDIR=%{_usrsrc}/kernels/%{kversion}

OFED_VERSION="$(ls -1vd %{_usrsrc}/ofa_kernel-* | xargs -r -n1 basename | \
  sed -e 's@^ofa_kernel-@@' | sort --version-sort | tail -n1)"
if test -z "${OFED_VERSION}"; then
  echo "Error: Could not detect the OFED version"
  exit 1
fi

%{__make} %{_smp_mflags} -C bnxt_re \
	KVER=%{kversion} \
	KDIR=%{_usrsrc}/kernels/%{kversion} \
	OFED_VERSION="${OFED_VERSION}"

%install
export KVER=%{kversion}

dest_dir_bnxt_en=${RPM_BUILD_ROOT}/lib/modules/${KVER}/extra/%{kmod_name}/
src_dir_bnxt_en=$(find . -name %{bnxt_en_driver}.ko)
dest_dir_bnxt_re=${RPM_BUILD_ROOT}/lib/modules/${KVER}/extra/%{kmod_name}/drivers/infiniband/hw/%{bnxt_re_driver}/
src_dir_bnxt_re=$(find . -name %{bnxt_re_driver}.ko)
mkdir -p ${dest_dir_bnxt_en}
mkdir -p ${dest_dir_bnxt_re}
install -m 744 ${src_dir_bnxt_en} ${dest_dir_bnxt_en}
install -m 744 ${src_dir_bnxt_re} ${dest_dir_bnxt_re}

install -d %{buildroot}%{_sysconfdir}/modprobe.d
install -m 644 %{SOURCE1} %{buildroot}%{_sysconfdir}/modprobe.d/bnxt.conf

### Adapted from i40e
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

%files
%defattr(644,root,root,755)
/lib/modules/%{kversion}/extra/%{kmod_name}/
%{_sysconfdir}/modprobe.d/bnxt.conf

%post
if [ -e "/boot/System.map-%{kversion}" ]; then
    /usr/sbin/depmod -aeF "/boot/System.map-%{kversion}" "%{kversion}" > /dev/null || :
fi

%postun
/usr/sbin/depmod -a >/dev/null 2>&1 || true

%clean
rm -rf %{buildroot}

%changelog
