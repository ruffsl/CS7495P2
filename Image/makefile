LIBS    = -lopencv_core -lopencv_nonfree -lopencv_features2d -lopencv_highgui -larmadillo -lgtsam -lboost_system -lopencv_imgproc -lboost_filesystem
CFLAGS  =  -g -std=c++11 -static-libstdc++ 
CXX     = clang++
OBJ = Extractor.o Image.o utils.o clustering.o

kalman_tool2: $(OBJ)
	$(CXX) $(CFLAGS) -o kalman_tool2 kalman_tool2.cpp $(OBJ) $(LIBS)

kalman_tool: $(OBJ)
	$(CXX) $(CFLAGS) -o kalman_tool kalman_tool.cpp $(OBJ) $(LIBS)

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

