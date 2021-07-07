#include <cstdint>
#include <vector>

// Defines key context and features of the code for the neural network to use.

struct Register {};

struct BasicBlock {
    enum class FlowType {
        Fallthrough,
        Conditional,
        Branch,
    };

    long start_address;
    long end_address;
};

struct Function {
    long start_address;
    long end_address;
};

// Output from neural network.

struct RegisterCalc {
    enum class Type {
        Add,
        Sub,
        Mul,
        Div,
        Mod,
        And,
        Or,
        Xor,
        Shl,
        Shr,
        Neg,
        Not,
    };
    Register* reg;
    Type type;
    long value;
};

struct FunctionCall {
    Function* target_function;
    long address;
    std::vector<Register> in_registers;
    Register out_register;
};

struct StructAccess {
    bool is_write;
    Register reg;
};

struct StructAccessStack : public StructAccess {
    TempStruct* struct_ptr;
};

struct StructAccessHeap : public StructAccess {
    long base_address;
    long offset;
};

// Definition of fundamental types.
struct TempFundamental {
    bool is_single_reg;
};

// Structs. Will capture arrays as well.
struct TempStruct : public TempFundamental {
    bool ctor_is_inline;
    Function* ctor;
};
