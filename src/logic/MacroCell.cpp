#include "MacroCell.h"

Quadrant* MacroCell_new(Quadrant* nw, Quadrant* ne,
                        Quadrant* sw, Quadrant* se) {
  MacroCell* macro_cell = (MacroCell*) malloc(sizeof(MacroCell));
  macro_cell->nw = (Quadrant*)nw;
  macro_cell->ne = (Quadrant*)ne;
  macro_cell->sw = (Quadrant*)sw;
  macro_cell->se = (Quadrant*)se;
  return (Quadrant*) macro_cell;
}

Quadrant* MiniCell_new(AtomicCell nw, AtomicCell ne,
                       AtomicCell sw, AtomicCell se) {
  MiniCell *mini_cell = (MiniCell*) malloc(sizeof(MiniCell));
  mini_cell->nw = nw;
  mini_cell->ne = ne;
  mini_cell->sw = sw;
  mini_cell->se = se;
  return (Quadrant*)mini_cell;
}


void MacroCell_debug(size_t level, Quadrant* quadrant) { 
  if (level == 1) {
    printf("MiniCell_new(%d,%d,%d,%d)", 
      quadrant->mini_cell.nw, quadrant->mini_cell.ne,
      quadrant->mini_cell.sw, quadrant->mini_cell.se);
  } else {
    printf("MacroCell_new(");
    MacroCell_debug(level-1, quadrant->macro_cell.nw);
    printf(",");
    MacroCell_debug(level-1, quadrant->macro_cell.ne);
    printf(",");
    MacroCell_debug(level-1, quadrant->macro_cell.sw);
    printf(",");
    MacroCell_debug(level-1, quadrant->macro_cell.se);
    printf(")");
  }
}


Quadrant* MacroCell_generate(size_t level) {
  if (level <= 1) {
    return MiniCell_new(0,0,0,0);
  } else {
    return MacroCell_new(MacroCell_generate(level-1),MacroCell_generate(level-1)
                         MacroCell_generate(level-1),MacroCell_generate(level-1));
  }
}


Quadrant* MacroCell_result(size_t level, Quadrant* quadrant) {



/*
  temp_nw = MacroCell_result(level-1, self.nw )
  temp_n  = MacroCell_result(level-1, MacroCell_new(self.nw.ne,self.ne.nw,self.nw.se,self.ne.sw) )
  temp_ne = MacroCell_result(level-1, self.ne )
  
  temp_w  = MacroCell_result(level-1, MacroCell_new(self.nw.sw,self.nw.se,self.sw.nw,self.sw.ne) )
  temp_c  = MacroCell_result(level-1, MacroCell_new(self.nw.se,self.ne.sw,self.sw.ne,self.se.nw) )
  temp_e  = MacroCell_result(level-1, MacroCell_new(self.ne.sw,self.ne.se,self.se.nw,self.se.ne) )
  
  temp_sw = MacroCell_result(level-1, self.sw )
  temp_s  = MacroCell_result(level-1, MacroCell_new(self.sw.ne,self.se.nw,self.sw.se,self.se.sw) )
  temp_se = MacroCell_result(level-1, self.se )
    
 
  res_nw = MacroCell_result( Macro_cell_new(temp_nw,temp_n,temp_w,temp_c,self.size-1) )
  res_ne = MacroCell_result( Macro_cell_new(temp_n,temp_ne,temp_c,temp_e,self.size-1) )
  res_sw = MacroCell_result( Macro_cell_new(temp_w,temp_c,temp_sw,temp_s,self.size-1) )
  res_se = MacroCell_result( Macro_cell_new(temp_c,temp_e,temp_s,temp_se,self.size-1) )

  new_cell = MacroCell_new(res_nw,res_ne,res_sw,res_se,self.size-1)
*/ 


}




















































































