#include "lab_m1/tema/tema.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema/transform2D.h"
#include "lab_m1/tema/object2D.h"

using namespace std;
using namespace m1;


tema::tema()
{
}


tema::~tema()
{
}


void tema::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);

    // Set side sizes
    squareSide = 100;
    bulletSide = 20;
    mapSide = 2000;

    // Initialize the translation steps
    translatePlayerX = 0;
    translatePlayerY = 0;
    translateEyeX = squareSide / 2 + 10;
    translateEyeY = squareSide / 2 + 10;
    translateGunX = squareSide;
    translateGunY = squareSide / 2;
    translateMapX = 0;
    translateMapY = 0;
    translateBulletX = 0;
    translateBulletY = 0;
    translateEnemyX = 1000;
    translateEnemyY = 1000;
    translateLegX = 0;
    translateLegY = 0;
    translateHealthBarX = 0;
    translateHealthBarY = 0;
    translateObstacle1X = 100;
    translateObstacle1Y = 300;
    translateObstacle2X = 1500;
    translateObstacle2Y = 1000;
    translateObstacle3X = 1400;
    translateObstacle3Y = 200;
    translateObstacle4X = 200;
    translateObstacle4Y = 1600;
    translateObstacle5X = 800;
    translateObstacle5Y = 600;
    translateObstacle6X = 200;
    translateObstacle6Y = 1300;

    // Initialize the scale factors
    scaleX = 1;
    scaleY = 1;
    scaleHealthBarX = 2.5f;
    scaleHealthBarY = 0.4f;
    scaleHealthBarFrameX = 2.5f;
    scaleHealthBarFrameY = 0.4f;
    scaleObstacle1X = 1.5f;
    scaleObstacle1Y = 4;
    scaleObstacle2X = 1.2f;
    scaleObstacle2Y = 5;
    scaleObstacle3X = 3;
    scaleObstacle3Y = 2;
    scaleObstacle4X = 4;
    scaleObstacle4Y = 1.2f;
    scaleObstacle5X = 2;
    scaleObstacle5Y = 2;
    scaleObstacle6X = 1.2f;
    scaleObstacle6Y = 4;

    // Initialize angular steps
    angularStep = 0;
    gunAngle = 0.78f;
    cursorAngle = 0;
    playerAngle = 0;

    // Initialize player and enemy speed
    speed = 500;

    // Initialize eye distance for player
    eyeDistance = 40;

    // Initialize bullet movement
    bulletStart = 0;
    bulletInterval = 0;

    // Initialize number of enemies killed
    enemiesKilled = 0;

    // Initialize number of hits taken from enemies
    damageTaken = 0;

    // Initialize health value
    health = 100;


    // Player mesh
    Mesh* player = object2D::CreateSquare("player", corner, squareSide, glm::vec3(1, 0.6f, 0.8f), true);
    AddMeshToList(player);

    // Eye mesh
    Mesh* eye = object2D::CreateSquare("eye", corner, squareSide, glm::vec3(0, 0, 0), true);
    AddMeshToList(eye);

    // Gun mesh
    Mesh* gun = object2D::CreateSquare("gun", corner, squareSide, glm::vec3(0.58f, 0.29f, 0), true);
    AddMeshToList(gun);

    // Map mesh
    Mesh* map = object2D::CreateSquare("map", corner, mapSide, glm::vec3(0.4f, 0.8f, 0), true);
    AddMeshToList(map);

    // Bullet mesh
    Mesh* bullet = object2D::CreateSquare("bullet", corner, bulletSide, glm::vec3(0, 0, 0), true);
    AddMeshToList(bullet);

    // Enemy mesh
    Mesh* enemy = object2D::CreateSquare("enemy", corner, squareSide, glm::vec3(0.8f, 0, 0), true);
    AddMeshToList(enemy);

    // Enemy leg
    Mesh* leg = object2D::CreateSquare("leg", corner, squareSide, glm::vec3(0.6f, 0, 0), true);
    AddMeshToList(leg);

    // Obstacle mesh
    Mesh* obstacle = object2D::CreateSquare("obstacle", corner, squareSide, glm::vec3(0.3f, 0.3f, 0.3f), true);
    AddMeshToList(obstacle);

    // Healthbar mesh
    Mesh* healthbar = object2D::CreateSquare("healthbar", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(healthbar); 
    Mesh* healthbarframe = object2D::CreateSquare("healthbarframe", corner, squareSide, glm::vec3(0.6f, 0, 0));
    AddMeshToList(healthbarframe);

    glLineWidth(3);
}


