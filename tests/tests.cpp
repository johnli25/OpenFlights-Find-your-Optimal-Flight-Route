#include <string>
#include <vector>
#include <cfloat>
#include "../catch/catch.hpp"
#include "../readFromFile.hpp"
#include "../Heap.h"

TEST_CASE("Verify that file_to_Airport works on a small example") {
	std::vector<Graph::Airport> airports = file_to_Airport("tests/airportsSmall.dat.txt");
	std::vector<Graph::Airport> expected;
	Graph::Airport airport1;
	airport1.id = 1;
	airport1.name = "Goroka Airport";
	airport1.city = "Goroka";
	airport1.latitude = -6.081689834590001;
	airport1.longitude = 145.391998291;
	expected.push_back(airport1);
	Graph::Airport airport2;
	airport2.id = 2;
	airport2.name = "Madang Airport";
	airport2.city = "Madang";
	airport2.latitude = -5.20707988739;
	airport2.longitude = 145.789001465;
	expected.push_back(airport2);
	Graph::Airport airport3;
	airport3.id = 3;
	airport3.name = "Mount Airport";
	airport3.city = "Mount";
	airport3.latitude = -5.826789855957031;
	airport3.longitude = 144.29600524902344;
	expected.push_back(airport3);
	Graph::Airport airport4;
	airport4.id = 4;
	airport4.name = "Nadzab Airport";
	airport4.city = "Nadzab";
	airport4.latitude = -6.569803;
	airport4.longitude = 146.725977;
	expected.push_back(airport4);
	// Make sure they have the same size
	REQUIRE(airports.size() == expected.size());
	// Compare airports
	if (airports.size() == expected.size()){
		for (int i = 0; i < airports.size(); i++){
			REQUIRE(expected[i].id == airports[i].id);
			REQUIRE(expected[i].name == airports[i].name);
			REQUIRE(expected[i].city == airports[i].city);
			REQUIRE(expected[i].latitude == airports[i].latitude);
			REQUIRE(expected[i].longitude == airports[i].longitude);
		}
	}
}

TEST_CASE("Verify that file_to_Route works on a medium example") {
	std::vector<Graph::Route> routes = file_to_Route("tests/routesSmall.dat.txt");
	REQUIRE(1 == routes[0].sourceAirportId);
	REQUIRE(2 == routes[0].destinationAirportId);
	REQUIRE(2 == routes[1].sourceAirportId);
	REQUIRE(1 == routes[1].destinationAirportId);
	REQUIRE(1 == routes[2].sourceAirportId);
	REQUIRE(3 == routes[2].destinationAirportId);
	REQUIRE(3 == routes[3].sourceAirportId);
	REQUIRE(1 == routes[3].destinationAirportId);
	REQUIRE(2 == routes[4].sourceAirportId);
	REQUIRE(3 == routes[4].destinationAirportId);
	REQUIRE(3 == routes[5].sourceAirportId);
	REQUIRE(4 == routes[5].destinationAirportId);
}

TEST_CASE("Verify that custom constructor work"){
	std::vector<Graph::Airport> airports = file_to_Airport("tests/airportsSmall.dat.txt");
	std::vector<Graph::Route> routes = file_to_Route("tests/routesSmall.dat.txt");
	Graph airports1(airports, routes);
	for (int i = 0; i < airports.size(); i++){
		// Get the id of the airport
		int id = airports[i].id;
		REQUIRE(airports1.airports[id].id == id);
		REQUIRE(airports1.airports[id].name == airports[i].name);
		REQUIRE(airports1.airports[id].city == airports[i].city);
		REQUIRE(airports1.airports[id].latitude == airports[i].latitude);
		REQUIRE(airports1.airports[id].longitude == airports[i].longitude);
	}
	// Check number of valid airports
	REQUIRE(4 == airports1.numAirports);
	// Get the pointer to the edge list
	Graph::Route * cur = airports1.allRoutes;
	int index = 0;
	while (cur != NULL){
		REQUIRE(cur->sourceAirportId == routes[index].sourceAirportId);
		REQUIRE(cur->destinationAirportId == routes[index].destinationAirportId);
		index++;
		cur = cur->next;
	}
}

