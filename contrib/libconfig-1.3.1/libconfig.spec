Name:		libconfig
Version:	1.3.1
Release:	1
Summary:	C/C++ Configuration File Library
Group:		System Environment/Libraries
License:	LGPLv2+
URL:		http://hyperrealm.com/main.php?s=libconfig
Source0:	http://www.hyperrealm.com/libconfig/%{name}-%{version}.tar.gz
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id} -u -n)
Packager:	Deneys S. Maartens  <dsm@tlabs.ac.za>
BuildRequires:	texinfo-tex

%description
Libconfig is a simple library for processing structured configuration
files. The file format is more compact and more readable than XML. And
unlike XML, it is type-aware, so it is not necessary to do string
parsing in application code.

%package devel
Summary:	%{name} development package
Group:		Development/Libraries
Requires:	%{name} = %{version}-%{release}
Requires:	pkgconfig
Requires(post):		/sbin/install-info
Requires(preun):	/sbin/install-info

%description devel
Development files for %{name}.

%prep
%setup -q
iconv -f iso-8859-1 -t utf-8 -o AUTHORS{.utf8,}
mv AUTHORS{.utf8,}

%build
%configure --disable-static
make %{?_smp_mflags}
make html pdf

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install install-pdf
rm -rf $RPM_BUILD_ROOT%{_libdir}/*.la

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%post devel
/sbin/install-info %{_infodir}/%{name}.info %{_infodir}/dir || :

%postun -p /sbin/ldconfig

%preun devel
if [ $1 = 0 ]; then
  /sbin/install-info --delete %{_infodir}/%{name}.info %{_infodir}/dir || :
fi

%files
%defattr(-,root,root,-)
%doc AUTHORS COPYING.LIB ChangeLog INSTALL NEWS README
%{_libdir}/%{name}.so*
%{_libdir}/%{name}++.so*

%files devel
%defattr(-,root,root,-)
%doc AUTHORS COPYING.LIB ChangeLog INSTALL NEWS README
%doc doc/%{name}.html
%doc test.cfg
%doc samples/c/*.c
%doc samples/c++/*.cpp
%{_includedir}/libconfig.*
%{_libdir}/libconfig*.so
%{_libdir}/pkgconfig/libconfig*.pc
%{_defaultdocdir}/%{name}/
%{_infodir}/libconfig.info*

%changelog

* Sun Aug  23 2008  Mark A. Lindner <mark_a_lindner@yahoo.com>  1.3.1
- updated for 1.3.1
* Mon Apr  7 2008  Mark A. Lindner <mark_a_lindner@yahoo.com>  1.3
- updated for 1.3
* Wed Aug 19 2007  Deneys S. Maartens  <dsm@tlabs.ac.za>  1.1.3-1
- create spec file

# -fin-