void tema::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.4f, 0.4f, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void tema::Update(float deltaTimeSeconds)
{
    // Center camera
    glm::vec3 camera = glm::vec3(translatePlayerX - window->GetResolution()[0] / 2 + squareSide / 2,
                                 translatePlayerY - window->GetResolution()[1] / 2 + squareSide / 2, 50);
    GetSceneCamera()->SetPosition(camera);
  
    // Healthbar
    {
        translateHealthBarX = translatePlayerX + 400;
        translateHealthBarY = translatePlayerY + 300;
        modelMatrixHealthBarFrame = transform2D::Translate(translateHealthBarX, translateHealthBarY);
        modelMatrixHealthBarFrame *= transform2D::Scale(scaleHealthBarFrameX, scaleHealthBarFrameY);
        RenderMesh2D(meshes["healthbarframe"], shaders["VertexColor"], modelMatrixHealthBarFrame);

        modelMatrixHealthBar = transform2D::Translate(translateHealthBarX, translateHealthBarY);
        modelMatrixHealthBar *= transform2D::Scale(scaleHealthBarX, scaleHealthBarY);
        RenderMesh2D(meshes["healthbar"], shaders["VertexColor"], modelMatrixHealthBar);
    }

    // Player
    {
        // Rotation after cursor
        cursorAngle = atan2(window->GetCursorPosition()[1] - window->GetResolution()[1]/2,
                            window->GetCursorPosition()[0] - window->GetResolution()[0]/2);

        modelMatrixPlayer = transform2D::Translate(translatePlayerX + squareSide / 2, translatePlayerY + squareSide / 2);
        modelMatrixPlayer *= transform2D::Rotate(- cursorAngle - 3.14f / 2);
        modelMatrixPlayer *= transform2D::Translate(-squareSide/2, -squareSide/2);

        // Eye 1 player 
        modelMatrixEye1 = modelMatrixPlayer * transform2D::Translate(translateEyeX, translateEyeY);
        modelMatrixEye1 *= transform2D::Scale(0.2f, 0.2f);
        RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrixEye1);

        // Eye 2 player 
        modelMatrixEye2 = modelMatrixPlayer * transform2D::Translate(translateEyeX - eyeDistance, translateEyeY);
        modelMatrixEye2 *= transform2D::Scale(0.2f, 0.2f);
        RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrixEye2);

        // Player
        RenderMesh2D(meshes["player"], shaders["VertexColor"], modelMatrixPlayer);

        // Gun
        modelMatrixGun = modelMatrixPlayer * transform2D::Translate(translateGunX, translateGunY);
        modelMatrixGun *= transform2D::Rotate(gunAngle);
        modelMatrixGun *= transform2D::Scale(0.2f, 1);
        RenderMesh2D(meshes["gun"], shaders["VertexColor"], modelMatrixGun);
    }

    // Enemy 
    {
        // Bullet-enemy collision
        if (modelMatrixBullet[2][0] < translateEnemyX + squareSide &&
            modelMatrixBullet[2][1] < translateEnemyY + squareSide &&
            modelMatrixBullet[2][0] > translateEnemyX &&
            modelMatrixBullet[2][1] > translateEnemyY + bulletSide) {
            // Random enemy respawn and enemy-map collision
            translateEnemyX = rand() % (int)mapSide;
            while (!(translateEnemyX < modelMatrixPlayer[2][0] - 500 || translateEnemyX > modelMatrixPlayer[2][0] + 500)) {
                translateEnemyX = rand() % (int)mapSide;
            }
            translateEnemyY = rand() % (int)mapSide;
            while (!(translateEnemyY < modelMatrixPlayer[2][1] - 500 || translateEnemyY > modelMatrixPlayer[2][1] + 500)) {
                translateEnemyY = rand() % (int)mapSide;
            }

            // Increase number of enemies killed
            enemiesKilled++;
            // Print the score
            printf("SCORE: %d - %d\n", enemiesKilled, damageTaken);
        }
        
        // Enemy-player collision
        if (translatePlayerX < translateEnemyX + squareSide &&
            translatePlayerY < translateEnemyY + squareSide &&
            translatePlayerX + squareSide > translateEnemyX &&
            translatePlayerY + squareSide > translateEnemyY) {
            translateEnemyX = rand() % (int)mapSide;
            while (!(translateEnemyX < modelMatrixPlayer[2][0] - 500 || translateEnemyX > modelMatrixPlayer[2][0] + 500)) {
                translateEnemyX = rand() % (int)mapSide;
            }
            translateEnemyY = rand() % (int)mapSide;
            while (!(translateEnemyY < modelMatrixPlayer[2][1] - 500 || translateEnemyY > modelMatrixPlayer[2][1] + 500)) {
                translateEnemyY = rand() % (int)mapSide;
            }

            // Increase number of hits taken
            damageTaken++;
            // Print the score
            printf("SCORE: %d - %d\n", enemiesKilled, damageTaken);

            // Decrease health level 
            if (scaleHealthBarX > 0) {
                scaleHealthBarX -= 0.25;
            }
            // Finish the game when health is 0
            else {
                printf("---GAME OVER---\n");
                printf("Final SCORE : %d - %d\n", enemiesKilled, damageTaken);
                window->Close();
            }
            
        }

        // Enemy rotation after player
        playerAngle = atan2((modelMatrixPlayer * transform2D::Translate(squareSide / 2, squareSide / 2))[2][0] 
                                            - (modelMatrixEnemy * transform2D::Translate(squareSide / 2, squareSide / 2))[2][0],
                            (modelMatrixPlayer * transform2D::Translate(squareSide / 2, squareSide / 2))[2][1] 
                                            - (modelMatrixEnemy * transform2D::Translate(squareSide / 2, squareSide / 2))[2][1]);
        
        float normalize = sqrt((translateEnemyX - translatePlayerX) * (translateEnemyX - translatePlayerX) +
            (translateEnemyY - translatePlayerY) * (translateEnemyY - translatePlayerY));

        // Enemy follows player
        translateEnemyX += (translatePlayerX - translateEnemyX) / normalize * deltaTimeSeconds * speed;
        translateEnemyY += (translatePlayerY - translateEnemyY) / normalize * deltaTimeSeconds * speed;
        modelMatrixEnemy = transform2D::Translate(translateEnemyX + squareSide / 2, translateEnemyY + squareSide / 2);
        modelMatrixEnemy *= transform2D::Rotate(-playerAngle);
        modelMatrixEnemy *= transform2D::Translate(-squareSide / 2, -squareSide / 2);

        // Enemy eye 1
        modelMatrixEnemyEye1 = modelMatrixEnemy * transform2D::Translate(translateEyeX, translateEyeY);
        modelMatrixEnemyEye1 *= transform2D::Scale(0.2f, 0.2f);
        RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrixEnemyEye1);

        // Enemy eye 2
        modelMatrixEnemyEye2 = modelMatrixEnemy * transform2D::Translate(translateEyeX - eyeDistance, translateEyeY);
        modelMatrixEnemyEye2 *= transform2D::Scale(0.2f, 0.2f);
        RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrixEnemyEye2);

        // Enemy leg 1
        translateLegX = 0;
        translateLegY = squareSide;
        modelMatrixEnemyLeg = modelMatrixEnemy * transform2D::Translate(translateLegX, translateLegY);
        modelMatrixEnemyLeg *= transform2D::Scale(0.3f, 0.3f);
        RenderMesh2D(meshes["leg"], shaders["VertexColor"], modelMatrixEnemyLeg);

        // Enemy leg 2
        translateLegX = squareSide - 30;
        translateLegY = squareSide;
        modelMatrixEnemyLeg = modelMatrixEnemy * transform2D::Translate(translateLegX, translateLegY);
        modelMatrixEnemyLeg *= transform2D::Scale(0.3f, 0.3f);
        RenderMesh2D(meshes["leg"], shaders["VertexColor"], modelMatrixEnemyLeg);

        // Enemy
        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrixEnemy);
    }

    // Bullet
    {
        bulletInterval += deltaTimeSeconds; // Time interval for a shot (> 0.5 sec)
         
        // Collision bullet-map and bullet-obstacles
        if ((modelMatrixBullet[2][0] < translateMapX + mapSide &&
            modelMatrixBullet[2][1] < translateMapY + mapSide &&
            modelMatrixBullet[2][0] > translateMapX &&
            modelMatrixBullet[2][1] > translateMapY + bulletSide)
            &&
            (!(translateObstacle1X < modelMatrixBullet[2][0] + bulletSide &&
                translateObstacle1X + squareSide * scaleObstacle1X > modelMatrixBullet[2][0] &&
                translateObstacle1Y < modelMatrixBullet[2][1] + bulletSide &&
                squareSide * scaleObstacle1Y + translateObstacle1Y > modelMatrixBullet[2][1]))
            &&
            (!(translateObstacle2X < modelMatrixBullet[2][0] + bulletSide &&
                translateObstacle2X + squareSide * scaleObstacle2X > modelMatrixBullet[2][0] &&
                translateObstacle2Y < modelMatrixBullet[2][1] + bulletSide &&
                squareSide * scaleObstacle2Y + translateObstacle2Y > modelMatrixBullet[2][1]))
            &&
            (!(translateObstacle3X < modelMatrixBullet[2][0] + bulletSide &&
                translateObstacle3X + squareSide * scaleObstacle3X > modelMatrixBullet[2][0] &&
                translateObstacle3Y < modelMatrixBullet[2][1] + bulletSide &&
                squareSide * scaleObstacle3Y + translateObstacle3Y > modelMatrixBullet[2][1]))
            &&
            (!(translateObstacle4X < modelMatrixBullet[2][0] + bulletSide &&
                translateObstacle4X + squareSide * scaleObstacle4X > modelMatrixBullet[2][0] &&
                translateObstacle4Y < modelMatrixBullet[2][1] + bulletSide &&
                squareSide * scaleObstacle4Y + translateObstacle4Y > modelMatrixBullet[2][1]))
            &&
            (!(translateObstacle5X < modelMatrixBullet[2][0] + bulletSide &&
                translateObstacle5X + squareSide * scaleObstacle5X > modelMatrixBullet[2][0] &&
                translateObstacle5Y < modelMatrixBullet[2][1] + bulletSide &&
                squareSide * scaleObstacle5Y + translateObstacle5Y > modelMatrixBullet[2][1]))
            &&
            (!(translateObstacle6X < modelMatrixBullet[2][0] + bulletSide &&
                translateObstacle6X + squareSide * scaleObstacle6X > modelMatrixBullet[2][0] &&
                translateObstacle6Y < modelMatrixBullet[2][1] + bulletSide &&
                squareSide * scaleObstacle6Y + translateObstacle6Y > modelMatrixBullet[2][1])))
        {
            bulletStart += 200 * deltaTimeSeconds;
            modelMatrixBullet *= transform2D::Translate(0, bulletStart);
            RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrixBullet);
        }
        else {
            modelMatrixBullet *= transform2D::Translate(3000, 3000);
        }
    }

    // Obstacle 1
    {
        modelMatrixObstacle = transform2D::Translate(translateObstacle1X, translateObstacle1Y);
        modelMatrixObstacle *= transform2D::Scale(scaleObstacle1X, scaleObstacle1Y);
        RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrixObstacle);
    }

    // Obstacle 2
    {
        modelMatrixObstacle = transform2D::Translate(translateObstacle2X, translateObstacle2Y);
        modelMatrixObstacle *= transform2D::Scale(scaleObstacle2X, scaleObstacle2Y);
        RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrixObstacle);
    }

    // Obstacle 3
    {
        modelMatrixObstacle = transform2D::Translate(translateObstacle3X, translateObstacle3Y);
        modelMatrixObstacle *= transform2D::Scale(scaleObstacle3X, scaleObstacle3Y);
        RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrixObstacle);
    }

    // Obstacle 4
    {
        modelMatrixObstacle = transform2D::Translate(translateObstacle4X, translateObstacle4Y);
        modelMatrixObstacle *= transform2D::Scale(scaleObstacle4X, scaleObstacle4Y);
        RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrixObstacle);
    }

    // Obstacle 5
    {
        modelMatrixObstacle = transform2D::Translate(translateObstacle5X, translateObstacle5Y);
        modelMatrixObstacle *= transform2D::Scale(scaleObstacle5X, scaleObstacle5Y);
        RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrixObstacle);
    }

    // Obstacle 6
    {
        modelMatrixObstacle = transform2D::Translate(translateObstacle6X, translateObstacle6Y);
        modelMatrixObstacle *= transform2D::Scale(scaleObstacle6X, scaleObstacle6Y);
        RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrixObstacle);
    }

    // Map
    {
        modelMatrixMap = transform2D::Translate(translateMapX, translateMapY);
        RenderMesh2D(meshes["map"], shaders["VertexColor"], modelMatrixMap);
    }

   
}


