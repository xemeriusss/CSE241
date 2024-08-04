#include <Shape.h>
#include <iostream>

using namespace std;

class Circle : public Shape {

    public:
    explicit Circle(double radius) : radius_(radius) {

        // Check for invalid radius
        if (radius_ <= 0) {
            std::cout << "Invalid radius: " << radius_ << std::endl;
        }
        
    }
    
    void accept(ShapeVisitor &v) override {
        v.visit(*this);
    }

    double radius() const {
        return radius_;
    }

    private:
    double radius_;



};