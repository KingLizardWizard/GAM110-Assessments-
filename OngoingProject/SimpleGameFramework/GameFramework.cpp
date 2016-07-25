#include "stdafx.h"
#include "GameFramework.h"
#include "CPUUtils.h"
#include "GameManager.h"
#include "GameInput.h"

#include <iostream>
#include <fstream>
#include <algorithm>

GameFramework::GameFramework() :
	currentTime(0)
{
	tickFrequency = CPUUtils::GetPerformanceCounterFrequency(0);

	// Initialise GDI+
	Gdiplus::GdiplusStartupInput gdiPlusStartupInput;
	Gdiplus::GdiplusStartup(&gdiPlusToken, &gdiPlusStartupInput, NULL);

	// Retrieve the mersenne twister range
	mersenneTwisterRange = static_cast<float>((mersenneTwister.max)() - (mersenneTwister.min)());

	// Seed the random number generator to a specific value
	SeedMersenneTwister(static_cast<unsigned int>(time(nullptr)));
}

GameFramework::~GameFramework()
{

	// Cleanup all images
	for (auto imageMapEntry : imageMap)
	{
		ImageWrapper* imageWrapperPtr = imageMapEntry.second;

		delete imageWrapperPtr;
	}

	// Cleanup all fonts
	for (auto fontMapEntry : fontMap)
	{
		Gdiplus::PrivateFontCollection* fontPtr = fontMapEntry.second;

		delete fontPtr;
	}

	// Tear down GDI+
	Gdiplus::GdiplusShutdown(gdiPlusToken);
}

GameFramework& GameFramework::Instance()
{
	static GameFramework instance;

	return instance;
}

void GameFramework::EndPlay()
{
	// Tear down the input and the game
	GameManagerInstance.EndPlay();
	GameInputInstance.EndPlay();
}

void GameFramework::Update()
{
	static LONGLONG lastUpdateTime = 0;

	// Retrieve our current tick count
	LONGLONG updateTime = CPUUtils::GetPerformanceCounterTicks(0);

	// Check if we had a valid tick count
	if (lastUpdateTime != 0)
	{
		// Calculate the time delta
		LONGLONG deltaTicks = updateTime - lastUpdateTime;
		double deltaTime = static_cast<double>(deltaTicks) / tickFrequency;

		// Update the current time
		currentTime += deltaTime;

		// Update the input manager
		GameInputInstance.Update(deltaTime);

		// Update the game manager
		GameManagerInstance.Update(deltaTime);
	}

	lastUpdateTime = updateTime;
}

void GameFramework::Render(CPaintDC &dc, const CRect& clientRect)
{
	// Make sure we store the screen bounds
	screenBounds = clientRect;

	CDC memoryDC;
	CBitmap bitmapDC;

	// Setup our in-memory device context
	memoryDC.CreateCompatibleDC(&dc);
	bitmapDC.CreateCompatibleBitmap(&dc, clientRect.Width(), clientRect.Height());
	memoryDC.SelectObject(&bitmapDC);

	// Run the internal render logic
	Gdiplus::Graphics canvas(memoryDC.GetSafeHdc());
	GameManagerInstance.Render(canvas, clientRect);

	// Copy from the in-memory device context to the main display
	dc.BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &memoryDC, 0, 0, SRCCOPY);
}

void GameFramework::DrawLine(Gdiplus::Graphics& canvas, const Vector2i& start, const Vector2i& end, const Gdiplus::Color& colour) const
{
	Gdiplus::Pen pen(colour, 1.0f);

	canvas.DrawLine(&pen, start, end);
}

void GameFramework::DrawRectangle(Gdiplus::Graphics& canvas, const AABBi& rectangle, bool filled, const Gdiplus::Color& colour) const
{
	// Draw the rectangle as filled or unfilled as required
	if (filled)
	{
		Gdiplus::SolidBrush brush(colour);

		canvas.FillRectangle(&brush, rectangle);
	}
	else
	{
		Gdiplus::Pen pen(colour, 1.0f);

		canvas.DrawRectangle(&pen, rectangle);
	}
}

void GameFramework::DrawCircle(Gdiplus::Graphics& canvas, const Vector2i& location, int radius, bool filled, const Gdiplus::Color& colour) const
{
	Gdiplus::Rect circleRect(location.X - radius, location.Y - radius, radius * 2, radius * 2);

	// Draw the circle filled or unfilled as required
	if (filled)
	{
		Gdiplus::SolidBrush brush(colour);
		canvas.FillPie(&brush, circleRect, 0, 360);
	}
	else
	{
		Gdiplus::Pen pen(colour, 1.0f);
		canvas.DrawPie(&pen, circleRect, 0, 360);
	}
}

