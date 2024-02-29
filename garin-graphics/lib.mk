GRAPHICS_LIBS = -lglfw3dll -lassimp

# Opciones para el compilador
GRAPHICS_FLAGS = -Igarin-graphics/includes -Igarin-graphics/src -Igarin-scene/include
GRAPHICS_MAIN  = garin-graphics/src/*.cpp
 
# Opciones para el enlazador
GRAPHICS_FLAGS_LIBS = garin-graphics/libs

# Ruta al archivo de código fuente de GLAD
GRAPHICS_GLAD_C = garin-graphics/libs/glad.c

# Ruta al directorio principal de tu código fuente
SRCMAINCPP = garin-graphics/src/*.cpp