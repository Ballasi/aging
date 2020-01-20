#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#include "MacroCell.h"

class Universe {
public:
	Universe();
	virtual ~Universe();
	void expand();
	void clear();

	std::size_t getPopulation();

private:
	MacroCell *root;

};

#endif /* UNIVERSE_H_ */
