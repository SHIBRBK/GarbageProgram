#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include"../AGS2/class/Common/DxCushion.h"
#include "Resource.h"
#include<vector>
const std::string Resource::PATH_MODEL = "Resource/Model/";
const std::string Resource::PATH_SOUND = "Resource/Sound/";
const std::string Resource::PATH_TEXTURE = "Resource/Texture/";
const std::string Resource::PATH_EFFECT = "Resource/Effect/";
const std::string Resource::PATH_SHADER = "Resource/Shader/";


int Resource::LoadModel(const std::string& filePath)
{
    int modelHandle = DxCushion::MyMV1LoadModel(filePath.c_str());
    if (modelHandle == -1)
    {
        MessageBox(NULL, (LPCSTR)("モデルの読み込みに失敗しました: " + filePath).c_str(), "エラー", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }
    return modelHandle;
}

void Resource::ReleaseModel(int modelHandle)
{
    DxCushion::MyMV1DeleteModel(modelHandle);
}

void Resource::ReleaseAllModels()
{
    DxCushion::MyMV1InitModel();
}

int Resource::LoadSound(const std::string& filePath)
{
    int soundHandle = DxCushion::MyLoadSoundMem(filePath.c_str());
    if (soundHandle == -1)
    {
        MessageBox(NULL, (LPCSTR)("サウンドの読み込みに失敗しました: " + filePath).c_str(), "エラー", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }
    return soundHandle;
}

void Resource::ReleaseSound(int soundHandle)
{
    DxCushion::MyDeleteSoundMem(soundHandle);
}

void Resource::ReleaseAllSounds()
{
    DxCushion::MyInitSoundMem();
}

int Resource::LoadTexture(const std::string& filePath)
{
    int textureHandle = DxCushion::MyLoadGraph(filePath.c_str());

    if (textureHandle == -1) {
        // テクスチャの読み込みに失敗した場合はエラーを表示して終了
        MessageBox(NULL, (LPCSTR)("テクスチャの読み込みに失敗しました: " + filePath).c_str(), "エラー", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    return textureHandle;
}

void Resource::ReleaseTexture(int textureHandle)
{
    DxCushion::MyDeleteGraph(textureHandle);
}

void Resource::ReleaseAllTextures()
{
    DxCushion::MyInitGraph();
}

int Resource::LoadEffect(const std::string& filePath)
{
    int effectHandle = LoadEffekseerEffect(filePath.c_str());
    if (effectHandle == -1) 
    {
        // テクスチャの読み込みに失敗した場合はエラーを表示して終了
        MessageBox(NULL, (LPCSTR)("テクスチャの読み込みに失敗しました: " + filePath).c_str(), "エラー", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

}

void Resource::ReleaseEffect(int textureHandle)
{
    DeleteEffekseerEffect(textureHandle);
}

void Resource::ReleaseAllEffect()
{
    Effkseer_End();
}

int Resource::MyLoadPixelShader(const std::string& filePath)
{
    int shaderHandle = LoadPixelShader(filePath.c_str());
    if (shaderHandle == -1)
    {
        // テクスチャの読み込みに失敗した場合はエラーを表示して終了
        MessageBox(NULL, (LPCSTR)("シェーダの読み込みに失敗しました: " + filePath).c_str(), "エラー", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }
}

void Resource::ReleasePixelShader(int textureHandle)
{
    DeleteShader(textureHandle);
}

void Resource::ReleaseAllPixelShader()
{
    InitShader();
}

int Resource::MyLoadVertexShader(const std::string& filePath)
{
    int shaderHandle = LoadVertexShader(filePath.c_str());
    if (shaderHandle == -1)
    {
        // テクスチャの読み込みに失敗した場合はエラーを表示して終了
        MessageBox(NULL, (LPCSTR)("シェーダの読み込みに失敗しました: " + filePath).c_str(), "エラー", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }
}

void Resource::ReleaseVertexShader(int textureHandle)
{
    DeleteShader(textureHandle);
}

void Resource::ReleaseAllVertexShader()
{
    InitShader();
}

void Resource::LoadModelsInFolder(const std::string& folderPath, std::vector<int>& modelHandles)
{
    HANDLE hFind;
    WIN32_FIND_DATA fd;
    std::string filter = folderPath + "*.mv1";

    hFind = FindFirstFile(filter.c_str(), &fd);
    if (hFind == INVALID_HANDLE_VALUE) {
        // ファイルが見つからなかった場合は何もしない
        return;
    }

    do {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            std::string filePath = folderPath + fd.cFileName;
            int modelHandle = LoadModel(filePath);
            modelHandles.push_back(modelHandle);
        }
    } while (FindNextFile(hFind, &fd) != 0);

    FindClose(hFind);
}