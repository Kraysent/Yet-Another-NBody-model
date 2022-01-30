.PHONY: all clean

CXX=g++
SDLCFLAGS=`sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image
COMMONFLAGS=-std=c++1z
BUILDDIR=Build

all: result

clean:
	rm $(BUILDDIR)/*.o

$(BUILDDIR)/Vector2.o: Vector2.cpp Vector2.h
	$(CXX) -c -o $(BUILDDIR)/Vector2.o Vector2.cpp $(COMMONFLAGS)

$(BUILDDIR)/Random.o: Random.cpp Random.h
	$(CXX) -c -o $(BUILDDIR)/Random.o Random.cpp $(COMMONFLAGS)

$(BUILDDIR)/Particle.o: Particle.cpp Particle.h Vector2.h
	$(CXX) -c -o $(BUILDDIR)/Particle.o Particle.cpp $(COMMONFLAGS)

$(BUILDDIR)/Cell.o: Cell.cpp Cell.h Particle.h
	$(CXX) -c -o $(BUILDDIR)/Cell.o Cell.cpp $(COMMONFLAGS)

$(BUILDDIR)/Field.o: Field.cpp Field.h Cell.h Particle.h FieldConfig.h
	$(CXX) -c -o $(BUILDDIR)/Field.o Field.cpp $(COMMONFLAGS)

$(BUILDDIR)/Display.o: Display.cpp Display.h Field.h DisplayConfig.h FieldConfig.h
	$(CXX) -c -o $(BUILDDIR)/Display.o Display.cpp $(SDLCFLAGS) $(COMMONFLAGS)

$(BUILDDIR)/ParticleFactory.o: ParticleFactory.cpp ParticleFactory.h Particle.h Vector2.h Random.h
	$(CXX) -c -o $(BUILDDIR)/ParticleFactory.o ParticleFactory.cpp $(COMMONFLAGS)

$(BUILDDIR)/main.o: main.cpp Field.h Random.h Display.h ParticleFactory.h FieldConfig.h DisplayConfig.h
	$(CXX) -c -o $(BUILDDIR)/main.o main.cpp $(SDLCFLAGS) $(COMMONFLAGS)

result: $(BUILDDIR)/Vector2.o $(BUILDDIR)/Random.o $(BUILDDIR)/Particle.o $(BUILDDIR)/Cell.o $(BUILDDIR)/Field.o $(BUILDDIR)/Display.o $(BUILDDIR)/ParticleFactory.o $(BUILDDIR)/main.o
	$(CXX) -o result $(BUILDDIR)/Vector2.o $(BUILDDIR)/Random.o $(BUILDDIR)/Particle.o $(BUILDDIR)/Cell.o $(BUILDDIR)/Field.o $(BUILDDIR)/Display.o $(BUILDDIR)/ParticleFactory.o $(BUILDDIR)/main.o $(SDLCFLAGS) $(COMMONFLAGS)
