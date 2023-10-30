#include "pch.h"
#include "WaterMaskUtil.h"
#include <gdiplus.h>
#include "SettingManager.h"
#include "Utility/ImPath.h"

CString CWaterMaskUtil::AddWaterMask(const CString& imageFilePath, const CString& title, time_t time, 
    const CString& model, const CString& workContent)
{
    // Load the original image
    Gdiplus::Bitmap originalImage(imageFilePath);

    // Create a Graphics object from the original image
    Gdiplus::Graphics graphics(&originalImage);

    // Create a background brush
    const CWaterMaskSetting& setting = CSettingManager::GetInstance()->GetWaterMaskSetting();
    Gdiplus::Color watermarkColor = ParseColorString(setting.m_bgColor);
    Gdiplus::SolidBrush watermarkBrush(watermarkColor);

    // Apply the watermark as a brush on the image
    int left = setting.m_offsetLeft;
    int top = originalImage.GetHeight() - setting.m_offsetBottom - setting.m_height;
    graphics.FillRectangle(&watermarkBrush, left, top, setting.m_width, setting.m_height);

    // Create a font for the watermark text
    Gdiplus::Font watermarkFont1(L"微软雅黑", 24);
    Gdiplus::Font watermarkFont2(L"微软雅黑", 20);

    // Create a solid brush for the watermark text
    Gdiplus::Color textColor1(ParseColorString(setting.m_fontColor1));
    Gdiplus::SolidBrush textBrush1(textColor1);
    Gdiplus::Color textColor2(ParseColorString(setting.m_fontColor2));
    Gdiplus::SolidBrush textBrush2(textColor2);

    // Draw title
    float titleX = left + 75;
    float titleY = top + 5;
    const Gdiplus::StringFormat* stringFormat = Gdiplus::StringFormat::GenericDefault();
    graphics.DrawString((LPCWSTR)title, -1, &watermarkFont1, Gdiplus::PointF(titleX, titleY), stringFormat, &textBrush1);

    // Draw time
    float timeX = titleX;
    float timeY = titleY + 34;
    CTime t(time);
    CString fdt = t.Format(_T("%Y.%m.%d %H:%M"));
    graphics.DrawString((LPCWSTR)fdt, -1, &watermarkFont2, Gdiplus::PointF(timeX, timeY), stringFormat, &textBrush1);

    // 施工区域
    float placeX = titleX - 20;
    float placeY = timeY + 20;
    graphics.DrawString((LPCWSTR)L"施工区域", -1, &watermarkFont2, Gdiplus::PointF(placeX, placeY), stringFormat, &textBrush1);
    float placeX2 = placeX + 150;
    graphics.DrawString((LPCWSTR)model, -1, &watermarkFont2, Gdiplus::PointF(placeX2, placeY), stringFormat, &textBrush1);
    Gdiplus::Pen linePen(Gdiplus::Color(160, 160, 160));

    // 横线
    float lineX = placeX2;
    float lineY = placeY + 50;
    float lineLength = 200;
    graphics.DrawLine(&linePen, Gdiplus::PointF(lineX, lineY), Gdiplus::PointF(lineX+lineLength, lineY));

    // 施工内容
    
    // 横线

    // 地点

    // 横线   

    // Save the modified image    
    CString newImageFilePath = CString(CImPath::GetCachePath().c_str()) + model + L".jpg";
    DeleteFile(newImageFilePath);
    auto result = originalImage.Save(newImageFilePath, &Gdiplus::ImageFormatJPEG);
    if (result != Gdiplus::Ok)
    {
        LOG_ERROR(L"failed to save the image with water mask, path is %s, error is %d",
            (LPCWSTR)newImageFilePath, result);
        return L"";
    }
    return newImageFilePath;
}

Gdiplus::Color CWaterMaskUtil::ParseColorString(const CString& colorString)
{
    BYTE alpha, red, green, blue;
    if (swscanf_s(colorString, L"%02hhx%02hhx%02hhx%02hhx", &alpha, &red, &green, &blue) == 4)
    {
        return Gdiplus::Color(alpha, red, green, blue);
    }

    return Gdiplus::Color(255, 0, 0, 0);
}