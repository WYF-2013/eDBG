export NDK_ROOT=/root/android-ndk-r27d
export GOTOOLCHAIN=local
export GOPROXY=https://goproxy.cn,direct

export PATH=$NDK_ROOT/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH

make clean && make
