#include "PooObject.h"



PooObject::PooObject(ComponentInput* input, ComponentPhysics* physics, ComponentGraphics* graphics)
	: input(input),
	physics(physics),
	graphics(graphics)
{
}


PooObject::~PooObject()
{
}
