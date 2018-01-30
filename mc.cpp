#include <stdio.h>
#include <time.h>
#include <cassert>
#include <math.h>
#include <stdlib.h>
#include "mc.h"

//---------------------------------------------------------------------------//
//                              Class Methods                                //
//---------------------------------------------------------------------------//

Position operator+(const Position &p) const {
  return Position(x+p.x, y+p.y); }
Position operator+=(const Position &p) {
  x += p.x; y += p.y;
  return *this; }

Position operator-(const Position &p) const {
  return Position(x-p.x, y-p.y); }
Position operator-=(const Position &p) {
  x -= p.x; y -= p.y;
  return *this; }

Position operator*(const double scalar) const {
  return Position(scalar*x, scalar*y, scalar*z); }
Position operator*=(const double scalar) {
  x *= scalar; y *= scalar;
  return *this; }

Position operator/(const double scalar) const {
  return Position(x/scalar, y/scalar); }
Position operator/=(const double scalar) {
  x /= scalar; y /= scalar;
  return *this;
}

void perturb(){

};

bool check_overlap(){

};

state::state(int M, double L){

};

void state::update(){

};


//---------------------------------------------------------------------------//
//                                main                                       //
//---------------------------------------------------------------------------//
