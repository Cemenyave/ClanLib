/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Mark Page
*/

#include "precomp.h"
#include "utils.h"

Utils::Utils()
{
	string_hello_world = "Hello World";
	five = 5;
}


void Utils::test_empty()
{
}

int Utils::test_return_int()
{
	return five;
}

std::string Utils::test_return_string()
{
	return string_hello_world;
}

std::string &Utils::test_return_string2()
{
	return string_hello_world;
}

void Utils::test_get_string(std::string &out_string)
{
	out_string = string_hello_world;
}
