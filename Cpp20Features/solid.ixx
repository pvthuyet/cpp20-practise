#include <iostream>

export module solid;

class Shape
{
public:
	virtual ~Shape() noexcept = default;
	virtual double getArea() const = 0;
	//virtual void draw() const = 0; // Violate ISP, and maybe SRP
};

class Drawable
{
public:
	virtual ~Drawable() noexcept = default;
	virtual void draw() const = 0;
	//virtual void draw(const Circle& c) const  // Violate SRP
	//{
	//	std::cout << "Circle\n";
	//}
};

/* Circle *************************************************************************************************/
class Circle;
class DrawCircle : public Drawable
{
public:
	virtual ~DrawCircle() noexcept = default;
	virtual void draw() const override
	{
		std::cout << "Circle\n";
	}
};

class Circle : public Shape
{
public:
	virtual ~Circle() noexcept = default;
	explicit Circle(double radius) : _radius{ radius } 
	{}

	virtual double getArea() const override
	{
		return 3.14 * _radius;
	}

	void draw() const
	{
		_painter.draw();
	}

private:
	double _radius;
	DrawCircle _painter;
};

/* Rectangle ***********************************************************************************************/
class Rectangle : public Shape
{
public:
	Rectangle() noexcept = default;
	virtual ~Rectangle() noexcept = default;
	explicit Rectangle(double w, double h) : 
		_width{ w }, _height { h }
	{}

	virtual void setw(double w) { _width = w; }
	virtual void seth(double h) { _width = h; }
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

/* Square ***********************************************************************************************/
class Square : public Rectangle // Violent LSP
{
public:
	virtual ~Square() noexcept {}
	explicit Square(double w)
	{
		setw(w);
		seth(w);
	}

	virtual void setw(double w) override { Rectangle::setw(w); Rectangle::seth(w); } // Work arround to fix LSP violent
	virtual void seth(double h) override { Rectangle::setw(h); Rectangle::seth(h); } // Work arround to fix LSP violent
};