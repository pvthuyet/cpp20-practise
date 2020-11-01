#include <iostream>

export module solid;

class Shape
{
public:
	virtual ~Shape() noexcept {}
	virtual double getArea() const = 0;
	//virtual void draw() const = 0; // Violate ISP, and maybe SRP
};

class Circle;
class Drawable
{
public:
	virtual ~Drawable() noexcept {}
	virtual void draw(const Circle& c) const 
	{
		std::cout << "Circle\n";
	}
};

/* Circle *************************************************************************************************/
class Circle : public Shape
{
public:
	virtual ~Circle() noexcept {}
	explicit Circle(double radius) : _radius{ radius } 
	{}

	virtual double getArea() const override
	{
		return 3.14 * _radius;
	}

	virtual void draw() const
	{
		_painter.draw(*this);
	}

private:
	double _radius;
	Drawable _painter;
};

/* Circle *************************************************************************************************/
class Rectangle : public Shape
{
public:
	virtual ~Rectangle() noexcept {}
	explicit Rectangle(double w, double h) : 
		_width{ w }, _height { h }
	{}

	virtual double getArea() const override
	{
		return _width * _height;
	}

	//virtual void draw() const override // I don't use draw
	//{
	//}

private:
	double _width;
	double _height;
};