#include "Universe.h"

Universe::Universe() {
	root = MacroCell::createNew();
}

std::size_t Universe::getPopulation(){
	return root->population;
}

Universe::~Universe() {
	delete root;
}


