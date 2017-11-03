CXX = clang++

SDL_LIB = `sdl-config --libs`
SDL_INCLUDE =  `sdl-config --cflags `

CXXFLAGS = -std=c++14 -O2 -g $(SDL_INCLUDE) -Iinclude -Ifixed_point/include -Istb  -IEASTL/include -IEASTL/test/packages/EABase/include/Common 

OBJS = SDLVersion/main.o \
        SDLVersion/CSDLRenderer.o \
        src/CRenderer.o \
        src/RaycastCommon.o \
        src/NativeBitmap.o \
        src/LoadPNG.o

LIBS = $(SDL_LIB)
TARGET = demo

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:   $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
