#include "polygons.h"

// Constructor for Triangle class, inheriting from the Polygon class
Triangle::Triangle(const string& _name, const vector<Point>& _vertices) 
	: Polygon(_name,_vertices) 
{
	// The only time it will fail is if the 3 vertices are not provided
	if (vertices.size() != 3)
		throw 1;
}

// Constructor for IsoscelesTriangle class, inheriting from the Triangle
// class
IsoscelesTriangle::IsoscelesTriangle(const string& _name, 
	const vector<Point>& _vertices) : Triangle(_name,_vertices)
{
	// Create vectors from the provided vertices
	Vector s1(vertices[0], vertices[1]);
	Vector s2(vertices[1], vertices[2]);
	Vector s3(vertices[0], vertices[2]);

	// The constructor will fail and throw an exception if there is not 
	// one pair of equal sides
	if (!((EqualSides(s1.Length(),s2.Length()) || 
			EqualSides(s1.Length(),s3.Length()) || 
			EqualSides(s2.Length(),s3.Length()))))
		throw 1;
}

// Constructor for the RightTriangle class, inheriting from the Triangle
// class
RightTriangle::RightTriangle(const string& _name, 
	const vector<Point>& _vertices) : Triangle(_name,_vertices) 
{
	// Create angles from the provided vertices
	double a1 = Angle(vertices[0], vertices[1], vertices[2]);
	double a2 = Angle(vertices[0], vertices[2], vertices[1]);
	double a3 = Angle(vertices[2], vertices[0], vertices[1]);

	// If there isn't a right angle, then a right triangle cannot be formed
	// and an exception is thrown
	if (!(RightAngle(a1) || RightAngle(a2) || RightAngle(a3)))
		throw 1;
}

// Constructor for the ObtuseTriangle class, inheriting from the Triangle class
ObtuseTriangle::ObtuseTriangle(const string& _name, 
	const vector<Point>& _vertices) : Triangle(_name,_vertices)
{
	double a1 = Angle(vertices[0], vertices[1], vertices[2]);
	double a2 = Angle(vertices[1], vertices[2], vertices[0]);
	double a3 = Angle(vertices[2], vertices[0], vertices[1]);

	// If there is no obtuse angle, then the constructor throws an exception
	if (!(ObtuseAngle(a1) || ObtuseAngle(a2) || ObtuseAngle(a3)))
		throw 1;
}

// Constructor for the IsoscelesRightTriangle class, inheriting from the
// Triangle class, IsoscelesTriangle class, and RightTriangle class. None of
// these constructors can throw exceptions, or else an IsoscelesRightTriangle
// will fail, which is why we do not need to do anything in the braces
IsoscelesRightTriangle::IsoscelesRightTriangle(const string& _name, 
	const vector<Point>& _vertices)
	: Triangle(_name,_vertices), IsoscelesTriangle(_name,_vertices), 
	RightTriangle(_name,_vertices) 
{}

// Constructor for the IsoscelesObtuseTriangle class, inheriting from the
// Triangle class, IsoscelesTriangle class, and ObtuseTriangle class. None of
// these constructors can throw exceptions, or else an IsoscelesObtuseTriangle
// will fail, which is why we do not need to do anything in the braces
IsoscelesObtuseTriangle::IsoscelesObtuseTriangle(const string& _name, 
	const vector<Point>& _vertices) 
	: Triangle(_name,_vertices), IsoscelesTriangle(_name,_vertices), 
	ObtuseTriangle(_name,_vertices) 
{}

// Constructor for the EquilateralTriangle class, inheriting from the Triangle
// class and IsoscelesTriangle class
EquilateralTriangle::EquilateralTriangle(const string& _name, 
	const vector<Point>& _vertices) 
: Triangle(_name,_vertices), IsoscelesTriangle(_name,_vertices) 
{
	Vector s1(vertices[0], vertices[1]);
	Vector s2(vertices[1], vertices[2]);
	Vector s3(vertices[0], vertices[2]);

	// If all of the sides are not equal, then the constructor will fail and 
	// throw an exception
	if (!(EqualSides(s1.Length(),s2.Length()) && 
		EqualSides(s1.Length(),s3.Length()) && 
		EqualSides(s2.Length(),s3.Length())))
		throw 1;
}

// Constructor for the Quadrilateral class, inheriting from the Polygon class
Quadrilateral::Quadrilateral(const string& _name, const vector<Point>& _vertices) 
	: Polygon(_name,_vertices)
{
	// The constructor will throw an exception if not exactly 4 vertices are 
	// provided
	if (vertices.size() != 4)
		throw 1;
}

