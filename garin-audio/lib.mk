AUDIO_LIBS = -lfmod_vc -lfmodL_vc

# Opciones para el compilador
AUDIO_FLAGS = -Igarin-audio/include -Igarin-audio/src -Igarin-audio/include/lua
AUDIO_SRCMAINCPP = garin-audio/src/*.cpp

# Opciones para el enlazador
AUDIO_FLAGS_LIBS = garin-audio/libs