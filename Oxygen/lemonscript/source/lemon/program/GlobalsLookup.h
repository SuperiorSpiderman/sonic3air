/*
*	Part of the Oxygen Engine / Sonic 3 A.I.R. software distribution.
*	Copyright (C) 2017-2022 by Eukaryot
*
*	Published under the GNU GPLv3 open source software license, see license.txt
*	or https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#pragma once

#include "lemon/program/Constant.h"
#include "lemon/program/ConstantArray.h"
#include "lemon/program/Define.h"
#include "lemon/program/Function.h"
#include "lemon/program/StringRef.h"
#include <unordered_map>


namespace lemon
{
	class Module;

	class GlobalsLookup
	{
	friend class Module;

	public:
		struct Identifier
		{
		public:
			enum class Type
			{
				UNDEFINED,
				GLOBAL_VARIABLE,
				CONSTANT,
				CONSTANT_ARRAY,
				DEFINE
			};

			inline Type getType() const  { return mType; }

			inline void set(Variable* variable)			  { mType = Type::GLOBAL_VARIABLE; mPointer = variable; }
			inline void set(Constant* constant)			  { mType = Type::CONSTANT;		   mPointer = constant; }
			inline void set(ConstantArray* constantArray) { mType = Type::CONSTANT_ARRAY;  mPointer = constantArray; }
			inline void set(Define* define)				  { mType = Type::DEFINE;		   mPointer = define; }

			template<typename T> const T& as() const  { return *static_cast<const T*>(mPointer); }

		private:
			Type mType = Type::UNDEFINED;
			const void* mPointer = nullptr;
		};

	public:
		void clear();
		void addDefinitionsFromModule(const Module& module);

		// All identifiers
		const Identifier* resolveIdentifierByHash(uint64 nameHash) const;

		// Functions
		const std::vector<Function*>& getFunctionsByName(uint64 nameHash) const;
		void registerFunction(Function& function);

		// Global variables
		void registerGlobalVariable(Variable& variable);

		// Constants
		void registerConstant(Constant& constant);

		// Constant arrays
		void registerConstantArray(ConstantArray& constantArray);

		// Defines
		void registerDefine(Define& define);

		// String literals
		const FlyweightString* getStringLiteralByHash(uint64 hash) const;

	private:
		// All identifiers
		std::unordered_map<uint64, Identifier> mAllIdentifiers;

		// Functions
		std::unordered_map<uint64, std::vector<Function*>> mFunctionsByName;	// Key is the hashed function name
		uint32 mNextFunctionID = 0;

		// Global variables
		uint32 mNextVariableID = 0;

		// Constant arrays
		uint32 mNextConstantArrayID = 0;

		// String literals
		StringLookup mStringLiterals;
	};

}
