#include <Calculator.h>

void Calculator::compute(std::unique_ptr<CalculatorCommand> command)
{
    currentValue_ = command->execute(currentValue_);
    stack_.push(std::move(command));
}

void Calculator::undoLast()
{
    if(stack_.empty())
    {
        return;
    }

    auto command = std::move(stack_.top());
    stack_.pop();

    currentValue_ = command->undo(currentValue_);
}

int Calculator::result() const
{
    return currentValue_;
}

void Calculator::clear()
{
    currentValue_ = 0;
    while(!stack_.empty())
    {
        stack_.pop();
    }
}

