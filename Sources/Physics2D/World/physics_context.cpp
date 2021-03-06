/*
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
**    Arkadiusz Kalinowski
*/

#include "Physics2D/precomp.h"
#include "../Box2D/Box2D.h"
#include "../Dynamics/body_impl.h"
#include "../Dynamics/fixture_impl.h"
#include "../Dynamics/Joints/joint_impl.h"
#include "API/Physics2D/World/physics_context.h"
#include "API/Physics2D/World/physics_world.h"
#include "physics_context_impl.h"
#include "physics_world_impl.h"

namespace clan
{
//																						_______________________
//																						C O N S T R U C T O R S
PhysicsContext::PhysicsContext()
{
}

PhysicsContext::PhysicsContext(PhysicsWorld &pw)
: impl(new PhysicsContext_Impl(pw.impl.get()))
{

}

//																						___________________
//																						A T T R I B U T E S

int PhysicsContext::max_bodies() const
{
	return impl->max_body_amount;
}

int PhysicsContext::max_fixtures() const
{
	return impl->max_fixture_amount;
}

int PhysicsContext::max_joints() const
{
	return impl->max_joint_amount;
}

Body PhysicsContext::get_body(int id)
{
	return impl->get_body_from_context(id);
}

Fixture PhysicsContext::get_fixture(int id)
{
	return impl->get_fixture_from_context(id);
}

Joint PhysicsContext::get_joint(int id)
{
	return impl->get_joint_from_context(id);
}

//																						___________________
//																						O P E R A T I O N S
}
