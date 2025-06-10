#include "new_object_template.h"

//////////////////////
// Struct NewObject //
//////////////////////

NewObject::NewObject()
{

}

NewObject::NewObject(const NewObject& other) : GameObject(other)
{

}

NewObject::~NewObject()
{

}

NewObject& NewObject::operator =(const NewObject& other)
{
    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

// None..

//////////////////////////////
// State Function Overrides //
//////////////////////////////

// None..

/////////////////////////
// Collision Overrides //
/////////////////////////

// None..