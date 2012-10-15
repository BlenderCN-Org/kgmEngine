include Makefile.mac

all:
	echo "compiling..."
	make -C kgmBase -f Makefile
	make -C kgmMath -f Makefile
	make -C kgmNet -f Makefile
	make -C kgmSystem -f Makefile
	make -C kgmGraphics -f Makefile
	make -C kgmPhysics -f Makefile
	make -C kgmGame -f Makefile

	echo "linking..."
	ar crs libkgmEngine.a kgmBase/*.o
	ar crs libkgmEngine.a kgmMath/*.o
	ar crs libkgmEngine.a kgmNet/*.o
	ar crs libkgmEngine.a kgmSystem/*.o
	ar crs libkgmEngine.a kgmPhysics/*.o
	ar crs libkgmEngine.a kgmGraphics/*.o
	ar crs libkgmEngine.a kgmGame/*.o

	echo "building"
	$(CC) -shared -o libkgmEngine.so $(DEFS) $(OBJS) $(DIRS) $(LIBS)

clean:
	echo "clear all..."
	$(RM) kgmBase/*.o
	$(RM) kgmMath/*.o
	$(RM) kgmNet/*.o
	$(RM) kgmSystem/*.o
	$(RM) kgmGraphics/*.o
	$(RM) kgmPhysics/*.o
	$(RM) kgmGame/*.o

distclean: clean	
	$(RM) libkgmEngine.a
	$(RM) libkgmEngine.so

kgmBase:	
	make -C kgmBase -f Makefile
	ar crs libkgmEngine.a kgmBase/*.o

kgmMath:
	make -C kgmMath -f Makefile
	ar crs libkgmEngine.a kgmMath/*.o

kgmNet:
	make -C kgmNet -f Makefile
	ar crs libkgmEngine.a kgmNet/*.o

kgmGraphics:
	make -C kgmGraphics -f Makefile
	ar crs libkgmEngine.a kgmAI/*.o

kgmPhysics:
	make -C kgmPhysics -f Makefile
	ar crs libkgmEngine.a kgmPhysics/*.o

kgmAI:
	make -C kgmAI -f Makefile
	ar crs libkgmEngine.a kgmGraphics/*.o

kgmGame:
	make -C kgmGame -f Makefile
	ar crs libkgmEngine.a kgmGame/*.o

android:



