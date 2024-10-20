#include "physics.h"

//////////////////
// Struct Force //
//////////////////

Force::Force()
{
    force      = bn::fixed_point(0, 0);
    decay_rate = 0;
    timer      = 0;
}

Force::Force(bn::fixed_point_t<12> _force, bn::fixed _decay_rate)
{
    force         = _force;
    decayed_force = _force;
    decay_rate    = _decay_rate;
    timer         = 0;
}

void Force::update()
{
    // Interpolate the base force to update decayed force
    decayed_force.set_x(lerp(force.x(), 0, timer).floor_integer());
    decayed_force.set_y(lerp(force.y(), 0, timer).floor_integer());

    // Update timer
    if(timer < 1) {timer += decay_rate;}
    if(timer > 1) {timer = 1;}
}

const bn::fixed Force::x()
{
    return decayed_force.x().floor_integer();
}

const bn::fixed Force::y()
{
    return decayed_force.y().floor_integer();
}

bool Force::isDecayed()
{
    if(decayed_force.x() == 0 && decayed_force.y() == 0) {return true;}
    return false;
}

//////////////////////
// Struct Rigidbody //
//////////////////////

RigidBody::RigidBody()
{

}

bn::fixed_point RigidBody::applyForces(GameObject& object)
{
    // Apply all of the forces in the RigidBody to the object
    bn::fixed_point final_dir(0, 0);
    
    bn::ivector<Force*>::iterator current = forces.begin();
    bn::ivector<Force*>::iterator last    = forces.end();
    while(current != last)
    {
        // Update object position with new force
        object.setX(object.x() + (*current)->x());
        object.setY(object.y() + (*current)->y());

        // Update final dir vector with new force
        final_dir.set_x(final_dir.x() + (*current)->x());
        final_dir.set_y(final_dir.y() + (*current)->y());

        // Update iterator
        current++;
    }

    return final_dir;
}

void RigidBody::applyDecay()
{
        // Update all of the forces in the RigidBody
    bn::ivector<Force*>::iterator current = forces.begin();
    bn::ivector<Force*>::iterator last    = forces.end();
    while(current != last)
    {
        // Update the force (apply decay to it). 
        (*current)->update();

        // If the force is decayed, remove it from RigidBody.
        if((*current)->isDecayed())
        {
            delete *current;
            current = forces.erase(current);
            last    = forces.end();
        }
        else{current++;}
    }
}

uint8 RigidBody::addForce(Force* force_p)
{
    if(!forces.full())
    {
	    forces.push_back(force_p);
	    return 1;
    }
    return 0;
}
