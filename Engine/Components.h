#pragma once


class ComponentInput
{
public:
	virtual ~ComponentInput() {}
	virtual void update() = 0;
};
class ComponentPhysics
{
public:
	virtual ~ComponentPhysics() {}
	virtual void update() = 0;
};
class ComponentGraphics
{
public:
	virtual ~ComponentGraphics() {}
	virtual void update() = 0;
};

