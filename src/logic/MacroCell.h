#ifndef MACROCELL_H
#define MACROCELL_H

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>


typedef int AtomicCell;
struct SMacroCell;
union UQuadrant;
struct SMiniCell;

typedef struct SMiniCell {
  struct SMiniCell* next;
  AtomicCell nw, ne;
  AtomicCell sw, se;
} MiniCell;

typedef struct SMacroCell {
  struct SMacroCell* next;
  union UQuadrant *result;
  union UQuadrant *nw, *ne;
  union UQuadrant *sw, *se;
} MacroCell;

typedef union UQuadrant {
  MiniCell minicell;
  struct SMacroCell macrocell;
} Quadrant;

#endif // MACROCELL_H