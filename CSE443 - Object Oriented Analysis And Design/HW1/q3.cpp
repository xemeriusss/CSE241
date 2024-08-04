#include <iostream>
#include <sstream>

// Shape interface
class Shape {
public:
    virtual void draw() const = 0;
    virtual std::string serialize() const = 0;
    virtual ~Shape() = default;
};

// ShapeModel class
template <typename ShapeT>
class ShapeModel : public Shape {
public:
    ShapeModel(const ShapeT& shape) : shape_(shape) {}

    void draw() const override {
        // Draw shape
    }

    std::string serialize() const override {
        std::stringstream ss;
        ss << "Serialized shape: " << shape_;
        return ss.str();
    }

private:
    ShapeT shape_;
};


// Library shapes
class LibCircle {
public:
    int32_t getRadius() const {
        return radius_;
    }

    void setRadius(const int32_t radius) {
        radius_ = radius;
    }

private:
    int32_t radius_;
};

class LibSquare {
public:
    int32_t getSide() const {
        return side_;
    }

    void setSide(const int32_t side) {
        side_ = side;
    }

private:
    int32_t side_;
};



// Function to demonstrate external polymorphism
void performOperations(const Shape& shape) {
    shape.draw();
    std::cout << "Serialized Output: " << shape.serialize() << std::endl;
}

int main() {
    // Create instances of library shapes
    LibCircle libCircle;
    libCircle.setRadius(5);

    LibSquare libSquare;
    libSquare.setSide(4);

    // Create instances of custom shapes using ShapeModel
    ShapeModel<LibCircle> myCircle(libCircle);
    ShapeModel<LibSquare> mySquare(libSquare);

    // Demonstrate external polymorphism
    performOperations(myCircle);
    performOperations(mySquare);

    return 0;
}


/*
#include <iostream>
#include <sstream>

// Shape interface
class Shape {
public:
    virtual void draw() const = 0;
    virtual std::string serialize() const = 0;
    virtual ~Shape() = default;
};

// Class template for MyCircle
template <class LibShape>
class MyCircle : public Shape {
public:
    MyCircle(const LibShape& libShape) : libShape_(libShape) {}

    void draw() const override {
        // Implement drawing logic for MyCircle using libShape_
        std::cout << "Drawing Circle with radius: " << libShape_.getRadius() << std::endl;
    }

    std::string serialize() const override {
        // Implement serialization logic for MyCircle using libShape_
        std::stringstream ss;
        ss << "Circle - Radius: " << libShape_.getRadius();
        return ss.str();
    }

private:
    LibShape libShape_;
};

// Class template for MySquare
template <class LibShape>
class MySquare : public Shape {
public:
    MySquare(const LibShape& libShape) : libShape_(libShape) {}

    void draw() const override {
        // Implement drawing logic for MySquare using libShape_
        std::cout << "Drawing Square with side: " << libShape_.getSide() << std::endl;
    }

    std::string serialize() const override {
        // Implement serialization logic for MySquare using libShape_
        std::stringstream ss;
        ss << "Square - Side: " << libShape_.getSide();
        return ss.str();
    }

private:
    LibShape libShape_;
};

// Library shape classes
class LibCircle {
public:
    int32_t getRadius() const {
        return radius_;
    }

    void setRadius(const int32_t radius) {
        radius_ = radius;
    }

private:
    int32_t radius_;
};

class LibSquare {
public:
    int32_t getSide() const {
        return side_;
    }

    void setSide(const int32_t side) {
        side_ = side;
    }

private:
    int32_t side_;
};

// Function to demonstrate external polymorphism
void performOperations(const Shape& shape) {
    shape.draw();
    std::cout << "Serialized Output: " << shape.serialize() << std::endl;
}

int main() {
    // Create instances of library shapes
    LibCircle libCircle;
    libCircle.setRadius(5);

    LibSquare libSquare;
    libSquare.setSide(4);

    // Create instances of custom shapes using MyCircle and MySquare
    MyCircle<LibCircle> myCircle(libCircle);
    MySquare<LibSquare> mySquare(libSquare);

    // Demonstrate external polymorphism
    performOperations(myCircle);
    performOperations(mySquare);

    return 0;
}

*/