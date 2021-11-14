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

#include "GUI.h"
#include "Render.h"
#include "../Control/Logger.h"

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
			windowRender->getMapLoader()->loadMap(items[selection]);
			windowRender->getMapSaver()->setLastSave(items[selection]);
			openWindowStatus = false;
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
	ImGui::OpenPopup("Save map");
	if (ImGui::BeginPopupModal("Save map")) {
		ImGui::Text("Saving the new map.");
		ImGui::Separator();
		if (ImGui::Button("Save")) {}
		ImGui::SameLine();
		if (ImGui::Button("Close")) {
			saveWindowStatus = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void GUI::saveAsWindow() {
	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(251, 130), ImGuiCond_FirstUseEver);
	ImGui::OpenPopup("Save as map");
	if (ImGui::BeginPopupModal("Save as map")) {

		ImGui::Text("Saving the new map.");

		ImGui::Separator();

		static char newFileName[64] = "save.csv";
		ImGui::InputText("New map name.", newFileName, 64);

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