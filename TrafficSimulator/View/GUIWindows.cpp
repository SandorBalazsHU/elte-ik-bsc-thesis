/**
 * @name Traffic Simulation
 * @file GUIWindows.cpp
 * @class GUI
 * @author Sándor Balázs - AZA6NL
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

void GUI::openWindow() {
	ImGui::SetNextWindowSize(ImVec2(400, 300));
	ImGui::OpenPopup("Open Map");
	if (ImGui::BeginPopupModal("Open Map", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Saved maps:");

		ImGui::Separator();

		std::vector<std::string> fileList = windowRender->getMapLoader()->listFiles();
		const int itemNumber = fileList.size();

		const char* items[50];

		for (size_t i = 0; i < itemNumber; i++) {
			items[i] = fileList[i].c_str();
		}

		static int selection = 0;
		ImGui::PushItemWidth(-1);
		ImGui::PushID(0);
		ImGui::ListBox("", &selection, items, itemNumber);
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
		if (ImGui::BeginPopupModal("Are you sure?")) {
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
			ImGui::EndPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("Delete")) {
			int status = windowRender->getMapLoader()->deleteSave(items[selection]);
			if (status == 0) ImGui::OpenPopup("Map deleted");
			if (status == 1) ImGui::OpenPopup("Map not found");
			if (status == 2) ImGui::OpenPopup("Map open file system error");
		}
		if (ImGui::BeginPopupModal("Map deleted")) {
			ImGui::Text("Map deleted.");
			if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		if (ImGui::BeginPopupModal("Map not found")) {
			ImGui::Text("Map not found.");
			if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		if (ImGui::BeginPopupModal("Map open file system error")) {
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
		ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiSetCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(251, 130), ImGuiCond_FirstUseEver);
		ImGui::OpenPopup("Map saved");
		if (ImGui::BeginPopupModal("Map saved")) {
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
	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(251, 130), ImGuiCond_FirstUseEver);
	ImGui::OpenPopup("Save map as");
	if (ImGui::BeginPopupModal("Save map as")) {

		ImGui::Text("Saving the new map.");

		ImGui::Separator();

		static char newFileName[64] = "save";
		ImGui::InputText("New map name.", newFileName, 64);

		if (ImGui::Button("Save")) {
			std::vector<std::string> saves = windowRender->getMapLoader()->listFiles();
			if (std::find(saves.begin(), saves.end(), newFileName) != saves.end()) {
				ImGui::OpenPopup("overwrite");
			} else {
				windowRender->getMapSaver()->saveMap(newFileName);
				ImGui::OpenPopup("Save map");
			}
		}
		if (ImGui::BeginPopupModal("overwrite")) {
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "This file already exist!");
			ImGui::Text("Do you want to overwrite: %s ?", newFileName);
			if (ImGui::Button("Save")) {
				windowRender->getMapSaver()->saveMap(newFileName);
				ImGui::OpenPopup("Save map");
			}
			if (ImGui::BeginPopupModal("Save map")) {
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
		if (ImGui::BeginPopupModal("Save map")) {
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
	if (ImGui::BeginPopupModal("New map")) {
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
		if (ImGui::BeginPopupModal("Are you sure?")) {
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
	if (ImGui::BeginPopupModal("Controls")) {
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
	if (ImGui::BeginPopupModal("About")) {
		ImGui::Text("Traffic Simulation");
		ImGui::Text("Created By: Sandor Balazs");
		ImGui::Text("AZA6NL");
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
	if (ImGui::BeginPopupModal("Are you sure you want to close?")) {
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
	if(ImGui::Begin("Graphic settings", NULL, graphicSettingsWindowStatus)) {

		ImGui::Text("Current running data:");
		ImGui::Separator();

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

		/*ImGui::Text("");
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "---");
		ImGui::Separator();
		ImGui::Text("---");
		ImGui::SameLine();*/

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
	if (ImGui::Begin("Special debug options.", NULL, debugOptionsWindowStatus)) {
	ImGui::Text("");
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
	if (ImGui::Begin("Current Running statistics:", NULL, runningStatisticsWindowStatus)) {

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
//&pathFinderTestWindowStatus
void GUI::pathFinderTestWindow() {
	pathFinderTestWindowFlag |= ImGuiWindowFlags_NoMove;
	//pathFinderTestWindowFlag |= ImGuiWindowFlags_NoResize;
	//pathFinderTestWindowFlag |= ImGuiWindowFlags_NoCollapse;
	if (ImGui::Begin("Pathfinder algorithm test.", &pathFinderTestWindowStatus, pathFinderTestWindowFlag)) {
		static ImGuiTextBuffer logger;
		std::stringstream log;

		if (ImGui::Button("Pathfinder algorithm test.")) {
			logger.clear();
			//Logging roads
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
			Graph graph(windowRender);
			graph.generateGraph();

			//Get start point from the graph.
			std::vector<size_t> startPoints = graph.getStartPoints();
			size_t startPoint = 0;
			if (startPoints.size() != 0) startPoint = startPoints[0];

			//Run Dijkstra.
			Dijkstra* dijkstra = graph.generateDijkstra(startPoint);

			//Get endpoint
			std::vector<size_t> endpoints = graph.getEndPoints();
			size_t endpoint = windowRender->getDynamicObjectsNumber();
			if(endpoints.size() != 0) endpoint = endpoints[0];

			//Get path
			std::vector<size_t> path = graph.getPath(dijkstra, endpoint);

			//Path coloring
			for (size_t i = 0; i < path.size(); i++) {
				if (windowRender->getDynamicObject(path[i]) != NULL) {
					windowRender->getDynamicObject(path[i])->setRGBcolor(glm::vec3(0.929f, 0.109f, 0.141f));
				}
			}

			//Graph edges logging.
			for (size_t i = 0; i < graph.getEdgesNumber(); i++) {
				if(graph.getEdge(i)->getID() != -1) log << "Edge ID: " << graph.getEdge(i)->getID() << " Coast: " << graph.getEdge(i)->getCoast() << " Endpoint A: " << graph.getEdge(i)->getEndpointA() << " Endpoint B: " << graph.getEdge(i)->getEndpointB() << std::endl;
			}
			log << std::endl;

			//Gaph points log.
			for (size_t i = 0; i < graph.getPointsNumber(); i++) {
				if (!graph.getPoint(i)->isErased()) {
					log << "Point ID: " << graph.getPoint(i)->getID() << " Edges count: " << graph.getPoint(i)->getEdges().size() << " Connected edges: ";
					for (size_t edge : graph.getPoint(i)->getEdges()) {
						log << " - " << edge;
					}
					if (graph.getPoint(i)->isStartPoint()) log << " START POINT ";
					if (graph.getPoint(i)->isEndPoint()) log << " END POINT ";
					log << std::endl;
				}
			}
			log << std::endl;

			//Log start and endpoints
			log << "Start: " << startPoint << " Target: " << endpoint << std::endl;
			log << std::endl;

			//Log dijkstra result
			for (size_t i = 0; i < dijkstra->from.size(); i++) {
				log << i << " ";
			}
			log << std::endl;
			for (size_t i = 0; i < dijkstra->from.size(); i++) {
				log << dijkstra->from[i] << " ";
			}
			log << std::endl;

			logger.append(log.str().c_str());
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