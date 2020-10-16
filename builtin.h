#pragma once

#include "type.h"
#include "scope.h"

namespace BuiltIn {
	using Object = Scope::Object;

	Object Int = { -1, NULL, Type::SCALAR_TYPE };
	Object* pInt = &Int;

	Object Char = { -1, NULL, Type::SCALAR_TYPE };
	Object* pChar = &Char;

	Object Bool = { -1, NULL, Type::SCALAR_TYPE };
	Object* pBool = &Bool;

	Object String = { -1, NULL, Type::SCALAR_TYPE };
	Object* pString = &String;

	Object Universal = { -1, NULL, Type::UNIVERSAL };
	Object* pUniversal = &Universal;
};