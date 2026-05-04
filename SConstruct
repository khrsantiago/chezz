#!/usr/bin/env python
import os
import sys

# Provee la ruta a la carpeta godot-cpp
env = SConscript("godot-cpp/SConstruct")

# Agrega los archivos de cabecera de tu proyecto
env.Append(CPPPATH=["src/"])

# Busca todos los archivos .cpp en la carpeta src
sources = Glob("src/*.cpp")

# Genera la librería compartida en la carpeta bin
# El nombre debe coincidir con lo que tienes en el archivo .gdextension
library = env.SharedLibrary(
    "bin/libchezz{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
    source=sources,
)

Default(library)
