# $Id: Make_check.mk 331 2016-04-22 20:18:41Z gerardo $
# Comprobaciones de código para P4
# ©2015 Pedro Delgado, para POO
#

# Es obligatorio el uso del compilador clang++
CXX      := clang++
RTTIFLAG := -fno-rtti
CXXFLAGS := $(shell llvm-config --cxxflags) $(RTTIFLAG)
# Descomente la siguiente línea de LDFLAGS para obtener un ejecutable enlazado
# estáticamente, más grande pero susceptible de distribuir al alumnado que
# tenga problemas para compilar o enlazar por falta de paquetes o versiones
# antiguas.
#LDFLAGS := -static
LLVMLDFLAGS := $(shell llvm-config --ldflags --system-libs --libs) $(LDFLAGS)

SOURCES   = catalogo_check.cpp
OBJECTS   = $(SOURCES:.cpp=.o)
EXES      = $(OBJECTS:.o=)
CLANGLIBS =                  \
	-lclangFrontend      \
	-lclangSerialization \
	-lclangDriver        \
	-lclangTooling       \
	-lclangParse         \
	-lclangSema          \
	-lclangAnalysis      \
	-lclangEdit          \
	-lclangAST           \
	-lclangASTMatchers   \
	-lclangLex           \
	-lclangBasic         \
	-lclangRewrite       \
	-lclangRewriteFrontend

all: $(EXES)

.PHONY: clean all

${EXES}: ${OBJECTS}
	$(CXX) -o $@ $^ $(CLANGLIBS) $(LLVMLDFLAGS)

clean:
	-${RM} ${RMFLAGS} $(EXES) $(OBJECTS)

