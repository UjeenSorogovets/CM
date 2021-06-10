#pragma once

#include "Component.h"


class ComponentPanel
{
public:
	vector<Component> components;

	int startPosX;
	int startPosY;

	int componentHeight = 100;
	int componentWidth = 100;

	int offsetX;
	int offsetY;

	bool horizontal = true;

	void push(Component component);
};

class FiltersPanel
{
public:
	vector<FilterComponent> components;

	int startPosX;
	int startPosY;

	int componentHeight = 50;
	int componentWidth = 180;

	int offsetX;
	int offsetY;

	bool horizontal = true;

	void push(FilterComponent component);
};