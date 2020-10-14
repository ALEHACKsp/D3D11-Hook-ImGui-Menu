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
		static bool bOption1, bOption2, bOption3, bOption4, bOption5, bOption6;
		static bool bOption7, bOption8, bOption9, bOption10, bOption11, bOption12;

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowSize(ImVec2(450, 300), ImGuiCond_FirstUseEver);

		static std::once_flag start_pos_set;
		std::call_once(start_pos_set, [&]() { ImGui::SetNextWindowPos(ImVec2(25, 25)); });

		if (Menu::bIsOpen)
		{
			ImGuiIO& io = ImGui::GetIO();

			ImGui::Begin("Main Menu##1", &Menu::bIsOpen);

			if (ImGui::CollapsingHeader("Configuration##1", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::TreeNodeEx("Selections##1", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Checkbox("Block mouse input", &InputHook::bBlockInput);
					ImGui::SameLine(); HelpMarker("Blocks the mouse input from interacting with the game if this is checked");
					ImGui::Checkbox("Unhook input when hidden", &InputHook::bUnhookWhenHidden);
					ImGui::SameLine(); HelpMarker("Unhooking the input when the main menu is hidden will stop you from interacting with the overlay widget, avoiding accidental clicks");
					ImGui::TreePop();
					ImGui::Spacing();
				}
			}

			if (ImGui::CollapsingHeader("Settings##1"))
			{
				if (ImGui::TreeNodeEx("Selections##2", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Columns(2, NULL, false);
					ImGui::Checkbox("Option1", &bOption1); ImGui::NextColumn();
					ImGui::Checkbox("Option2", &bOption2); ImGui::NextColumn();
					ImGui::Checkbox("Option3", &bOption3); ImGui::NextColumn();
					ImGui::Checkbox("Option4", &bOption4); ImGui::SameLine(); 
					HelpMarker("Help marker for option 4"); ImGui::NextColumn();
					ImGui::Checkbox("Option5", &bOption5); ImGui::NextColumn();
					ImGui::Checkbox("Option6", &bOption6); ImGui::NextColumn();
					ImGui::Columns(1);
					ImGui::Separator();
					ImGui::Columns(2, NULL, false);
					ImGui::Checkbox("Option7", &bOption7); ImGui::NextColumn();
					ImGui::Checkbox("Option8", &bOption8); ImGui::NextColumn();
					ImGui::Checkbox("Option9", &bOption9); ImGui::NextColumn();
					ImGui::Checkbox("Option10", &bOption10); ImGui::NextColumn();
					ImGui::Checkbox("Option11", &bOption11); ImGui::NextColumn();
					ImGui::Checkbox("Option12", &bOption12); ImGui::SameLine();
					HelpMarker("Help marker for option 12"); ImGui::NextColumn();
					ImGui::NextColumn();
					ImGui::Columns(1);
					ImGui::TreePop();
				}
			}

		}

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}