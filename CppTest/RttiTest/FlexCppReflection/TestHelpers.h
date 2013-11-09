/***************************************************************************
 *   Copyright (C) 2008 by tyler shaub   *
 *   tyler.shaub@gmail.com   *
 *                                                                         *
 *   See COPYING	*
 ***************************************************************************/

#pragma once

#include <iostream>

#include "TypeInfoHelpers.hpp"
#include "TypeManagerHelpers.hpp"
#include "CreateTypeInfo.hpp"

#ifndef __PRETTY_FUNCTION__
#	define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#define PRINT_FN { std::cout << __PRETTY_FUNCTION__ << std::endl; }

