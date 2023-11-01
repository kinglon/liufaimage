#include "pch.h"
#include "WaterMaskUtil.h"
#include <gdiplus.h>
#include "SettingManager.h"
#include "Utility/ImPath.h"

using namespace Gdiplus;

CLSID CWaterMaskUtil::GetEncoderClsid(const WCHAR* format)
{
    UINT num = 0;   // number of image encoders
    UINT size = 0;  // size of the image encoder array in bytes

    ImageCodecInfo* pImageCodecInfo = nullptr;

    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return CLSID_NULL;  // No image encoders found

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == nullptr)
        return CLSID_NULL;  // Memory allocation failed

    GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j)
    {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
        {
            CLSID clsid;
            clsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return clsid;
        }
    }

    free(pImageCodecInfo);
    return CLSID_NULL;  // No matching encoder found
}

CString CWaterMaskUtil::AddWaterMask(const CString& imageFilePath, const CString& title, time_t time, 
    const CString& model, const CString& workContent)
{
    const CWaterMaskSetting& setting = CSettingManager::GetInstance()->GetWaterMaskSetting();

    // Load the original image
    Gdiplus::Bitmap originalImage(imageFilePath);

    // Create a Graphics object from the original image
    Gdiplus::Graphics graphics(&originalImage);
    
    Gdiplus::Rect wholeRegionRect(setting.m_offsetLeft,
        originalImage.GetHeight() - setting.m_offsetBottom - setting.m_height,
        setting.m_width, setting.m_height);

    // 画上部分背景
    Gdiplus::Color topRegionBgColor((byte)180, (byte)64, (byte)89, (byte)190);
    Gdiplus::SolidBrush topRegionBgBrush(topRegionBgColor);    
    Gdiplus::Rect topRegionRect(wholeRegionRect.X, wholeRegionRect.Y,
        wholeRegionRect.Width, wholeRegionRect.Height / 3);
    graphics.FillRectangle(&topRegionBgBrush, topRegionRect);

    // 画下部分背景    
    Gdiplus::Color bottomRegionBgColor((byte)150, (byte)192, (byte)192, (byte)192);
    Gdiplus::SolidBrush bottomRegionBgBrush(bottomRegionBgColor);
    Gdiplus::Rect bottomRegionRect(wholeRegionRect.X, wholeRegionRect.Y + topRegionRect.Height,
        wholeRegionRect.Width, wholeRegionRect.Height - topRegionRect.Height);
    graphics.FillRectangle(&bottomRegionBgBrush, bottomRegionRect);

    const float nTopTextOffset = 168.0f;
    const Gdiplus::StringFormat* stringFormat = Gdiplus::StringFormat::GenericDefault();
    Gdiplus::Color textColor1(ParseColorString(setting.m_fontColor1));
    Gdiplus::SolidBrush textBrush1(textColor1);
    Gdiplus::Color textColor2(ParseColorString(setting.m_fontColor2));
    Gdiplus::SolidBrush textBrush2(textColor2);

    // 写标题
    Gdiplus::Font titleFont(L"微软雅黑", 50);
    Gdiplus::PointF titleOrigin(wholeRegionRect.X + nTopTextOffset, wholeRegionRect.Y + 10.0f);
    graphics.DrawString((LPCWSTR)title, -1, &titleFont, titleOrigin, stringFormat, &textBrush1);

    // 写时间
    Gdiplus::Font timeFont(L"微软雅黑", 40);
    Gdiplus::PointF timeOrigin(wholeRegionRect.X + nTopTextOffset, wholeRegionRect.Y + 105.0f);
    CTime t(time);
    CString fdt = t.Format(_T("%Y.%m.%d  %H:%M"));
    graphics.DrawString((LPCWSTR)fdt, -1, &timeFont, timeOrigin, stringFormat, &textBrush1);

    const float bottomColumn1Offset = 54.0f;
    const float bottomColumn2Offset = 365.0f;
    const float rowTopPadding = 35.0f;
    float nLineHeight = bottomRegionRect.Height / 3.0f;
    Gdiplus::Font bottomFont(L"微软雅黑", 40);

    // 写施工区域
    Gdiplus::PointF row1Column1(bottomRegionRect.X + bottomColumn1Offset, bottomRegionRect.Y + rowTopPadding);
    graphics.DrawString(L"施 工 区 域", -1, &bottomFont, row1Column1, stringFormat, &textBrush2);
    Gdiplus::PointF row1Column2(bottomRegionRect.X + bottomColumn2Offset, bottomRegionRect.Y + rowTopPadding);
    graphics.DrawString(model, -1, &bottomFont, row1Column2, stringFormat, &textBrush2);

    // 写施工内容
    Gdiplus::PointF row2Column1(bottomRegionRect.X + bottomColumn1Offset, bottomRegionRect.Y + nLineHeight + rowTopPadding);
    graphics.DrawString(L"施 工 内 容", -1, &bottomFont, row2Column1, stringFormat, &textBrush2);
    Gdiplus::PointF row2Column2(bottomRegionRect.X + bottomColumn2Offset, bottomRegionRect.Y + nLineHeight + rowTopPadding);
    graphics.DrawString(workContent, -1, &bottomFont, row2Column2, stringFormat, &textBrush2);

    // 写地点
    Gdiplus::PointF row3Column1(bottomRegionRect.X + bottomColumn1Offset, bottomRegionRect.Y + nLineHeight*2 + rowTopPadding);
    graphics.DrawString(L"地          点", -1, &bottomFont, row3Column1, stringFormat, &textBrush2);
    Gdiplus::PointF row3Column2(bottomRegionRect.X + bottomColumn2Offset, bottomRegionRect.Y + nLineHeight*2 + rowTopPadding);
    graphics.DrawString(L"", -1, &bottomFont, row2Column2, stringFormat, &textBrush2);

    // 画三条横线
    Gdiplus::Pen linePen(Gdiplus::Color(80, 80, 80), 2);
    const float bottomLinePadding = 20.0f;
    Gdiplus::PointF line1StartPos(bottomRegionRect.X + bottomColumn2Offset, bottomRegionRect.Y + nLineHeight - bottomLinePadding);
    Gdiplus::PointF line1EndPos(bottomRegionRect.X + setting.m_width - bottomColumn1Offset, line1StartPos.Y);
    graphics.DrawLine(&linePen, line1StartPos, line1EndPos);
    Gdiplus::PointF line2StartPos(line1StartPos.X, line1StartPos.Y + nLineHeight);
    Gdiplus::PointF line2EndPos(line1EndPos.X, line1EndPos.Y + nLineHeight);
    graphics.DrawLine(&linePen, line2StartPos, line2EndPos);
    Gdiplus::PointF line3StartPos(line2StartPos.X, line2StartPos.Y + nLineHeight);
    Gdiplus::PointF line3EndPos(line2EndPos.X, line2EndPos.Y + nLineHeight);
    graphics.DrawLine(&linePen, line3StartPos, line3EndPos);    

    // Save the modified image    
    CString newImageFilePath = CString(CImPath::GetCachePath().c_str()) + model + L".jpg";
    DeleteFile(newImageFilePath);   
    CLSID jpgEncoderClsid = GetEncoderClsid(L"image/jpeg");
    if (jpgEncoderClsid == CLSID_NULL)
    {
        LOG_ERROR(L"failed to save the image with water mask, not have jpeg encoder");
        return L"";
    }

    auto result = originalImage.Save(newImageFilePath, &jpgEncoderClsid);
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