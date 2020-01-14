#include "MacroCell.h"

/* Constructor for a single cell */
MacroCell::MacroCell(bool living)
: alive(living),level(0),nw(nullptr),ne(nullptr),sw(nullptr),se(nullptr), population(alive ? 1 : 0)
{
}

/* General case constructor for a MacroCell using its four children */
MacroCell::MacroCell(MacroCell *nw_, MacroCell *ne_, MacroCell *sw_, MacroCell *se_)
: nw(nw_), ne(ne_),sw(sw_),se(se_),level(nw->level + 1),
  population(nw->population + ne->population + sw->population + se->population),
  alive(population > 0)
{
}

MacroCell* MacroCell::create(MacroCell *nw_, MacroCell *ne_, MacroCell *sw_, MacroCell *se_) {
	return new MacroCell(nw_,ne_,sw_,se_);
}

MacroCell* MacroCell::create(bool alive) {
	return new MacroCell(alive);
}

MacroCell* MacroCell::createNew()
{
	return (new MacroCell(false))->emptyTree(3);
}

MacroCell* MacroCell::centeredSubCell() {
	return create((MacroCell*)nw->se,
	              (MacroCell*)ne->sw,
	              (MacroCell*)sw->ne,
	              (MacroCell*)se->nw);
}

MacroCell* MacroCell::centeredHorizontal(MacroCell *w, MacroCell *e) {
	return create((MacroCell*)w->ne->se,
	              (MacroCell*)e->nw->sw,
	              (MacroCell*)w->se->ne,
	              (MacroCell*)e->sw->nw);
}

MacroCell* MacroCell::centeredVertical(MacroCell *n, MacroCell *s) {
	return create((MacroCell*)n->sw->se,
	              (MacroCell*)n->se->sw,
	              (MacroCell*)s->nw->ne,
	              (MacroCell*)s->ne->nw);
}

MacroCell* MacroCell::centeredSubSubCell() {
	return create((MacroCell*)nw->se->se,
	              (MacroCell*)ne->sw->sw,
	              (MacroCell*)sw->ne->ne,
	              (MacroCell*)se->nw->nw);
}
/*
 * Main recursion of the Hashlife algorithm
 * The RESULT of the current MacroCell is computed using
 * nine lower level MacroCells.
 */
MacroCell* MacroCell::nextStep() {
	if(population == 0)
		return nw;
	if(level == 2)
		return naiveSimulation();

	MacroCell* m00 = ((MacroCell*) nw)->centeredSubCell();
	MacroCell* m01 = centeredHorizontal((MacroCell*) nw,(MacroCell*) ne);
	MacroCell* m02 = ((MacroCell*) ne)->centeredSubCell();

	MacroCell* m10 = centeredVertical((MacroCell*) nw,(MacroCell*) sw);
	MacroCell* m11 = centeredSubSubCell();
	MacroCell* m12 = centeredVertical((MacroCell*) ne,(MacroCell*) se);

	MacroCell* m20 = ((MacroCell*) sw)->centeredSubCell();
	MacroCell* m21 = centeredHorizontal((MacroCell*) sw,(MacroCell*) se);
	MacroCell* m22 = ((MacroCell*) se)->centeredSubCell();

	return create(create(m00,m01,m10,m11)->nextStep(),
	              create(m01,m02,m11,m12)->nextStep(),
	              create(m10,m11,m20,m21)->nextStep(),
	              create(m11,m12,m21,m22)->nextStep());

}

/* Computes the next state of a single cell using
 * a bitmask to represent the states of neighboring cells.
 *
 * [8][9][10]
 * [4](5)[6]
 * [1][2][3]
 *
 *	Bit n°5 is the state of the current cell
 */
MacroCell* MacroCell::oneGeneration(int bitmask) {
	if(bitmask == 0)
		return create(false);

	int self_state = (bitmask >> 5) & 1;
	bitmask &= 0x757;
	int neighbors = 0;
	while(bitmask != 0) {
		neighbors++;
		bitmask &= bitmask - 1;
	}
	if(neighbors == 3 || (neighbors == 2 && self_state != 0))
		return create(true);
	else
		return create(false);
}

/* Uses naive simulation to compute the next state
 * of the four center cells at level 2 (4x4 MacroCell)
 */
MacroCell* MacroCell::naiveSimulation() {
	int allbits = 0;
	for(int y = -2; y < 2; y++)
		for(int x = -2; x < 2; x++)
			allbits = (allbits << 1) + this->getBit(x, y);
	return create(oneGeneration(allbits >> 5),
	              oneGeneration(allbits >> 4),
	              oneGeneration(allbits >> 1),
	              oneGeneration(allbits));
}

/* Recursive method making the single cell at coordinates (x,y) alive */
MacroCell* MacroCell::setBit(long x, long y) {
	if(level == 0)
		return new MacroCell(true);
	long offset = 1 << (level - 2);
	if(x < 0)
	{
		if(y < 0)
			return create(((MacroCell*)nw)->setBit(x+offset, y+offset),(MacroCell*)ne,(MacroCell*)sw,(MacroCell*)se);
		else
			return create((MacroCell*)nw, (MacroCell*)ne, ((MacroCell*)sw)->setBit(x+offset, y-offset), (MacroCell*)se);
	} else {
		if (y < 0)
			return create((MacroCell*)nw, ((MacroCell*)ne)->setBit(x-offset, y+offset), (MacroCell*)sw, (MacroCell*)se) ;
		else
			return create((MacroCell*)nw, (MacroCell*)ne, (MacroCell*)sw, ((MacroCell*)se)->setBit(x-offset, y-offset)) ;
	}
}

bool MacroCell::getBit(long x, long y)
{
	if(level == 0)
		return alive;
	long offset = 1 << (level - 2);
	if(x < 0)
		{
			if(y < 0)
				return ((MacroCell*)nw)->getBit(x+offset, y+offset);
			else
				return ((MacroCell*)sw)->getBit(x+offset, y-offset);
		} else {
			if (y < 0)
				return ((MacroCell*)ne)->getBit(x-offset, y+offset);
			else
				return ((MacroCell*)se)->getBit(x-offset, y-offset) ;
		}
}

MacroCell* MacroCell::emptyTree(int level) {
	if(level == 0)
		return create(false);
	MacroCell *m = emptyTree(level - 1);
	return create(m,m,m,m);
}

/* Creates a new MacroCell a level higher with
 * the current MacroCell at its center.
 */
MacroCell* MacroCell::expandUniverse() {
	MacroCell *border = emptyTree(level - 1);
	return create(create(border, border, border, (MacroCell*) nw),
	              create(border, border, (MacroCell*) ne, border),
	              create(border, (MacroCell*) sw, border, border),
	              create((MacroCell*) se, border, border, border));
}

