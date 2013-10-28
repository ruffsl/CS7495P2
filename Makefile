INCLUDE_PATH = \
	-I/usr/include \
	-I/usr/local/include \
	-I.

BIN_PATH = build/

OBJ_PATH = obj/

LIBS_PATH = \
	-L/lib \
	-L/usr/lib \
	-L/usr/local/lib

LIBS    = libgtsam opencv boost
CFLAGS  = -std=c++11 -c -g -O3 -Wall -ansi `pkg-config --libs --cflags $(LIBS)` $(INCLUDE_PATH)

OBJ = \
	$(OBJ_PATH)main.o \
	$(OBJ_PATH)Extractor.o \
	$(OBJ_PATH)Image.o \
	$(OBJ_PATH)utils.o \
	$(OBJ_PATH)clustering.o \
	$(OBJ_PATH)clustering_test.o \
	$(OBJ_PATH)gmm.o \
	$(OBJ_PATH)numerics.o

C_SOURCES = \
	Image/main.cpp \
	Image/Extractor.cpp \
	Image/Image.cpp \
	Image/utils.cpp \
	Image/clustering.cpp \
	Image/clustering_test.cpp \
	Image/gmm.cpp \
	Image/numerics.cpp \

COMPILO = g++

extract : $(OBJ) 
	$(COMPILO) -o $(BIN_PATH)extract $(OBJ) $(LIBS)
	
clustering : $(OBJ)
	$(COMPILO) -o $(BIN_PATH)clustering $(OBJ) $(LIBS)

clean : 
	rm -f $(OBJ_PATH)*.o *~ *.bak 

clear : 
	make clean;
	rm -f $(BIN_PATH)extract $(BIN_PATH)clustering makefile_depend

%.o : %.cc
	$(COMPILO) $(CFLAGS) $<

makefile_depend:
	touch makefile_depend

depend : makefile_depend
	makedepend -fmakefile_depend $(INCLUDE_PATH) $(C_SOURCES) 

include makefile_depend
