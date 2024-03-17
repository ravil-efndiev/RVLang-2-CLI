#include "IRGenerator.hpp"
#include "cli/Utils.hpp"

namespace Rvlang
{
    using namespace llvm;

    IRGenerator::IRGenerator()
    {
        m_Context = std::make_unique<LLVMContext>();
        m_Module = std::make_unique<Module>("Rvlang", *m_Context);
        m_Builder = std::make_unique<IRBuilder<>>(*m_Context);
    }

    IRGenerator::~IRGenerator() {}

    Value* IRGenerator::Codegen(const std::shared_ptr<Node>& node)
    {
        switch (node->GetType())
        {
            case NodeType::Variable:
                return GenVariable(Utils::As<VariableNode>(node));
            case NodeType::Number:
                return GenNumber(Utils::As<NumberNode>(node));
            case NodeType::BinaryOperation:
                return GenBinaryOp(Utils::As<BinaryOperationNode>(node));
            case NodeType::FunctionCall:
                return GenFunctionCall(Utils::As<FunctionCallNode>(node));
            default:
                return nullptr;
        }
    }

    Value* IRGenerator::GenVariable(const std::shared_ptr<VariableNode>& varNode)
    {
        Value* val = m_SymbolTable[varNode->Name];
        if (!val)
        {
            LogError("Unknown variable: `" + varNode->Name + "`");
            return nullptr;
        }
        return val;
    }
    
    Value* IRGenerator::GenNumber(const std::shared_ptr<NumberNode>& numNode)
    {
        switch (numNode->ValueType) 
        {
            case NumberType::i32:
                return ConstantInt::getSigned(Type::getInt32Ty(*m_Context), std::stoi(numNode->Number.Text));
            case NumberType::f32:
                return ConstantFP::get(*m_Context, APFloat(std::stof(numNode->Number.Text)));
            default:
                LogError("Unsupported type yet");
                return nullptr;
        }
    }
    
    Value* IRGenerator::GenBinaryOp(const std::shared_ptr<BinaryOperationNode>& binNode)
    {
        Value* left = Codegen(binNode->LeftNode);
        Value* right = Codegen(binNode->RightNode);

        if (!left || !right)
            return nullptr;
        
        switch (binNode->Operator.Type.Name) 
        {
            case PLUS:
                return m_Builder->CreateFAdd(left, right, "addtmp");
            case MINUS:
                return m_Builder->CreateFSub(left, right, "subtmp");
            case MULT:
                return m_Builder->CreateFMul(left, right, "multmp");
            case DIV:
                return m_Builder->CreateFDiv(left, right, "divtmp");
            default:
                LogError("unknown binary operator");
                return nullptr;
        }
    }
    
    Value* IRGenerator::GenFunctionCall(const std::shared_ptr<FunctionCallNode>& fnNode)
    {
        Function* func = m_Module->getFunction(fnNode->FunctionName);
        if (!func)
            return nullptr;

        if (func->arg_size() != fnNode->Arguments.size()) 
        {
            LogError("wrong number of arguments passed in function call");
            return nullptr;
        }

        std::vector<Value*> argvs;
        for (const auto& arg : fnNode->Arguments)
        {
            Value* argv = Codegen(arg);
            if (!argv)
                return nullptr;

            argvs.push_back(argv);
        }

        return m_Builder->CreateCall(func, argvs, "calltmp");
    }
    
    Function* IRGenerator::GenFunctionProto(const std::shared_ptr<FunctionPrototypeNode>& fnNode)
    {
        std::vector<llvm::Type*> argTypes;
        for (const auto& arg : fnNode->Arguments)
        {
            Type* type = GenType(arg->Type);
            if (!type) 
            {
                LogError("unsupported function argument type");
                return nullptr;
            }
            argTypes.push_back(type);
        }
        Type* returnType = GenType(fnNode->ReturnType);
        if (!returnType)
        {
            LogError("unsupported function return type");
            return nullptr;
        }

        FunctionType* ft = FunctionType::get(returnType, argTypes, false);

        Function* func = Function::Create(ft, Function::ExternalLinkage, fnNode->FunctionName, m_Module.get());

        uint32_t i = 0;
        for (auto& arg : func->args()) 
            arg.setName(fnNode->Arguments[i++]->Name);
        
        return func;
    }
    
    Type* IRGenerator::GenType(VariableType type)
    {
        switch (type)
        {
            case VariableType::i32:
                return Type::getInt32Ty(*m_Context);
            case VariableType::f32:
                return Type::getFloatTy(*m_Context);
            case VariableType::Void:
                return Type::getVoidTy(*m_Context);
            default:
                return nullptr;
        }
    }
}
