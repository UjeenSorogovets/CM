#pragma once
#include <Component.h>
#include "ofApp.h"
#include "MediaViewer.h"
#include "Component.h"
#include "ComponentPanel.h"

extern vector<Component> allComponents;
extern ComponentPanel horizontalPanel;
//static ComponentPanel verticalPanel;
extern FiltersPanel verticalPanel;


void resizePlayerToMedia(MediaViewer* mediaViewer, ComponentType mediaType);

bool inViewerCondition(int x, int y, MediaViewer* mediaViewer);

FilterComponent createFilterButton(ofApp* ofApp, string label);

FilterComponent createFilterButton(ofApp* ofApp, string label, std::function<vector<Component>(vector<Component>)> filterFunc);

Component createMediaButton(ofApp* ofApp, string defaultPath = "");

void runInCurrentPlayer(MediaViewer* mediaViewer, int res);

void playPausePlayer(MediaViewer* mediaViewer);

int catchMediaButton(int x, int y, ComponentPanel* componentPanel);

void updateAll(Component component);

void updateAll(ComponentPanel componentPanel);

void updateAll(FilterComponent component);

void updateAll(FiltersPanel componentPanel);