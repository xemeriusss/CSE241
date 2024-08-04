#include <ShapeVisitor.h>

class Shape {

    public:
    virtual ~Shape() = default; // virtual destructor

    virtual void accept(ShapeVisitor &v) = 0; // pure virtual function
};