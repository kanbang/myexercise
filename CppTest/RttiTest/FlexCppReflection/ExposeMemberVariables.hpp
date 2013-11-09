#pragma once

#include "MemberVariable.hpp"

#define FLEX_EXPOSE_MEM_VARS(T)	friend struct ::Flex_::MemberVariableList<T>;