// Constructor for the Arrow class, inheriting from the Quadrilateral class
Arrow::Arrow(const string& _name, const vector<Point>& _vertices) 
	: Quadrilateral(_name,_vertices) 
{
	// Create sides from the vertices
	Vector s1(vertices[0], vertices[1]);
	Vector s2(vertices[1], vertices[2]);
	Vector s3(vertices[2], vertices[3]);
	Vector s4(vertices[3], vertices[0]);

	// Create angles from the vertices
	double a1 = Angle(vertices[0], vertices[1], vertices[2]);
	double a2 = Angle(vertices[1], vertices[2], vertices[3]);
	double a3 = Angle(vertices[2], vertices[3], vertices[0]);
	double a4 = Angle(vertices[3], vertices[0], vertices[1]);

	// Set boolean values to check whether each angle is a reflex angle
	// or not
	bool reflex_1 = ReflexAngle(a1);
	bool reflex_2 = ReflexAngle(a2);
	bool reflex_3 = ReflexAngle(a3);
	bool reflex_4 = ReflexAngle(a4);	

	// If there is not one pair of adjacent equal sides and one reflex 
	// angle, then the constructor will throw an exception
	if (!(((EqualSides(s1.Length(),s2.Length())
		&& EqualSides(s3.Length(),s4.Length())) || (EqualSides(s2.Length(),
			s3.Length()) && EqualSides(s4.Length(),s1.Length()))) && 
		(reflex_1 || reflex_2 || reflex_3 || reflex_4)))
		throw 1;
}

// Constructor for the Kite class, inheriting from the Quadrilateral class
Kite::Kite(const string& _name, const vector<Point>& _vertices) 
	: Quadrilateral(_name,_vertices) 
{
	Vector s1(vertices[0], vertices[1]);
	Vector s2(vertices[1], vertices[2]);
	Vector s3(vertices[2], vertices[3]);
	Vector s4(vertices[3], vertices[0]);

	// If there isn't a pair of adjacent equal sides, then an exception is 
	// thrown and the constructor fails
	if (!(((EqualSides(s1.Length(),s2.Length())
		&& EqualSides(s3.Length(),s4.Length())) || (EqualSides(s2.Length(),
			s3.Length()) && EqualSides(s4.Length(),s1.Length())))))
		throw 1;
}

// Constructor for the Trapoeozid class, inheriting from the Quadrilateral class
Trapezoid::Trapezoid(const string& _name, const vector<Point>& _vertices) 
	: Quadrilateral(_name,_vertices) 
{
	Vector s1(vertices[0], vertices[1]);
	Vector s2(vertices[1], vertices[2]);
	Vector s3(vertices[2], vertices[3]);
	Vector s4(vertices[3], vertices[0]);

	// If there is not one pair of parallel sides, the constructor will 
	// throw an exception
	if (!(Parallel(s1,s3) || Parallel(s2,s4)))
		throw 1;
}

// Constructor for the IsoscelesTrapezoid class, inheriting from the 
// Quadrilateral class and the Trapezoid class
IsoscelesTrapezoid::IsoscelesTrapezoid(const string& _name, 
	const vector<Point>& _vertices) 
	: Quadrilateral(_name,_vertices), Trapezoid(_name,_vertices) 
{
	// Create sides from the vertices
	Vector s1(vertices[0], vertices[1]);
	Vector s2(vertices[1], vertices[2]);
	Vector s3(vertices[2], vertices[3]);
	Vector s4(vertices[3], vertices[0]);

	// Create angles from the vertices
	double a1 = Angle(vertices[0], vertices[1], vertices[2]);
	double a2 = Angle(vertices[1], vertices[2], vertices[3]);
	double a3 = Angle(vertices[2], vertices[3], vertices[0]);
	double a4 = Angle(vertices[3], vertices[0], vertices[1]);

	// Set boolean values to check if there exists a pair of equal angles
	bool pair1 = EqualAngles(a1,a2);
	bool pair2 = EqualAngles(a3,a4);

	// If there isn't an pair of sides opposite from each other that are equal
	// and no pairs of equal angles, the constructor throws an exception
	if (!((EqualSides(s1.Length(),s3.Length()) ||
		EqualSides(s2.Length(),s4.Length())) && pair1 && pair2))
		throw 1;
}

// Constructor for the Parallelogram class, inheriting from the Quadrilateral
// class and the Trapezoid class
Parallelogram::Parallelogram(const string& _name, const vector<Point>& _vertices) 
	: Quadrilateral(_name,_vertices), Trapezoid(_name,_vertices) 
{
	Vector s1(vertices[0], vertices[1]);
	Vector s2(vertices[1], vertices[2]);
	Vector s3(vertices[2], vertices[3]);
	Vector s4(vertices[3], vertices[0]);

	// If there aren't two pairs of parallel sides, the constructor throws
	// an exception
	if (!(Parallel(s1,s3) && Parallel(s2,s4)))
		throw 1;
}

