#ifndef MACROCELL_H_
#define MACROCELL_H_

#include <stdlib.h>

class MacroCell {

public:

	MacroCell(bool alive);
	MacroCell(MacroCell *nw_, MacroCell *ne_, MacroCell *sw_, MacroCell *se_);
	virtual ~MacroCell();

	MacroCell* create(bool alive);
	MacroCell* create(MacroCell *nw_, MacroCell *ne_, MacroCell *sw_, MacroCell *se_);
	static MacroCell* createNew();

	MacroCell* setBit(long x, long y);
	bool getBit(long x, long y);

	MacroCell* emptyTree(int level);
	MacroCell* expandUniverse();

	MacroCell* oneGeneration(int bitmask);
	MacroCell* naiveSimulation();

	MacroCell* centeredSubCell();
	MacroCell* centeredHorizontal(MacroCell *w, MacroCell *e);
	MacroCell* centeredVertical(MacroCell *n, MacroCell *s);
	MacroCell* centeredSubSubCell();

	MacroCell* nextStep();

	MacroCell *nw, *ne, *sw, *se;
	bool alive;
	int level;
	std::size_t population;
};

#endif /* MACROCELL_H_ */
