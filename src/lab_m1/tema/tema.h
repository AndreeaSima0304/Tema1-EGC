#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class tema : public gfxc::SimpleScene
    {
     public:
        tema();
        ~tema();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        float cx, cy;
        float squareSide, bulletSide, mapSide;
        float speed;
        float translatePlayerX, translatePlayerY, translateEyeX, translateEyeY,
              translateMapX, translateMapY,
              translateEnemyX, translateEnemyY, translateLegX, translateLegY,
              translateBulletX, translateBulletY, translateHealthBarX, translateHealthBarY,
              translateObstacle1X, translateObstacle1Y,
              translateObstacle2X, translateObstacle2Y,
              translateObstacle3X, translateObstacle3Y,
              translateObstacle4X, translateObstacle4Y,
              translateObstacle5X, translateObstacle5Y,
              translateObstacle6X, translateObstacle6Y;
        float scaleX, scaleY, scaleHealthBarX, scaleHealthBarY, scaleHealthBarFrameX, scaleHealthBarFrameY,
              scaleObstacle1X, scaleObstacle1Y, scaleObstacle2X, scaleObstacle2Y, 
              scaleObstacle3X, scaleObstacle3Y, scaleObstacle4X, scaleObstacle4Y, 
              scaleObstacle5X, scaleObstacle5Y, scaleObstacle6X, scaleObstacle6Y;
        float angularStep, cursorAngle, playerAngle;
        float eyeDistance;
        float bulletStart;
        float bulletInterval;
        float health;
        int enemiesKilled;
        int damageTaken;
        glm::mat3 modelMatrixPlayer;
        glm::mat3 modelMatrixEye1;
        glm::mat3 modelMatrixEye2;
        glm::mat3 modelMatrixMap;
        glm::mat3 modelMatrixEnemy;
        glm::mat3 modelMatrixEnemyEye1;
        glm::mat3 modelMatrixEnemyEye2;
        glm::mat3 modelMatrixEnemyLeg;
        glm::mat3 modelMatrixBullet;
        glm::mat3 modelMatrixObstacle;
        glm::mat3 modelMatrixHealthBar;
        glm::mat3 modelMatrixHealthBarFrame;
       
    };
}   // namespace m1