// Constructor for the Rectangle class, inheriting from the Quadrilateral class,
// Trapezoid class, IsoscelesTrapezoid class, and Parallelogram class
Rectangle::Rectangle(const string& _name, const vector<Point>& _vertices) 
	: Quadrilateral(_name,_vertices), Trapezoid(_name,_vertices), 
	Parallelogram(_name,_vertices), IsoscelesTrapezoid(_name,_vertices)
{
	double a1 = Angle(vertices[0], vertices[1], vertices[2]);
	double a2 = Angle(vertices[1], vertices[2], vertices[3]);
	double a3 = Angle(vertices[2], vertices[3], vertices[0]);
	double a4 = Angle(vertices[3], vertices[0], vertices[1]);

	// Set boolean values to check if each angle is a right angle or not
	bool right_1 = RightAngle(a1);
	bool right_2 = RightAngle(a2);
	bool right_3 = RightAngle(a3);
	bool right_4 = RightAngle(a4);	

	// If they are not all right angles, then the constructor will throw an
	// exception
	if (!(right_1 && right_2 && right_3 && right_4))
		throw 1;
}

// Constructor for the Rhombus class, inheriting from the Trapezoid class, the 
// Quadrilateral class, the Kite class, and the Parallelogram class
Rhombus::Rhombus(const string& _name, const vector<Point>& _vertices) 
	: Quadrilateral(_name,_vertices), Trapezoid(_name,_vertices),  
	Parallelogram(_name,_vertices), Kite(_name,_vertices)
{
	Vector s1(vertices[0], vertices[1]);
	Vector s2(vertices[1], vertices[2]);
	Vector s3(vertices[2], vertices[3]);
	Vector s4(vertices[3], vertices[0]);

	// Check to see if all the sides are equal
	bool first = EqualSides(s1.Length(),s2.Length());
	bool second = EqualSides(s2.Length(),s3.Length());
	bool third = EqualSides(s3.Length(), s4.Length());
	bool fourth = EqualSides(s4.Length(), s1.Length());

	// If these sides are not all equal, then the constructor will throw an 
	// exception
	if (!(first && second && third && fourth))
		throw 1;
}

// Constructor for the Square class, inheriting from the Quadrilateral class,
// the Parallelogram class, the Trapezoid class, the Rectangle class, and 
// the Rhombus class. No checks need to be done because if all of these
// constructors pass and none throws an exception, then the resulting shape
// is a square
Square::Square(const string& _name, const vector<Point>& _vertices) 
	: Quadrilateral(_name,_vertices), Trapezoid(_name,_vertices), 
	Parallelogram(_name,_vertices), Rectangle(_name,_vertices), 
	Rhombus(_name,_vertices) 
{}

// Checks to see if a Polygon has all equal sides
bool Polygon::HasAllEqualSides()
{
	vector<Vector> sides;

	// Create sides given n vertices, checking to see if we are on the last
	// vertex. If we are, then we must have vertices[0] as the second argument
	// for the Vector helper function. Each side is pushed back to an STL
	// vector
	for(int i = 0; i < vertices.size(); i++)
	{
		if (i+1 < vertices.size())
			sides.push_back(Vector(vertices[i],vertices[i+1]));
		else
			sides.push_back(Vector(vertices[i],vertices[0]));
	}

	// We then check to see if all sides are equal. If a pair isn't, we 
	// immediately return false
	for (int i = 0; i < sides.size(); i++)
	{	
		if (i+1 < sides.size())
		{
			if(!EqualSides(sides[i].Length(),sides[i+1].Length()))
				return false;			
		}
		else
		{
			if(!EqualSides(sides[i].Length(),sides[0].Length()))
				return false;
		}
	}

	// If they're all equal, then we return true
	return true;
}

// Checks to see if a Polygon has all equal angles
bool Polygon::HasAllEqualAngles()
{
	vector<double> angles;

	// Create angles given n vertices, checking to see if we are at the second
	// to last vertex (we then use vertices[0] as the last argument of Angle)
	// or the last vertex (we use vertices[0] as the second argument and 
	// vertices[1] as the last). Each angle is pushed back to an STL vector.
	for(int i = 0; i < vertices.size(); i++)
	{
		if (i+2 < vertices.size())
			angles.push_back(Angle(vertices[i],vertices[i+1],vertices[i+2]));
		else if (i+1 < vertices.size())
			angles.push_back(Angle(vertices[i],vertices[i+1],vertices[0]));
		else
			angles.push_back(Angle(vertices[i],vertices[0],vertices[1]));
	}

	// If two angles aren't equal to each other, immediately return false
	for (int i = 0; i < angles.size(); i++)
	{	
		if (i+1 < angles.size())
		{
			if(!EqualAngles(angles[i],angles[i+1]))
				return false;			
		}
		else
		{
			if(!EqualAngles(angles[i],angles[0]))
				return false;
		}
	}

	// If all angles are equal, then return true
	return true;
}

