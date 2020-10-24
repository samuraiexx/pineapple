#pragma once

namespace Type {
	enum class Kind {
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

	inline bool IS_TYPE_KIND(Kind k) {
			return (k) == Kind::ARRAY_TYPE ||
						    (k) == Kind::STRUCT_TYPE ||
					     (k) == Kind::ALIAS_TYPE ||
							   (k) == Kind::SCALAR_TYPE;
	}
}