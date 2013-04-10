ANDROID_NDK_BIN:=$(NDK_ROOT)/toolchains/arm-linux-androideabi-4.6/prebuilt/linux-x86_64/bin

# Compilation Tools
CC  := $(ANDROID_NDK_BIN)/arm-linux-androideabi-gcc
CPP := $(ANDROID_NDK_BIN)/arm-linux-androideabi-g++
AR  := $(ANDROID_NDK_BIN)/arm-linux-androideabi-ar

# Compiler flags
# Specify all the flags below which you want to use for your compilation, For this simple example, we only need to specify the include directory path
CFLAGS          := -I$(NDK_ROOT)/platforms/android-9/arch-arm/usr/include
CFLAGS          += -DANDROID -DGLES_2 -fpermissive -g #-Werror  

subdirs := $(wildcard ../kgm*/)
sources := $(wildcard $(addsuffix *.cpp,$(subdirs)))
objects := $(patsubst %.cpp,%.o,$(sources))

OUT = libkgmEngine.a

all: $(OUT)

$(objects) : %.o : %.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(OUT): $(objects)
	$(AR) -crs $(OUT) $(objects)


clean:
	rm -f $(objects)
	rm -f $(OUT)