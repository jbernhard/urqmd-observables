// processes UrQMD output into event-by-event observables
// for each event, a line is output:
//   Nch Nch_in_pT_range Q2re Q2im ...

#include<algorithm>
#include<cmath>
#include<complex>
#include<iomanip>
#include<iostream>
#include<string>
#include<vector>


// if true, replace D -> E in doubles
#define OLD_URQMD true

// kinematic cuts
const double eta_max = 0.5;
const double pT_min = 0.2;
const double pT_max = 5.0;

// range of Qn to calculate
const unsigned Qn_min = 2;
const unsigned Qn_max = 4;


int main() {
  // massive speedup reading stdin
  // https://stackoverflow.com/q/9371238
  std::cin.sync_with_stdio(false);

  // set output format
  std::cout << std::scientific << std::setprecision(10);

  // cache for later use
  const double exp2eta_min = std::exp(-2.*eta_max);
  const double exp2eta_max = std::exp(2.*eta_max);
  const double pT_squared_min = pT_min*pT_min;
  const double pT_squared_max = pT_max*pT_max;

  std::string line;
  size_t Nch_all = 0, Nch_in_pT_range = 0;
  std::vector<std::complex<double>> Qn_vectors(Qn_max - Qn_min + 1);

  // read stdin
  while (std::cin) {
    // read line
    std::getline(std::cin, line);

    // check if this is a particle line
    if (line.size() != 434) {
      // event has ended
      // output event data
      if (Nch_all > 0) {
        std::cout << Nch_all << ' '
                  << Nch_in_pT_range;
        for (size_t i = 0; i < Qn_vectors.size(); ++i) {
          auto Qn = Qn_vectors[i];
          std::cout << ' ' << Qn.real()
                    << ' ' << Qn.imag();
        }
        std::cout << '\n';
        // reset variables
        Nch_all = 0;
        Nch_in_pT_range = 0;
        std::fill(Qn_vectors.begin(), Qn_vectors.end(), 0);
      }
      continue;
    }

    // use only charged particles
    auto charge = std::stoi(line.substr(225, 2));
    if (charge == 0)
      continue;

#if OLD_URQMD
    // make Fortran doubles readable
    std::replace(line.begin(), line.end(), 'D', 'E');
#endif

    // read momentum vector
    auto px = std::stod(line.substr(121, 23));
    auto py = std::stod(line.substr(145, 23));
    auto pz = std::stod(line.substr(169, 23));

    // |eta| cut
    auto pmag = std::sqrt(px*px + py*py + pz*pz);
    auto exp2eta = (pmag+pz)/(pmag-pz);
    if (exp2eta > exp2eta_max || exp2eta < exp2eta_min)
      continue;

    // keep this particle
    ++Nch_all;

    // pT cut
    auto pT_squared = px*px + py*py;
    if (pT_squared > pT_squared_max || pT_squared < pT_squared_min)
      continue;

    // include this particle in flows
    ++Nch_in_pT_range;
    auto phi = std::atan2(py, px);
    for (auto n = Qn_min; n <= Qn_max; ++n) {
      auto i = n - Qn_min;
      Qn_vectors[i] += std::exp(std::complex<double>(0., n*phi));
    }
  }

  return 0;
}
