#include "Game.h"
#include "Utilities.h"

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm\gtc\matrix_transform.hpp>

Game::Game()
{
}

Game::~Game()
{
	delete updateTimer;

	StaticGeometry.Unload();
}

void Game::initializeGame()
{
	updateTimer = new Timer();

	InitFullScreenQuad();
	ModeDisplay.Init();
	glEnable(GL_DEPTH_TEST);

	if (!StaticGeometry.Load("./Assets/Shaders/StaticGeometry.vert", "./Assets/Shaders/Phong.frag"))
	{
		std::cout << "Shader failed to Initalize.\n";
		system("pause");
		exit(0);
	}

	if (!NoLight.Load("./Assets/Shaders/StaticGeometry.vert", "./Assets/Shaders/PassThrough.frag"))
	{
		std::cout << "Shader failed to Initalize.\n";
		system("pause");
		exit(0);
	}

	GoatMesh = std::shared_ptr<Mesh>(new Mesh());
	if (!GoatMesh->LoadFromFile("./Assets/Models/Run1.obj"))
	{
		std::cout << "Model failed to load.\n";
		system("pause");
		exit(0);
	}

	LanceMesh = std::shared_ptr<Mesh>(new Mesh());
	if (!LanceMesh->LoadFromFile("./Assets/Models/LanceRun1.obj"))
	{
		std::cout << "Model failed to load.\n";
		system("pause");
		exit(0);
	}

	GroundMesh = std::shared_ptr<Mesh>(new Mesh());
	if (!GroundMesh->LoadFromFile("./Assets/Models/Ground.obj"))
	{
		std::cout << "Model failed to load.\n";
		system("pause");
		exit(0);
	}

	GoatTexture = std::shared_ptr<Texture>(new Texture());
	if (!GoatTexture->Load("./Assets/Textures/GoatKnight.png"))
	{
		system("pause");
		exit(0);
	}

	LanceTexture = std::shared_ptr<Texture>(new Texture());
	if (!LanceTexture->Load("./Assets/Textures/Fire.png"))
	{
		system("pause");
		exit(0);
	}

	GroundTexture = std::shared_ptr<Texture>(new Texture());
	if (!GroundTexture->Load("./Assets/Textures/Ground.png"))
	{
		system("pause");
		exit(0);
	}


	// Entities
	Goat.Init(GoatMesh, GoatTexture);
	Lance.Init(LanceMesh, LanceTexture);
	Ground.Init(GroundMesh, GroundTexture);

	CameraProjection = glm::perspective(60.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 10000.0f);
}

void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	/* FPS Control */
	horizontalAngle += MOUSE_SPEED * float(WINDOW_WIDTH_MID - xpos);
	verticalAngle += MOUSE_SPEED * float(WINDOW_HEIGHT_MID - ypos);

	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
		);

	glm::vec3 up = glm::cross(right, direction);

	if (KeyWDown)
	{
		position += direction * deltaTime * MOVEMENT_SPEED;
	}
	if (KeySDown)
	{
		position -= direction * deltaTime * MOVEMENT_SPEED;
	}
	if (KeyDDown)
	{
		position += right * deltaTime * MOVEMENT_SPEED;
	}
	if (KeyADown)
	{
		position -= right * deltaTime * MOVEMENT_SPEED;
	}

	CameraTransform = glm::lookAt(position, position + direction, up);


	glutWarpPointer(WINDOW_WIDTH_MID, WINDOW_HEIGHT_MID);

}

void Game::draw()
{
	/* Black background */
	glClearColor(0.1, 0.2, 0.3, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Render the scene */
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	/* States */
	if (WireframeOn)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	StaticGeometry.Bind();
	StaticGeometry.SendUniformMat4("uModel", &GoatKnightTransform[0][0], false);
	StaticGeometry.SendUniformMat4("uView", &CameraTransform[0][0], false);
	StaticGeometry.SendUniformMat4("uProj", &CameraProjection[0][0], false);
	StaticGeometry.SendUniform("uTex", 0);
	StaticGeometry.SendUniformMat4("CameraPosition", &CameraTransform[0][0], false);
	StaticGeometry.SendUniform("LightAmbient", glm::vec3(0.15f, 0.15f, 0.15f));

	StaticGeometry.SendUniform("LightSpecularExponent", 50.0f);
	StaticGeometry.SendUniform("Attenuation_Constant", 1.0f);
	StaticGeometry.SendUniform("Attenuation_Linear", 0.1f);
	StaticGeometry.SendUniform("Attenuation_Quadratic", 0.01f);

	Goat.Draw();
	Lance.Draw();
	Ground.Draw();

	StaticGeometry.Unbind();

	renderText();

	glutSwapBuffers();

}

void Game::renderText()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	/* Render Text for mode and values */
	if (TextDisplayOn)
	{
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
		float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);

		ModeDisplay.program.Bind();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::vec4 Color = { 1, 0, 0, 1 };
		FT_Set_Pixel_Sizes(ModeDisplay.face, 0, 24);
		ModeDisplay.program.SendUniform("uColor", Color);

		if (WireframeOn)
		{
			ModeDisplay.Render("1: Wireframe mode is ON", -1 + 8 * sx, 1 - 50 * sy, sx, sy);
		}
		else
		{
			ModeDisplay.Render("1: Wireframe mode is OFF", -1 + 8 * sx, 1 - 50 * sy, sx, sy);
		}

		glDisable(GL_BLEND);
		ModeDisplay.program.Unbind();
	}
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	switch (key)
	{
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	case '1':
		WireframeOn = !WireframeOn;
		break;
	case '4':
		break;
	case '6':
		TextDisplayOn = !TextDisplayOn;
		break;
	case 'w':
		KeyWDown = true;
		break;
	case 'a':
		KeyADown = true;
		break;
	case 's':
		KeySDown = true;
		break;
	case 'd':
		KeyDDown = true;
		break;
	}
}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	switch (key)
	{
	case 32: // the space bar
		break;
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	case 'w':
		KeyWDown = false;
		break;
	case 'a':
		KeyADown = false;
		break;
	case 's':
		KeySDown = false;
		break;
	case 'd':
		KeyDDown = false;
		break;
	}
}

void Game::mouseClicked(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		switch (button)
		{
		case GLUT_LEFT_BUTTON:

			break;
		case GLUT_RIGHT_BUTTON:

			break;
		case GLUT_MIDDLE_BUTTON:

			break;
		}
	}
	else
	{

	}
}

/*
 * mouseMoved(x,y)
 * - this occurs only when the mouse is pressed down
 *   and the mouse has moved.  you are given the x,y locations
 *   in window coordinates (from the top left corner) and thus
 *   must be converted to screen coordinates using the screen to window pixels ratio
 *   and the y must be flipped to make the bottom left corner the origin.
 */
void Game::mouseMoved(int x, int y)
{
	xpos = (float)x;
	ypos = (float)y;
}