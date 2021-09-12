#include "ImguiManager.h"
#include"CreateBuffer.h"
#include"Library.h"

#include"imgui/imgui.h"
#include"imgui/imgui_impl_dx12.h"
#include"imgui/imgui_impl_win32.h"

using namespace MelLib;
ImguiManager* ImguiManager::GetInstance()
{
    static ImguiManager i;
    return &i;
}

bool ImguiManager::Initialize(ID3D12Device* pDev, ID3D12GraphicsCommandList* pCmdList)
{
    if (!releaseDrawFlag)
    {

#ifdef _DEBUG

#else

        return true;

#endif // _DEBUG

    }

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
    if (!releaseDrawFlag)
    {

#ifdef _DEBUG

#else

        return true;

#endif // _DEBUG

    }

    ImGui_ImplWin32_Shutdown();
    ImGui_ImplDX12_Shutdown();
}

void MelLib::ImguiManager::Begin()
{
    if (!releaseDrawFlag)
    {

#ifdef _DEBUG

#else

        return true;

#endif // _DEBUG

    }

    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void MelLib::ImguiManager::Draw()
{
    if (!releaseDrawFlag)
    {

#ifdef _DEBUG

#else

        return true;

#endif // _DEBUG

    }

    //•`‰æˆ—
    ImGui::Render();
    pCmdList->SetDescriptorHeaps(1, desHeap.GetAddressOf());
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), pCmdList);
}


void MelLib::ImguiManager::DrawWindow
(
    const std::string& name,
    const Vector2& pos, 
    const Vector2& size
)
{
    if (!releaseDrawFlag)
    {

#ifdef _DEBUG

#else

        return true;

#endif // _DEBUG

    }
    ImGui::Begin(name.c_str());
    
    ImGui::SetWindowPos(ImVec2(pos.x, pos.y), ImGuiCond_::ImGuiCond_FirstUseEver);
    ImGui::SetWindowSize(ImVec2(size.x, size.y), ImGuiCond_::ImGuiCond_FirstUseEver);

    ImGui::End();

    
}
