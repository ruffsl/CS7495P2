LIBS    = -lopencv_core -lopencv_nonfree -lopencv_features2d -lopencv_highgui -larmadillo -lgtsam -lboost_system -lopencv_imgproc
CFLAGS  =  -g 
CXX     = clang++
OBJ = Extractor.o Image.o utils.o clustering.o

clustertool: $(OBJ)
	$(CXX) $(CFLAGS) -o clustertool clustertool.cpp $(OBJ) $(LIBS)

main : $(OBJ) 
	$(CXX) $(CFLAGS) -o $ main main.cpp $(OBJ) $(LIBS)

clean : 
	rm -f $(OBJ_PATH)*.o *~ *.bak 

clear : 
	make clean;
	rm -f $(BIN_PATH)extract $(BIN_PATH)clustering makefile_depend

%.o : %.cpp
	$(CXX) -c $(CFLAGS) $<

