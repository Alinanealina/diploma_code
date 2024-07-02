#include "header.h"
using namespace sf;
double character::start_i = 1, character::start_j = 1;
character::character() : a(0), jh(0), ground(true), jump(true), dead(false), right(true), mute(false), sx(slen), sy(2 * slen), spr_sx(48), spr_sy(67), rect(Vector2f(sx, sy))
{
    Image image;
    image.loadFromFile("../../a/sp.png");
    image.createMaskFromColor(Color(195, 195, 195));
    image.createMaskFromColor(Color(136, 0, 21));
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    sprite.setTextureRect(IntRect(0, 0, spr_sx, spr_sy));
    sprite.setOrigin(spr_sx / 2, spr_sy);
    restart();

    sbuf.loadFromFile("../../bgm/MaouDamashii_Footsteps01.wav");
    sound.setBuffer(sbuf);
    sound.setVolume(0);
}
void character::restart()
{
    ground = true;
    jump = true;
    dead = false;
    right = true;
    jh = 0;
    x = start_j * slen;
    y = start_i * slen - sy;
    rect.setPosition(x, y);
    sprite.setPosition(x + sx / 2, y + sy);
}
double character::get_x()
{
    return x;
}
double character::get_y()
{
    return y;
}
void character::draw(RenderWindow& window)
{
    window.draw(sprite);
}
void character::to_black()
{
    sprite.setColor(Color::Black);
    sound.setVolume(25);
}

bool character::check_block(int i, int j, int a)
{
    int kj;
    for (kj = j; kj <= j + sx / slen; kj++)
    {
        if (level::mapping[i][kj] == a)
            return true;
    }
    return false;
}
int character::check_trigger(double& x, double& y)
{
    int i = floor(y / slen + sy / slen - 1), j = floor(x / slen), kj;
    for (kj = j; kj <= j + sx / slen; kj++)
    {
        if (level::mapping[i][kj] >= trigger)
        {
            x = kj * slen;
            y = i * slen;
            return level::mapping[i][kj];
        }
    }
    return 0;
}
bool character::is_solid(int i, int j)
{
    if ((level::mapping[i][j] > 0) && (level::mapping[i][j] < spike_up))
        return true;
    return false;
}
bool character::death_check(int i, int j)
{
    int i2 = sy / slen, k;
    FloatRect r = sprite.getGlobalBounds();
    for (k = i; k < i + i2; k++)
    {
        if (check_block(k, j, spike_up) || check_block(k, j, spike_down) ||
            check_block(k, j, fake_spike_up) || check_block(k, j, fake_spike_down) ||
            check_block(k, j, killbox))
            return true;
    }
    for (k = 0; k < level::en.size(); k++)
    {
        if (r.intersects(level::en[k]->get_rect()))
            return true;
    }
    return false;
}

void character::anim(int i, int k)
{
    a += 0.004;
    if (a >= i)
        a = 0;
    if (right)
        sprite.setTextureRect(IntRect((floor(a) + k) * (spr_sx + 1), 0, spr_sx, spr_sy));
    else
        sprite.setTextureRect(IntRect((floor(a) + k) * (spr_sx + 1) + spr_sx, 0, -spr_sx, spr_sy));

    if (!mute && (i == 4) && ((int)floor(a) % 2 != 0))
        sound.play();
}
void character::control(double t, bool mute)
{
    int i = floor(y / slen), j = floor(x / slen);
    double dx = 0, dy = 0;
    this->mute = mute;
    if (dead)
        return;

    if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
    {
        dx -= 0.2 * t;
        right = false;
        anim(4, 3);
    }
    if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
    {
        dx += 0.2 * t;
        right = true;
        anim(4, 3);
    }
    if (dx == 0)
        anim(3, 0);

    if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) && (ground || jump))
    {
        ground = false;
        jump = false;
        jh = -0.6;
    }
    if (!ground)
    {
        jh += 0.001 * t;
        dy = jh * t;
    }
    else
        fall_check(i, j);
    if (dy < 0)
        anim(1, 7);
    else if (dy > 0)
        anim(1, 8);

    if (dy > 0)
        collision_fall(i, j, dy);
    else if (dy < 0)
        collision_rise(i, j, dy);
    collision_x(i, j, dx);

    dead = death_check(i, j);
    if (Keyboard::isKeyPressed(Keyboard::E))
    {
        if (check_block(i + sy / slen - 1, j, door))
        {
            next = true;
            level::chp = false;
        }
        else if (check_block(i + sy / slen - 1, j, fake_door))
            dead = true;
    }
    if (dead)
        a = 0;

    rect.move(dx, dy);
    sprite.move(dx, dy);
    x = rect.getPosition().x;
    y = rect.getPosition().y;
}

void character::collision_x(int i, int j, double& dx)
{
    int i2 = sy / slen, j2 = sx / slen, k, M = level::mapping[0].size();
    for (k = i; k < i + i2; k++)
    {
        if (((dx > 0) && (j + j2 < M) && is_solid(k, j + j2) && (level::mapping[k][j + j2] != semi_sol) && (dx + x >= j * slen)) ||     // право
            ((dx < 0) && (j - 1 >= 0) && is_solid(k, j - 1) && (level::mapping[k][j - 1] != semi_sol) && (dx + x < j * slen)))       // лево
        {
            dx = j * slen - x;
            break;
        }
    }
}
void character::collision_fall(int i, int j, double& dy)
{
    int i2 = sy / slen, j2 = sx / slen, ki = i + i2 - 1, kj, N = level::mapping.size(), c = 0;
    for (kj = j; kj <= j + j2; kj++)
    {
        if (!((i + i2 < N) &&
            (is_solid(i + i2, kj) || (level::mapping[i + i2][kj] == spike_down)) && ((level::mapping[ki][kj] <= 0) || (level::mapping[ki][kj] >= trigger))))
            c++;
    }
    if (c <= j2)
    {
        dy = i * slen - y;
        ground = true;
        jump = true;
        if (!mute)
            sound.play();
    }
}
void character::collision_rise(int i, int j, double& dy)
{
    int j2 = sx / slen, ki = i, kj, c = 0;
    for (kj = j; kj <= j + j2; kj++)
    {
        if (!((floor((dy + y) / slen) < i) && (i - 1 >= 0) &&
            (level::mapping[i - 1][kj] != semi_sol) &&
            (is_solid(i - 1, kj) || (level::mapping[i - 1][kj] == spike_up)) && ((level::mapping[ki][kj] <= 0) || (level::mapping[ki][kj] >= trigger))))
            c++;
    }
    if (c <= j2)
    {
        dy = i * slen - y;
        jh = 0;
    }
}

void character::fall_check(int i, int j)
{
    int kj, i2 = sy / slen, N = level::mapping.size();
    for (kj = j; kj <= j + sx / slen; kj++)
    {
        if (!((i + i2 < N) && !is_solid(i + i2, kj) && (level::mapping[i + i2][kj] != spike_down)))
            break;
    }
    if (kj > j + sx / slen)
    {
        jh = 0;
        ground = false;
    }
}