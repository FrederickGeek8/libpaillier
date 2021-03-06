#!/bin/bash

# set -x

CURRENT=`pwd`
__pr="--print-path"
__name="xcode-select"
DEVELOPER=`${__name} ${__pr}`

MIN_IOS="9.0"
SDKVERSION=`xcrun -sdk iphoneos --show-sdk-version`

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


build()
{
	ARCH=$1
	SDK=$2
	PLATFORM=$3
	ARGS=$4

	make clean

	export PATH="${PLATFORM}/Developer/usr/bin:${DEVELOPER}/usr/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"

	mkdir "${CURRENT}/lib/${ARCH}"

	EXTRAS=""
	if [ "${ARCH}" != "x64_86" ]; then
		EXTRAS="-miphoneos-version-min=${MIN_IOS} -no-integrated-as -arch ${ARCH} -target ${ARCH}-apple-darwin"
	fi

	export CFLAGS="-g -Wall ${BITCODE} -isysroot ${SDK} -Wno-error -Wno-implicit-function-declaration ${EXTRAS} "

	echo "make in progress for ${ARCH}"
	make -j `sysctl -n hw.logicalcpu_max` lib
    mv "${CURRENT}/lib/libpaillier.a" "${CURRENT}/lib/${ARCH}/libpaillier.a"
}

CURRENT=`pwd`


build "armv7" "${IPHONEOS_SDK}" "${IPHONEOS_PLATFORM}"
# exit 1
build "arm64" "${IPHONEOS_SDK}" "${IPHONEOS_PLATFORM}"
build "i386" "${IPHONESIMULATOR_SDK}" "${IPHONESIMULATOR_PLATFORM}"
build "x64_86" "${OSX_SDK}" "${OSX_PLATFORM}"


echo "doing lipo"

lipo \
        "${CURRENT}/lib/armv7/libpaillier.a" \
        "${CURRENT}/lib/arm64/libpaillier.a" \
        "${CURRENT}/lib/i386/libpaillier.a" \
        "${CURRENT}/lib/x64_86/libpaillier.a" \
        -create -output libpaillier.a

echo "lib done"