void GameFramework::DrawImage(Gdiplus::Graphics& canvas, const Vector2i& location, ImageWrapper* image)
{
	Gdiplus::Matrix transform;
	canvas.GetTransform(&transform);

	canvas.TranslateTransform(static_cast<Gdiplus::REAL>(location.X),
		static_cast<Gdiplus::REAL>(location.Y));
	canvas.FillRectangle(image->GetTextureBrushPtr(), Gdiplus::Rect(0, 0, image->GetWidth(), image->GetHeight()));

	canvas.SetTransform(&transform);
}

void GameFramework::DrawText(Gdiplus::Graphics& canvas, const Vector2i& location, int fontSize, const std::string& fontName,
	const std::string& text, const Gdiplus::Color& textColour) const
{
	Gdiplus::FontFamily family(CString(fontName.c_str()));

	// Check if the font is one in our custom list
	FontMap::const_iterator foundFont = fontMap.find(fontName);
	if (foundFont != fontMap.end())
	{
		const Gdiplus::PrivateFontCollection* fontCollection = foundFont->second;

		// Attempt to retrieve the font family
		int numFound = 0;
		fontCollection->GetFamilies(1, &family, &numFound);
	}

	// Setup the font
	Gdiplus::Font font(&family, static_cast<Gdiplus::REAL>(fontSize), Gdiplus::FontStyleRegular, Gdiplus::UnitPoint);

	// Create our text brush
	Gdiplus::SolidBrush brush(textColour);

	// Draw the text
	CString textString(text.c_str());
	canvas.DrawString(textString, textString.GetLength(), &font, location, &brush);
}

Vector2i GameFramework::MeasureText(Gdiplus::Graphics& canvas, int fontSize, const std::string& fontName, const std::string& text)
{
	Gdiplus::FontFamily family(CString(fontName.c_str()));

	// Check if the font is one in our custom list
	FontMap::const_iterator foundFont = fontMap.find(fontName);
	if (foundFont != fontMap.end())
	{
		const Gdiplus::PrivateFontCollection* fontCollection = foundFont->second;

		// Attempt to retrieve the font family
		int numFound = 0;
		fontCollection->GetFamilies(1, &family, &numFound);
	}

	// Setup the font
	Gdiplus::Font font(&family, static_cast<Gdiplus::REAL>(fontSize), Gdiplus::FontStyleRegular, Gdiplus::UnitPoint);

	// Measure the text
	CString textString(text.c_str());
	Gdiplus::RectF layoutRect;
	Gdiplus::RectF boundsRect;
	canvas.MeasureString(textString, textString.GetLength(), &font, layoutRect, &boundsRect);

	return Vector2i(static_cast<int>(boundsRect.Width), static_cast<int>(boundsRect.Height));
}

bool GameFramework::LoadFontResource(const std::string& filePath, const std::string& uniqueName)
{
	Gdiplus::PrivateFontCollection* fontCollection = new Gdiplus::PrivateFontCollection();

	Gdiplus::Status result = fontCollection->AddFontFile(CString(filePath.c_str()));

	// If the font loaded successfully then add it to the map and return true
	if (result == Gdiplus::Ok)
	{
		fontMap[uniqueName] = fontCollection;
		return true;
	}

	// Font failed to load so delete the collection
	delete fontCollection;

	return false;
}

ImageWrapper* GameFramework::LoadImageResource(const std::string& filePath, const std::string& uniqueName)
{
	ImageWrapper* imageWrapperPtr = new ImageWrapper(Gdiplus::Image::FromFile(CString(filePath.c_str())));

	imageMap[uniqueName] = imageWrapperPtr;

	return imageWrapperPtr;
}

ImageWrapper* GameFramework::GetLoadedImage(const std::string& uniqueName)
{
	ImageMap::iterator imageIt = imageMap.find(uniqueName);

	return imageIt != imageMap.end() ? imageIt->second : nullptr;
}

int GameFramework::RandomIntBetween(int minValue, int maxValue)
{
	std::uniform_int_distribution<int> intDistribution(minValue, maxValue);

	return intDistribution(mersenneTwister);
}

float GameFramework::RandomFloatBetween(float minValue, float maxValue)
{
	std::uniform_real_distribution<float> floatDistribution(minValue, maxValue);

	return floatDistribution(mersenneTwister);
}
