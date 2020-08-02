#!/bin/bash

# set -x

CURRENT=`pwd`
__pr="--print-path"
__name="xcode-select"
DEVELOPER=`${__name} ${__pr}`

MIN_IOS="9.0"
SDKVERSION=`xcrun -sdk iphoneos --show-sdk-version`

MPFR_VERSION="4.0.2"

BITCODE="-fembed-bitcode"
if [ "${SDKVERSION}" == "9.0" ]; then
	BITCODE=""
fi

OSX_PLATFORM=`xcrun --sdk macosx --show-sdk-platform-path`
OSX_SDK=`xcrun --sdk macosx --show-sdk-path`

IPHONEOS_PLATFORM=`xcrun --sdk iphoneos --show-sdk-platform-path`
IPHONEOS_SDK=`xcrun --sdk iphoneos --show-sdk-path`

IPHONESIMULATOR_PLATFORM=`xcrun --sdk iphonesimulator --show-sdk-platform-path`
IPHONESIMULATOR_SDK=`xcrun --sdk iphonesimulator --show-sdk-path`

CLANG=`xcrun --sdk iphoneos --find clang`
CLANGPP=`xcrun --sdk iphoneos --find clang++`


downloadMPFR()
{
    if [ ! -s ${CURRENT}/mpfr-${MPFR_VERSION}.tar.bz2 ]; then
        echo "Downloading MPFR"
        curl -L -o ${CURRENT}/mpfr-${MPFR_VERSION}.tar.bz2 http://www.mpfr.org/mpfr-current/mpfr-${MPFR_VERSION}.tar.bz2
    fi
}


build()
{
	ARCH=$1
	SDK=$2
	PLATFORM=$3
	ARGS=$4

	make clean
	make distclean

	export PATH="${PLATFORM}/Developer/usr/bin:${DEVELOPER}/usr/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"

	mkdir mpfrlib-${ARCH}

	EXTRAS=""
	if [ "${ARCH}" != "x64_86" ]; then
		EXTRAS="-miphoneos-version-min=${MIN_IOS} -no-integrated-as -arch ${ARCH} -target ${ARCH}-apple-darwin"
	fi

	CFLAGS="${BITCODE} -isysroot ${SDK} -Wno-error -Wno-implicit-function-declaration ${EXTRAS} "

	GMP_LIB_PATH="${CURRENT}/../gmp/gmplib-${ARCH}"
	GMP_PATH="${CURRENT}/../gmp"

	./configure  --prefix="${CURRENT}/mpfrlib-${ARCH}" CC="${CLANG}  ${CFLAGS}" CPP="${CLANG} -E" --with-gmp="${GMP_LIB_PATH}" \
	--host=aarch64-apple-darwin --disable-assembly --enable-static --disable-shared ${ARGS}

	echo "make in progress for ${ARCH}"
	make -j `sysctl -n hw.logicalcpu_max` &> "${CURRENT}/mpfrlib-${ARCH}-build.log"
	# if [ "${ARCH}" == "i386" ]; then
	# 	echo "check in progress for ${ARCH}"
	# 	make check &> "${CURRENT}/mpfrlib-${ARCH}-check.log"
	# fi
	echo "install in progress for ${ARCH}"
	make install &> "${CURRENT}/mpfrlib-${ARCH}-install.log"
}

downloadMPFR

rm -rf mpfr
tar xfj "mpfr-${MPFR_VERSION}.tar.bz2"
mv mpfr-${MPFR_VERSION} mpfr

cd mpfr
CURRENT=`pwd`


build "armv7" "${IPHONEOS_SDK}" "${IPHONEOS_PLATFORM}"
build "arm64" "${IPHONEOS_SDK}" "${IPHONEOS_PLATFORM}"
build "i386" "${IPHONESIMULATOR_SDK}" "${IPHONESIMULATOR_PLATFORM}"
build "x64_86" "${OSX_SDK}" "${OSX_PLATFORM}"

mkdir include
cp -r ${CURRENT}/mpfrlib-x64_86/include include/

mkdir lib

echo "doing lipo"

lipo \
        "${CURRENT}/mpfrlib-armv7/lib/libmpfr.a" \
        "${CURRENT}/mpfrlib-arm64/lib/libmpfr.a" \
        "${CURRENT}/mpfrlib-i386/lib/libmpfr.a" \
        "${CURRENT}/mpfrlib-x64_86/lib/libmpfr.a" \
        -create -output lib/libmpfr.a

echo "lib done"
echo "${CURRENT}/lib/libmpfr.a"
echo "${CURRENT}/include"