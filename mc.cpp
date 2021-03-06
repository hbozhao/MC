#include <iostream>
#include <stdio.h>
#include <time.h>
#include <cassert>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include "mc.h"
#include <fstream>

const double PI  =3.141592653589793238463;

double Particle::distance2(Particle &p, double L) {
  double dx = sqrt(pow((this->pos.x - p.pos.x),2));
  double dy = sqrt(pow((this->pos.y - p.pos.y),2));

  if (dx > L/2) {
    dx = dx - L;
  }
  if (dy > L/2) {
    dy = dy - L;
  }
  return pow(dx,2) + pow(dy,2);
}

double Particle::distance(Particle &p, double L) {
  return sqrt(this->distance2(p, L));
}

bool Particle::perturb(Position dv, State *s, double L){
  double x_new, y_new;
  x_new = pos.x + dv.x;
  y_new = pos.y + dv.y;
  if (x_new > L) {
    x_new -= L;
  }
  if (x_new < 0) {
    x_new += L;
  }
  if (y_new > L) {
    y_new -= L;
  }
  if (y_new < 0) {
    y_new += L;
  }
  Position pos_new = {x_new, y_new};
  Particle candidate = Particle(pos_new, this->radius, this->id);
  bool isOverlap = s->check_overlap(candidate);
  if (!isOverlap) {
    *this = candidate;
  }
  return !isOverlap;
};

std::ostream &operator<< (std::ostream &os, Position &pos) {
  os << pos.x << " " << pos.y << " " << 0;
  return os;
}

std::ostream &operator<< (std::ostream &os, Particle &p) {
  //os << "particle " << p.id << " (x,y) = " << p.pos << std::endl;
    //os << "Ar" << " " << p.pos <<std::endl;
    os << p.pos <<std::endl;
    return os;
}

bool State::check_overlap(Particle &p){
  // for (auto it = ap.begin(); it != ap.end(); ++it) {
  for (int i = 0; i < ap.size(); ++i) {
    if (ap[i].id == p.id) continue;
    if (p.distance2(ap[i], this->L) < pow(2*p.radius,2)) {
      // std::cout << p.distance2(ap[i], this->L, true) << std::endl;
      // std::cout << ap[i].id << " " << ap[i] << " " << p.id << " " << p << " " << ap[i].pos.x - p.pos.x << ap[i].distance2(p, this->L, false) << p.radius << std::endl;
      return true;
    }
  }
  return false;
};

State::State(int M, double L, double radius){
  this->L = L;
  double a = L/M;
  Position pos;
  // Particle p;
  int id;
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      id = M*i + j;
      pos = {a/2 + a*j, a/2 + a*i};
      ap.emplace_back(Particle(pos, radius, id));  //why is "new" not needed here?
    }
  }
  attempt = 0;
  success = 0;
};

void State::update(int id, Position dv) {
  if (ap[id].perturb(dv, this, L)) {success++;}
  attempt++;
  // bool isSuccess = ap[id].perturb(dv, *this);
  // isSuccess ? success++;
};

std::ostream &operator<< (std::ostream & os, State &s) {

  //os << "Number of success = " << s.success << std::endl;
  //os << s.ap.size() << std::endl;
  //os << "Atoms. Timestep: " << s.attempt << std::endl;

  for (auto it = s.ap.begin(); it != s.ap.end(); ++it) {
    os << *it;
  }
  return os;
}

std::ostream &operator<< (std::ostream &os, std::vector<double> &v) {
  for (int i = 0; i < v.size(); i++) {
    os << v[i] << std::endl;
  }
  return os;
}

void State::cumrdf(std::vector<double> &prevRDF, double radius) {
  double dist, binsize, maxdist, density, norm;
  int binnum, nbin, np;
  double hist_tot = 0;
  nbin = prevRDF.size();
  maxdist = 10*radius;
  binsize = maxdist / nbin;
  np = ap.size();
  density = np/pow(L,2);
  std::vector<double> hist(nbin, 0);

  for (int i = 0; i < np; i++) {
    for (int j = i+1; j < np; j++) {
      dist = ap[i].distance(ap[j], L);
      binnum = (int) floor(dist/binsize);
      if (binnum >= nbin) continue;
      // std::cout << binnum;
      hist[binnum]+=2;
      hist_tot+=2;
    }
  }
  norm = ((M_PI*pow(maxdist,2)))/hist_tot; //- 16/3/L*pow(maxdist,3) + 1/(2*pow(L,2))*(pow(maxdist,4)) )
  for (int i = 0; i < nbin; i++) {
    prevRDF[i] += hist[i]* norm /(M_PI*(pow((i+1)*binsize,2)-pow(i*binsize,2)));
    // -16/3/L*(pow((i+1)*binsize,3)-pow(i*binsize,3))
    // +1/(2*pow(L,2))*(pow((i+1)*binsize,4)-pow(i*binsize,4)));
  }

  // std::cout << prevRDF;
    // std::cout << prevRDF[i];
}



int main() {
  double L, radius, mag;
  int M, Nsteps, nbin;
  Position dv;

  std::cout << "Enter box length" << std::endl;
  std::cin >> L;
  std::cout << "Enter the number of particles per length" << std::endl;
  std::cin >> M;
  std::cout << "Enter the particle radius" << std::endl;
  std::cin >> radius;
  std::cout << "Volume fraction" << M_PI * pow(radius, 2) * pow((double) M/L, 2) << std::endl;
  std::cout << "Enter the perturbation magnitude" << std::endl;
  std::cin >> mag;
  std::cout << "Enter the number of steps" << std::endl;
  std::cin >> Nsteps;

  State state(M, L, radius);
  // std::cout << "Original" << std::endl;
  // std::cout << state << std::endl;

  std::cout << "Enter nbin" << std::endl;
  std::cin >> nbin;
  std::vector<double> RDF(nbin, 0);

  srand(1);

  std::clock_t time;
  double duration;
  std::ofstream mc_file; //("rdf.vtf")
  mc_file.open ("mc.vtf");
  // time = std::clock();

  mc_file << "atom " << "0:"<< M*M << "\t" <<"radius " << radius << " name C"<<std::endl;
  mc_file << "timestep" << std::endl;
  mc_file << "unitcell" << " " << L << " " << L << " " << 0 << std::endl;
  mc_file << state<<std::endl;
  for (int i = 0; i < Nsteps; i++) {
    for (int j = 0; j < pow(M,2); j++) {
      dv.x = (2*(double) rand() / (RAND_MAX)-1) * mag;
      dv.y = (2*(double) rand() / (RAND_MAX)-1) * mag;
      state.update(j, dv);
    }
    mc_file << "timestep" << std::endl;
    mc_file << state<<std::endl;

    // duration = ( std::clock() - time ) / (double) CLOCKS_PER_SEC;
    // time = std::clock();
    // std::cout << i << " takes " << duration << " s" << std::endl;
    state.cumrdf(RDF,radius);
  }
  mc_file.close();

  for (int i = 0; i < nbin; i++) {
    RDF[i] = RDF[i] / Nsteps;
  }
  std::ofstream rdf_file;
  rdf_file.open ("rdf.ave");
  rdf_file << RDF << std::endl;
  rdf_file.close();
  std::cout << "DONE!!";

}
