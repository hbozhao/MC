#ifndef _mc_h
#define _mc_h
#include <iostream>
#include <vector>

class Position;
class Particle;
class State;

struct Position {
  double x, y;
};

// typedef std::vector<particle> allparticle; //initialization either in main or in a class

class Particle {
  private:
    Position pos;
    double radius;
    int id;

  public:


    Particle(Position pos, double radius, int id): pos(pos), radius(radius), id(id) {};
    double distance2(Particle &p, double L);
    double distance(Particle &p, double L);
    bool perturb(Position dv, State *s, double L);
    friend std::ostream &operator<< (std::ostream &os, Particle &p);
    // friend bool State::check_overlap(Particle &p);
};

class State {
  double L;
  std::vector<Particle> ap;
  int attempt, success;

  public:
    State(int M, double L, double radius);
    void update(int id, Position dv);
    bool check_overlap(Particle &p, double radius);
    friend std::ostream &operator<< (std::ostream & os, State &s);
    //rdf

};

#endif
