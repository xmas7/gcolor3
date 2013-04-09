pkgname=gcolor3-git
pkgbase=gcolor3
pkgver=2013.04.08
pkgrel=1
pkgdesc="A simple color selection dialog in GTK3"
arch=(any)
url="https://github.com/Unia/gcolor3"
license=(GPL)
depends=('gtk3')
md5sums=()

_gitroot="https://github.com/Unia/$pkgbase"
_gitname="$pkgbase"

pkgver() {
	cd "$srcdir/$_gitname"
	git log -1 --format="%cd" --date=short | sed 's|-|.|g'
}

build() {
	cd "$srcdir"
	msg "Connecting to GIT server..."

	if [ -d ${_gitname} ] ; then
		cd ${_gitname}/
		git pull
		msg "The local files are updated."
		cd "$srcdir"
	else
		git clone ${_gitroot} ${_gitname}
	fi
	msg "GIT checkout done or server timeout"

	cd "$pkgbase/source"
	make
}

package() {
	cd "$srcdir/$pkgbase/source"
	make DESTDIR="$pkgdir" install
}
