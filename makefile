CC = g++
CCFLAGS = -std=c++11 -g 
LDFLAGS = -lfltk
SOURCES = main.cpp NeuralNet.cpp Neuron.cpp Trainer.cpp Frontend.cpp PixelButton.cpp
HEADERS = NeuralNet.h Neuron.h Trainer.h Frontend.h PixelButton.h
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = net


$(TARGET): $(OBJECTS)
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ $^

$(OBJECTS): $(HEADERS)

clean:
	rm *.o $(TARGET)
