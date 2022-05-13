CXX=g++

BUILDDIR=build

STATIC=main disp prog util prop
OBJ_STATIC=$(patsubst %, $(BUILDDIR)/%.o, $(STATIC))

HDR=math.h col.h

LDFLAGS+=-lGLEW -lGL
LDFLAGS+=-lSDL2

.PHONY: clean

all: mk_build make

$(BUILDDIR)/%.o: %.cpp %.h
	$(CXX) -c $< -o $@ $(LDFLAGS)

$(BUILDDIR)/main.o: main.cpp
	$(CXX) -c $< -o $@ $(LDFLAGS)

make: $(OBJ_STATIC) $(HDR)
	$(CXX) $(OBJ_STATIC) $(LDFLAGS)

mk_build:
	mkdir -p $(BUILDDIR)

clean:
	rm $(BUILDDIR)/*.o a.out
