#include "Graph.h"

// Vertex::Vertex(){
// 	id = -1;
// 	degree = 0;
// }

// Vertex::Vertex(const int& id){
// 	this->id = id;
// 	degree = 0;
// }

// Vertex::~Vertex(){
	
// }

Graph::Graph(){
	
}

Graph::~Graph(){
	if(!vertices_.empty()){
		for(auto u : vertices_) delete u;
	}
}