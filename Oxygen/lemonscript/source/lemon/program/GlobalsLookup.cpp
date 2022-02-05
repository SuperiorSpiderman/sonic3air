/*
*	Part of the Oxygen Engine / Sonic 3 A.I.R. software distribution.
*	Copyright (C) 2017-2022 by Eukaryot
*
*	Published under the GNU GPLv3 open source software license, see license.txt
*	or https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#include "lemon/pch.h"
#include "lemon/program/GlobalsLookup.h"
#include "lemon/program/Module.h"


namespace lemon
{

	void GlobalsLookup::clear()
	{
		mAllIdentifiers.clear();
		mFunctionsByName.clear();
	}

	void GlobalsLookup::addDefinitionsFromModule(const Module& module)
	{
		for (Function* function : module.mFunctions)
		{
			registerFunction(*function);
		}
		for (Variable* variable : module.mGlobalVariables)
		{
			registerGlobalVariable(*variable);
		}
		for (Constant* constant : module.mConstants)
		{
			registerConstant(*constant);
		}
		for (ConstantArray* constantArray : module.mConstantArrays)
		{
			registerConstantArray(*constantArray);
		}
		for (Define* define : module.mDefines)
		{
			registerDefine(*define);
		}

		RMX_ASSERT(mNextFunctionID == module.mFirstFunctionID, "Mismatch in function ID when adding module '" << module.getModuleName() << "' (" << mNextFunctionID << " vs. " << module.mFirstFunctionID << ")");
		RMX_ASSERT(mNextVariableID == module.mFirstVariableID, "Mismatch in variable ID when adding module '" << module.getModuleName() << "' (" << mNextVariableID << " vs. " << module.mFirstVariableID << ")");
		RMX_ASSERT(mNextConstantArrayID == module.mFirstConstantArrayID, "Mismatch in constant array ID when adding module '" << module.getModuleName() << "' (" << mNextConstantArrayID << " vs. " << module.mFirstConstantArrayID << ")");
		mNextFunctionID += (uint32)module.mFunctions.size();
		mNextVariableID += (uint32)module.mGlobalVariables.size();
		mNextConstantArrayID += (uint32)module.mConstantArrays.size();
	}

	const GlobalsLookup::Identifier* GlobalsLookup::resolveIdentifierByHash(uint64 nameHash) const
	{
		return mapFind(mAllIdentifiers, nameHash);
	}

	const std::vector<Function*>& GlobalsLookup::getFunctionsByName(uint64 nameHash) const
	{
		static const std::vector<Function*> EMPTY_FUNCTIONS;
		const auto it = mFunctionsByName.find(nameHash);
		return (it == mFunctionsByName.end()) ? EMPTY_FUNCTIONS : it->second;
	}

	void GlobalsLookup::registerFunction(Function& function)
	{
		const uint64 nameHash = function.getName().getHash();
		mFunctionsByName[nameHash].push_back(&function);
	}

	void GlobalsLookup::registerGlobalVariable(Variable& variable)
	{
		const uint64 nameHash = variable.getName().getHash();
		mAllIdentifiers[nameHash].set(&variable);
	}

	void GlobalsLookup::registerConstant(Constant& constant)
	{
		const uint64 nameHash = constant.getName().getHash();
		mAllIdentifiers[nameHash].set(&constant);
	}

	void GlobalsLookup::registerConstantArray(ConstantArray& constantArray)
	{
		const uint64 nameHash = constantArray.getName().getHash();
		mAllIdentifiers[nameHash].set(&constantArray);
	}

	void GlobalsLookup::registerDefine(Define& define)
	{
		const uint64 nameHash = define.getName().getHash();
		mAllIdentifiers[nameHash].set(&define);
	}

	const FlyweightString* GlobalsLookup::getStringLiteralByHash(uint64 hash) const
	{
		return mStringLiterals.getStringByHash(hash);
	}

}
