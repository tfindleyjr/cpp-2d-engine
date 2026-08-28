#include "Camera.h"

#include <algorithm>

Camera::Camera(
    float viewWidth,
    float viewHeight
)
    : x(0.0f),
      y(0.0f),
      viewWidth(viewWidth),
      viewHeight(viewHeight),
      worldWidth(viewWidth),
      worldHeight(viewHeight)
{
}

void Camera::SetWorldBounds(
    float newWorldWidth,
    float newWorldHeight
)
{
    worldWidth = std::max(newWorldWidth, viewWidth);
    worldHeight = std::max(newWorldHeight, viewHeight);
}

void Camera::Follow(
    float targetX,
    float targetY,
    float targetWidth,
    float targetHeight
)
{
    x = targetX + (targetWidth * 0.5f) - (viewWidth * 0.5f);
    y = targetY + (targetHeight * 0.5f) - (viewHeight * 0.5f);

    const float maxX = std::max(0.0f, worldWidth - viewWidth);
    const float maxY = std::max(0.0f, worldHeight - viewHeight);

    x = std::clamp(x, 0.0f, maxX);
    y = std::clamp(y, 0.0f, maxY);
}

float Camera::GetX() const
{
    return x;
}

float Camera::GetY() const
{
    return y;
}

float Camera::WorldToScreenX(float worldX) const
{
    return worldX - x;
}

float Camera::WorldToScreenY(float worldY) const
{
    return worldY - y;
}
