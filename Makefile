CXX=g++

BUILDDIR=build

STATIC=main disp prog util prop
OBJ_STATIC=$(patsubst %, $(BUILDDIR)/%.o, $(STATIC))

HDR=col.h

GL=-lGLEW -lGL
SDL=-lSDL2
LDFLAGS+=$(SDL)
LDFLAGS+=$(GL)

.PHONY: clean

all: make

$(BUILDDIR)/%.o: %.cpp %.h
	$(CXX) -c $< -o $@ $(LDFLAGS)

$(BUILDDIR)/main.o: main.cpp
	$(CXX) -c $< -o $@ $(LDFLAGS)

make: $(OBJ_STATIC) $(HDR)
	$(CXX) $(OBJ_STATIC) $(LDFLAGS)

clean:
	rm $(BUILDDIR)/*.o a.out
