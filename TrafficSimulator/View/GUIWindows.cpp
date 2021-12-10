/**
 * @name Traffic Simulation
 * @file GUIWindows.cpp
 * @class GUI
 * @author S�ndor Bal�zs - AZA6NL
 * @date 2021.11.08.
 * @brief GUI Descriptor class.
 * Contact: sandorbalazs9402@gmail.com
 * KSP
*/

#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include "GUI.h"
#include "fpsCounter.h"
#include "Render.h"
#include "WorkWindow.h"
#include "../Control/Logger.h"
#include "../Model/Graph.h"
#include "../Model/Vehicle.h"
#include "Animator.h"

void GUI::openWindow() {
	ImGui::SetNextWindowSize(ImVec2(480, 360));
	ImGui::OpenPopup("Open Map");
	if (ImGui::BeginPopupModal("Open Map"))
	{
		ImGui::Text("Saved maps:");

		ImGui::Separator();

		std::vector<std::string> fileList = windowRender->getMapLoader()->listFiles();
		int itemNumber = fileList.size();

		const char* items[500];

		if(itemNumber >= 500) itemNumber = 500;

		for (size_t i = 0; i < itemNumber; i++) {
			items[i] = fileList[i].c_str();
		}

		static int selection = 0;
		ImGui::PushItemWidth(-1);
		ImGui::PushID(0);
		ImGui::ListBox("", &selection, items, itemNumber, 15);
		ImGui::PopID();


		if (ImGui::Button("Open")) {
			if (windowRender->getObjectsNumber() > 1 || windowRender->getDynamicObjectsNumber() > 0) {
				ImGui::OpenPopup("Are you sure?");
			}
			else {
				windowRender->clear();
				windowRender->getMapLoader()->loadMap(items[selection]);
				windowRender->getMapSaver()->setLastSave(items[selection]);
				openWindowStatus = false;
			}
		}
		if (ImGui::BeginPopupModal("Are you sure?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Are you sure overwrite the current map?");
			ImGui::Separator();
			if (windowRender->getMapSaver()->getLastSave() == windowRender->getMapSaver()->unsavedMarker) {
				ImGui::Text("");
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "The current map is UNSAVED!");
				ImGui::Text("");
			}else{
				ImGui::Text("The last save time:  %s", windowRender->getMapSaver()->getLastSaveTime().c_str());
				ImGui::Text("Now:                 %s", Logger::currentDateTime().c_str());
				ImGui::Text("The last saved file: %s", windowRender->getMapSaver()->getLastSave());
			}
			ImGui::Separator();
			ImGui::Text("The loadable file:   %s", items[selection]);
			ImGui::Separator();
			if (ImGui::Button("Open")) {
				windowRender->clear();
				windowRender->getMapLoader()->loadMap(items[selection]);
				windowRender->getMapSaver()->setLastSave(items[selection]);
				openWindowStatus = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("Delete")) {
			ImGui::OpenPopup("Are you sure you want to delete?");
		}
		if (ImGui::BeginPopupModal("Are you sure you want to delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Are you sure you want to delete?");
			if (ImGui::Button("Delete")) {
				int status = windowRender->getMapLoader()->deleteSave(items[selection]);
				if (status == 0) ImGui::OpenPopup("Map deleted");
				if (status == 1) ImGui::OpenPopup("Map not found");
				if (status == 2) ImGui::OpenPopup("Map open file system error");
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		if (ImGui::BeginPopupModal("Map deleted", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Map deleted.");
			if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		if (ImGui::BeginPopupModal("Map not found", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Map not found.");
			if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		if (ImGui::BeginPopupModal("Map open file system error", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("File system error.");
			if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Close")) {
			openWindowStatus = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void GUI::saveWindow() {
	const std::string lastSave = windowRender->getMapSaver()->getLastSave();
	if (lastSave == windowRender->getMapSaver()->unsavedMarker) {
		saveWindowStatus = false;
		ImGui::CloseCurrentPopup();
		saveAsWindowStatus = true;
	} else {
		windowRender->getMapSaver()->saveMap(lastSave);
		ImGui::OpenPopup("Map saved");
		if (ImGui::BeginPopupModal("Map saved", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("%s map saved!", lastSave);
			ImGui::Separator();
			if (ImGui::Button("Close")) {
				saveWindowStatus = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
}

void GUI::saveAsWindow() {
	ImGui::OpenPopup("Save map as");
	if (ImGui::BeginPopupModal("Save map as", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

		ImGui::Text("Saving the new map.");

		ImGui::Separator();

		static char newFileName[64] = "save";
		ImGui::InputText("New map name.", newFileName, 64);

		if (ImGui::Button("Save")) {
			std::vector<std::string> saves = windowRender->getMapLoader()->listFiles();
			if (std::find(saves.begin(), saves.end(), newFileName) != saves.end()) {
				ImGui::OpenPopup("Overwrite");
			} else {
				windowRender->getMapSaver()->saveMap(newFileName);
				ImGui::OpenPopup("Save map");
			}
		}
		if (ImGui::BeginPopupModal("Overwrite", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "This file already exist!");
			ImGui::Text("Do you want to overwrite: %s ?", newFileName);
			if (ImGui::Button("Save")) {
				windowRender->getMapSaver()->saveMap(newFileName);
				ImGui::OpenPopup("Save map");
			}
			if (ImGui::BeginPopupModal("Save map", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
				ImGui::Text("New map saved as: %s", newFileName);
				if (ImGui::Button("Close")) {
					saveAsWindowStatus = false;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		if (ImGui::BeginPopupModal("Save map", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("New map saved as: %s", newFileName);
			if (ImGui::Button("Close")) {
				saveAsWindowStatus = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();

		if (ImGui::Button("Close")) {
			saveAsWindowStatus = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void GUI::newMapConfirmWindow() {
	ImGui::OpenPopup("New map");
	if (ImGui::BeginPopupModal("New map", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Are you sure you open a new map?");
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "All unsaved changes will be lost.");

		ImGui::Text("");
		ImGui::Separator();
		ImGui::Text("");

		ImGui::Text("The last save time:  %s", windowRender->getMapSaver()->getLastSaveTime().c_str());
		ImGui::Text("Now:                 %s", Logger::currentDateTime().c_str());

		ImGui::Text("The last saved file: %s", windowRender->getMapSaver()->getLastSave());

		ImGui::Text("");
		ImGui::Separator();
		ImGui::Text("");

		if (ImGui::Button("New")) {
			ImGui::OpenPopup("Are you sure?");
		}
		if (ImGui::BeginPopupModal("Are you sure?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Are you sure you open a new map?");
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "All unsaved changes will be lost.");
			if (ImGui::Button("New")) {
				windowRender->clear();
				newMapConfirmWindowStatus = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Close")) {
				newMapConfirmWindowStatus = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Close")) {
			newMapConfirmWindowStatus = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void GUI::controlsWindow() {
	ImGui::OpenPopup("Controls");
	if (ImGui::BeginPopupModal("Controls", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Image((void*)(intptr_t)objectStorage->getTexture("controls.png"), ImVec2(859, 409));
		if (ImGui::Button("Close")) {
			controlsWindowStatus = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void GUI::aboutWindow() {
	ImGui::OpenPopup("About");
	if (ImGui::BeginPopupModal("About", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Traffic Simulation.");
		ImGui::Text("Created By: Sandor Balazs");
		ImGui::Text("AZA6NL");
		ImGui::Text("Thesis for Computer Science BSC in ELTE IK Budapest.");
		ImGui::Text("sandorbalazs9402@gmail.com");
		if (ImGui::Button("Close")) {
			aboutWindowStatus = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

/**
 * @brief Application closing checker window.
 * @return The user want to close?
*/
void GUI::closeWindow() {
	ImGui::OpenPopup("Are you sure you want to close?");
	if (ImGui::BeginPopupModal("Are you sure you want to close?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Are you sure you want to close?");
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "All unsaved changes will be lost.");

		ImGui::Text("");
		ImGui::Separator();
		ImGui::Text("");

		ImGui::Text("The last save time:  %s", windowRender->getMapSaver()->getLastSaveTime().c_str());
		ImGui::Text("Now:                 %s", Logger::currentDateTime().c_str());

		ImGui::Text("The last saved file: %s", windowRender->getMapSaver()->getLastSave());

		ImGui::Text("");
		ImGui::Separator();
		ImGui::Text("");

		if (ImGui::Button("Back")) {
			closingCheckerWindowStatus = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Close")) {
			closingCheckerWindowStatus = false;
			ImGui::CloseCurrentPopup();
			workingWindow->closeNow();
		}

		ImGui::EndPopup();
	}
}

/**
 * @brief Window descriptor for the graphic settings window.
*/
void GUI::graphicSettingsWindow() {
	if(ImGui::Begin("Graphic settings", &graphicSettingsWindowStatus, graphicSettingsWindowFlag)) {

		ImGui::Text("Current running data:");
		ImGui::Separator();

		ImGui::Text("");
		fpsGraph();

		ImGui::Text("");
		ImGui::Text("Graphic settings:");
		ImGui::Separator();
		ImGui::Text("");

		static bool fpsLimiterIsOn = false;
		ImGui::Checkbox("FPS Limiter", &fpsLimiterIsOn);
		if (fpsLimiterIsOn) {
			fpsCounter::fpsLimiterOn();
		} else {
			fpsCounter::fpsLimiterOff();
		}
		ImGui::Text("");

		static int fpsLimit = 60;
		ImGui::InputInt("FPS Limit", &fpsLimit);
		ImGui::SameLine(); showHelpMarker("WUT?");
		fpsCounter::setFpsLimit(fpsLimit);

		ImGui::Text("");
		static bool vSyncIsOn = true;
		ImGui::Checkbox("V-Sync", &vSyncIsOn);
		if (vSyncIsOn) {
			windowRender->vsyncOn();
		}
		else {
			windowRender->vsyncOff();
		}

		ImGui::Text("");
		static bool multisamplingIsOn = true;
		ImGui::Checkbox("Multisampling", &multisamplingIsOn);
		if (multisamplingIsOn) {
			windowRender->multisamplingOn();
		}
		else {
			windowRender->multisamplingOff();
		}

		ImGui::Text("");
		ImGui::Text("Lighting settings:");
		ImGui::Separator();

		if (ImGui::Button("Reset light settings")) {
			windowRender->lightPosition = glm::vec3(55, 55, 55);
			windowRender->lightCameraTracking = true;
			windowRender->ambientLight = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
			windowRender->diffuseLight = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);
			windowRender->specularLight = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			windowRender->ambientMaterial = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
			windowRender->diffuseMaterial = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			windowRender->specularMaterial = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
			windowRender->specularPower = 32.0f;
		}

		ImGui::Text("");
		ImGui::Checkbox("Light Camera tracking", &windowRender->lightCameraTracking);

		if (!windowRender->lightCameraTracking) {
			static float lightPosition[3] = { 0.0f, 0.0f, 0.0f };
			for (size_t i = 0; i < 3; i++) {
				lightPosition[i] = windowRender->lightPosition[i];
			}
			ImGui::SliderFloat3("Light position", lightPosition, -200.0f, 500.0f);
			for (size_t i = 0; i < 3; i++) {
				windowRender->lightPosition[i] = lightPosition[i];
			}
		}

		ImGui::Text("");
		ImGui::Text("Ambient light");
		ImGui::Separator();
		static bool ambientLightComponents = true;
		ImGui::Checkbox("Ambient light components", &ambientLightComponents);
		if (!ambientLightComponents) {
			static float ambientLightPower = windowRender->ambientLight[0];
			ImGui::SliderFloat("Ambient light", &ambientLightPower, 0.0f, 1.0f);
			ImGui::SameLine(); showHelpMarker("CTRL+click to input value.");
			for (size_t i = 0; i < 3; i++) {
				windowRender->ambientLight[i] = ambientLightPower;
			}
		} else {
			static float ambientLight[3] = { 0.0f, 0.0f, 0.0f };
			for (size_t i = 0; i < 3; i++) {
				ambientLight[i] = windowRender->ambientLight[i];
			}
			ImGui::SliderFloat3("Ambient lights", ambientLight, 0.0f, 1.0f);
			for (size_t i = 0; i < 3; i++) {
				windowRender->ambientLight[i] = ambientLight[i];
			}
		}

		ImGui::Text("");
		ImGui::Text("Diffuse light");
		ImGui::Separator();
		static bool diffuseLightComponents = true;
		ImGui::Checkbox("Diffuse light components", &diffuseLightComponents);
		if (!diffuseLightComponents) {
			static float diffuseLightPower = windowRender->diffuseLight[0];
			ImGui::SliderFloat("Diffuse light", &diffuseLightPower, 0.0f, 1.0f);
			ImGui::SameLine(); showHelpMarker("CTRL+click to input value.");
			for (size_t i = 0; i < 3; i++) {
				windowRender->diffuseLight[i] = diffuseLightPower;
			}
		}
		else {
			static float diffuseLight[3] = { 0.0f, 0.0f, 0.0f };
			for (size_t i = 0; i < 3; i++) {
				diffuseLight[i] = windowRender->diffuseLight[i];
			}
			ImGui::SliderFloat3("Diffuse light", diffuseLight, 0.0f, 1.0f);
			for (size_t i = 0; i < 3; i++) {
				windowRender->diffuseLight[i] = diffuseLight[i];
			}
		}

		ImGui::Text("");
		ImGui::Text("Specular light");
		ImGui::Separator();
		static bool specularLightComponents = true;
		ImGui::Checkbox("Specular light components", &specularLightComponents);
		if (!specularLightComponents) {
			static float specularLightPower = windowRender->specularLight[0];
			ImGui::SliderFloat("Specular light", &specularLightPower, 0.0f, 1.0f);
			ImGui::SameLine(); showHelpMarker("CTRL+click to input value.");
			for (size_t i = 0; i < 3; i++) {
				windowRender->specularLight[i] = specularLightPower;
			}
		} else {
			static float specularLight[3] = { 0.0f, 0.0f, 0.0f };
			for (size_t i = 0; i < 3; i++) {
				specularLight[i] = windowRender->specularLight[i];
			}
			ImGui::SliderFloat3("Specular lights", specularLight, 0.0f, 1.0f);
			for (size_t i = 0; i < 3; i++) {
				windowRender->specularLight[i] = specularLight[i];
			}
		}

		ImGui::Text("");
		ImGui::Text("Specular light power: ");
		ImGui::Separator();
		ImGui::SliderFloat("Specular Light Power", &windowRender->specularPower, 0.0f, 100.0f);

		ImGui::Text("");
		ImGui::Separator();
		if (ImGui::Button("Close")) {
			graphicSettingsWindowStatus = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::End();
	}
}

/**
 * @brief Window for the debug options.
*/
void GUI::debugOptionsWindow() {
	if (ImGui::Begin("Special debug options.", &debugOptionsWindowStatus, NULL)) {
	ImGui::Text("Special debug options:");
	ImGui::Separator();
	ImGui::Text("");

	static bool photoModeIsOn = false;
	ImGui::Checkbox("Object Photo Mode", &photoModeIsOn);
	if (photoModeIsOn) {
		windowRender->photoModeOn();
	}
	else {
		windowRender->photoModeOff();
	}

	ImGui::Text("");
	ImGui::Checkbox("Show Road Hit Sharpe", &windowRender->roadHhitSphare);
	ImGui::Separator();
	ImGui::Indent();
		ImGui::Text("");
		ImGui::Checkbox("Show roads Hit Sharpe", &windowRender->showRoadHitSphereMiddle);

		ImGui::Text("");
		ImGui::Checkbox("Show roads side 1 markers", &windowRender->showRoadSide01);

		ImGui::Text("");
		ImGui::Checkbox("Show roads side 2 markers", &windowRender->showRoadSide02);

		ImGui::Text("");
		ImGui::Checkbox("Show roads track 1 markers", &windowRender->showRoadTrack01);

		ImGui::Text("");
		ImGui::Checkbox("Show roads track 2 markers", &windowRender->showRoadTrack02);
	ImGui::Unindent();

	ImGui::Text("");
	ImGui::Checkbox("Show Hit Sharpe", &windowRender->hitSphare);

	ImGui::Text("");
	ImGui::Checkbox("Show Vehicle Hit Sharpe", &windowRender->vehicleHitSphare);

	ImGui::Text("");
	ImGui::Checkbox("Show Objects Wire frame", &windowRender->objectsWireframe);
	ImGui::Text("");
	ImGui::Checkbox("Show Roads Wire frame", &windowRender->roadWireframe);
	ImGui::Text("");

	/*ImGui::Text("");
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "---");
	ImGui::Separator();
	ImGui::Text("---");
	ImGui::SameLine();*/

	ImGui::Text("");
	ImGui::Separator();
	if (ImGui::Button("Close")) {
		debugOptionsWindowStatus = false;
		ImGui::CloseCurrentPopup();
	}
	ImGui::End();
	}
}


/**
 * @brief Window descriptor for the graphic settings window.
*/
void GUI::runningStatisticsWindow() {
	if (ImGui::Begin("Current Running statistics:", &runningStatisticsWindowStatus, NULL)) {

		ImGui::Text("Current Running statistics:");
		ImGui::Separator();
		ImGui::Text("");

		fpsGraph();
		ImGui::Separator();

		ImGui::Text("");
		ImGui::Text("The current save:");
		ImGui::Text(windowRender->getMapSaver()->getLastSave().c_str());

		ImGui::Text("");
		ImGui::Text("The last save time:");
		ImGui::Text(windowRender->getMapSaver()->getLastSaveTime().c_str());

		ImGui::Text("");
		ImGui::Text("The current system time:");
		ImGui::Text(Logger::currentDateTime().c_str());

		ImGui::Text("");
		ImGui::Text("The objects count on the scene:");
		ImGui::SameLine();
		ImGui::Text(std::to_string(windowRender->getObjectsNumber()).c_str());
		
		ImGui::Text("");
		ImGui::Text("The roads count on the scene:");
		ImGui::SameLine();
		ImGui::Text(std::to_string(windowRender->getDynamicObjectsNumber()).c_str());

		ImGui::Text("");
		ImGui::Text("The vehicles count on the scene:");
		ImGui::SameLine();
		ImGui::Text(std::to_string(windowRender->getVehiclesNumber()).c_str());

		ImGui::Text("");
		ImGui::Separator();
		if (ImGui::Button("Close")) {
			runningStatisticsWindowStatus = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::End();
	}
}

/**
 * @brief Pathfinder test window.
*/
void GUI::pathFinderTestWindow() {
	//pathFinderTestWindowFlag |= ImGuiWindowFlags_NoMove;
	pathFinderTestWindowFlag |= ImGuiWindowFlags_NoCollapse;
	if (ImGui::Begin("Pathfinder algorithm.", &pathFinderTestWindowStatus, pathFinderTestWindowFlag)) {
		static ImGuiTextBuffer logger;
		std::stringstream log;


		std::vector<size_t> startPoints = animator->getGraph()->getStartPoints();
		static int selectedStartPoint = 0;
		std::string startpoints = "";
		for (size_t i = 0; i < startPoints.size(); i++) {
			startpoints = startpoints + std::to_string(startPoints[i]) + '\0';
		}
		startpoints = startpoints + '\0' + '\0';
		ImGui::PushItemWidth(50);
		ImGui::Combo("Startpoint", &selectedStartPoint, startpoints.c_str());
		ImGui::SameLine();

		std::vector<size_t> endPoints = animator->getGraph()->getEndPoints();
		static int selectedEndPoint = 0;
		std::string endpoints = "";
		for (size_t i = 0; i < endPoints.size(); i++) {
			endpoints = endpoints + std::to_string(endPoints[i]) + '\0';
		}
		endpoints = endpoints + '\0' + '\0';
		ImGui::PushItemWidth(50);
		ImGui::Combo("Endpoint", &selectedEndPoint, endpoints.c_str());
		ImGui::SameLine();
		//std::cout << selectedStartPoint  << "   " << selectedEndPoint << std::endl;

		if (ImGui::Button("RUN Pathfinder algorithm.")) {
			logger.clear();
			//Logging roads
			log << "Roads:" << std::endl << "-------------------------------------------------------------------------" << std::endl;
			for (size_t i = 0; i < windowRender->getDynamicObjectsNumber(); i++) {
				if (windowRender->getDynamicObject(i) != NULL) {
					log << "RoadID: " << windowRender->getDynamicObject(i)->getRenderID();
					log << " Stick A: ";
					if (windowRender->getDynamicObject(i)->stickA != -1) {
						log << windowRender->getDynamicObject(i)->stickA;
					}
					else {
						log << -1;
					}
					log << " StickMark A: " << windowRender->getDynamicObject(i)->stickMarkA;
					log << " Stick B: ";
					if (windowRender->getDynamicObject(i)->stickB != -1) {
						log << windowRender->getDynamicObject(i)->stickB;
					}
					else {
						log << -1;
					}
					log << " StickMark B: " << windowRender->getDynamicObject(i)->stickMarkB;
					log << " Marker A: ";
					if (windowRender->getDynamicObject(i)->stickB != -1) {
						log << windowRender->getDynamicObject(i)->stickB;
					}
					else {
						log << -1;
					}
					log << " Marker A: ";
					if (windowRender->getDynamicObject(i)->markerA != -1) {
						log << windowRender->getDynamicObject(i)->markerA;
					}
					else {
						log << -1;
					}
					log << " Marker B: ";
					if (windowRender->getDynamicObject(i)->markerB != -1) {
						log << windowRender->getDynamicObject(i)->markerB;;
					}
					else {
						log << -1;
					}
					log << std::endl;
				}
			}
			log << std::endl;

			//Clean colors
			for (size_t i = 0; i < windowRender->getDynamicObjectsNumber(); i++) {
				if (windowRender->getDynamicObject(i) != NULL) {
					windowRender->getDynamicObject(i)->setRGBcolor(glm::vec3(1, 1, 1));
				}
			}

			//Generate graph.
			Graph* graph = animator->getGraph();

			size_t startPoint = startPoints[selectedStartPoint];
			size_t endPoint = endPoints[selectedEndPoint];

			/*/Get start point from the graph.
			std::vector<size_t> startPoints = graph->getStartPoints();
			size_t startPoint = 0;
			if (startPoints.size() != 0) startPoint = startPoints[0];*/

			//Run Dijkstra.
			Dijkstra* dijkstra = graph->generateDijkstra(startPoint);

			/*/Get endpoint
			std::vector<size_t> endpoints = graph->getEndPoints();
			size_t endpoint = windowRender->getDynamicObjectsNumber();
			if(endpoints.size() != 0) endpoint = endpoints[0];*/

			//Get path
			path = graph->getPath(dijkstra, endPoint);
			coloringCounter = 0;

			//Graph edges logging.
			log << "Edges:" << std::endl << "-------------------------------------------------------------------------" << std::endl;
			for (size_t i = 0; i < graph->getEdgesNumber(); i++) {
				if(graph->getEdge(i)->getID() != -1) log << "Edge ID: " << graph->getEdge(i)->getID() << " Coast: " << graph->getEdge(i)->getCoast() << " Endpoint A: " << graph->getEdge(i)->getEndpointA() << " Endpoint B: " << graph->getEdge(i)->getEndpointB() << std::endl;
			}
			log << std::endl;

			//Gaph points log.
			log << "Points:" << std::endl << "-------------------------------------------------------------------------" << std::endl;
			for (size_t i = 0; i < graph->getPointsNumber(); i++) {
				if (!graph->getPoint(i)->isErased()) {
					log << "Point ID: " << graph->getPoint(i)->getID() << " Edges count: " << graph->getPoint(i)->getEdges().size() << " Connected edges: ";
					for (size_t edge : graph->getPoint(i)->getEdges()) {
						log << " - " << edge;
					}
					if (graph->getPoint(i)->isStartPoint()) log << " START POINT ";
					if (graph->getPoint(i)->isEndPoint()) log << " END POINT ";
					log << std::endl;
				}
			}
			log << std::endl;

			//Log start and endpoints
			log << "Start and Endpoints:" << std::endl << "-------------------------------------------------------------------------" << std::endl;
			log << "Start: " << startPoint << " Target: " << endPoint << std::endl;
			log << std::endl << std::endl;

			log << "Adjacency matrix:" << std::endl << "-------------------------------------------------------------------------" << std::endl;
			//Log Adjacency matrix
			for (size_t i = 0; i < dijkstra->matrix.size(); i++) {
				if (i == 0) {
					log << "     ";
					for (size_t j = 0; j < dijkstra->matrix[i].size(); j++) {
						log << j << "  ";
					}
					log << std::endl << "  +  ";
					for (size_t j = 0; j < dijkstra->matrix[i].size(); j++) {
						log << "---";
					}
					log << std::endl;
				}
				log << i << " | ";
				for (size_t j = 0; j < dijkstra->matrix[i].size(); j++) {
					log << dijkstra->matrix[i][j] << " ";
				}
				log << std::endl;
			}
			log << std::endl;

			//Log dijkstra result
			log << "Calculated path array:" << std::endl << "-------------------------------------------------------------------------" << std::endl;
			for (size_t i = 0; i < dijkstra->from.size(); i++) {
				log << i << " ";
			}
			log << std::endl;
			for (size_t i = 0; i < dijkstra->from.size(); i++) {
				log << dijkstra->from[i] << " ";
			}
			log << std::endl << std::endl;

			log << "Physical path:" << std::endl << "-------------------------------------------------------------------------" << std::endl;
			//Log path
			for (size_t i = 0; i < path.size(); i++) {
				log << path[i] << " ";
			}
			log << std::endl;

			logger.append(log.str().c_str());
			delete dijkstra;
		}
		ImGui::SameLine();
		if (ImGui::Button("Full path")) {
			for (size_t i = 0; i < path.size(); i++) {
				if (windowRender->getDynamicObject(path[i]) != NULL) {
					windowRender->getDynamicObject(path[i])->setRGBcolor(glm::vec3(0.929f, 0.109f, 0.141f));
				}
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Next step -->")) {
			if (windowRender->getDynamicObject(path[coloringCounter]) != NULL) {
				windowRender->getDynamicObject(path[coloringCounter])->setRGBcolor(glm::vec3(0.929f, 0.109f, 0.141f));
			}
			if (coloringCounter < path.size()) coloringCounter++;
		}

		ImGui::SameLine();
		if (ImGui::Button("Clear")) {
			for (size_t i = 0; i < windowRender->getDynamicObjectsNumber(); i++) {
				if (windowRender->getDynamicObject(i) != NULL) {
					windowRender->getDynamicObject(i)->setRGBcolor(glm::vec3(1, 1, 1));
				}
			}
			coloringCounter = path.size() - 1;
		}

		ImGui::SameLine();
		if (ImGui::Button("Close")) {
			pathFinderTestWindowStatus = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::Separator();

		ImGui::BeginChild("Log");
		ImGui::TextUnformatted(logger.begin(), logger.end());
		ImGui::EndChild();

		ImGui::End();
	}
}

/**
 * @brief The window for the simulation settings.
*/
void GUI::simulationSettingsWindow() {

	if (ImGui::Begin("Simulation Settings.", &simulationSettingsWindowStatus, NULL)) {
		ImGui::Text("Simulation Settings:");
		ImGui::Separator();
		ImGui::Text("");
		ImGui::Checkbox("Smart pathfinder", &Vehicle::repath);
		ImGui::SliderInt("Vehicle weight", &Vehicle::vehicleWeight, 0, 500);
		ImGui::SliderInt("Animation frequency", &Animator::updateFrequency, 0, 80);
		ImGui::SliderInt("Start frequency", &Animator::vehicleStarterUpdateFrequency, 100, 3000);

		ImGui::Text("");
		ImGui::Separator();
		if (ImGui::Button("Close")) {
			simulationSettingsWindowStatus = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::End();
	}

}