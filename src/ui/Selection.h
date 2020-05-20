#ifndef _SELECTION_H
#define _SELECTION_H

#include <model/Vec2.h>

enum class SelectionState {None, First, Second};

typedef struct SSelection {
  Vec2 first, second;
  SelectionState state;

  bool copy_available;
  Vec2 c_first, c_second;
} Selection;

#endif
