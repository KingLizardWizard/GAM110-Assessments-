#pragma once

#include <string>
#include <sstream>
#include <gdiplus.h>
#include <memory>
#include <random>
#include <map>

class GameFramework;

/** ImageWrapper wraps around an image and stores both the image and it's size. */
class ImageWrapper
{
public:
	/**
	 * Gets the width of the image.
	 *
	 * @return The width of the image.
	 */
	UINT GetWidth() const
	{
		return imageWidth;
	}

	/**
	 * Gets the height of the image.
	 *
	 * @return The height of the image.
	 */
	UINT GetHeight() const
	{
		return imageHeight;
	}

private:
	ImageWrapper(Gdiplus::Image* imagePtr)
	{
		imageWidth = imagePtr->GetWidth();
		imageHeight = imagePtr->GetHeight();

		textureBrushPtr = new Gdiplus::TextureBrush(imagePtr);
	}

	~ImageWrapper()
	{
		delete textureBrushPtr;
	}

	ImageWrapper(const ImageWrapper&) {}
	ImageWrapper& operator=(const ImageWrapper&) { return *this; }

	Gdiplus::TextureBrush* GetTextureBrushPtr()
	{
		return textureBrushPtr;
	}

	Gdiplus::TextureBrush* textureBrushPtr;
	UINT imageWidth;
	UINT imageHeight;

	friend GameFramework;
};

class GameFramework
{
public:
	static GameFramework& Instance();

	void EndPlay();

	void Update();

	void Render(CPaintDC &dc, const CRect& clientRect);

	/**
	 * Retrieves the current time.
	 *
	 * @return The current time in seconds..
	 */
	double CurrentTime() { return currentTime; }

	/**
	 * Draws a line.
	 *
	 * @param [in,out] canvas The canvas.
	 * @param start			  The start of the line.
	 * @param end			  The end of the line.
	 * @param colour		  The colour.
	 */
	void DrawLine(Gdiplus::Graphics& canvas, const Vector2i& start, const Vector2i& end, const Gdiplus::Color& colour) const;

	/**
	 * Draws a rectangle.
	 *
	 * @param [in,out] canvas The canvas.
	 * @param rectangle		  The bounding box of the rectangle.
	 * @param filled		  true if the rectangle should be filled, false to draw an outline.
	 * @param colour		  The colour.
	 */
	void DrawRectangle(Gdiplus::Graphics& canvas, const AABBi& rectangle, bool filled, const Gdiplus::Color& colour) const;

	/**
	 * Draws a circle.
	 *
	 * @param [in,out] canvas The canvas.
	 * @param location		  The centre point of the circle.
	 * @param radius		  The radius.
	 * @param filled		  true if the circle should be filled, false to draw an outline.
	 * @param colour		  The colour.
	 */
	void DrawCircle(Gdiplus::Graphics& canvas, const Vector2i& location, int radius, bool filled, const Gdiplus::Color& colour) const;

	/**
	* Draws an image.
	*
	* @param [in,out] canvas The canvas.
	* @param location		  The location to draw the image at.
	* @param [in,out] image  A pointer to the image to draw.
	*/
	void DrawImage(Gdiplus::Graphics& canvas, const Vector2i& location, ImageWrapper* image);

	/**
	 * Draws text.
	 *
	 * @param [in,out] canvas The canvas.
	 * @param location		  The top left location of the text.
	 * @param fontSize		  Size of the font.
	 * @param fontName		  Name of the font.
	 * @param text			  The text to draw.
	 * @param textColour	  The text colour.
	 */
	void DrawText(Gdiplus::Graphics& canvas, const Vector2i& location, int fontSize, const std::string& fontName,
		const std::string& text, const Gdiplus::Color& textColour) const;

	/**
	 * Measures the size of a block of text.
	 *
	 * @param [in,out] canvas The canvas.
	 * @param fontSize		  Size of the font.
	 * @param fontName		  Name of the font.
	 * @param text			  The text to measure.
	 *
	 * @return A Vector2i containing the width and height of the text.
	 */
	Vector2i GameFramework::MeasureText(Gdiplus::Graphics& canvas, int fontSize, const std::string& fontName, const std::string& text);

	/**
	* Loads a True Type Font (ttf).
	*
	* @param filePath   Full pathname of the file.
	* @param uniqueName Unique name to give to the font. This name can then be used with DrawText and MeasureText.
	*
	* @return true if it succeeds, false if it fails.
	*/
	bool LoadFontResource(const std::string& filePath, const std::string& uniqueName);

	/**
	 * Loads an image. After loading the image can be retrieved using the unique name provided.
	 *
	 * @param filePath   Full pathname of the file.
	 * @param uniqueName Unique name to give to the image.
	 *
	 * @return nullptr if it fails, otherwise returns a pointer to the image.
	 */
	ImageWrapper* LoadImageResource(const std::string& filePath, const std::string& uniqueName);

	/**
	 * Retrieves a previous loaded image.
	 *
	 * @param uniqueName The unique name of the image to retrieve.
	 *
	 * @return nullptr if it fails, otherwise returns a pointer to the image.
	 */
	ImageWrapper* GetLoadedImage(const std::string& uniqueName);

	/**
	 * Seeds the mersenne twister based random number generator.
	 *
	 * @param seed The value to seed the random number generator with.
	 */
	void SeedMersenneTwister(unsigned int seed)
	{
		mersenneTwister = std::mt19937(seed);
	}

	/**
	 * Generates a random integer between two values. The generator is inclusive of the minimum and maximum values.
	 *
	 * @param minValue The minimum value.
	 * @param maxValue The maximum value.
	 *
	 * @return A random integer.
	 */
	int RandomIntBetween(int minValue, int maxValue);

	/**
	 * Generates a random float between two values. The generator is inclusive of the minimum and maximum values.
	 *
	 * @param minValue The minimum value.
	 * @param maxValue The maximum value.
	 *
	 * @return A random float.
	 */
	float RandomFloatBetween(float minValue, float maxValue);

private:
	GameFramework();
	~GameFramework();

	LONGLONG tickFrequency;
	double currentTime;

	ULONG_PTR gdiPlusToken;

	AABBi screenBounds;

	std::mt19937 mersenneTwister;
	float mersenneTwisterRange;

	typedef std::map<std::string, ImageWrapper*> ImageMap;
	ImageMap imageMap;

	typedef std::map<std::string, Gdiplus::PrivateFontCollection*> FontMap;
	FontMap fontMap;
};

/**
 * Retrieves the GameFramework instance for the game
 *
 * @return GameFramework::Instance() The game framework instance
 */
#define GameFrameworkInstance (GameFramework::Instance())

/**
 * Logs out debug information. Data can be separated using the << operator
 * eg. DebugLog("This" << " is " << " a " << " test!");
 *
 * @param expression The expression to log to the console.
 */
#define DebugLog(expression)								\
{															\
	std::ostringstream outputStream;						\
															\
	outputStream << expression << std::endl;				\
															\
	OutputDebugStringA(outputStream.str().c_str());			\
}
