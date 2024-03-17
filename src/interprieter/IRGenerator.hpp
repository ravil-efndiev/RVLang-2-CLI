#pragma once
#include "AST.hpp"

namespace Rvlang
{
    class IRGenerator
    {
    public:
        IRGenerator();
        ~IRGenerator();

        llvm::Value* Codegen(const std::shared_ptr<Node>& node);
        
    private:
        llvm::Value* GenVariable(const std::shared_ptr<VariableNode>& varNode);
        llvm::Value* GenNumber(const std::shared_ptr<NumberNode>& numNode);
        llvm::Value* GenBinaryOp(const std::shared_ptr<BinaryOperationNode>& binNode);
        llvm::Value* GenFunctionCall(const std::shared_ptr<FunctionCallNode>& fnNode);
        llvm::Function* GenFunctionProto(const std::shared_ptr<FunctionPrototypeNode>& fnNode);
        llvm::Type* GenType(VariableType type);

    private:
        std::unique_ptr<llvm::LLVMContext> m_Context;
        std::unique_ptr<llvm::Module> m_Module;
        std::unique_ptr<llvm::IRBuilder<>> m_Builder;
        std::unordered_map<std::string, llvm::Value*> m_SymbolTable;
    };
}

