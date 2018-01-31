#include <iostream>
#include <stdio.h>
#include <time.h>
#include <cassert>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "mc.h"

double Particle::distance2(Particle &p, double L) {
  double dx = abs(this->pos.x - p.pos.x);
  double dy = abs(this->pos.y - p.pos.y);
  if (dx > L/2) {
    dx -= L/2;
  }
  if (dy > L/2) {
    dy -= L/2;
  }
  return pow(dx,2) + pow(dy,2);
}

double Particle::distance(Particle &p, double L) {
  return sqrt(this->distance2(p, L));
}

bool Particle::perturb(Position dv, State *s, double L){
  double x_new, y_new;
  x_new = this->pos.x + dv.x;
  y_new = this->pos.y + dv.y;
  if (x_new > L) {
    x_new -= L;
  }
  if (y_new > L) {
    y_new -= L;
  }
  Position pos = {x_new, y_new};
  Particle candidate = Particle(pos, this->radius, this->id);
  bool isOverlap = s->check_overlap(candidate, this->radius);
  if (!isOverlap) {
    *this = candidate;
  }
  return !isOverlap;
};

std::ostream &operator<< (std::ostream &os, Position &pos) {
  os << "(" << pos.x << " " << pos.y << ")";
}

std::ostream &operator<< (std::ostream &os, Particle &p) {
  os << "particle " << p.id << " (x,y) = " << p.pos << std::endl;
}

bool State::check_overlap(Particle &p, double radius){
  for (auto it = ap.begin(); it != ap.end(); ++it) {
    if (p.distance2(*it, this->L) < pow(2*radius,2)) {
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
  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < M; ++j) {
      id = M*i + j;
      pos = {a/2 + a*j, a/2 + a*i};
      ap.push_back(Particle(pos, radius, id));
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
  os << s.success;
}

int main() {
  double L, radius, mag;
  int M, Nsteps;
  Position dv;

  std::cout << "Enter box length" << std::endl;
  std::cin >> L;
  std::cout << "Enter the number of particles per length" << std::endl;
  std::cin >> M;
  std::cout << "Enter the particle radius" << std::endl;
  std::cin >> radius;
  std::cout << "Enter the perturbation magnitude" << std::endl;
  std::cin >> mag;
  std::cout << "Enter the number of steps" << std::endl;
  std::cin >> Nsteps;

  State state(M, L, radius);
  srand(1);
  for (int i = 0; i < Nsteps; i++) {
    for (int j = 0; j < pow(M,2); j++) {
      dv.x = (double) rand() / (RAND_MAX) * mag;
      dv.y = (double) rand() / (RAND_MAX) * mag;
      state.update(j, dv);
    }
  }
  std::cout << state;
}
