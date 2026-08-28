#pragma once

class Camera
{
public:
    Camera(
        float viewWidth = 1280.0f,
        float viewHeight = 720.0f
    );

    void SetWorldBounds(
        float worldWidth,
        float worldHeight
    );

    void Follow(
        float targetX,
        float targetY,
        float targetWidth,
        float targetHeight
    );

    float GetX() const;
    float GetY() const;

    float WorldToScreenX(float worldX) const;
    float WorldToScreenY(float worldY) const;

private:
    float x;
    float y;

    float viewWidth;
    float viewHeight;

    float worldWidth;
    float worldHeight;
};
