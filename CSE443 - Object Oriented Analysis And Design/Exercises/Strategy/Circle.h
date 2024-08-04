class Circle : public Shape {
    public:
        
        explicit Circle(double radius, std::unique_ptr<DrawStrategy<Circle>> drawer):
            radius_(radius),
            drawer_(std::move(drawer))
        {
            /* Checking that the given radius is valid and that
            the given 'std::unique_ptr' is not a nullptr */ 
        }

        void draw() const override {
            drawer_->draw(*this);
        }

        double getRadius() const {
            return radius_;
        }

    private:
        double radius_;
        std::unique_ptr<DrawStrategy<Circle>> drawer_;
            
};