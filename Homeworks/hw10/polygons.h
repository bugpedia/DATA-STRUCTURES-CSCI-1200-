#ifndef POLYGONS_H_
#define POLYGONS_H_

#include <string>
#include <vector>
#include "utilities.h"

using namespace std;

// ****** BASE CLASS *********************************************************/
class Polygon
{
public:
	Polygon(const string& _name, const vector<Point>& _vertices) 
	: name(_name), vertices(_vertices) {}

	// member functions, labelled with virtual
	virtual bool HasAllEqualSides();
	virtual bool HasAllEqualAngles();
	virtual bool HasARightAngle(); 	
	virtual bool HasAnObtuseAngle();
	virtual bool HasAnAcuteAngle();
	virtual bool IsConvex();
	virtual bool IsConcave(); 

	// accessor
	virtual const string getName() {return name;}

protected:
	// variables are labelled as protected for inheritance reasons
	string name;
	vector<Point> vertices;
};

/*****************************************************************************/
// TRIANGLE CLASSES

class Triangle : public Polygon
{
public:
	Triangle(const string& _name, const vector<Point>& _vertices); 
	
	// Simplifying calculations for certain member functions for all triangles
	virtual bool HasAnAcuteAngle() {return true;}
	virtual bool IsConvex() {return true;}
	virtual bool IsConcave() {return false;}
};


class IsoscelesTriangle : virtual public Triangle
{
public:
	IsoscelesTriangle(const string& _name, const vector<Point>& _vertices); 
};


class RightTriangle : virtual public Triangle
{
public:
	RightTriangle(const string& _name, const vector<Point>& _vertices);
	
	// All right triangles have right angles
	virtual bool HasARightAngle() {return true;}
};


class ObtuseTriangle : virtual public Triangle
{
public:
	ObtuseTriangle(const string& _name, const vector<Point>& _vertices);
	
	// All obtuse triangles have obtuse angles, and thus can't have right angles
	virtual bool HasARightAngle() {return false;}
	virtual bool HasAnObtuseAngle() {return true;}
};


class IsoscelesRightTriangle : public IsoscelesTriangle, public RightTriangle
{
public:
	IsoscelesRightTriangle(const string& _name, const vector<Point>& _vertices); 
};


class IsoscelesObtuseTriangle : public IsoscelesTriangle, public ObtuseTriangle
{
public:
	IsoscelesObtuseTriangle(const string& _name, const vector<Point>& _vertices);
};


class EquilateralTriangle : public IsoscelesTriangle
{
public:
	EquilateralTriangle(const string& _name, const vector<Point>& _vertices);

	// All equilateral triangles have equal sides and equal angles
	virtual bool HasAllEqualSides() {return true;}
	virtual bool HasAllEqualAngles() {return true;}
};

/*****************************************************************************/
// QUADRILATERAL CLASSES

class Quadrilateral : public Polygon
{
public:
	Quadrilateral(const string& _name, const vector<Point>& _vertices);
};


class Arrow : public Quadrilateral
{
public:
	Arrow(const string& _name, const vector<Point>& _vertices);

	// Arrows are concave, therefore not convex
	virtual bool IsConvex() {return false;}
	virtual bool IsConcave() {return true;}
};


class Kite : virtual public Quadrilateral
{
public:
	Kite(const string& _name, const vector<Point>& _vertices);
};


class Trapezoid : virtual public Quadrilateral
{
public:
	Trapezoid(const string& _name, const vector<Point>& _vertices);
};


class IsoscelesTrapezoid : virtual public Trapezoid
{
public:
	IsoscelesTrapezoid(const string& _name, const vector<Point>& _vertices);
};


class Parallelogram : virtual public Trapezoid
{
public:
	Parallelogram(const string& _name, const vector<Point>& _vertices);
};


class Rectangle : public IsoscelesTrapezoid, virtual public Parallelogram
{
public:
	Rectangle(const string& _name, const vector<Point>& _vertices);

	// Rectangles have all right angles (all = 90 degrees)
	virtual bool HasAllEqualAngles() {return true;}
	virtual bool HasARightAngle() {return true;}	
};


class Rhombus : public Kite, virtual public Parallelogram
{
public:
	Rhombus(const string& _name, const vector<Point>& _vertices);

	// Rhombi have all equal sides
	virtual bool HasAllEqualSides() {return true;}
};


class Square : public Rectangle, public Rhombus
{
public:
	Square(const string& _name, const vector<Point>& _vertices);
};



#endif