# Maintainer: Unia <jthidskes@outlook.com> 

pkgname=gcolor3-git
_gitname=gcolor3
pkgver=2013.04.12
pkgrel=1
pkgdesc="A simple color selection dialog in GTK3"
arch=('i686' 'x86_64')
url="https://github.com/Unia/gcolor3"
license=('GPL2')
depends=('gtk3')
makedepends=('git')
source=('git://github.com/Unia/gcolor3.git')
md5sums=('SKIP')

pkgver() {
  cd $_gitname
  git log -1 --format="%cd" --date=short | sed 's|-|.|g'
}

build() {
  cd $_gitname/
  make
}

package() {
  cd $_gitname/
  make PREFIX=/usr DESTDIR="$pkgdir" install
}
