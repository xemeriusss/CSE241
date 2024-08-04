#include <CalculatorCommand.h>
#include <memory>
#include <stack>

class Calculator {
    public:
        void compute(std::unique_ptr<CalculatorCommand> command);
        void undoLast();

        int result() const;
        void clear();

    private:
        using CommandStack = std::stack<std::unique_ptr<CalculatorCommand>>;

        int currentValue_{};
        CommandStack stack_;
};