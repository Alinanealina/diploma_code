#include "header.h"
using namespace sf;
enemy::enemy() : shown(false), a(0), vx(0), vy(0), sx(0), sy(0)
{
    Image img;
    img.loadFromFile("../../a/en.png");
    img.createMaskFromColor(Color(195, 195, 195));
    tex.loadFromImage(img);
    sprite.setTexture(tex);
}
double enemy::get_x()
{
	return sprite.getPosition().x;
}
double enemy::get_y()
{
    return sprite.getPosition().y;
}
FloatRect enemy::get_rect()
{
    return sprite.getGlobalBounds();
}
void enemy::draw(RenderWindow& window)
{
    anim();
    window.draw(sprite);
}
bool enemy::border_check()
{
    int n = level::mapping.size(), m = level::mapping[0].size();
    if ((sprite.getPosition().x >= -3 * slen) && (sprite.getPosition().x < (m + 3) * slen) &&
        (sprite.getPosition().y >= -3 * slen) && (sprite.getPosition().y < (n + 3) * slen))
        return true;
    return false;
}
void enemy::move(double t)
{
    if (!border_check())
        return;
    sprite.move(vx * t, vy * t);
}

en1::en1(double x, double y)
{
    vx = -0.05;
    sx = 32;
    sy = 32;
    sprite.setPosition(x, y);
    sprite.setTextureRect(IntRect(0, 66, sx, sy));
}
void en1::anim()
{
    a += 0.005;
    if (a >= 4)
        a = 0;
    if (vx < 0)
        sprite.setTextureRect(IntRect(floor(a) * (sx + 1), 66, sx, sy));
    else
        sprite.setTextureRect(IntRect(floor(a) * (sx + 1) + sx, 66, -sx, sy));
}
void en1::move(double t)
{
    int i = floor(sprite.getPosition().y / slen), j = floor((sprite.getPosition().x + vx * t) / slen);
    if (!border_check())
        return;
    vy = 0;
    if ((level::mapping[i][j] < killbox) && ((level::mapping[i][j] > 0) || (level::mapping[i][j + 1] > 0)))
        vx *= -1;
    if (((level::mapping[i + 1][j] <= 0) || (level::mapping[i + 1][j] >= killbox)) &&
        ((level::mapping[i + 1][j + 1] <= 0) || (level::mapping[i + 1][j + 1] >= killbox)))
        vy += 0.15 * t;
    else
        vy = i * slen - sprite.getPosition().y;
    sprite.move(vx * t, vy);
}

en2::en2(double x, double y, double vy)
{
    this->vy = vy;
    sx = 32;
    sy = 55;
    sprite.setPosition(x, y);
    sprite.setTextureRect(IntRect(0, 99, sx, sy));
}
void en2::anim()
{
    a += 0.005;
    if (a >= 2)
        a = 0;
    if (vy < 0)
        sprite.setTextureRect(IntRect(floor(a) * (sx + 1), 99, sx, sy));
    else
        sprite.setTextureRect(IntRect(floor(a) * (sx + 1), 99 + sy, sx, -sy));
}

en3::en3(double x, double y, double vx, double vy)
{
    this->vx = vx;
    this->vy = vy;
    sx = 63;
    sy = 63;
    sprite.setPosition(x, y);
    sprite.setTextureRect(IntRect(132, 66, sx, sy));
}
void en3::anim()
{
    a += 0.003;
    if (a >= 2)
        a = 0;
    sprite.setTextureRect(IntRect(floor(a) * (sx + 1) + 132, 66, sx, sy));
}

en4::en4(double x, double y)
{
    sx = 39;
    sy = 65;
    sprite.setPosition(x, y - slen);
    sprite.setTextureRect(IntRect(0, 0, sx, sy));
}
void en4::anim()
{
    a += 0.003;
    if (a >= 6)
        a = 4;
    sprite.setTextureRect(IntRect(floor(a) * (sx + 1), 0, sx, sy));
}