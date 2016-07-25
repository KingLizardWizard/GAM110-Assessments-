#pragma once

#include "GameFramework.h"

class GameInput
{
public:
	static GameInput& Instance();

	/** BeginPlay is called when the game first launches. It is only called once per launch. */
	void BeginPlay();

	/** EndPlay is called when the game is closing down. It is only called once and only on exit. */
	void EndPlay();

	/**
	* Update is called every frame. Update is always called before render
	*
	* @param deltaTime The time in seconds since Update was last called.
	*/
	void Update(double deltaTime);

	/**
	 * This method is called for each individual key press when the key is pressed.
	 *
	 * @param keyCode	  The key code for the key that was pressed.
	 * @param repeatCount Number of repeats (if a key is held down the repeat count increases).
	 */
	void OnKeyDown(UINT keyCode, UINT repeatCount);

	/**
	* This method is called for each individual key press when the key is released.
	*
	 * @param keyCode	  The key code for the key that was pressed.
	 * @param repeatCount Number of repeats (if a key is held down the repeat count increases).
	 */
	void OnKeyUp(UINT keyCode, UINT repeatCount);

	/**
	 * This method is called when the left mouse button is pressed.
	 *
	 * @param point The location of the mouse cursor at the time of the event.
	 */
	void OnLButtonDown(const Vector2i& point);

	/**
	* This method is called when the left mouse button is released.
	*
	 * @param point The location of the mouse cursor at the time of the event.
	 */
	void OnLButtonUp(const Vector2i& point);

	/**
	* This method is called when the left mouse button is double-clicked.
	*
	 * @param point The location of the mouse cursor at the time of the event.
	 */
	void OnLButtonDblClk(const Vector2i& point);

	/**
	* This method is called when the right mouse button is pressed.
	*
	 * @param point The location of the mouse cursor at the time of the event.
	 */
	void OnRButtonDown(const Vector2i& point);

	/**
	* This method is called when the right mouse button is released.
	*
	 * @param point The location of the mouse cursor at the time of the event.
	 */
	void OnRButtonUp(const Vector2i& point);

	/**
	* This method is called when the right mouse button is double-clicked.
	*
	 * @param point The location of the mouse cursor at the time of the event.
	 */
	void OnRButtonDblClk(const Vector2i& point);

	/**
	 * This method is called whenever the mouse moves.
	 *
	 * @param point The current location of the mouse cursor.
	 */
	void OnMouseMove(const Vector2i& point);

private:
	GameInput();
	~GameInput();
};

/**
 * Retrieves the Game Input Instance for the game.
 *
 * @param The game input instance
 */
#define GameInputInstance (GameInput::Instance())
