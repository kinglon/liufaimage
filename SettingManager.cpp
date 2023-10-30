#include "pch.h"
#include "SettingManager.h"
#include <json/json.h>
#include <fstream>
#include "Utility/ImPath.h"
#include "Utility/ImCharset.h"

CSettingManager* CSettingManager::GetInstance()
{
	static CSettingManager* pInstance = new CSettingManager();
	return pInstance;
}

void CSettingManager::Load()
{
    std::wstring strConfFilePath = CImPath::GetConfPath() + L"configs.json";
    std::ifstream inputFile(strConfFilePath);
    if (!inputFile.is_open())
    {
        LOG_ERROR(L"failed to open the basic configure file : %s", strConfFilePath.c_str());
        return;
    }

    std::string jsonString;
    std::string line;
    while (std::getline(inputFile, line))
    {
        jsonString += line;
    }
    inputFile.close();

    Json::Value root;
    Json::CharReaderBuilder builder;
    Json::CharReader* reader = builder.newCharReader();
    std::string errors;
    bool parsingSuccessful = reader->parse(jsonString.c_str(), jsonString.c_str() + jsonString.size(), &root, &errors);
    delete reader;
    if (!parsingSuccessful)
    {
        LOG_ERROR(L"failed to parse the basic configure");
        return;
    }

    if (root.isMember("log_level"))
    {
        m_nLogLevel = root["log_level"].asInt();
    }

    if (root.isMember("title"))
    {
        m_strWaterMarkTitle = CImCharset::UTF8ToUnicode(root["title"].asString().c_str()).c_str();
    }

    if (root.isMember("model_prefix"))
    {
        m_strModelPrefix = CImCharset::UTF8ToUnicode(root["model_prefix"].asString().c_str()).c_str();
    }

    if (root.isMember("work_content"))
    {
        m_strWorkContent = CImCharset::UTF8ToUnicode(root["work_content"].asString().c_str()).c_str();
    }

    m_modeIndexItems.clear();
    if (root.isMember("model_next_index"))
    {
        CModelIndexItem modelIndexItem;
        for (Json::Value& childValue : root["model_next_index"])
        {
            modelIndexItem.m_year = childValue["year"].asString();
            modelIndexItem.m_yd = childValue["YD"].asInt();
            modelIndexItem.m_v = childValue["V"].asInt();
            modelIndexItem.m_a = childValue["A"].asInt();
            m_modeIndexItems.push_back(modelIndexItem);
        }
    }

    if (root.isMember("water_mask"))
    {
        Json::Value& waterMask = root["water_mask"];
        m_waterMaskSetting.m_offsetLeft = waterMask["offset_left"].asInt();
        m_waterMaskSetting.m_offsetBottom = waterMask["offset_bottom"].asInt();
        m_waterMaskSetting.m_width = waterMask["width"].asInt();
        m_waterMaskSetting.m_height = waterMask["height"].asInt();
        m_waterMaskSetting.m_bgColor = CImCharset::UTF8ToUnicode(waterMask["bgcolor"].asString().c_str()).c_str();
        m_waterMaskSetting.m_fontColor1 = CImCharset::UTF8ToUnicode(waterMask["font_color1"].asString().c_str()).c_str();
        m_waterMaskSetting.m_fontColor2 = CImCharset::UTF8ToUnicode(waterMask["font_color2"].asString().c_str()).c_str();     
    }
}

void CSettingManager::SetModelItemIndex(const std::vector<CModelIndexItem>& modelIndexItems)
{
    m_modeIndexItems = modelIndexItems;
    Save();
}

void CSettingManager::Save()
{
    Json::Value root = Json::objectValue;
    root["log_level"] = m_nLogLevel;
    root["title"] = CImCharset::UnicodeToUTF8(m_strWaterMarkTitle);
    root["model_prefix"] = CImCharset::UnicodeToUTF8(m_strModelPrefix);
    root["work_content"] = CImCharset::UnicodeToUTF8(m_strWorkContent);

    for (auto& item : m_modeIndexItems)
    {
        Json::Value childValue = Json::objectValue;
        childValue["year"] = item.m_year;
        childValue["YD"] = item.m_yd;
        childValue["V"] = item.m_v;
        childValue["A"] = item.m_a;
        root["model_next_index"].append(childValue);
    }

    Json::Value waterMask;
    waterMask["offset_left"] = m_waterMaskSetting.m_offsetLeft;
    waterMask["offset_bottom"] = m_waterMaskSetting.m_offsetBottom;
    waterMask["width"] = m_waterMaskSetting.m_width;
    waterMask["height"] = m_waterMaskSetting.m_height;
    waterMask["bgcolor"] = CImCharset::UnicodeToUTF8((LPCWSTR)m_waterMaskSetting.m_bgColor);
    waterMask["font_color1"] = CImCharset::UnicodeToUTF8((LPCWSTR)m_waterMaskSetting.m_fontColor1);
    waterMask["font_color2"] = CImCharset::UnicodeToUTF8((LPCWSTR)m_waterMaskSetting.m_fontColor2);
    root["water_mask"] = waterMask;

    std::wstring strConfFilePath = CImPath::GetConfPath() + L"configs.json";
    std::ofstream outputFile(strConfFilePath);
    if (outputFile.is_open())
    {
        Json::StreamWriterBuilder writer;
        std::string jsonString = Json::writeString(writer, root);
        outputFile << jsonString;
        outputFile.close();
    }
    else
    {
        LOG_ERROR(L"failed to open the basic configure file : %s", strConfFilePath.c_str());
    }
}