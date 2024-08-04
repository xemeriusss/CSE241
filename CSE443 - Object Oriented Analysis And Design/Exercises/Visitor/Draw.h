#include <Circle.h>
#include <ShapeVisitor.h>
#include <Square.h>

class Draw : public ShapeVisitor {

    public:

    void visit(Circle& circle) override {
        std::cout << "Drawing a circle with radius " << circle.radius() << std::endl;

        // Draw the circle
        
        
    }

    void visit(Square& square) override {
        std::cout << "Drawing a square with side " << square.side() << std::endl;

        // Draw the square
        
    }
};