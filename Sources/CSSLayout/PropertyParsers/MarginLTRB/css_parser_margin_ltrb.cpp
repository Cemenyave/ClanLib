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
*/

#include "CSSLayout/precomp.h"
#include "css_parser_margin_ltrb.h"
#include "API/CSSLayout/PropertyValues/css_value_margin_width.h"

namespace clan
{

std::vector<std::string> CSSParserMarginLTRB::get_names()
{
	std::vector<std::string> names;
	names.push_back("margin-left");
	names.push_back("margin-top");
	names.push_back("margin-right");
	names.push_back("margin-bottom");
	return names;
}

void CSSParserMarginLTRB::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueMarginWidth> width(new CSSValueMarginWidth());
	if (equals(name, "margin-top"))
		width->value_type = CSSValueMarginWidth::top_value;
	else if (equals(name, "margin-right"))
		width->value_type = CSSValueMarginWidth::right_value;
	else if (equals(name, "margin-bottom"))
		width->value_type = CSSValueMarginWidth::bottom_value;
	else if (equals(name, "margin-left"))
		width->value_type = CSSValueMarginWidth::left_value;

	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && pos == tokens.size())
	{
		if (equals(token.value, "auto"))
			width->type = CSSValueMarginWidth::type_auto;
		else if (equals(token.value, "inherit"))
			width->type = CSSValueMarginWidth::type_inherit;
		else
			return;
	}
	else if (is_length(token) && pos == tokens.size())
	{
		CSSLength length;
		if (parse_length(token, length))
		{
			width->type = CSSValueMarginWidth::type_length;
			width->length = length;
		}
		else
		{
			return;
		}
	}
	else if (token.type == CSSToken::type_percentage && pos == tokens.size())
	{
		width->type = CSSValueMarginWidth::type_percentage;
		width->percentage = StringHelp::text_to_float(token.value);
	}
	else if (token.type == CSSToken::type_delim && token.value == "-")
	{
		token = next_token(pos, tokens);
		if (is_length(token) && pos == tokens.size())
		{
			CSSLength length;
			if (parse_length(token, length))
			{
				length.value = -length.value;
				width->type = CSSValueMarginWidth::type_length;
				width->length = length;
			}
			else
			{
				return;
			}
		}
		else if (token.type == CSSToken::type_percentage && pos == tokens.size())
		{
			width->type = CSSValueMarginWidth::type_percentage;
			width->percentage = -StringHelp::text_to_float(token.value);
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}

	inout_values.push_back(std::move(width));
}

}
