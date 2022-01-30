.PHONY: all clean

CXX=g++
SDLCFLAGS=`sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image
BUILDDIR=Build

all: result

clean:
	rm $(BUILDDIR)/*.o

$(BUILDDIR)/Vector2.o: Vector2.cpp Vector2.h
	$(CXX) -c -o $(BUILDDIR)/Vector2.o Vector2.cpp

$(BUILDDIR)/Random.o: Random.cpp Random.h
	$(CXX) -c -o $(BUILDDIR)/Random.o Random.cpp

$(BUILDDIR)/Particle.o: Particle.cpp Particle.h Vector2.h
	$(CXX) -c -o $(BUILDDIR)/Particle.o Particle.cpp

$(BUILDDIR)/Cell.o: Cell.cpp Cell.h Particle.h
	$(CXX) -c -o $(BUILDDIR)/Cell.o Cell.cpp 

$(BUILDDIR)/Field.o: Field.cpp Field.h Cell.h Particle.h FieldConfig.h
	$(CXX) -c -o $(BUILDDIR)/Field.o Field.cpp

$(BUILDDIR)/Display.o: Display.cpp Display.h Field.h DisplayConfig.h FieldConfig.h
	$(CXX) -c -o $(BUILDDIR)/Display.o Display.cpp $(SDLCFLAGS)

$(BUILDDIR)/ParticleFactory.o: ParticleFactory.cpp ParticleFactory.h Particle.h Vector2.h Random.h
	$(CXX) -c -o $(BUILDDIR)/ParticleFactory.o ParticleFactory.cpp	

$(BUILDDIR)/main.o: main.cpp Field.h Random.h Display.h ParticleFactory.h FieldConfig.h DisplayConfig.h
	$(CXX) -c -o $(BUILDDIR)/main.o main.cpp $(SDLCFLAGS)

result: $(BUILDDIR)/Vector2.o $(BUILDDIR)/Random.o $(BUILDDIR)/Particle.o $(BUILDDIR)/Cell.o $(BUILDDIR)/Field.o $(BUILDDIR)/Display.o $(BUILDDIR)/ParticleFactory.o $(BUILDDIR)/main.o
	$(CXX) -o result $(BUILDDIR)/Vector2.o $(BUILDDIR)/Random.o $(BUILDDIR)/Particle.o $(BUILDDIR)/Cell.o $(BUILDDIR)/Field.o $(BUILDDIR)/Display.o $(BUILDDIR)/ParticleFactory.o $(BUILDDIR)/main.o $(SDLCFLAGS)
