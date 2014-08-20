#include<algorithm>
#include<cmath>
#include<complex>
#include<iomanip>
#include<iostream>
#include<string>
#include<vector>


#define OLD_URQMD true
const double eta_max = 0.5;
const double pT_min = 0.2;
const double pT_max = 5.0;
const unsigned Qn_min = 2;
const unsigned Qn_max = 4;


int main() {
  // massive speedup reading stdin
  // https://stackoverflow.com/q/9371238
  std::cin.sync_with_stdio(false);

  std::cout << std::scientific << std::setprecision(10);

  const double exp2eta_min = std::exp(-2.*eta_max);
  const double exp2eta_max = std::exp(2.*eta_max);
  const double pT_squared_min = pT_min*pT_min;
  const double pT_squared_max = pT_max*pT_max;

  std::string line;
  bool event = false;
  size_t Nch_all = 0, Nch_in_pT_range = 0;
  std::vector<std::complex<double>> Qn_vectors(Qn_max - Qn_min + 1);

  while (std::cin) {
    std::getline(std::cin, line);
    if (line.size() != 434) {
      if (event) {
        std::cout << Nch_all << ' '
                  << Nch_in_pT_range;
        for (size_t i = 0; i < Qn_vectors.size(); ++i ) {
          auto Qn = Qn_vectors[i];
          std::cout << ' ' << Qn.real()
                    << ' ' << Qn.imag();
        }
        std::cout << '\n';
        event = false;
        Nch_all = 0;
        Nch_in_pT_range = 0;
        std::fill(Qn_vectors.begin(), Qn_vectors.end(), 0);
      }
      continue;
    }

    event = true;

    //int charge = std::stoi(line.substr(225, 2));
    //if (charge == 0)
    //  continue;

#if OLD_URQMD
    // make Fortran doubles readable
    std::replace(line.begin(), line.end(), 'D', 'E');
#endif

    auto px = std::stod(line.substr(121, 23));
    auto py = std::stod(line.substr(145, 23));
    auto pz = std::stod(line.substr(169, 23));

    auto pmag = std::sqrt(px*px + py*py + pz*pz);
    auto exp2eta = (pmag+pz)/(pmag-pz);

    if (exp2eta > exp2eta_max || exp2eta < exp2eta_min)
      continue;

    ++Nch_all;

    auto pT_squared = px*px + py*py;

    if (pT_squared > pT_squared_max || pT_squared < pT_squared_min)
      continue;

    ++Nch_in_pT_range;
    auto phi = std::atan2(py, px);

    for (auto n = Qn_min; n <= Qn_max; ++n) {
      auto i = n - Qn_min;
      Qn_vectors[i] += std::exp(std::complex<double>(0., n*phi));
    }
  }

  return 0;
}
