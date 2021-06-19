#pragma once

#include <memory>
#include <vector>
#include <array>

#include "grComp/Type.h"
//#include "grComp/Parser.h"


namespace grComp {

#define AST_TYPES \
	/* eager types */ \
	/* ExprASTs */ \
	AST_TYPE_MODIFIER(NumberExprAST) \
	AST_TYPE_MODIFIER(VariableExprAST) \
	AST_TYPE_MODIFIER(BinaryExprAST) \
	AST_TYPE_MODIFIER(CallExprAST) \
	\
	AST_TYPE_MODIFIER(PrototypeAST) \
	\
	/* lazy types */ \
	/* DefinitionASTs */ \
	AST_TYPE_MODIFIER(FunctionDefAST) \
	/* Top level */ \
	AST_TYPE_MODIFIER(ModuleAST)



#define AST_TYPE_MODIFIER(x) x##_t,
	enum class AstType : char {
		ErrorAST_t = 0,
		AST_TYPES
	};
#undef AST_TYPE_MODIFIER


	//base class for all ASTs
	class BaseAST {
		friend class EagerAST;
		friend class LazyAST;

	private:
		const AstType ast_type = AstType::ErrorAST_t;
		BaseAST(const AstType& ast_type);
	public:
		AstType get_ast_type() inline const { return ast_type;  }


		template<class T>
		static constexpr inline AstType ClassTypeToAstType() {};

		template<AstType T>
		struct AstTypeToClassType {};

		template<AstType ast>
		static inline bool is_lazy();
		template<AstType ast>
		static inline bool is_expression();
		template<AstType ast>
		static inline bool is_definition();

		template<typename ast>
		static inline bool is_lazy();
		template<typename ast>
		static inline bool is_expression();
		template<typename ast>
		static inline bool is_definition();


#define RUNTIME_FUNC(xFUNC) \
		static constexpr bool xFUNC(const AstType& type); \
		inline constexpr bool xFUNC();

		RUNTIME_FUNC(is_lazy)
		RUNTIME_FUNC(is_expression)
		RUNTIME_FUNC(is_definition)

#undef RUNTIME_FUNC
	};

	// evaluated immediately
	class EagerAST : public BaseAST {
	protected:
		EagerAST(const AstType& ast_type);
	};


	class Parser;
	// TODO: actually implement it! and replace EagerAST by LazyAST in big asts like ModuleAST, FunctionAST etc...
	// evaluated asynchronously
	class LazyAST : public BaseAST {
	protected:
		const grComp::Parser* m_parser;

		LazyAST(const AstType& ast_type);

		/*void eval_before() {
			m_parser->eval_until(this.id);
		}*/
	};




	class ExprAST : public EagerAST {
	public:
		std::unique_ptr<grComp::Type> Type;
		//virtual ~ExprAST() {}
	protected:
		ExprAST(const AstType& ast_type);
	};

	/// NumberExprAST - Expression class for numeric literals like "1.0".
	class NumberExprAST : public ExprAST {
	public:
		union {
			double ValR;
			long long ValZ;
			unsigned long long ValN;
		};

		NumberExprAST(double Val);
		/*NumberExprAST(long long Val) : ValZ(Val) {}
		NumberExprAST(unsigned long long Val) : ValN(Val) {}*/

	};

	/// VariableExprAST - Expression class for referencing a variable, like "a".
	class VariableExprAST : public ExprAST {
	public:
		std::string Name;

		VariableExprAST(const std::string &Name);
	};

	/// BinaryExprAST - Expression class for a binary operator.
	class BinaryExprAST : public ExprAST {
	public:
		signed short Op;
		std::unique_ptr<ExprAST> LHS, RHS;

		BinaryExprAST(signed short op, std::unique_ptr<ExprAST> LHS,
			std::unique_ptr<ExprAST> RHS);
		
	};

	/// CallExprAST - Expression class for function calls.
	class CallExprAST : public ExprAST {
	public:
		std::string Callee;
		std::vector<std::unique_ptr<ExprAST>> Args;

		CallExprAST(const std::string &Callee,
			std::vector<std::unique_ptr<ExprAST>> Args);
	};

	/// DefinitionAST : represents function, class, struct, enum... definitions
	class DefinitionAST : public EagerAST {
	protected:
		DefinitionAST(const AstType& ast_type);
	};

	/// PrototypeAST 
	class PrototypeAST : public EagerAST {
	public:
		std::string Name;
		std::vector<std::string> Args;

		PrototypeAST(const std::string &name, std::vector<std::string> Args);

		const std::string &getName() const { return Name; }
	};

	/// FunctionAST - This class represents a function definition itself.
	class FunctionDefAST : public DefinitionAST {
	public:
		std::unique_ptr<PrototypeAST> Proto;
		std::unique_ptr<ExprAST> Body;

		FunctionDefAST(std::unique_ptr<PrototypeAST> Proto,
			std::unique_ptr<ExprAST> Body);
	};

	/// ModuleAST - this ast represents the whole module file
	class ModuleAST : public EagerAST {
	public:
		std::vector<std::unique_ptr<DefinitionAST>> lines;

		ModuleAST();
	};


	std::string ASTtoStr(const BaseAST* ast);

#define RUNTIME_FUNC(xFUNC) \
	inline constexpr bool BaseAST::##xFUNC() { return xFUNC(get_ast_type()); } \
	constexpr bool BaseAST::##xFUNC(const AstType& type) { \
		switch (type){ \
			AST_TYPES \
		default: return false; \
		} \
	}

#define AST_TYPE_MODIFIER(xTYPE) case AstType::##xTYPE##_t: return is_lazy<xTYPE>();
RUNTIME_FUNC(is_lazy)
#undef AST_TYPE_MODIFIER
#define AST_TYPE_MODIFIER(xTYPE) case AstType::##xTYPE##_t: return is_expression<xTYPE>();
RUNTIME_FUNC(is_expression)
#undef AST_TYPE_MODIFIER
#define AST_TYPE_MODIFIER(xTYPE) case AstType::##xTYPE##_t: return is_definition<xTYPE>();
RUNTIME_FUNC(is_definition)
#undef AST_TYPE_MODIFIER

#undef AST_TYPE_MODIFIER
#undef RUNTIME_FUNC
};


