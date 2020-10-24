#pragma once

#include "type.h"
#include "scope.h"

namespace BuiltIn {
	using Kind = Type::Kind;

	Scope::Object Int = { -1, NULL, Kind::SCALAR_TYPE };
	Scope::Object* pInt = &Int;

	Scope::Object Char = { -1, NULL, Kind::SCALAR_TYPE };
	Scope::Object* pChar = &Char;

	Scope::Object Bool = { -1, NULL, Kind::SCALAR_TYPE };
	Scope::Object* pBool = &Bool;

	Scope::Object String = { -1, NULL, Kind::SCALAR_TYPE };
	Scope::Object* pString = &String;

	Scope::Object Universal = { -1, NULL, Kind::UNIVERSAL };
	Scope::Object* pUniversal = &Universal;
};