#include "gui.h"

namespace gui
{
	bool playing = false;
	void ShowMacroMenu(bool p_open) {
		ImGui::Begin("MacroMenu", &p_open, default_window_flags);
		ImGui::GetWindowViewport()->Flags = default_viewport_flags;

		{
			gui::TextureButton((ImGuiID)"_pktb1",
				tex::GetTextureID("images\\buttons\\icon_arrow_left.png").id,
				ImGuiButtonFlags_None); ImGui::SameLine();

			bool play = gui::TextureButton((ImGuiID)"_pktb2",
				tex::GetTextureID(playing ? "images\\buttons\\icon_pause.png" : "images\\buttons\\icon_play.png").id,
				ImGuiButtonFlags_None); ImGui::SameLine();

			gui::TextureButton((ImGuiID)"_pktb3",
				tex::GetTextureID("images\\buttons\\icon_arrow_right.png").id,
				ImGuiButtonFlags_None);

			if (play) {
				playing = !playing;
			}
		}

		ImGui::End();
	}

	struct TextureButtonData
	{
		ImTextureID texture = 0;
		int width = 0;
		int height = 0;
	};
	inline const char btn_img_default[] = "images/buttons/button.png";
	inline const char btn_img_hover[] = "images/buttons/button_hover.png";
	inline const char btn_img_active[] = "images/buttons/button_active.png";
	TextureButtonData btn_default_data;
	TextureButtonData btn_hover_data;
	TextureButtonData btn_active_data;
	ImVec2 btn_size;
	ImVec2 btn_uv0 = ImVec2(0.0f, 0.0f);
	ImVec2 btn_uv1 = ImVec2(1.0f, 1.0f);
	ImVec4 btn_bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 btn_tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	void LoadNativeTextures()
	{
		bool ret = tex::LoadTextureFromFile(btn_img_default, (GLuint*)(void*)&btn_default_data.texture, &btn_default_data.width, &btn_default_data.height);
		IM_ASSERT(ret);
		ret = tex::LoadTextureFromFile(btn_img_hover, (GLuint*)(void*)&btn_hover_data.texture, &btn_hover_data.width, &btn_hover_data.height);
		IM_ASSERT(ret);
		ret = tex::LoadTextureFromFile(btn_img_active, (GLuint*)(void*)&btn_active_data.texture, &btn_active_data.width, &btn_active_data.height);
		IM_ASSERT(ret);

		btn_size = ImVec2(btn_default_data.width, btn_default_data.height);
	}

	// Custom Textures
	bool TextureButton(ImGuiID id, ImTextureID texture_icon_id, ImTextureID texture_default_id, ImTextureID texture_hover_id, ImTextureID texture_active_id, ImGuiButtonFlags flags)
	{
		ImTextureID texture_id = texture_default_id;
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		const ImVec2 padding = g.Style.FramePadding;
		const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + btn_size + padding * 2.0f);
		ImGui::ItemSize(bb);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);
		ImVec2 icon_offset;

		// Determine image
		if (hovered)
		{
			//std::cout << "Hovered!";
			texture_id = texture_hover_id;
			icon_offset = ImVec2(0, 1);
		}
		if (pressed || held)
		{
			//std::cout << "Pressed!";
			texture_id = texture_active_id;
			icon_offset = ImVec2(0, 2);
		}

		// Render
		//const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		const ImU32 col = ImGui::GetColorU32(ImGuiCol_Button);
		ImGui::RenderNavHighlight(bb, id);
		ImGui::RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, g.Style.FrameRounding));
		if (btn_bg_col.w > 0.0f)
			window->DrawList->AddRectFilled(bb.Min + padding, bb.Max - padding, ImGui::GetColorU32(btn_bg_col));
		window->DrawList->AddImage(texture_id, bb.Min + padding, bb.Max - padding, btn_uv0, btn_uv1, ImGui::GetColorU32(btn_tint_col));
		if (texture_icon_id)
			window->DrawList->AddImage(texture_icon_id, bb.Min + padding + icon_offset, bb.Max - padding + icon_offset, btn_uv0, btn_uv1, ImGui::GetColorU32(btn_tint_col));

		return pressed;
	}

	// Native textures
	bool TextureButton(ImGuiID id, ImTextureID texture_icon_id, ImGuiButtonFlags flags)
	{
		return TextureButton(id, texture_icon_id, btn_default_data.texture, btn_hover_data.texture, btn_active_data.texture, flags);
	}
}