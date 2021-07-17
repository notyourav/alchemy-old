#pragma once

#include <span>
#include <vector>
#include "disassembler/pcode.h"

class Disassembler;

enum class FlowType {
    Fallthrough,
    Conditional,
    Jump,
};

struct BasicBlock {
    size_t size() const { return instructions.size(); }

    std::span<const PInst> instructions;
    BasicBlock* next;
    FlowType flow_type;
};

struct Function {
    Function* next;
    std::span<const BasicBlock> blocks;
};

class Program {
public:
    struct SectionInfo {
        long offset;
        size_t size;
    };

    Program(const char* path, Disassembler& dis);

    long text_offset() const { return mTextInfo.offset; }
    size_t text_size() const { return mTextInfo.size; }

    std::span<const PInst> instructions() const { return mInstructions; }
    std::span<const BasicBlock> blocks() const { return mBlocks; }

private:
    void createFunctions();
    void createBasicBlocks();

    SectionInfo mTextInfo;
    std::vector<PInst> mInstructions;
    std::vector<BasicBlock> mBlocks;
};
