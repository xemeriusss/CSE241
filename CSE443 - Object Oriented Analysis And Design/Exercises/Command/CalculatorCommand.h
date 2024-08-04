class CalculatorCommand
{
    public:
        virtual ~CalculatorCommand() = default;

        virtual int execute(int i) = 0;
        virtual int undo(int i) = 0;
};