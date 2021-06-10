#include "ComponentPanel.h"

void ComponentPanel::push(Component component)
	{
		if (component.path != "")
		{
			int size = components.size();
			component.startPosX = startPosX;
			component.startPosY = startPosY;

			component.component->setHeight(componentHeight);
			component.component->setWidth(componentWidth, 0);

			component.height = componentHeight;
			component.width = componentWidth;

			if (size == 0)
			{
				component.component->setPosition(startPosX, startPosY);

				cout << "add" << component.startPosX << endl;
			}
			else
			{
				if (horizontal)
				{
					component.startPosX = startPosX + component.width * (size)+offsetX * (size);
					component.component->setLabel(to_string(size));
					cout << "add" << component.startPosX << endl;
				}
				else
				{
					component.startPosY = startPosY * (size + 1) + offsetY * (size);
				}
				component.component->setPosition(component.startPosX, component.startPosY);
			}

			component.currentPosX = component.startPosX;
			component.currentPosY = component.startPosY;

			/*cout << "start:" << endl;
			cout << "X = " << component.startPosX << "\nY = " << component.startPosY << endl;
			cout << "current:" << endl;
			cout << "X = " << component.currentPosX << "\nY = " << component.currentPosY << endl;*/

			components.push_back(component);
		}
	}

void FiltersPanel::push(FilterComponent component)
	{
		if (component.path != "")
		{
			int size = components.size();
			component.startPosX = startPosX;
			component.startPosY = startPosY;

			component.component->setHeight(componentHeight);
			component.component->setWidth(componentWidth, 0);

			component.height = componentHeight;
			component.width = componentWidth;

			if (size == 0)
			{
				component.component->setPosition(startPosX, startPosY);

				cout << "add" << component.startPosX << endl;
			}
			else
			{
				if (horizontal)
				{
					component.startPosX = startPosX + component.width * (size)+offsetX * (size);
					component.component->setLabel(to_string(size));
					cout << "add" << component.startPosX << endl;
				}
				else
				{
					component.startPosY = startPosY + component.height * (size)+offsetY * (size);
				}
				component.component->setPosition(component.startPosX, component.startPosY);
			}

			component.currentPosX = component.startPosX;
			component.currentPosY = component.startPosY;

			components.push_back(component);
		}
	}