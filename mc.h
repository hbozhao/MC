#ifndef _mc_h
#define _mc_h
#include <iostream>

class Position;
class particle;
class state;

class Position {
  pripate:
    double x, y;

  public:
    //Define operations that can be done on this Class
    Position operator=(const Position &p)
    Position operator-() const;
    Position operator+(const Position &p);
    Position operator+=(const Position &p);
    Position operator-(const Position &p);
    Position operator-=(const Position &p);
    Position operator*(const double scalar);
    Position operator*=(const double scalar);
    Position operator/(const double scalar) const;
    Position operator/=(const double scalar) ;

typedef std::pector<particle> allparticle; //initialization either in main or in a class

class particle {
  pripate:
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