TEST_CASE("Verify that copy constructor work"){
	std::vector<Graph::Airport> airports = file_to_Airport("tests/airportsSmall.dat.txt");
	std::vector<Graph::Route> routes = file_to_Route("tests/routesSmall.dat.txt");
	Graph airports1(airports, routes);
	Graph airports2(airports1);
	for (int i = 0; i < airports1.airports.size(); i++){
		// If it is a valid airport, check if both graphs have the same airport
		if (airports1.airports[i].id != 0){
			REQUIRE(airports1.airports[i].id == airports2.airports[i].id);
			REQUIRE(airports1.airports[i].name == airports2.airports[i].name);
			REQUIRE(airports1.airports[i].city == airports2.airports[i].city);
			REQUIRE(airports1.airports[i].latitude == airports2.airports[i].latitude);
			REQUIRE(airports1.airports[i].longitude == airports2.airports[i].longitude);
			// Check their adjacency lists
			std::vector<Graph::Route*> cur1 = airports1.airports[i].routes;
			std::vector<Graph::Route*> cur2 = airports2.airports[i].routes;
			for (int index = 0; index < cur1.size(); index++){
				REQUIRE(cur1[index]->sourceAirportId == cur2[index]->sourceAirportId);
				REQUIRE(cur1[index]->destinationAirportId == cur2[index]->destinationAirportId);
				REQUIRE(cur1[index]->distance == cur2[index]->distance);
			}
		}
	}
	// Check number of valid airports
	REQUIRE(airports1.numAirports == airports2.numAirports);
	// Check their edge lists
	Graph::Route * cur1 = airports1.allRoutes;
	Graph::Route * cur2 = airports2.allRoutes;
	while (cur1 != NULL){
		REQUIRE(cur1->sourceAirportId == cur2->sourceAirportId);
		REQUIRE(cur1->destinationAirportId == cur2->destinationAirportId);
		REQUIRE(cur1->distance == cur2->distance);
		cur1 = cur1->next;
		cur2 = cur2->next;
	}
}

TEST_CASE("Verify that reset work"){
	std::vector<Graph::Airport> airports = file_to_Airport("tests/airportsSmall.dat.txt");
	std::vector<Graph::Route> routes = file_to_Route("tests/routesSmall.dat.txt");
	Graph airports1(airports, routes);
	for (int i = 0; i < airports1.airports.size(); i++){
		airports1.airports[i].distance = i;
		airports1.airports[i].heuristic = i;
		airports1.airports[i].visited = 1;
		airports1.airports[i].predecessorId = i;
	}
	airports1.reset();
	for (int i = 0; i < airports1.airports.size(); i++){
		if (airports1.airports[i].id != 0){
			REQUIRE(airports1.airports[i].distance == 0);
			REQUIRE(airports1.airports[i].heuristic == 0);
			REQUIRE(airports1.airports[i].visited == 0);
			REQUIRE(airports1.airports[i].predecessorId == 0);
		}
	}
}

// Helper function to test heapify_down
void helpTestHeapifyDown(const std::vector<Graph::Airport> & airports){
	Heap heap;
	std::vector<double> sortedDist;
	std::vector<double> sortedHeur;
	for (size_t i = 0; i < airports.size(); i++){
		heap.push(airports[i]);
		sortedDist.push_back(airports[i].distance);
		sortedHeur.push_back(airports[i].heuristic);
	}
	std::vector<double> sorted;
	for (int i = 0; i < sortedDist.size(); i++){
		sorted.push_back(sortedDist[i] + sortedHeur[i]);
	}
	std::sort(sorted.begin(), sorted.end());
	for (size_t index = 0; index < sorted.size(); index++){
		Graph::Airport ap = heap.pop();
		REQUIRE(sorted[index] == (ap.distance + ap.heuristic));
	}
}

