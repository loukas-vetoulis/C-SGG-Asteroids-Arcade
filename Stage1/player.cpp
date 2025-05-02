#include "player.h"
#include <sgg/graphics.h>
#include "gamestate.h"
#include "util.h"
#include <cmath>
#include <iostream>

void Player::movePlayer(float dt)
{
    float delta_time = dt / 1000.0f;

    float move = 0.0f;
    float move_y = 0.0f;
    if (graphics::getKeyState(graphics::SCANCODE_A))
        move -= 1.0f;
    if (graphics::getKeyState(graphics::SCANCODE_D))
        move += 1.0f;

    m_vx = std::min(m_max_velocity, m_vx + delta_time * move * m_accel);
    m_vx = std::max(-m_max_velocity, m_vx);
    m_vx -= 0.2f * m_vx / (0.1f + fabs(m_vx));

    if (fabs(m_vx) < 0.01f)
        m_vx = 0.0f;
    m_pos_x += delta_time * m_vx / 2;

    if (graphics::getKeyState(graphics::SCANCODE_W))
        move_y -= 1.0f;
    if (graphics::getKeyState(graphics::SCANCODE_S))
        move_y += 1.0f;

    m_vy = std::min(m_max_velocity, m_vy + delta_time * move_y * m_accel);
    m_vy = std::max(-m_max_velocity, m_vy);
    m_vy -= 0.2f * m_vy / (0.1f + fabs(m_vy));

    if (fabs(m_vy) < 0.01f)
        m_vy = 0.0f;
    m_pos_y += delta_time * m_vy / 2;
}

void Player::shootBullet(float dt)
{
    float deltatime = dt / 1000.0f;

    // Get the current state of SCANCODE_0
    graphics::MouseState mouse;
    graphics::getMouseState(mouse);

    bool isPressed = mouse.button_left_released;

    // Check for key release
    if (isPressed) // Key was pressed before and is now released
    {
        // Add a new bullet
        Bullet bullet(m_pos_x, m_pos_y, bullesize, bullesize,-bullespeed * cosf(m_rotation - M_PI/2+ M_PI),bullespeed * sinf(m_rotation - M_PI/2+ M_PI));
        m_bullets.push_back(bullet);
        graphics::playSound(m_state->getFullAssetPath("SpaceBeam.mp3"), 1.0f);
    }

    for (auto& b : m_bullets)
    {
        b.m_pos_x += b.dx * deltatime;
        b.m_pos_y += b.dy * deltatime;
    }

    if (m_bullets.size() > 0)
    {
        auto i = remove_if(m_bullets.begin(), m_bullets.end(), [&](Bullet o) {return
            (o.m_pos_x < 0) || o.m_pos_y < 0 || o.m_pos_x >= m_state->getCanvasWidth() || o.m_pos_y >= m_state->getCanvasHeight(); });
        if (i != m_bullets.end())
           m_bullets.erase(i);
    }   

}

void Player::rotate()
{
    // Get mouse state
    graphics::MouseState mouse;
    graphics::getMouseState(mouse);

    // Convert mouse position from window to canvas coordinates
    float canvas_mouse_x = graphics::windowToCanvasX(mouse.cur_pos_x);
    float canvas_mouse_y = graphics::windowToCanvasY(mouse.cur_pos_y);

    // Calculate the angle between the player and the mouse
    m_rotation = calculateAngle(m_pos_x, m_pos_y, canvas_mouse_x, canvas_mouse_y);
}

void Player::update(float dt)
{
    movePlayer(dt);
    shootBullet(dt);
    rotate();
    wrapCordinates(m_pos_x, m_pos_y);

    GameObject::update(dt);
}

void Player::draw()
{

    // Draw the player at its absolute position
    graphics::setOrientation(m_rotation * radian_to_degrees); // Convert radians to degrees
    graphics::drawRect(m_pos_x, m_pos_y, 0.5f, 0.5f, m_brush_player);
    graphics::resetPose();
    

    for (const auto& b : m_bullets)
    {
        graphics::drawRect(b.m_pos_x, b.m_pos_y, bullesize, bullesize, bullet_brush);
    }

    if (m_state->m_debbuging) {
        debugDraw();
    }
}


void Player::init()
{
    m_pos_x = m_state->getCanvasWidth()/2;
    m_pos_y = m_state->getCanvasHeight() / 2;
    m_width = 0.5f;
    m_accel = 40.0f;
    health = 3;

    m_brush_player.fill_opacity = 1.0f;
    m_brush_player.outline_opacity = 0.0f;
    m_brush_player.texture = m_state->getFullAssetPath("spaceship.png");
    
    init_bullet_brush();
}

void Player::debugDraw()
{

    graphics::Brush debug_brush;
    SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
    SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
    debug_brush.fill_opacity = 0.1f;
    debug_brush.outline_opacity = 1.0f;
    graphics::drawRect(m_pos_x, m_pos_y, m_width, m_height/2, debug_brush);

    // Draw Bullets
    for (int i = 0; i < m_state->getPlayer()->m_bullets.size() > 0; i++)
        drawBullet(i, debug_brush);

    char s[20];
    sprintf_s(s, "(% 5.2f,%5.2f)", m_pos_x, m_pos_y);
    SETCOLOR(debug_brush.fill_color, 1, 0, 0);
    debug_brush.fill_opacity = 1.0f;
    graphics::drawText(m_pos_x, m_pos_y, 0.15f, s, debug_brush);

}

float Player::calculateAngle(float x1, float y1, float x2, float y2)
{
    float angle = atan2(x2 - x1,y2 - y1); // Angle in radians
    return angle; // No need to normalize as atan2 handles it
}

void Player::init_bullet_brush()
{
    bullet_brush.fill_color[0] = 0.53f; // Red channel
    bullet_brush.fill_color[1] = 0.81f; // Green channel
    bullet_brush.fill_color[2] = 0.92f; // Blue channel
    bullet_brush.fill_opacity = 1.0f;   // Fully opaque
    bullet_brush.outline_opacity = 0.0f; // No outline
}

void Player::drawBullet(int i, graphics::Brush debug_brush)
{
    // Create an iterator and advance it to the i-th element

    auto it = m_state->getPlayer()->m_bullets.begin();
    std::advance(it, i);  // Move the iterator to the i-th position

    // Dereference the iterator to get the bullet reference
    Bullet& bullet = *it;

    float x = bullet.m_pos_x;
    float y = bullet.m_pos_y;

    if (m_state->m_debbuging)
        graphics::drawRect(x, y, bullet.m_width, bullet.m_height, debug_brush);
}


