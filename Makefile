#INCLUDE LIBS
include garin-graphics/lib.mk
include garin-scene/lib.mk
include garin-maths/lib.mk
include garin-core/lib.mk
include garin-ui/lib.mk
include garin-physics/lib.mk
include garin-scripting/lib.mk
include garin-audio/lib.mk
include bin/projects/test/Game.mk
include garin-vr/lib.mk


GENERAL_INCLUDES = -Igarin-engine/includes/
SRC_DIR = garin-engine/src/*.cpp


# Variables
CXX = g++
CXXFLAGS = -g -fPIC --std=c++17 -Wall -Wextra -pthread -DGLM_ENABLE_EXPERIMENTAL -D_DEBUG -Wattributes 
INCLUDE_DIR = $(GRAPHICS_FLAGS) $(SCENEGARIN) $(MATHS_FLAGS) $(garin_core_includes) $(GARIN_UI_INCLUDES) $(PHYSICS_LIB_INCLUDES) $(CSSCRIPT_FLAGS) $(AUDIO_FLAGS) $(GAME_INCLUDE) $(VRGARIN_INCLUDES) $(GENERAL_INCLUDES)

LIB_DIR = -L$(GRAPHICS_FLAGS_LIBS) -L$(MATHS_FLAGS_LIBS) -L$(garin_core_includes_dir) -L$(AUDIO_FLAGS_LIBS) $(PHYSICS_LIBS) $(VRGARINLIBSDIR)

SOURCES = $(wildcard $(SRC_DIR) $(GRAPHICS_MAIN) $(GARINCOREMAINS) $(SCENEGARINMAINCPP) $(GARIN_UI_CPP) $(PHYSICS_LIB_CPP) $(AUDIO_SRCMAINCPP) $(GAME_CPP) $(MATHS_SOURCE) $(VRGARINMAINS))

C_CLASSES = $(GRAPHICS_GLAD_C)

TARGET = bin/Garin-Engine

LIBS = $(GRAPHICS_LIBS) $(MATHS_LIBS) $(VRGARINLIBS) $(AUDIO_LIBS) $(PHYSICS_LIBS_FILES)

all: $(TARGET)

$(TARGET): $(SOURCES) $(C_CLASSES)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) $(LIB_DIR) $(SOURCES) $(C_CLASSES) -o $(TARGET) $(LIBS) 

clean:
	rm -f *.o

.PHONY: all clean