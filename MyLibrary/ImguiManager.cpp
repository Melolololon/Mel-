#include "ImguiManager.h"
#include"CreateBuffer.h"
#include"Library.h"

#include"Values.h"

#include"imgui/imgui_impl_dx12.h"
#include"imgui/imgui_impl_win32.h"

using namespace MelLib;

bool MelLib::ImguiManager::CheckReleaseDrawFlag()
{
    if (!releaseDrawFlag)
    {
#ifdef _DEBUG
        return false;
#else
        return true;

#endif // _DEBUG

    }
    return false;
}

ImguiManager* ImguiManager::GetInstance()
{
    static ImguiManager i;
    return &i;
}

bool ImguiManager::Initialize(ID3D12Device* pDev, ID3D12GraphicsCommandList* pCmdList)
{

    if (CheckReleaseDrawFlag())return true;

    pDevice = pDev;
    this->pCmdList = pCmdList;

    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    desc.NodeMask = 0;
    desc.NumDescriptors = 1;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

    pDev->CreateDescriptorHeap(&desc,IID_PPV_ARGS(&desHeap));

    //imgui‰Šú‰»
    if (!ImGui::CreateContext()) return false;
    if (!ImGui_ImplWin32_Init(Library::GetHWND()))return false;
    if (!ImGui_ImplDX12_Init
    (
        pDev,
        3,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        desHeap.Get(),
        desHeap->GetCPUDescriptorHandleForHeapStart(),
        desHeap->GetGPUDescriptorHandleForHeapStart()
    ))return false;

 

}

void MelLib::ImguiManager::Finalize()
{
    if (CheckReleaseDrawFlag())return;

    ImGui_ImplWin32_Shutdown();
    ImGui_ImplDX12_Shutdown();
}

void MelLib::ImguiManager::Begin()
{

    if (CheckReleaseDrawFlag())return;

    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void MelLib::ImguiManager::Draw()
{

    if (CheckReleaseDrawFlag())return;

    //•`‰æˆ—
    ImGui::Render();
    pCmdList->SetDescriptorHeaps(1, desHeap.GetAddressOf());
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), pCmdList);
}


void MelLib::ImguiManager::BeginDrawWindow
(
    const std::string& name
)
{
    if (CheckReleaseDrawFlag())return;

    ImGui::Begin(name.c_str());
    
  

}

void MelLib::ImguiManager::EndDrawWindow()
{
    if (CheckReleaseDrawFlag())return;

    ImGui::End();
}

void MelLib::ImguiManager::SetPosition(const Vector2& pos)
{
    if (CheckReleaseDrawFlag())return;

    ImGui::SetWindowPos(ImVec2(pos.x, pos.y), ImGuiCond_::ImGuiCond_FirstUseEver);
}

void MelLib::ImguiManager::SetSize(const Vector2& size)
{
    if (CheckReleaseDrawFlag())return;
    ImGui::SetWindowSize(ImVec2(size.x, size.y), ImGuiCond_::ImGuiCond_FirstUseEver);
}


bool MelLib::ImguiManager::DrawRadioButton(const std::string& label, int* pInt, const int num)
{
    if (CheckReleaseDrawFlag())return false;

    return ImGui::RadioButton(label.c_str(), pInt, num);
}

bool MelLib::ImguiManager::DrawCheckBox(const std::string& label, bool* flag)
{
    if (CheckReleaseDrawFlag())return false;

    return ImGui::Checkbox(label.c_str(), flag);
}

bool MelLib::ImguiManager::DrawSliderInt(const std::string& label, int* pInt, const int numMin, const int numMax)
{
    if (CheckReleaseDrawFlag())return false;

    return ImGui::SliderInt(label.c_str(), pInt, numMin, numMax);
}

bool MelLib::ImguiManager::DrawSliderFloat(const std::string& label, float* pFloat, const float numMin, const float numMax)
{
    if (CheckReleaseDrawFlag())return false;

    return ImGui::SliderFloat(label.c_str(), pFloat, numMin, numMax);
}

bool MelLib::ImguiManager::DrawColorPocker(const std::string& label, Color* pColor, const ImGuiColorEditFlags flag)
{
    if (CheckReleaseDrawFlag())return false;

    Value4<float>fCor4 = pColor->ToFloat();
    float color[4] = { fCor4.v1,fCor4.v2,fCor4.v3,fCor4.v4 };
    bool result = ImGui::ColorPicker4(label.c_str(), color, flag);
    *pColor = Color(color[0] * 255.0f, color[1] * 255.0f, color[2] * 255.0f, color[3] * 255.0f);
    return result;
}
