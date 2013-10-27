LIBS    = -lopencv_core -lopencv_nonfree -lopencv_features2d -lopencv_highgui -larmadillo
CFLAGS  =  -g -std=c++11 
CXX     = clang++
OBJ = Extractor.o Image.o utils.o clustering.o

main : $(OBJ) 
	$(CXX) $(CFLAGS) -o $ main main.cpp $(OBJ) $(LIBS)

clean : 
	rm -f $(OBJ_PATH)*.o *~ *.bak 

clear : 
	make clean;
	rm -f $(BIN_PATH)extract $(BIN_PATH)clustering makefile_depend

%.o : %.cpp
	$(CXX) -c $(CFLAGS) $<

