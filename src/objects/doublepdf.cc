#include "doublepdf.h"

DoublePDF::DoublePDF(uint timesteps, uint vcell)
{
    std::vector<std::vector<std::vector<std::vector<double> > > > pdf(timesteps, std::vector<std::vector<std::vector<double> > >
                                                                   (vcell, std::vector<std::vector<double> > (vcell,
                                                                                    std::vector<double> (vcell,0.0))));
    this->pdf_ = pdf;
}

double &DoublePDF::at(uint o, uint l, uint m, uint n)
{
    return this->pdf_.at(o).at(l).at(m).at(n);
}
