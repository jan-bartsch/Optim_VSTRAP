#include "doublepdf.h"

double &DoublePDF::at(uint o, uint l, uint m, uint n) {
  return this->pdf_.at(o).at(l).at(m).at(n);
}
