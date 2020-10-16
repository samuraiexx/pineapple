#pragma once

#define IS_TYPE_KIND(k) ((k) == ARRAY_TYPE || \
                         (k) == STRUCT_TYPE || \
						 (k) == ALIAS_TYPE || \
					     (K) == SCALAR_TYPE)

namespace Type {
	enum Kind {
		NO_KIND_DEF = -1,
		VAR,
		PARAM,
		FUNCTION,
		FIELD,
		ARRAY_TYPE,
		STRUCT_TYPE,
		ALIAS_TYPE,
		SCALAR_TYPE,
		UNIVERSAL
	};
}