void tema::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void tema::OnInputUpdate(float deltaTime, int mods)
{
    // Move with W and collision with upper side of map
    if (window->KeyHold(GLFW_KEY_W) && (translatePlayerY < translateMapY + mapSide - squareSide)) {
            // Collision player-obstacle 1
        if (!(translateObstacle1X < translatePlayerX + squareSide &&
            translateObstacle1X + squareSide*scaleObstacle1X > translatePlayerX &&
            translateObstacle1Y < translatePlayerY + squareSide + deltaTime * speed &&
            squareSide*scaleObstacle1Y + translateObstacle1Y > translatePlayerY)
            &&
            // Collision player-obstacle 2
            !(translateObstacle2X < translatePlayerX + squareSide &&
            translateObstacle2X + squareSide * scaleObstacle2X > translatePlayerX &&
            translateObstacle2Y < translatePlayerY + squareSide + deltaTime * speed &&
            squareSide * scaleObstacle2Y + translateObstacle2Y > translatePlayerY)
            &&
            // Collision player-obstacle 3
            !(translateObstacle3X < translatePlayerX + squareSide &&
                translateObstacle3X + squareSide * scaleObstacle3X > translatePlayerX &&
                translateObstacle3Y < translatePlayerY + squareSide + deltaTime * speed &&
                squareSide * scaleObstacle3Y + translateObstacle3Y > translatePlayerY)
            &&
            // Collision player-obstacle 4
            !(translateObstacle4X < translatePlayerX + squareSide &&
                translateObstacle4X + squareSide * scaleObstacle4X > translatePlayerX &&
                translateObstacle4Y < translatePlayerY + squareSide + deltaTime * speed &&
                squareSide * scaleObstacle4Y + translateObstacle4Y > translatePlayerY)
            &&
            // Collision player-obstacle 5
            !(translateObstacle5X < translatePlayerX + squareSide &&
                translateObstacle5X + squareSide * scaleObstacle5X > translatePlayerX &&
                translateObstacle5Y < translatePlayerY + squareSide + deltaTime * speed &&
                squareSide * scaleObstacle5Y + translateObstacle5Y > translatePlayerY)
            &&
            // Collision player-obstacle 6
            !(translateObstacle6X < translatePlayerX + squareSide &&
                translateObstacle6X + squareSide * scaleObstacle6X > translatePlayerX &&
                translateObstacle6Y < translatePlayerY + squareSide + deltaTime * speed &&
                squareSide * scaleObstacle6Y + translateObstacle6Y > translatePlayerY))
            
            translatePlayerY += deltaTime * speed;
    }
    // Move with S and collision with lower side of map
    if (window->KeyHold(GLFW_KEY_S) && translatePlayerY > translateMapY) {
            // Collision player-obstacle 1
        if (!(translateObstacle1X < translatePlayerX + squareSide &&
            translateObstacle1X + squareSide * scaleObstacle1X > translatePlayerX &&
            translateObstacle1Y < translatePlayerY + squareSide &&
            squareSide * scaleObstacle1Y + translateObstacle1Y + deltaTime * speed > translatePlayerY)
            &&
            // Collision player-obstacle 2
            !(translateObstacle2X < translatePlayerX + squareSide &&
                translateObstacle2X + squareSide * scaleObstacle2X > translatePlayerX &&
                translateObstacle2Y < translatePlayerY + squareSide &&
                squareSide * scaleObstacle2Y + translateObstacle2Y + deltaTime * speed > translatePlayerY)
            &&
            // Collision player-obstacle 3
            !(translateObstacle3X < translatePlayerX + squareSide &&
                translateObstacle3X + squareSide * scaleObstacle3X > translatePlayerX &&
                translateObstacle3Y < translatePlayerY + squareSide &&
                squareSide * scaleObstacle3Y + translateObstacle3Y + deltaTime * speed > translatePlayerY)
            &&
            // Collision player-obstacle 4
            !(translateObstacle4X < translatePlayerX + squareSide &&
                translateObstacle4X + squareSide * scaleObstacle4X > translatePlayerX &&
                translateObstacle4Y < translatePlayerY + squareSide &&
                squareSide * scaleObstacle4Y + translateObstacle4Y + deltaTime * speed > translatePlayerY)
            &&
            // Collision player-obstacle 5
            !(translateObstacle5X < translatePlayerX + squareSide &&
                translateObstacle5X + squareSide * scaleObstacle5X > translatePlayerX &&
                translateObstacle5Y < translatePlayerY + squareSide &&
                squareSide * scaleObstacle5Y + translateObstacle5Y + deltaTime * speed > translatePlayerY)
            &&
            // Collision player-obstacle 6
            !(translateObstacle6X < translatePlayerX + squareSide &&
                translateObstacle6X + squareSide * scaleObstacle6X > translatePlayerX &&
                translateObstacle6Y < translatePlayerY + squareSide &&
                squareSide * scaleObstacle6Y + translateObstacle6Y + deltaTime * speed > translatePlayerY))
            
            translatePlayerY -= deltaTime * speed;
    }
    // Move with A and collision with left side of map
    if (window->KeyHold(GLFW_KEY_A) && translatePlayerX > translateMapX) {
            // Collision player-obstacle 1
        if (!(translateObstacle1X < translatePlayerX + squareSide &&
            translateObstacle1X + squareSide * scaleObstacle1X + deltaTime * speed > translatePlayerX &&
            translateObstacle1Y < translatePlayerY + squareSide &&
            squareSide * scaleObstacle1Y + translateObstacle1Y > translatePlayerY)
            &&
            // Collision player-obstacle 2
            !(translateObstacle2X < translatePlayerX + squareSide &&
                translateObstacle2X + squareSide * scaleObstacle2X + deltaTime * speed > translatePlayerX &&
                translateObstacle2Y < translatePlayerY + squareSide &&
                squareSide * scaleObstacle2Y + translateObstacle2Y > translatePlayerY)
            &&
            // Collision player-obstacle 3
            !(translateObstacle3X < translatePlayerX + squareSide &&
                translateObstacle3X + squareSide * scaleObstacle3X + deltaTime * speed > translatePlayerX &&
                translateObstacle3Y < translatePlayerY + squareSide &&
                squareSide * scaleObstacle3Y + translateObstacle3Y > translatePlayerY)
            &&
            // Collision player-obstacle 4
            !(translateObstacle4X < translatePlayerX + squareSide &&
                translateObstacle4X + squareSide * scaleObstacle4X + deltaTime * speed > translatePlayerX &&
                translateObstacle4Y < translatePlayerY + squareSide &&
                squareSide * scaleObstacle4Y + translateObstacle4Y > translatePlayerY)
            &&
            // Collision player-obstacle 5
            !(translateObstacle5X < translatePlayerX + squareSide &&
                translateObstacle5X + squareSide * scaleObstacle5X + deltaTime * speed > translatePlayerX &&
                translateObstacle5Y < translatePlayerY + squareSide &&
                squareSide * scaleObstacle5Y + translateObstacle5Y > translatePlayerY)
            &&
            // Collision player-obstacle 6
            !(translateObstacle6X < translatePlayerX + squareSide &&
                translateObstacle6X + squareSide * scaleObstacle6X + deltaTime * speed > translatePlayerX &&
                translateObstacle6Y < translatePlayerY + squareSide &&
                squareSide * scaleObstacle6Y + translateObstacle6Y > translatePlayerY))
            
            translatePlayerX -= deltaTime * speed;
    }
    // Move with D and collision with right side of map
    if (window->KeyHold(GLFW_KEY_D) && translatePlayerX < translateMapX + mapSide - squareSide) {
            // Collision player-obstacle 1
        if (!(translateObstacle1X < translatePlayerX + squareSide + deltaTime * speed &&
            translateObstacle1X + squareSide * scaleObstacle1X > translatePlayerX &&
            translateObstacle1Y < translatePlayerY + squareSide &&
            squareSide * scaleObstacle1Y + translateObstacle1Y > translatePlayerY)
            &&
            // Collision player-obstacle 2
            !(translateObstacle2X < translatePlayerX + squareSide + deltaTime * speed &&
                translateObstacle2X + squareSide * scaleObstacle2X > translatePlayerX &&
                translateObstacle2Y < translatePlayerY + squareSide &&
                squareSide * scaleObstacle2Y + translateObstacle2Y > translatePlayerY)
            &&
            // Collision player-obstacle 3
            !(translateObstacle3X < translatePlayerX + squareSide + deltaTime * speed &&
                translateObstacle3X + squareSide * scaleObstacle3X > translatePlayerX &&
                translateObstacle3Y < translatePlayerY + squareSide &&
                squareSide * scaleObstacle3Y + translateObstacle3Y > translatePlayerY)
            &&
            // Collision player-obstacle 4
            !(translateObstacle4X < translatePlayerX + squareSide + deltaTime * speed &&
                translateObstacle4X + squareSide * scaleObstacle4X > translatePlayerX &&
                translateObstacle4Y < translatePlayerY + squareSide &&
                squareSide * scaleObstacle4Y + translateObstacle4Y > translatePlayerY)
            &&
            // Collision player-obstacle 5
            !(translateObstacle5X < translatePlayerX + squareSide + deltaTime * speed &&
                translateObstacle5X + squareSide * scaleObstacle5X > translatePlayerX &&
                translateObstacle5Y < translatePlayerY + squareSide &&
                squareSide * scaleObstacle5Y + translateObstacle5Y > translatePlayerY)
            &&
            // Collision player-obstacle 6
            !(translateObstacle6X < translatePlayerX + squareSide + deltaTime * speed &&
                translateObstacle6X + squareSide * scaleObstacle6X > translatePlayerX &&
                translateObstacle6Y < translatePlayerY + squareSide &&
                squareSide * scaleObstacle6Y + translateObstacle6Y > translatePlayerY))
            
            translatePlayerX += deltaTime * speed;
    }
}


void tema::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void tema::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void tema::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void tema::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event

    // Bullet movement
    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && bulletInterval > 0.5f) {
        bulletInterval = 0; // Reset time interval for a shot
        bulletStart = 0; 
        // Spawn the bullet
        modelMatrixBullet = modelMatrixPlayer;
        modelMatrixBullet *= transform2D::Translate(squareSide / 2, squareSide/2);
    }
}


void tema::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void tema::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void tema::OnWindowResize(int width, int height)
{
}
