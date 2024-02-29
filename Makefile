#INCLUDE LIBS
include garin-graphics/lib.mk
include garin-scene/lib.mk
include garin-maths/lib.mk
include garin-core/lib.mk
include garin-ui/lib.mk
#include garin-vr/lib.mk

# Variables
CXX = g++
CXXFLAGS = -g --std=c++20 -Wall -DGLM_ENABLE_EXPERIMENTAL #-DNDEBUG
INCLUDE_DIR = $(GRAPHICS_FLAGS) $(SCENEGARIN) $(MATHS_FLAGS) $(garin_core_includes) $(GARIN_UI_INCLUDES) #$(VRGARIN)

LIB_DIR = -L$(GRAPHICS_FLAGS_LIBS) -L$(MATHS_FLAGS_LIBS) $(garin_core_includes_dir)#-L$(VRGARINLIBSDIR)
SRC_DIR = src/

SOURCES = $(wildcard $(SRC_DIR)/*.cpp $(GRAPHICS_MAIN) $(GARINCOREMAINS) $(SCENEGARINMAINCPP) $(GARIN_UI_CPP)) #$(VRGARINMAINS)

C_CLASSES = $(GRAPHICS_GLAD_C)

TARGET = bin/Garin-Engine

LIBS = $(GRAPHICS_LIBS) $(MATHS_LIBS) $(VRGARINLIBS)

all: $(TARGET)

$(TARGET): $(SOURCES) $(C_CLASSES)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) $(LIB_DIR) $(SOURCES) $(C_CLASSES) -o $(TARGET) $(LIBS)

clean:
	rm -f *.o

.PHONY: all clean