TEST_CASE("Verify that heapify_down works on small data"){
	// Make invalid airport
	Graph::Airport filler;
	std::vector<Graph::Airport> airports;
	airports.push_back(filler);
	for (int i = 1; i < 5; i++){
		Graph::Airport ap;
		ap.id = i;
		ap.distance = i;
		ap.heuristic = 0;
		airports.push_back(ap);
	}
	helpTestHeapifyDown(airports);
}

TEST_CASE("Verify that heapify_down works on large data"){
	std::vector<double> distances = {47.5, 35.6, -20, 40, -10, 48.93, 89, 90, 109, 20, -31, -20.52, -20, 35.8, 30};
	std::vector<double> heuristic = {-10.4, 2.5, -40.1, 20, -30, 44, 99, 10.41, -10, 22, -1.2, -34, -10.3, -25, 30};
	// Make invalid airport
	Graph::Airport filler;
	std::vector<Graph::Airport> airports;
	airports.push_back(filler);
	for (int i = 1; i <= distances.size(); i++){
		Graph::Airport ap;
		ap.id = i;
		ap.distance = distances[i-1];
		ap.heuristic = heuristic[i-1];
		airports.push_back(ap);
	}
	helpTestHeapifyDown(airports);
}

TEST_CASE("Verify that heapify_down works for infinity distances 1"){
	// Make invalid airport
	Graph::Airport filler;
	std::vector<Graph::Airport> airports;
	airports.push_back(filler);
	for (int i = 1; i < 10; i++){
		Graph::Airport ap;
		ap.id = i;
		ap.distance = DBL_MAX;
		ap.heuristic = i;
		airports.push_back(ap);
	}
	helpTestHeapifyDown(airports);
}

TEST_CASE("Verify that heapify_down works for infinity distances 2"){
	std::vector<double> heuristic = {-10.1, 2.6, -40, 20, -30, 44, 99.221, -10, -10, 22.3, -1, -3.24, -10, -25.41, 30};
	// Make invalid airport
	Graph::Airport filler;
	std::vector<Graph::Airport> airports;
	airports.push_back(filler);
	for (int i = 1; i <= heuristic.size(); i++){
		Graph::Airport ap;
		ap.id = i;
		ap.distance = DBL_MAX;
		ap.heuristic = heuristic[i-1];
		airports.push_back(ap);
	}
	helpTestHeapifyDown(airports);
}

TEST_CASE("Verify that heapify_down works on vertex tables with invalid airports"){
	// Make invalid airport
	Graph::Airport filler;
	std::vector<Graph::Airport> airports;
	airports.push_back(filler);
	for (int i = 1; i < 20; i++){
		if (i%2 == 0){
			Graph::Airport ap;
			ap.id = i;
			ap.distance = i;
			ap.heuristic = i;
			airports.push_back(ap);
		}else{
			Graph::Airport invalid;
			airports.push_back(invalid);
		}
	}
	helpTestHeapifyDown(airports);
}

TEST_CASE("Verify that heapify_down works on vertex tables with invalid airports 2"){
	std::vector<double> distances = {15.122, 32, -20.533, 40, -10, 48, 89.11, 90, 109.83, 20, -31.86, -20, -20.51, 35, 30};
	std::vector<double> heuristic = {-10, 26.31, -40.3111, 20, -30, 44, 99, 10.664, -10, 22, -1, -34.12, -10, -25.753, 30.3131};
	// Make invalid airport
	Graph::Airport filler;
	std::vector<Graph::Airport> airports;
	airports.push_back(filler);
	for (int i = 1; i < distances.size(); i++){
		if (i%2 == 0){
			Graph::Airport ap;
			ap.id = i;
			ap.distance = distances[i-1];
			ap.heuristic = heuristic[i-1];
			airports.push_back(ap);
		}else{
			Graph::Airport invalid;
			airports.push_back(invalid);
		}
	}
	helpTestHeapifyDown(airports);
}

