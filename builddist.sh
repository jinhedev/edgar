if [[ $EUID -ne 0 ]]; then
	echo "This must be run as root"
	exit 1
fi

if [[ $# -ne 1 ]]; then
	echo ""
	echo "Usage $0 <version>"
	echo ""
	exit 1
fi

###### VARIABLES ############

APPVERSION="$1"
APPRELEASE="1"

ARCH="i586"

APPNAME="edgar"

APPDIR="$APPNAME-$APPVERSION/"

DISTNAME="$APPNAME-$APPVERSION-$APPRELEASE.tar.gz"
DATADIRS="data music sound gfx font"

RPMROOT="/usr/src/rpm"

SPECNAME="$APPNAME.spec"

###### Main Operation ########

echo "Creating Distribution for $APPNAME (Version $APPVERSION, Release $APPRELEASE)"

echo "Cleaning..."

rm -rf dist/*

mkdir -p dist

cd dist

echo "Getting Subversion Tag $APPVERSION-$APPRELEASE"

svn export http://reddwarf/svn/Edgar/tags/$APPVERSION $APPDIR

echo "Removing unwanted data files..."

rm -rf $APPDIR/dev
rm $APPDIR/disclaimer.txt
rm $APPDIR/*.spec
rm $APPDIR/*.sh
rm $APPDIR/*.nsi
rm $APPDIR/*.txt
rm $APPDIR/*.dat
rm $APPDIR/makefile.windows
rm $APPDIR/makefile.amiga

for i in `find . -name *.wav | grep -v doc`;do
	rm $i
done

for i in `find . -name *.xcf | grep -v doc`;do
	rm $i
done

for i in `find . -name *.gif | grep -v doc`;do
	rm $i
done

for i in `find . -name *.blend* | grep -v doc`;do
	rm $i
done

for i in `find . -name *.jpg | grep -v doc`;do
	rm $i
done

echo "Creating GZIP... $DISTNAME"

cp $APPDIR/edgar.spec.base $APPDIR/edgar.spec

for f in `ls $APPDIR/locale/*.mo`; do \
	FILE=`echo $f | cut -d'/' -f2 | cut -d'.' -f1`
	echo "/usr/share/locale/$FILE/LC_MESSAGES/edgar.mo" >> edgar.spec
done

tar zhcf $DISTNAME $APPDIR

echo "Removing Copied Data..."

rm -rf $APPDIR

echo "Preparing to Build..."

cp $DISTNAME $RPMROOT/SOURCES

echo "Building RPMs..."

cd ..

rpmbuild -bb $SPECNAME --target $ARCH --define "name $APPNAME" --define "version $APPVERSION" --define "release $APPRELEASE"

echo "Retrieving RPMs"

mv $RPMROOT/RPMS/$ARCH/*.* dist/

echo "Running Alien..."

cd dist

alien -k *.rpm

echo "Renaming..."

mv *.deb edgar-$APPVERSION-$APPRELEASE.i386.deb

cd ..

chmod +x buildwindows.sh

./buildwindows.sh $1

echo "All Done..."
