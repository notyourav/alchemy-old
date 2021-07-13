#include "analysis/inline.h"
#include "disassembler/program.h"
#include <span>


// Compare the similarity of the operands. Operands which are contextual (e.g. register) have a lower score,
// and operands which are more likely to be the same (e.g. immediates) have a higher score.
int scoreInstructionOperands(const Instruction& instruction, const Instruction& otherInstruction) {
        int score = 0;

    if (instruction.opcode != otherInstruction.opcode)
        return 0;

    for (unsigned long i = 0; i < instruction.operands.size(); ++i) {
        const Operand& operand = instruction.operands[i];
        const Operand& otherOperand = otherInstruction.operands[i];

        switch (operand.type) {
            case Imm:
                if (operand.data == otherOperand.data)
                    score += 10;
                break;
            case Rn:
            case Rd:
            case Rt2:
            case Rt:
            case Rs:
            case Rm:
                if (operand.data == otherOperand.data)
                    score += 5;
                break;
            case cond:
                if (operand.data == otherOperand.data)
                    score += 20;
                break;
            default:
                break;
        }
    }

    return score / instruction.operands.size();
}

// Compare the range of instructions with the rest of the program.
// If a similar set of instructions is found, accounting for minor differences,
// then it is probably an inline.

// TODO: iterating over every instruction wouldnt be efficient. First we need
// Program class to store basic blocks and iterate over those.
std::vector<InlineCandidate> searchInline(const Program& program, std::span<Instruction> function) {
    std::vector<InlineCandidate> candidates;
    (void)program;
    (void)function;

    return candidates;
}