TEST_CASE("Verify that build_heap works"){
	// Make invalid airport
	Graph::Airport filler;
	std::vector<Graph::Airport> airports;
	airports.push_back(filler);
	std::vector<double> sorted;
	for (int i = 1; i < 5; i++){
		Graph::Airport ap;
		ap.id = i;
		ap.distance = i;
		sorted.push_back(i);
		ap.heuristic = 0;
		airports.push_back(ap);
	}
	Heap heap(airports);
	std::vector<Graph::Airport> test;
	heap.getElems(test);
	for (int i = 0; i < sorted.size(); i++){
		REQUIRE(sorted[i] == test[i].distance);
	}
}

TEST_CASE("Verify that build_heap works 2"){
	std::vector<double> distances = {15, 32, -20, 40, -10, 48, 89, 90, 109, 20, -31, -20, -20, 35, 30};
	// Make invalid airport
	Graph::Airport filler;
	std::vector<Graph::Airport> airports;
	airports.push_back(filler);
	for (int i = 1; i <= distances.size(); i++){
		Graph::Airport ap;
		ap.id = i;
		ap.distance = distances[i-1];
		ap.heuristic = 0;
		airports.push_back(ap);
	}
	std::vector<double> expected = {-31, -10, -20, 40, 15, -20, 30, 90, 109, 20, 32, 48, -20, 35, 89};
	Heap heap(airports);
	std::vector<Graph::Airport> test;
	heap.getElems(test);
	for (int i = 0; i < expected.size(); i++){
		REQUIRE(expected[i] == test[i].distance);
	}
}

TEST_CASE("Verify that build_heap works 3"){
	std::vector<Graph::Airport> ap = file_to_Airport("tests/airportsSmall.dat.txt");
	std::vector<Graph::Route> routes = file_to_Route("tests/routesSmall.dat.txt");
	Graph airports1(ap, routes);
	std::vector<Graph::Airport> airports = airports1.airports;
	for (int i = 0; i < airports.size(); i++){
		airports[i].distance = i;
	}
	Heap heap(airports);
	std::vector<Graph::Airport> test;
	heap.getElems(test);
	for (int i = 0; i < test.size(); i++){
		REQUIRE(i+1 == test[i].distance);
	}
}

TEST_CASE("Verify that build_heap works 4"){
	std::vector<double> distances = {15, 32, -20, 40, -10};
	std::vector<Graph::Airport> ap = file_to_Airport("tests/airportsSmall.dat.txt");
	std::vector<Graph::Route> routes = file_to_Route("tests/routesSmall.dat.txt");
	Graph airports1(ap, routes);
	std::vector<Graph::Airport> airports = airports1.airports;
	for (int i = 0; i < airports.size(); i++){
		airports[i].distance = distances[i];
	}
	std::vector<double> expected = {-20, -10, 40, 32};
	Heap heap(airports);
	std::vector<Graph::Airport> test;
	heap.getElems(test);
	for (int i = 0; i < test.size(); i++){
		REQUIRE(expected[i] == test[i].distance);
	}
}

TEST_CASE("Verify that updateElem works on id out of bound"){
	std::vector<double> distances = {15, 32, -20, 40, -10};
	std::vector<Graph::Airport> ap = file_to_Airport("tests/airportsSmall.dat.txt");
	std::vector<Graph::Route> routes = file_to_Route("tests/routesSmall.dat.txt");
	Graph airports1(ap, routes);
	std::vector<Graph::Airport> airports = airports1.airports;
	for (int i = 0; i < airports.size(); i++){
		airports[i].distance = distances[i];
	}
	std::vector<double> expected = {-20, -10, 40, 32};
	Heap heap(airports);
	Graph::Airport newAirport;
	// Should not update anything since the id is not in the heap
	newAirport.id = 10;
	newAirport.distance = -40;
	heap.updateElem(newAirport);
	std::vector<Graph::Airport> test;
	heap.getElems(test);
	// The heap should be the same as before since updateElem should do nothing
	for (int i = 0; i < test.size(); i++){
		REQUIRE(expected[i] == test[i].distance);
	}
}

