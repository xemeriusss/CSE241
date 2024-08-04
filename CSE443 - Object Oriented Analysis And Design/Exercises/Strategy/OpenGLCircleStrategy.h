class OpenGLCircleStrategy : public DrawStrategy<Circle> {
    public:
        explicit OpenGLCircleStrategy(double radius)
        {
            /* Checking that the given radius is valid */ 
        }

        void draw(Circle const& circle) const override {
            /* Drawing the circle using OpenGL */
        }

    private:
        double radius_;
        
};
