#include "menu.h"
#include <windowsx.h>
#include "imgui195/imgui.h"
#include "imgui195/imgui_impl_win32.h"
#include "imgui195/imgui_impl_opengl2.h"
#include "hack.h"

// Globals
ImVec2 menuSize(400, 200);
WNDPROC originalWndProc = nullptr;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef int(__cdecl* tSDL_ShowCursor)(int);

tSDL_ShowCursor SDL_ShowCursor;

const char* aimBotOptions[] = { "auto", "hold" };

LRESULT __stdcall hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (hack::bMenu) {
		if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
			return true;
		}
		ImGuiIO& io = ImGui::GetIO();
		switch (uMsg) {
		case WM_LBUTTONDOWN:
			io.MouseDown[1] = !io.MouseDown[0];
			return 0;
		case WM_RBUTTONDOWN:
			io.MouseDown[1] = !io.MouseDown[1];
			return 0;
		// Todo: Fix mouse input still in game
		case WM_MOUSEMOVE:
			io.MousePos.x = GET_X_LPARAM(lParam);
			io.MousePos.y = GET_Y_LPARAM(lParam);
			return 0;
		}
	}
	return CallWindowProc(originalWndProc, hWnd, uMsg, wParam, lParam);
}

void Menu::Init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	SDL_ShowCursor = (tSDL_ShowCursor)offsets::showCursorAddr;
}

void Menu::SetHook() {
	HWND hwnd = nullptr;
	while (hwnd == nullptr) {
		hwnd = FindWindowA(NULL, "AssaultCube");
	}
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplOpenGL2_Init();
	originalWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)hWndProc);
}


void Menu::RenderMenu() {
	if (hack::bMenu) {
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		SDL_ShowCursor(1);
		ImGui::Begin("Assault Cube Menu");
		ImGui::SetWindowSize(menuSize, ImGuiCond_FirstUseEver);
		if (ImGui::BeginTabBar("Tab Bar", ImGuiTabBarFlags_None)) {
			if (ImGui::BeginTabItem("Visual")) {
				ImGui::Checkbox("ESP", &hack::bESP);
				ImGui::Checkbox("Player Names", &hack::bPlayerNames);
				ImGui::SeparatorText("Color Editors");
				ImGui::Text("ESP Box Color:");
				ImGui::ColorEdit3("ESP Color", (float*)&hack::rgb::playerBoxColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				ImGui::Text("Player Name Color: ");
				ImGui::ColorEdit3("Name Color", (float*)&hack::rgb::playerNameColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Aimbot")) {
				ImGui::Checkbox("Aimbot", &hack::bAimbot);
				ImGui::Combo("Aimbot Mode", &hack::aimBotMode, aimBotOptions, IM_ARRAYSIZE(aimBotOptions));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Misc")) {
				ImGui::Checkbox("Godmode", &hack::bHealth);
				ImGui::Checkbox("Unlimited Ammo", &hack::bAmmo);
				ImGui::Checkbox("No Recoil", &hack::bRecoil);
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	} else {
		SDL_ShowCursor(0);
	}
	
}


void Menu::Shutdown() {
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	SetWindowLongPtr(FindWindowA(NULL, "AssaultCube"), GWLP_WNDPROC, (LONG_PTR)originalWndProc);
}