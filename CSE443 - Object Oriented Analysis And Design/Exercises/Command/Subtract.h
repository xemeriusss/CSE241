#include <CalculatorCommand.h>

class Subtract : public CalculatorCommand
{
    public:
        explicit Subtract(int newOperand) : operand(newOperand) {}

        int execute(int i) override
        {
            return i - operand;
        }

        int undo(int i) override
        {
            return i + operand;
        }

    private:
        int operand{}; // The operand to subtract from the calculator's value

};