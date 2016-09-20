#pragma once
#include <windows.h>
#include <memory>
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Texture.h"
#include "Text.h"
#include "Entity.h"

#include <glm\glm.hpp>

#include "Timer.h"


#define WINDOW_WIDTH			800
#define WINDOW_WIDTH_MID		WINDOW_WIDTH / 2
#define WINDOW_HEIGHT			600
#define WINDOW_HEIGHT_MID		WINDOW_HEIGHT / 2
#define FRAMES_PER_SECOND		60

#define MOVEMENT_SPEED			10.0f
#define MOUSE_SPEED				0.001f

class Game {
public:
	Game();
	~Game();

	void initializeGame();
	void update();
	void draw();
	void renderText();

	/* input callback functions */
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);

	/* Data Members */
	std::unique_ptr<Timer> updateTimer;
	float TotalGameTime = 0.0f;
	float InterpolationParam = 0.0f;

	ShaderProgram StaticGeometry;

	std::shared_ptr<Mesh> GoatMesh;
	std::shared_ptr<Mesh> LanceMesh;
	std::shared_ptr<Mesh> GroundMesh;
	std::shared_ptr<Texture> GoatTexture;
	std::shared_ptr<Texture> LanceTexture;
	std::shared_ptr<Texture> GroundTexture;

	Entity Goat;
	Entity Lance;
	Entity Ground;

	glm::mat4 GoatKnightTransform;
	glm::mat4 CameraTransform;
	glm::mat4 CameraProjection;

	// states
	bool WireframeOn = false;
	bool TextDisplayOn = false;

	Text ModeDisplay;

	/* For FPS Control */
	bool KeyWDown = false;
	bool KeyADown = false;
	bool KeySDown = false;
	bool KeyDDown = false;
	float xpos = 0.0f;
	float ypos = 0.0f;
	glm::vec3 position = glm::vec3(0, 2, -5);
	float horizontalAngle = 0.0f;
	float verticalAngle = 0.0f;

private:
};
