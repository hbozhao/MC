#ifndef _mc_h
#define _mc_h
#include <iostream>

class Position;
class particle;
class state;

struct Position {
  double x, y;
};

typedef std::vector<particle> allparticle; //initialization either in main or in a class

class particle {
  private:
    Position pos;
    double radius;
    int id;

  public:
    particle(Position pos, double radius): pos(pos), radius(radius);

    void perturb();
    bool check_overlap();
};

class state {

  std::vector<particle> allparticle;
  public:
    state(int M, double L);
    void update();
    //rdf

}
#endif
