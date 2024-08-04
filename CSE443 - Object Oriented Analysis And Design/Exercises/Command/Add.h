#include <CalculatorCommand.h>

class Add : public CalculatorCommand
{
    public:
        explicit Add(int newOperand) : operand(newOperand) {}

        int execute(int i) override
        {
            return i + operand;
        }

        int undo(int i) override
        {
            return i - operand;
        }

    private:
        int operand{}; // The operand to add to the calculator's value

};