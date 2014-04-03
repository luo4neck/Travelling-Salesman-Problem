#tsp3: tsp3.cc 
#	g++ -Wall tsp3.cc -O3 -o tsp3

tsp1: tsp1.cc 
	g++ -Wall tsp1.cc -O3 -o tsp1


#test: tsp3
#	./tsp3 ulysses16.tsp


test: tsp1
	./tsp1 -n 6 
	./tsp1 -f map.dat 