// Checks to see if a Polygon has a right angle
bool Polygon::HasARightAngle()
{
	vector<double> angles;

	// Construct angles in same manner as in Polygon::HasAllEqualAngles()
	for(int i = 0; i < vertices.size(); i++)
	{
		if (i+2 < vertices.size())
			angles.push_back(Angle(vertices[i],vertices[i+1],vertices[i+2]));
		else if (i+1 < vertices.size())
			angles.push_back(Angle(vertices[i],vertices[i+1],vertices[0]));
		else
			angles.push_back(Angle(vertices[i],vertices[0],vertices[1]));
	}

	// The instant we find a right angle, return true
	for (int i = 0; i < angles.size(); i++)
	{	
		if(RightAngle(angles[i]))
			return true;
	}

	// If there aren't any, return false
	return false;
}

// Checks to see if a Polygon has an obtuse angle
bool Polygon::HasAnObtuseAngle()
{
	vector<double> angles;

	// Construct angles in same manner as in Polygon::HasAllEqualAngles()
	for(int i = 0; i < vertices.size(); i++)
	{
		if (i+2 < vertices.size())
			angles.push_back(Angle(vertices[i],vertices[i+1],vertices[i+2]));
		else if (i+1 < vertices.size())
			angles.push_back(Angle(vertices[i],vertices[i+1],vertices[0]));
		else
			angles.push_back(Angle(vertices[i],vertices[0],vertices[1]));
	}

	// The instant we find an obtuse angle, return true
	for (int i = 0; i < angles.size(); i++)
	{	
		if(ObtuseAngle(angles[i]))
			return true;
	}

	// Otherwise, return false
	return false;
}

// Checks to see if a Polygon has an actue angle
bool Polygon::HasAnAcuteAngle()
{
	vector<double> angles;

	// Construct angles in same manner as in Polygon::HasAllEqualAngles()
	for(int i = 0; i < vertices.size(); i++)
	{
		if (i+2 < vertices.size())
			angles.push_back(Angle(vertices[i],vertices[i+1],vertices[i+2]));
		else if (i+1 < vertices.size())
			angles.push_back(Angle(vertices[i],vertices[i+1],vertices[0]));
		else
			angles.push_back(Angle(vertices[i],vertices[0],vertices[1]));
	}

	// The instant we find an actue angle, return true
	for (int i = 0; i < angles.size(); i++)
	{	
		if(AcuteAngle(angles[i]))
			return true;
	}

	// Otherwise, return false
	return false;
}

// Checks to see if a Polygon is convex
bool Polygon::IsConvex()
{
	vector<double> angles;

	// Construct angles in same manner as in Polygon::HasAllEqualAngles()
	for(int i = 0; i < vertices.size(); i++)
	{
		if (i+2 < vertices.size())
			angles.push_back(Angle(vertices[i],vertices[i+1],vertices[i+2]));
		else if (i+1 < vertices.size())
			angles.push_back(Angle(vertices[i],vertices[i+1],vertices[0]));
		else
			angles.push_back(Angle(vertices[i],vertices[0],vertices[1]));
	}

	// A polygon is concave if it has a reflex angle, so if a reflex angle is 
	// found, instantly return false, otherwise return true after checking all
	// angles
	for (int i = 0; i < angles.size(); i++)
	{	
		if(ReflexAngle(angles[i]))
			return false;
	}

	return true;
}

// Checks to see if a Polygon is concave
bool Polygon::IsConcave()
{
	vector<double> angles;

	// Construct angles in same manner as in Polygon::HasAllEqualAngles()
	for(int i = 0; i < vertices.size(); i++)
	{
		if (i+2 < vertices.size())
			angles.push_back(Angle(vertices[i],vertices[i+1],vertices[i+2]));
		else if (i+1 < vertices.size())
			angles.push_back(Angle(vertices[i],vertices[i+1],vertices[0]));
		else
			angles.push_back(Angle(vertices[i],vertices[0],vertices[1]));
	}

	// A polygon is concave if it has a reflex angle, so if a reflex angle is 
	// found, instantly return true, otherwise return false after checking all
	// angles
	for (int i = 0; i < angles.size(); i++)
	{	
		if(ReflexAngle(angles[i]))
			return true;
	}

	return false;
}