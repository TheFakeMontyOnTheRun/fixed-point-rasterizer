CXX = em++
CXXFLAGS = -O3 -g -DEA_PLATFORM_SAMSUNG_TV  --preload-file res --use-preload-plugins -Iinclude -Ifixed_point/include -std=c++14 -Istb -IEASTL/include -IEASTL/test/packages/EABase/include/Common -std=c++14 -Iinclude -Ifixed_point/include -Istb -ferror-limit=1  -s USE_ZLIB=1 -s USE_LIBPNG=1 -s ALLOW_MEMORY_GROWTH=1 -DUSE_ITEMS_INSTANTLY

LDFLAGS =  -O3 -s USE_ZLIB=1 -s USE_LIBPNG=1 -s --preload-file res --use-preload-plugins  -s ALLOW_MEMORY_GROWTH=1

CXX = em++
CC = emcc

OBJS = SDLVersion/main.o \
        SDLVersion/CSDLRenderer.o \
        src/CRenderer.o \
        src/RasterizerCommon.o \
        src/NativeBitmap.o \
        src/LoadPNG.o
	
TARGET = demo.html

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(CXXFLAGS)

all:   $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
	rm demo.data
	rm demo.js