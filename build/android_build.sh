#!/bin/bash

#TARGET_ARCH	Supported architectures:
#x86 (default)
#x86_64
#armeabi
#armeabi-v7a
TARGET_ARCH="armeabi"

#TARGET_TRANSPORT	Supported transport types:
#ALL (default)
#IP
#BT
#BLE
#Note: You may also build permutations of the above options by passing a comma-separated list.
TARGET_TRANSPORT=""


#RELEASE	Supported release modes:
#1 (release mode) (default)
#0 (debug mode)
RELEASE="1"


#SECURED	If DTLS (Datagram Transport Layer Security) is enabled or not.
#0 (DTLS disabled) (default)
#1 (DTLS enabled)

SECURED=""
#ANDROID_HOME	This takes the path to the Android SDK (if installed). If not, the SCons script will try to install Android SDK.
ANDROID_HOME=""

#ANDROID_NDK	This takes the path to the Android NDK (if installed). If not, the SCons script will try to install Android NDK.
ANDROID_NDK=""

#ANDROID_GRADLE	This takes the path to the Gradle bin (if installed). If not, the SCons script will install Gradle.
ANDROID_GRADLE=""

SCONS="scons"
FLAG=" TARGET_OS=android"

if [ "$TARGET_ARCH" ]; then
	FLAG=$FLAG" TARGET_ARCH="$TARGET_ARCH
fi

if [ "$TARGET_TRANSPORT" ]; then
	FLAG=$FLAG" TARGET_TRANSPORT="$TARGET_TRANSPORT
fi

if [ "$RELEASE" ]; then
	FLAG=$FLAG" RELEASE="$RELEASE
fi

if [ "$SECURED" ]; then
	FLAG=$FLAG" SECURED="$SECURED
fi

if [ "$ANDROID_HOME" ]; then
	FLAG=$FLAG" ANDROID_HOME="$ANDROID_HOME
fi

if [ "$ANDROID_NDK" ]; then
	FLAG=$FLAG" ANDROID_NDK="$ANDROID_NDK
fi

if [ "$ANDROID_GRADLE" ]; then
	FLAG=$FLAG" ANDROID_GRADLE="$ANDROID_GRADLE
fi

script_dir=$(cd "$(dirname "$0")" && pwd)
cd $script_dir/../
echo "root directory : "$(pwd)

$SCONS$FLAG

