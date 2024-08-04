class Circle;
class Square;

class ShapeVisitor {

    public:
    virtual ~ShapeVisitor() = default;

    virtual void visit(Circle& circle) = 0; // pure virtual function
    virtual void visit(Square& square) = 0;

    // 
};