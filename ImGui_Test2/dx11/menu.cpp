#include <menu.h>

namespace DX11
{
	bool Menu::bIsOpen = true;

	static void HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	void Menu::Render()
	{
		static bool no_titlebar = false;
		static bool no_scrollbar = false;
		static bool no_menu = true;
		static bool no_move = false;
		static bool no_resize = false;
		static bool no_collapse = false;
		static bool no_nav = false;
		static bool no_background = false;

		ImGuiWindowFlags window_flags = 0;
		if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
		if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
		if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
		if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
		if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
		if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
		if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
		if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowSize(ImVec2(450, 300), ImGuiCond_FirstUseEver);

		static std::once_flag start_pos_set;
		std::call_once(start_pos_set, [&]() { ImGui::SetNextWindowPos(ImVec2(25, 25)); });

		if (Menu::bIsOpen) 
		{
			ImGuiIO& io = ImGui::GetIO();
			//ImGui::GetStyle().WindowPadding = ImVec2(5, 5);

			ImGui::Begin("Main Menu", &Menu::bIsOpen, window_flags);

			if (ImGui::CollapsingHeader("Sample Menu A"))
			{
				ImGui::Checkbox("Block mouse input", &InputHook::bBlockInput); ImGui::SameLine();
				HelpMarker("Blocks the mouse input from interacting with the game if this is checked");
				ImGui::Checkbox("Drag only with titlebar", &io.ConfigWindowsMoveFromTitleBarOnly);

				static int clicked = 0;
				if (ImGui::Button("Button"))
					clicked++;
				if (clicked & 1)
				{
					ImGui::SameLine();
					ImGui::Text("Thanks for clicking me!");
				}
			}

			if (ImGui::CollapsingHeader("Sample Menu B"))
			{
				ImGui::Columns(2);
				ImGui::Checkbox("No titlebar", &no_titlebar); ImGui::NextColumn();
				ImGui::Checkbox("No scrollbar", &no_scrollbar); ImGui::NextColumn();
				ImGui::Checkbox("No menu", &no_menu); ImGui::NextColumn();
				ImGui::Checkbox("No move", &no_move); ImGui::NextColumn();
				ImGui::Checkbox("No resize", &no_resize); ImGui::NextColumn();
				ImGui::Checkbox("No collapse", &no_collapse); ImGui::NextColumn();
				ImGui::Checkbox("No nav", &no_nav); ImGui::NextColumn();
				ImGui::Checkbox("No background", &no_background);
				ImGui::Columns(1);
				ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
			}

			if (ImGui::CollapsingHeader("Sample Menu C"))
			{
				ImGui::Text("Blah blah blah...");
			}

			if (ImGui::CollapsingHeader("Sample Menu D"))
			{
				ImGui::Text("Blah blah blah...");
			}

			if (ImGui::CollapsingHeader("Sample Menu E"))
			{
				ImGui::Text("Blah blah blah...");
			}
		}

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}