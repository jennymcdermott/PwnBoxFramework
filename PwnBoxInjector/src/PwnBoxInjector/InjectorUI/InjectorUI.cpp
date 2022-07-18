#include "InjectorUI.h"
#include <imgui.h>

// ImGuiWindowFlags_AlwaysAutoResize
namespace PwnBoxFramework
{
	void InjectorGUI::OnImGuiRender()
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		static bool open = true;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{

			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		}
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("PwnBoxGui Dockspace", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("PwnBoxGuiDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		ImGui::Begin("PwnBoxInjector GUI", &open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
		renderInjectorWindow();
		ImGui::End();
		ImGui::End();
	}

	void InjectorGUI::renderInjectorWindow()
	{
		ImGui::Text("Select your target process");
		ImGui::SameLine();
		ImGui::PushID(0);
		static const char* selectedItem = injector->GetProcessList()[1].c_str();
		if (ImGui::BeginCombo("##combo", selectedItem,ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_PopupAlignLeft))
		{
			for (int i = 1; i < injector->GetProcessList().size(); i++)
			{
				bool isSelected = (selectedItem == injector->GetProcessList()[i].c_str());
				if (ImGui::Selectable(injector->GetProcessList()[i].c_str(), isSelected))
					selectedItem = injector->GetProcessList()[i].c_str();

				if (isSelected)
					ImGui::SetItemDefaultFocus();
				m_Data.targetProc.assign(selectedItem);
			}
			ImGui::EndCombo();	
		}
		ImGui::PopID();
		ImGui::NewLine();
		ImGui::PushID(1);
		ImGui::PushItemWidth(70.0f);
		if (ImGui::Button("Refresh", ImVec2(70, 20)))
			injector->RefreshProcessList();
		ImGui::PopID();
		ImGui::NewLine();
		ImGui::Separator();
		static char Buf[256]{};
		ImGui::InputText("Input Target Dll: ", Buf, IM_ARRAYSIZE(Buf));
		m_Data.targetDll.assign(Buf);
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnOffset(1, 110.0f);
		ImGui::NextColumn();
		ImGui::PushID("Target DLL: ");
		ImGui::Text("");
		ImGui::SameLine();
		ImGui::Text(m_Data.targetDll.c_str());
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::PushID("Target Process: ");
		ImGui::Text("");
		ImGui::SameLine();
		ImGui::Text(m_Data.targetProc.c_str());
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::PushItemWidth(90.0f);
		if (ImGui::Button("Inject Dll", ImVec2(90, 20)))
		{
			injector->InjectDll(m_Data.targetDll, m_Data.targetProc);
		}
		ImGui::Columns(1);
	}
}