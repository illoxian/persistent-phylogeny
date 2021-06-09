
./bin/ppp.exe: ./obj/main.o ./obj/rbgraph.o ./obj/globals.o ./obj/functions.o ./obj/sparsematrix.o ./obj/graph.o ./obj/edge.o ./obj/clique.o
	g++ ./obj/main.o ./obj/rbgraph.o ./obj/globals.o ./obj/functions.o ./obj/sparsematrix.o ./obj/graph.o ./obj/edge.o ./obj/clique.o -o ./bin/ppp.exe -lboost_program_options
	
./obj/main.o: ./src/main.cpp
	mkdir ./obj/
	mkdir ./bin/
	g++ -c -I./src/ ./src/main.cpp -o ./obj/main.o
	
./obj/functions.o: ./src/functions.cpp
	g++ -c ./src/functions.cpp -o ./obj/functions.o

./obj/rbgraph.o: ./src/rbgraph.cpp
	g++ -c ./src/rbgraph.cpp -o ./obj/rbgraph.o
	
./obj/globals.o: ./src/globals.cpp
	g++ -c ./src/globals.cpp -o ./obj/globals.o
	
./obj/graph.o: ./src/LexBFS-master/src/Graph.cpp
	g++ -c -I./src/LexBFS-master/src/ ./src/LexBFS-master/src/Graph.cpp -o ./obj/graph.o
	
./obj/sparsematrix.o: ./src/LexBFS-master/src/SparseMatrix.cpp
	g++ -c -I./src/LexBFS-master/src/ ./src/LexBFS-master/src/SparseMatrix.cpp -o ./obj/sparsematrix.o
	
./obj/clique.o: ./src/LexBFS-master/src/Clique.cpp
	g++ -c -I./src/LexBFS-master/src/ ./src/LexBFS-master/src/Clique.cpp -o ./obj/clique.o
	
./obj/edge.o: ./src/LexBFS-master/src/Edge.cpp
	g++ -c ./src/LexBFS-master/src/Edge.cpp -o ./obj/edge.o
	
clean:
	rm -r ./obj ./bin