TEST_CASE("Verify that updateElem works on data"){
	std::vector<double> distances = {15, 32, -20, 40, -10};
	std::vector<Graph::Airport> ap = file_to_Airport("tests/airportsSmall.dat.txt");
	std::vector<Graph::Route> routes = file_to_Route("tests/routesSmall.dat.txt");
	Graph airports1(ap, routes);
	std::vector<Graph::Airport> airports = airports1.airports;
	for (int i = 0; i < airports.size(); i++){
		airports[i].distance = distances[i];
	}
	std::vector<double> expected = {-40, -10, -20, 32};
	Heap heap(airports);
	Graph::Airport newAirport;
	newAirport.id = 3;
	newAirport.distance = -40;
	heap.updateElem(newAirport);
	std::vector<Graph::Airport> test;
	heap.getElems(test);
	for (int i = 0; i < test.size(); i++){
		REQUIRE(expected[i] == test[i].distance);
	}
}

TEST_CASE("Verify that updateElem works up with distance"){
	std::vector<double> distances = {15, 32, -20, 40, -10, 48, 89, 90, 109, 20, -31, -20, -20, 35, 30};
	// Make invalid airport
	Graph::Airport filler;
	std::vector<Graph::Airport> airports;
	airports.push_back(filler);
	for (int i = 1; i <= distances.size(); i++){
		Graph::Airport ap;
		ap.id = i;
		ap.distance = distances[i-1];
		ap.heuristic = 0;
		airports.push_back(ap);
	}
	std::vector<double> expected = {-40, -10, -31, 40, 15, -20, 30, 90, 109, 20, 32, 48, -20, 35, 89};
	Heap heap(airports);
	Graph::Airport newAirport;
	newAirport.id = 3;
	newAirport.distance = -40;
	heap.updateElem(newAirport);
	std::vector<Graph::Airport> test;
	heap.getElems(test);
	for (int i = 0; i < test.size(); i++){
		REQUIRE(expected[i] == test[i].distance);
	}
}

TEST_CASE("Verify that updateElem works up with heuristic"){
	std::vector<double> distances = {15, 32, -24, 40, -10, 48, 89, 90, 109, 20, -31, -20, -22, 35, 30};
	// Make invalid airport
	Graph::Airport filler;
	std::vector<Graph::Airport> airports;
	airports.push_back(filler);
	for (int i = 1; i <= distances.size(); i++){
		Graph::Airport ap;
		ap.id = i;
		ap.distance = distances[i-1];
		ap.heuristic = 0;
		airports.push_back(ap);
	}
	std::vector<double> expected = {-21, -10, -31, 40, 15, -22, 30, 90, 109, 20, 32, -20, 48, 35, 89};
	Heap heap(airports);
	Graph::Airport newAirport;
	newAirport.id = 3;
	newAirport.distance = -21;
	newAirport.heuristic = -20;
	heap.updateElem(newAirport);
	std::vector<Graph::Airport> test;
	heap.getElems(test);
	for (int i = 0; i < test.size(); i++){
		REQUIRE(expected[i] == test[i].distance);
	}
}

TEST_CASE("Verify that updateElem works down with heuristic"){
	std::vector<double> distances = {15, 32, -24, 40, -10, 48, 89, 90, 109, 20, -31, -20, -22, 35, 30};
	// Make invalid airport
	Graph::Airport filler;
	std::vector<Graph::Airport> airports;
	airports.push_back(filler);
	for (int i = 1; i <= distances.size(); i++){
		Graph::Airport ap;
		ap.id = i;
		ap.distance = distances[i-1];
		ap.heuristic = 0;
		airports.push_back(ap);
	}
	std::vector<double> expected = {-31, -10, -22, 40, 15, -20, 30, 90, 109, 20, 32, -60, 48, 35, 89};
	Heap heap(airports);
	Graph::Airport newAirport;
	newAirport.id = 3;
	newAirport.distance = -60;
	newAirport.heuristic = 300;
	heap.updateElem(newAirport);
	std::vector<Graph::Airport> test;
	heap.getElems(test);
	for (int i = 0; i < test.size(); i++){
		REQUIRE(expected[i] == test[i].distance);
	}
}

TEST_CASE("BFS_basic"){

}
