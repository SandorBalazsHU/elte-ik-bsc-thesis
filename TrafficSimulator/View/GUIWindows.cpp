/**
 * @name Traffic Simulation
 * @file GUIWindows.cpp
 * @class GUI
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief GUI Descriptor class.
 * Contact: sandorbalazs9402@gmail.com
*/

// KSP

#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <math.h>
#include "GUI.h"
#include "fpsCounter.h"
#include "Render.h"
#include "WorkWindow.h"
#include "../Control/Logger.h"
#include "../Control/ConfigSaver.h"
#include "../Model/Graph.h"
#include "../Model/Vehicle.h"
#include "Animator.h"

/**
 * @brief Description of the opening window.
*/
void GUI::openWindow() {
	ImGui::SetNextWindowSize(ImVec2(480, 360));
	ImGui::OpenPopup("Open Map");
	if (ImGui::BeginPopupModal("Open Map"))
	{
		ImGui::Text("Saved maps:");

		ImGui::Separator();

		std::vector<std::string> fileList = windowRender->getMapLoader()->listFiles();
		size_t itemNumber = fileList.size();

		const char* items[500];

		if(itemNumber >= 500) itemNumber = 500;

		for (size_t i = 0; i < itemNumber; i++) {
			items[i] = fileList[i].c_str();
		}

		static int selection = 0;
		ImGui::PushItemWidth(-1);
		ImGui::PushID(0);
		ImGui::ListBox("", &selection, items, (int) itemNumber, 15);
		ImGui::PopID();


		if (ImGui::Button("Open")) {
			if (windowRender->getObjectsNumber() > 1 || windowRender->getDynamicObjectsNumber() > 0) {
				ImGui::OpenPopup("Are you sure?");
			} else {
				closeAllWindows("openWindowStatus");
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
				closeAllWindows("openWindowStatus");
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

/**
 * @brief the save window descriptor.
*/
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

/**
 * @brief the save as window descriptor.
*/
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

/**
 * @brief Window for the new map confirmation.
*/
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
				closeAllWindows("newMapConfirmWindowStatus");
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

/**
 * @brief Window for the controls tutorial.
*/
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

/**
 * @brief Window for the about tab.
*/
void GUI::aboutWindow() {
	ImGui::OpenPopup("About");
	if (ImGui::BeginPopupModal("About", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("");
		ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "                  Traffic Simulation.");
		ImGui::Text("");
		ImGui::Separator();
		ImGui::Text("");
		ImGui::Text("Created By: Sandor Balazs");
		ImGui::Text("            AZA6NL");
		ImGui::Text("            sandorbalazs9402@gmail.com");
		ImGui::Text("");
		ImGui::Separator();
		ImGui::Text("");
		ImGui::Text("Github:");
		ImGui::Text("https://github.com/SandorBalazsHU/elte-ik-bsc-thesis");
		ImGui::Text("");
		ImGui::Separator();
		ImGui::Text("");
		ImGui::Text("Thesis for Computer Science BSC in ELTE IK Budapest.");
		ImGui::Text("");
		ImGui::Text("          "); ImGui::SameLine();
		ImGui::Image((void*)(intptr_t)objectStorage->getTexture("elte.png"), ImVec2(200, 200));
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
		std::stringstream log;

		const GLubyte* vendor = glGetString(GL_VENDOR);
		log << "GPU Vendor: " << vendor << std::endl;
		const GLubyte* renderer = glGetString(GL_RENDERER);
		log << "GPU Model:  " << renderer << std::endl;
		log << "Current displays: " << std::endl;
		SDL_DisplayMode current;
		// Get current display mode of all displays.
		for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i) {
			int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);
			if (should_be_zero == 0) log << i << ". Display " << current.w << "x" << current.h << "dpx @ " << current.refresh_rate << "hz" << std::endl;
		}
		ImGui::Separator();

		ImGui::Text("");
		ImGui::Text("Hardware info:");
		ImGui::Text(log.str().c_str());

		ImGui::Text("");
		ImGui::Text("Graphic settings:");
		ImGui::Separator();
		ImGui::Text("");

		ImGui::Text("Window mode:"); ImGui::SameLine();
		static bool setWindowModeNow = false;
		ImGui::Checkbox("Set it now", &setWindowModeNow);
		if (!setWindowModeNow) {
			ConfigSaver configSaver;
			static bool configurationSaved = false;
			if (ImGui::Button("Windowed")) {
				configSaver.setProperty("displayMode", "windowed");
				configurationSaved = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Borderless")) {
				configSaver.setProperty("displayMode", "borderless");
				configurationSaved = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Fullscreen")) {
				configSaver.setProperty("displayMode", "fullscreen");
				configurationSaved = true;
			}
			if (configurationSaved) ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Configuration saved! \nYou must restart the application.");
		}

		if (setWindowModeNow) {
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "WARNING! \nImmediately window mode change \nwithout restart may be dangerous.");
			if (ImGui::Button("Windowed")) windowRender->setToWindowed(); ImGui::SameLine();
			if (ImGui::Button("Borderless")) windowRender->setToBorderless(); ImGui::SameLine();
			if (ImGui::Button("Fullscreen")) windowRender->setToFullScreen();
		}
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
		fpsCounter::setFpsLimit((float) fpsLimit);

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
	pathFinderTestWindowFlag |= ImGuiWindowFlags_NoCollapse;
	if (ImGui::Begin("Pathfinder algorithm.", &pathFinderTestWindowStatus, pathFinderTestWindowFlag)) {
		static bool pathfinderIsValidNow = false;
		static ImGuiTextBuffer logger;
		std::stringstream log;

		if (pathFinderOpenTimeStamp == SDL_GetTicks()) {
			logger.clear();
			clearRoadColor();
			coloringCounter = path.size() - 1;
			pathfinderIsValidNow = false;
		}

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
			clearRoadColor();

			//Generate graph.
			Graph* graph = animator->getGraph();

			//Get start and endpoints
			size_t startPoint = startPoints[selectedStartPoint];
			size_t endPoint = endPoints[selectedEndPoint];

			//Run Dijkstra.
			Dijkstra* dijkstra = graph->generateDijkstra(startPoint);

			//Get path
			path = graph->getPath(dijkstra, endPoint);
			coloringCounter = 0;

			//Graph edges logging.
			log << "Edges:" << std::endl << "-------------------------------------------------------------------------" << std::endl;
			for (size_t i = 0; i < graph->getEdgesNumber(); i++) {
				if(graph->getEdge(i)->getID() != -1) log << "Edge ID: " << graph->getEdge(i)->getID() << " Coast: " << graph->getEdge(i)->getCoast() << " Endpoint A: " << graph->getEdge(i)->getEndpointA() << " Endpoint B: " << graph->getEdge(i)->getEndpointB() << std::endl;
			}
			log << std::endl;

			//Graph points log.
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

			//Log Dijkstra result
			log << "Calculated path array:" << std::endl << "-------------------------------------------------------------------------" << std::endl;
			for (size_t i = 0; i < dijkstra->from.size(); i++) {
				log << i << " ";
			}
			log << std::endl;
			for (size_t i = 0; i < dijkstra->from.size(); i++) {
				if (dijkstra->from[i] != -1) {
					log << dijkstra->from[i] << " ";
				} else {
					log << "x" << " ";
				}
			}
			log << std::endl << std::endl;

			log << "Physical path:" << std::endl << "-------------------------------------------------------------------------" << std::endl;
			//Log path
			for (size_t i = 0; i < path.size(); i++) {
				log << path[i] << " ";
			}
			if(path.size() == 0) log << "NO PATH!";
			log << std::endl;

			logger.append(log.str().c_str());
			delete dijkstra;
			pathfinderIsValidNow = true;
		}

		if (pathfinderIsValidNow) {
			ImGui::SameLine();
			if (ImGui::Button("Full path")) {
				for (size_t i = 0; i < path.size(); i++) {
					if (windowRender->getDynamicObject(path[i]) != NULL) {
						windowRender->getDynamicObject(path[i])->setRGBcolor(glm::vec3(0.929f, 0.109f, 0.141f));
					}
				}
				pathfinderIsValidNow = false;
			}

			ImGui::SameLine();
			if (ImGui::Button("Next step -->")) {
				if (coloringCounter < path.size()) {
					if (windowRender->getDynamicObject(path[coloringCounter]) != NULL) {
						windowRender->getDynamicObject(path[coloringCounter])->setRGBcolor(glm::vec3(0.929f, 0.109f, 0.141f));
					}
					coloringCounter++;
					if(coloringCounter >= path.size()) pathfinderIsValidNow = false;
				} else {
					pathfinderIsValidNow = false;
				}
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Clear")) {
			logger.clear();
			clearRoadColor();
			coloringCounter = path.size() - 1;
			pathfinderIsValidNow = false;
		}

		ImGui::SameLine();
		if (ImGui::Button("Close")) {
			logger.clear();
			coloringCounter = path.size() - 1;
			pathfinderIsValidNow = false;
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
		ImGui::Text("");
		ImGui::Checkbox("Vehicle collision prevention", &Vehicle::collisionPrevention);
		ImGui::Text("");
		ImGui::SliderInt("Vehicle distance", &Vehicle::collisionCheckDistance, 0, 50);
		ImGui::Text("");
		ImGui::Separator();
		ImGui::Text("");
		ImGui::SliderInt("Vehicle weight", &Vehicle::vehicleWeight, 0, 500);
		ImGui::Text("");
		ImGui::SliderInt("Animation frequency", &Animator::updateFrequency, 0, 80);
		ImGui::Text("");
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

/**
 * @brief Window for SimulationStatistics.
*/
void GUI::simulationStatisticsWindow() {
	if (ImGui::Begin("Simulation Statistics", NULL, NULL)) {
		ImGui::Text("Simulation Statistics");
		ImGui::Separator();
		ImGui::Text("");

		//All cost plot.
		std::map<int, std::vector<int>> allCost;
		std::vector<size_t> endPoints = animator->getGraph()->getEndPoints();
		for (size_t i = 0; i < endPoints.size(); i++) {
			allCost.insert(std::pair<int, std::vector<int>>(endPoints[i], std::vector<int>()));
		}

		for (size_t i = 0; i < animator->getVehicleModelSize(); i++) {
			if(animator->getVehicleModel(i)->isFinished())
			allCost[animator->getVehicleModel(i)->destinationID].push_back(animator->getVehicleModel(i)->allCost);
		}

		std::vector<float> allCostStat;
		for (const auto& [destinationID, costs] : allCost) {
			if (costs.size() > 0) {
				float averageCost = 0;
				for (size_t i = 0; i < costs.size(); i++) {
					averageCost += costs[i];
				}
				allCostStat.push_back(averageCost / costs.size());
			}
		}

		static int graphTop = 1000;
		ImGui::PlotHistogram("AVG Cost", &allCostStat[0], (int)allCostStat.size(), 0, "Average cost", 0.0f, graphTop, ImVec2(0, 80));
		ImGui::SliderInt("AVG Cost", &graphTop, 0, 1000);
		ImGui::InputInt("AVG Cost ", &graphTop);
		
		ImGui::Text("");
		for (size_t i = 0; i < allCostStat.size(); i++) {
			ImGui::Text("%i#, rec.: %i, avg coast: %f", endPoints[i], this->animator->getGraph()->getPointByID(endPoints[i])->receivedVehicles, allCostStat[i]);
		}
		ImGui::Text("");

		//All hops plot.
		std::map<int, std::vector<int>> hops;
		for (size_t i = 0; i < endPoints.size(); i++) {
			hops.insert(std::pair<int, std::vector<int>>(endPoints[i], std::vector<int>()));
		}

		for (size_t i = 0; i < animator->getVehicleModelSize(); i++) {
			if (animator->getVehicleModel(i)->isFinished())
				hops[animator->getVehicleModel(i)->destinationID].push_back(animator->getVehicleModel(i)->hopCounter);
		}

		std::vector<float> hopsStat;
		for (const auto& [destinationID, allHop] : hops) {
			if (allHop.size() > 0) {
				float averageHops = 0;
				for (size_t i = 0; i < allHop.size(); i++) {
					averageHops += allHop[i];
				}
				hopsStat.push_back(averageHops / allHop.size());
			}
		}

		static int graphTop2 = 10;
		ImGui::PlotHistogram("AVG hop", &hopsStat[0], (int)hopsStat.size(), 0, "Average hop", 0.0f, graphTop2, ImVec2(0, 80));
		ImGui::SliderInt("AVG hop", &graphTop2, 0, 1000);
		ImGui::InputInt("AVG hop ", &graphTop2);

		ImGui::Text("");
		for (size_t i = 0; i < hopsStat.size(); i++) {
			ImGui::Text("%i#, rec.: %i, avg hop: %f", endPoints[i], this->animator->getGraph()->getPointByID(endPoints[i])->receivedVehicles, hopsStat[i]);
		}
		ImGui::Text("");

		ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Road coloring mode:");
		ImGui::Separator();
		static int roadColoringMode = 0;
		static bool roadColoringRun;
		ImGui::Text("");
		if (!roadColoringRun) {
			ImGui::RadioButton("By current vehicle count.", &roadColoringMode, 0);
			ImGui::RadioButton("By all vehicle count.", &roadColoringMode, 1);
			ImGui::RadioButton("By road coast.", &roadColoringMode, 2);
			ImGui::Text("");
			if (ImGui::Button("Run road coloring.")) {
				roadColoringRun = true;
				this->clearRoadColor();
			}
		} else {
			switch (roadColoringMode) {
			case 0:
				ImGui::Text("Road coloring by current vehicle count:");
				break;
			case 1:
				ImGui::Text("Road coloring by all vehicle count:");
				break;
			case 2:
				ImGui::Text("Road coloring by current road coast:");
				break;
			}
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "RUNNUNG ...");
			ImGui::Text("");
			if (ImGui::Button("Stop road coloring.")) {
				roadColoringRun = false;
				this->clearRoadColor();
			}
		}

		if (roadColoringMode == 0 && roadColoringRun) {
			for (size_t i = 0; i < windowRender->getDynamicObjectsNumber(); i++) {
				if (windowRender->getDynamicObject(i) != NULL) {
					float vehicleCount = (float)animator->getGraph()->getEdgeByID(windowRender->getDynamicObject(i)->modelID)->getVehicleCount();
					if (vehicleCount > 0) {
						windowRender->getDynamicObject(i)->setRGBcolor(glm::vec3(1.0f / std::sqrt(vehicleCount), 0.15f, 0.15f));
					} else {
						windowRender->getDynamicObject(i)->setRGBcolor(glm::vec3(1, 1, 1));
					}

				}
			}
		}

		if (roadColoringMode == 1 && roadColoringRun) {
			for (size_t i = 0; i < windowRender->getDynamicObjectsNumber(); i++) {
				if (windowRender->getDynamicObject(i) != NULL) {
					float allVehicleCount = (float)animator->getGraph()->getEdgeByID(windowRender->getDynamicObject(i)->modelID)->getAllVehicleCount();
					if (allVehicleCount > 0) {
					windowRender->getDynamicObject(i)->setRGBcolor(glm::vec3(0.15f, 1.0f / std::sqrt(allVehicleCount), 0.15f));
					} else {
						windowRender->getDynamicObject(i)->setRGBcolor(glm::vec3(1, 1, 1));
					}
				}
			}
		}

		if (roadColoringMode == 2 && roadColoringRun) {
			for (size_t i = 0; i < windowRender->getDynamicObjectsNumber(); i++) {
				if (windowRender->getDynamicObject(i) != NULL) {
					float roadCoast = (float)animator->getGraph()->getEdgeByID(windowRender->getDynamicObject(i)->modelID)->getCoast();
					if (roadCoast > 0) {
						windowRender->getDynamicObject(i)->setRGBcolor(glm::vec3(0.15f, 0.15f, 1.0f / std::sqrt(roadCoast/100.0f)));
					}
					else {
						windowRender->getDynamicObject(i)->setRGBcolor(glm::vec3(1, 1, 1));
					}
				}
			}
		}

		ImGui::Text("");
		ImGui::Text("Other simulation statistics:");
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
			simulationStatisticsWindowStatus = false;
			ImGui::CloseCurrentPopup();
			this->clearRoadColor();
		}
		ImGui::End();
	}
}

/**
 * @brief Finalize the map.
*/
void GUI::finalise() {
	animator->finalize();
	if (animator->getGraph()->getEndPoints().size() > 0 && animator->getGraph()->getStartPoints().size() > 0) {
		editorLock = true;
		windowRender->lockEditor();
		mapEditorWindow = false;
		simulationWindow = true;
		animator->finalize();
	} else {
		finalisingErrorWindow = true;
	}

	for (auto const& startPoint : animator->separatedEndPoints) {
		if (startPoint.second.size() != 0) {
			finaliseWarningWindowStatus = true;
			break;
		}
	}
}

/**
 * @brief Show the finalizing warning window.
*/
void GUI::finaliseWarningWindow() {
	if (ImGui::Begin("WARNING! Unreachable endpoints!", &finaliseWarningWindowStatus, NULL)) {
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "WARNING! \nThe map contains unreachable endpoints!");
		ImGui::Separator();
		ImGui::Text("");
		for (auto const& startPoint : animator->separatedEndPoints) {
			for (auto x : startPoint.second) {
				ImGui::Text("The endpoint %i is inaccessible from startpoint %i.", x, startPoint.first);
			}
		}
		ImGui::Text("");
		ImGui::Separator();
		if (ImGui::Button("Close")) {
			finaliseWarningWindowStatus = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::End();
	}
}

/**
 * @brief Back to the edit mode
*/
void GUI::backToEditMode() {
	editorLock = false;
	windowRender->freeEditor();
	mapEditorWindow = true;
	simulationWindow = false;
	selectedStartPoint = -1;
	selectedEndPoint = -1;
	selectedRoad = -1;
	selectedVehicle = -1;
}

/**
 * @brief Start simulation
*/
void GUI::startSimulation() {
	animator->start();
	simulationStart = true;
	simulationPaused = false;
}

/**
 * @brief Stop running simulation.
*/
void GUI::stopSimulation() {
	animator->stop();
	simulationStart = false;
}

/**
 * @brief Pause running simulation.
*/
void GUI::pauseSimulation() {
	animator->pause();
	simulationPaused = true;
}

/**
 * @brief Clear the road colors.
*/
void GUI::clearRoadColor() {
	for (size_t i = 0; i < windowRender->getDynamicObjectsNumber(); i++) {
		if (windowRender->getDynamicObject(i) != NULL) {
			windowRender->getDynamicObject(i)->setRGBcolor(glm::vec3(1, 1, 1));
		}
	}
}

/**
 * @brief Show help window.
*/
void GUI::helpWindow() {
	if (ImGui::Begin("Traffic Simulator Help", &helpWindowStatus, NULL)) {
		ImGui::Text("Traffic Simulator Help");
		ImGui::Separator();
		ImGui::Text("");
		ImGui::Text("To the full documentation see: \nhttps://github.com/SandorBalazsHU/elte-ik-bsc-thesis/tree/master/thesis");
		ImGui::Text("");
		ImGui::Text("A common usecase:");
		ImGui::Text("");
		ImGui::Text("1.) Open a map with File/open option, or edit a map:");
		ImGui::Text("    - For the edit controlls see: Help/Controls");
		ImGui::Text("    - Add roads, move and connect the roads together.");
		ImGui::Text("    - Add minimum one start and endpoint and stick thees to the roads endpoints.");


		ImGui::Separator();
		if (ImGui::Button("Close")) {
			helpWindowStatus = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::End();
	}
}