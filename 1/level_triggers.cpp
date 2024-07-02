#include "header.h"
using namespace std;
using namespace sf;
level0::level0() : level("../../maps/map0.png", "../../a/base.png"), up(true), fr(0)
{
    Image img;
    img.loadFromFile("../../a/base.png");
    img.createMaskFromColor(Color(195, 195, 195));
    tex.loadFromImage(img);
    spr.setTexture(tex);
    spr.setScale(2, 2);

    font.loadFromFile("../../arial.ttf");
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(Color::White);
}
void level0::restart()
{
    up = true;
    fr = 0;
}
void level0::build_spec(double t, RenderWindow& window, double x, double y)
{
    double a = mapping.size() * slen / 2 + 30;
    spr.setPosition(3 * slen, a + fr);
    spr.setTextureRect(IntRect(192, 0, 33, 51));
    window.draw(spr);

    spr.setPosition(3 * slen + spr.getGlobalBounds().width / 2 + 3, a + spr.getGlobalBounds().height + 6 + fr);
    spr.setTextureRect(IntRect(225, 0, 15, 33));
    window.draw(spr);

    text.setString("- jump\n\n- go left\n\n- go right\n\n- use lever/door\n\n- turn on/off music");
    text.setPosition(spr.getPosition().x + spr.getGlobalBounds().width + 7, a + 3 + fr);
    window.draw(text);

    if (up)
        fr -= 0.015;
    else
        fr += 0.015;
    if (abs(fr) >= 15)
        up = !up;
}
void level0::trig_work(int trig, int i, int j, double& vx, double& vy)
{
    int n = mapping.size() - border, m = mapping[0].size() - 1;
    switch (trig - trigger)
    {
    case 0:
    {
        delete_trig(trig, i, true);
        y_view = false;
        break;
    }
    case lever - trigger:
    {
        if (Keyboard::isKeyPressed(Keyboard::E))
        {
            mapping[n - 4][4] = door;
            mapping[i][j] = lever_used;
        }
        break;
    }
    }
}

level1::level1() : level("../../maps/map1.png", "../../a/base.png")
{

}
void level1::restart()
{
    bl1.clear();
    bl2.clear();
}
void level1::build_spec(double t, RenderWindow& window, double x, double y)
{
    start_fakes(t, window);
}
void level1::trig_work(int trig, int i, int j, double& vx, double& vy)
{
    int ki, kj, n = mapping.size() - border, m = mapping[0].size() - 1;
    switch (trig - trigger)
    {
    case lever - trigger:
    {
        if (Keyboard::isKeyPressed(Keyboard::E))
        {
            mapping[i][j] = lever_used;
            mapping[n - 6][j - 12] = door;
            for (ki = i - 2; ki <= i; ki++)
            {
                mapping[ki][j - 7] = trigger + 5;
                mapping[ki][j - 8] = invis;
                mapping[ki + 3][j - 9] = fake_regular;
            }
        }
        break;
    }
    case 0:
    {
        delete_trig(trig, j);
        for (kj = character::start_j - 1; kj < character::start_j + 2; kj++)
            mapping[character::start_i - 5][kj] = regular;
        bl1.clear();
        for (ki = n - 3; ki <= n - 2; ki++)
        {
            bl1.push_back(vector<fake_block>());
            for (kj = j + 3; kj < j + 7; kj++)
            {
                bl1[ki - n + 3].push_back(fill_fb(kj * slen, ki * slen, 0, moving_fake, regular, kj - 8, ki, -0.6, 0));
                mapping[ki][kj] = moving_fake;
            }
        }
        break;
    }
    case 1:
    {
        delete_trig(trig, j);
        bl2.clear();
        for (ki = n - 3; ki <= n - 2; ki++)
        {
            bl2.push_back(vector<fake_block>());
            for (kj = j; kj < j + 5; kj++)
            {
                bl2[ki - n + 3].push_back(fill_fb(kj * slen, ki * slen, 0, moving_fake2, regular, kj, ki + 4, 0, 0.3));
                mapping[ki][kj] = moving_fake2;
            }
        }
        break;
    }
    case 2:
    {
        delete_trig(trig, j);
        bl1.clear();
        bl2.clear();
        for (ki = n - 3; ki <= n - 2; ki++)
        {
            bl1.push_back(vector<fake_block>());
            bl2.push_back(vector<fake_block>());
            for (kj = j + 1; kj <= j + 15; kj++)
            {
                if ((ki == n - 3) && ((kj - j - 1) % 5 > 1))
                {
                    bl1[ki - n + 3].push_back(fill_fb(kj * slen, ki * slen, 0, moving_fake, regular, kj, ki - 5 - 2 * ((kj - j - 1) / 5), 0, -0.5));
                    mapping[ki][kj] = moving_fake;
                }
                else
                {
                    bl2[ki - n + 3].push_back(fill_fb(kj * slen, ki * slen, 0, moving_fake, regular, kj, ki + 4, 0, 0.5));
                    mapping[ki][kj] = moving_fake;
                }
            }
        }
        break;
    }
    case 3:
    {
        mapping[i][j] = 0;
        mapping[i + 1][j] = 0;
        break;
    }
    case 4:
    {
        delete_trig(trig, j);
        for (kj = j; kj <= j + 7; kj++)
            mapping[n - 8][kj] = 0;
        break;
    }
    case 5:
    {
        delete_trig(trig, j);
        for (kj = m - 2; kj > m - 5; kj--)
            mapping[n - 8][kj] = 0;
        break;
    }
    case 6:
    {
        mapping[i][j] = spike_up;
        break;
    }
    case 7:
    {
        delete_trig(trig, i, true);
        for (; mapping[i][j] == 0; j--);
        mapping[i + 4][j + 7] = fake_door;
        for (; mapping[i - 2][j] == 0; j++)
            mapping[i - 2][j] = regular;
        break;
    }
    case 8:
    {
        delete_trig(trig, i, true);
        mapping[i - 1][j] = spike_down;
        break;
    }
    }
}

level2::level2() : level("../../maps/map2.png", "../../a/base.png"), spr_sx(48), spr_sy(67)
{

}
void level2::restart()
{
    Image img;
    img.loadFromFile("../../a/sp.png");
    img.createMaskFromColor(Color(195, 195, 195));
    tex.loadFromImage(img);
    spr.setTexture(tex);
    spr.setOrigin(spr_sx / 2, spr_sy);
    spr.setPosition(W, H);
    spr.setColor(Color(150, 150, 255, 150));

    rect.setPosition(W / 2, 0);
    rect.setFillColor(Color(150, 150, 255, 100));

    bl1.clear();
    bl2.clear();
}
void level2::build_spec(double t, RenderWindow& window, double x, double y)
{
    double dx = round(spr.getPosition().x);
    rect.setSize(Vector2f(W / 2, mapping.size() * slen));
    rect.setPosition((mapping[0].size() - 2) * slen / 2, 0);
    window.draw(rect);

    spr.setPosition(W + 3 * slen / 2 - x, y + 2 * slen);
    if (dx < round(spr.getPosition().x))
        spr.setTextureRect(IntRect(0, 0, spr_sx, spr_sy));
    else if (dx > round(spr.getPosition().x))
        spr.setTextureRect(IntRect(spr_sx, 0, -spr_sx, spr_sy));
    window.draw(spr);

    start_fakes(t, window);
}
void level2::trig_work(int trig, int i, int j, double& vx, double& vy)
{
    int ki, kj, n = mapping.size() - border, m = mapping[0].size() - 1;
    switch (trig - trigger)
    {
    case start_trig - trigger:
    {
        x_view = false;
        vx = W / 2 + slen / 2;
        break;
    }
    case checkpoint - trigger:
    {
        set_checkpoint(i, j);
        x_view = false;
        vx = W / 2 + slen / 2;
        break;
    }
    case 0:
    {
        mapping[i][j] = 0;
        mapping[i + 1][j] = spike_up;
        mapping[i + 2][j] = 0;
        break;
    }
    case 1:
    {
        delete_trig(trig, j);
        for (kj = i; mapping[kj][j - 2] != regular; kj--);
        for (ki = kj + 1; ki <= kj + 3; ki++)
            mapping[ki][j - 2] = regular;
        break;
    }
    case 2:
    {
        delete_trig(trig, i, true);
        for (ki = j; mapping[i + 1][ki] != regular; ki--);
        for (kj = ki + 1; kj <= ki + 3; kj++)
            mapping[i + 1][kj] = regular;
        break;
    }
    case 3:
    {
        delete_trig(trig, i, true);
        en.push_back(new en2((m - 11) * slen, (i + 8) * slen, -0.7)); // -0.75
        break;
    }
    case 4:
    {
        delete_trig(trig, i, true);
        bl1.clear();
        bl1.push_back(vector<fake_block>());
        bl1[0].push_back(fill_fb((m - 5) * slen, (i + 8) * slen, regular, fake_spike_up, invis, m - 5, i - 21, 0, -0.6));
        mapping[i + 8][m - 5] = fake_spike_up;
        break;
    }
    case 5:
    {
        delete_trig(trig, i, true);
        bl2.clear();
        bl2.push_back(vector<fake_block>());
        bl2[0].push_back(fill_fb((m - 6) * slen, (i + 1) * slen, spike_up, moving_fake, regular, m - 7, i + 1, -1, 0));
        mapping[i + 1][m - 6] = moving_fake;
        bl2[0].push_back(fill_fb((m - 5) * slen, (i + 1) * slen, spike_up, moving_fake2, regular, m - 4, i + 1, 1, 0));
        mapping[i + 1][m - 5] = moving_fake2;
        break;
    }
    case 6:
    {
        delete_trig(trig, j);
        for (ki = i; mapping[ki][j] != regular; ki++);
        mapping[ki - 1][j + 1] = fake_regular;
        break;
    }
    case 7:
    {
        mapping[i][j] = 0;
        mapping[i + 1][j] = 0;
        break;
    }
    case lever - trigger:
    {
        if (Keyboard::isKeyPressed(Keyboard::E))
        {
            mapping[i][j] = lever_used;
            for (; mapping[i][j] != regular; j--);
            for (ki = i + 1; ki < i + 38; ki++)
            {
                for (kj = j + 1; kj < j + 10; kj++)
                {
                    mapping[ki][2 * j - kj] = mapping[ki][kj];
                    if (ki == 13)
                        mapping[ki][kj] = trigger + 8;
                    else if (ki == 28)
                        mapping[ki][kj] = trigger + 9;
                    else
                        mapping[ki][kj] = 0;
                }
            }
            for (kj = m - 4; kj > m - 7; kj--)
            {
                mapping[ki][kj] = 0;
                mapping[ki][m - 2 - kj] = 0;
            }
            mapping[ki + 2][j] = 0;
            mapping[ki + 3][j] = 0;

            for (kj = 2; kj < 6; kj++)
                mapping[26 + border][kj] = trigger + 10;

            for (kj = 3; kj < 5; kj++)
                mapping[23 + border][kj] = trigger + 11;
        }
        break;
    }
    case 8:
    {
        delete_trig(trig, i, true);
        en.push_back(new en2((m - 11) * slen, (i - 8) * slen, 0.55)); // 0.5
        break;
    }
    case 9:
    {
        delete_trig(trig, i, true);
        bl1.clear();
        bl1.push_back(vector<fake_block>());
        bl1[0].push_back(fill_fb((m - 5) * slen, (i - 8) * slen, 0, fake_spike_down, invis, m - 5, i + 24, 0, 0.6));
        mapping[i - 8][m - 5] = fake_spike_down;
        break;
    }
    case 10:
    {
        delete_trig(trig, i, true);
        bl2.clear();
        bl2.push_back(vector<fake_block>());
        bl2[0].push_back(fill_fb(5 * slen, (i + 1) * slen, 0, moving_fake, regular, 4, i + 1, -1, 0));
        mapping[i + 1][5] = moving_fake;
        bl2[0].push_back(fill_fb(2 * slen, (i + 1) * slen, 0, moving_fake2, regular, 3, i + 1, 1, 0));
        mapping[i + 1][2] = moving_fake2;
        mapping[i][j] = trigger + 12;
        break;
    }
    case 11:
    {
        mapping[i][j] = 0;
        mapping[i - 2][j] = regular;
        break;
    }
    case 12:
    {
        mapping[i][j] = 0;
        mapping[i + 1][1] = regular;
        break;
    }
    }
}

level3::level3() : level("../../maps/map3.png", "../../a/base.png"), ind(0), p(13), f(false), g(false), h(true)
{
    
}
void level3::restart()
{
    ind = 0;
    p = 15;
    f = false;
    g = false;
    h = true;
}
void level3::build_spec(double t, RenderWindow& window, double x, double y)
{
    double ct1, ct2;
    int i, m = mapping[0].size() - 1;
    if (f)
    {
        ct1 = cl1.getElapsedTime().asSeconds();
        if (ct1 >= 1.5)
        {
            for (i = border + 6; mapping[i][11] == 0; i++)
                mapping[i][11] = regular;

            if (p > 25)
                f = false;
            else if (cl1.getElapsedTime().asMilliseconds() % 150 == 0)
            {
                en.push_back(new en3(p * slen, (border - 1) * slen, -0.1, 0.35)); // -0.108, 0.36
                p += 3;
            }
        }
    }
    if (g)
    {
        ct2 = cl2.getElapsedTime().asSeconds();
        if (ct2 >= 6)
        {
            for (i = 8; i < 11; i++)
                mapping[border + 8][i] = 0;
            g = false;
        }
        else if (h && (ct2 >= 3))
        {
            en.push_back(new en4(6.5 * slen, (border + 7) * slen));
            h = false;
        }
    }
}
void level3::trig_work(int trig, int i, int j, double& vx, double& vy)
{
    int ki, kj, n = mapping.size() - border, m = mapping[0].size() - 1;
    switch (trig - trigger)
    {
    case start_trig - trigger:
    {
        x_view = false;
        y_view = false;
        vx = W / 2 + 3 * slen + slen / 2;
        vy = H / 2 + slen / 2;
        mapping[i][j] = 0;
        en.push_back(new en1(2 * slen, (border + 10) * slen));
        break;
    }
    case lever - trigger:
    {
        if (Keyboard::isKeyPressed(Keyboard::E))
        {
            switch (ind++)
            {
            case 0:
            {
                for (kj = j - 2; kj > j - 5; kj--)
                    mapping[i + 1][kj] = 0;
                f = true;
                cl1.restart();
                break;
            }
            case 1:
            {
                for (kj = j + 3; kj < j + 6; kj++)
                    mapping[i + 1][kj] = regular;
                for (kj = j - 1; kj <= j; kj++)
                {
                    mapping[i + 4][kj] = 0;
                    mapping[i + 5][kj] = fake_regular;
                }
                mapping[i + 4][j - 2] = fake_regular;
                g = true;
                cl2.restart();
                break;
            }
            }
            mapping[i][j] = lever_used;
        }
        break;
    }
    case 0:
    {
        delete_trig(trig, j);
        for (; mapping[i][j] != regular; i++);
        for (ki = i - 1; mapping[ki][j - 2] != regular; ki--)
            mapping[ki][j - 2] = regular;
        break;
    }
    case 1:
    {
        delete_trig(trig, j);
        x_view = false;
        break;
    }
    case 3:
    {
        delete_trig(trig, i, true);
        for (; mapping[i][j] != regular; j++);
        for (kj = j - 1; mapping[i + 1][kj] == 0; kj--)
            mapping[i + 1][kj] = regular;
        for (kj = j - 1; mapping[i + 6][kj] == 0; kj--)
            mapping[i + 5][kj] = 0;
        break;
    }
    case fake_door2 - trigger:
    {
        if (Keyboard::isKeyPressed(Keyboard::E))
        {
            for (kj = j; mapping[i + 2][kj] != regular; kj--);
            for (; mapping[i + 2][j] != regular; j++);
            for (++kj; kj < j; kj++)
                mapping[i + 1][kj] = 0;
            x_view = true;
            y_view = true;
        }
        break;
    }
    case checkpoint - trigger:
    {
        set_checkpoint(i, j);
        break;
    }
    case 2:
    {
        delete_trig(trig, i, true);
        for (; mapping[i - 3][j] != 0; j++);
        for (kj = j; kj < j + 3; kj++)
        {
            mapping[i][kj] = 0;
            mapping[i - 3][kj] = regular;
        }
        break;
    }
    case 4:
    {
        delete_trig(trig, j);
        vy = H / 2 + slen / 2;
        x_view = true;
        y_view = false;
        break;
    }
    case 5:
    {
        delete_trig(trig, i, true);
        for (; mapping[i][j] == 0; j--);
        en.push_back(new en2((j + 3) * slen, (i - 2) * slen - H / 2, 2.7));
        en.push_back(new en2((j + 13) * slen, (i - 2) * slen - H / 2, 2.7));
        break;
    }
    case 6:
    {
        delete_trig(trig, i, true);
        for (; mapping[i][j] == 0; j--);
        en.push_back(new en2((j + 7) * slen, (i - 2) * slen - H / 2, 2.7));
        en.push_back(new en2((j + 17) * slen, (i - 2) * slen - H / 2, 2.7));
        break;
    }
    case 7:
    {
        delete_trig(trig, i, true);
        for (; mapping[i][j] == 0; j--);
        en.push_back(new en2((j + 2) * slen, (i - 2) * slen - H / 2, 2.7));
        en.push_back(new en2((j + 10) * slen, (i - 2) * slen - H / 2, 2.7));
        break;
    }
    case 8:
    {
        delete_trig(trig, j);
        en.push_back(new en3(j * slen, (i - 3) * slen - H / 2, -0.12, 0.35)); // -0.117, 0.36
        break;
    }
    case 9:
    {
        delete_trig(trig, j);
        for (; mapping[i][j] == 0; j++);
        for (; mapping[i][j] == regular; j++);
        for (; mapping[i][j] == 0; j++);
        for (; mapping[i][j] == regular; i--);
        for (; mapping[i][j] == 0; i--);
        for (ki = i - 3; ki < i; ki++)
            mapping[ki][j] = trigger + 10;
        break;
    }
    case 10:
    {
        delete_trig(trig, j);
        mapping[n - 7][3] = 0;
        mapping[n - 3][3] = door;
        for (; mapping[i][j - 6] == 0; i++);
        for (ki = i + 1; mapping[ki][j - 6] == 0; ki++)
            mapping[ki][j - 6] = trigger + 11;
        break;
    }
    case 11:
    {
        delete_trig(trig, j);
        en.push_back(new en3(9 * slen, n * slen, 0.12, -0.35));
        break;
    }
    }
}

level4::level4() : level("../../maps/map4.png", "../../a/base.png")
{

}
void level4::restart()
{

}
void level4::build_spec(double t, RenderWindow& window, double x, double y)
{

}
void level4::trig_work(int trig, int i, int j, double& vx, double& vy)
{

}

level5::level5() : level("../../maps/map5.png", "../../a/base.png")
{

}
void level5::restart()
{
    bl1.clear();
}
void level5::build_spec(double t, RenderWindow& window, double x, double y)
{
    start_fakes(t, window);
}
void level5::trig_work(int trig, int i, int j, double& vx, double& vy)
{
    switch (trig - trigger)
    {
    case 0:
    {
        delete_trig(trig, j);
        for (; mapping[i][j] == 0; i++);
        bl1.clear();
        bl1.push_back(vector<fake_block>());
        bl1[0].push_back(fill_fb(j * slen, i * slen, 0, fake_spike_up, invis, j, border, 0, -0.7));
        mapping[i][j] = fake_spike_up;
        break;
    }
    case 1:
    {
        delete_trig(trig, j);
        bl1.clear();
        bl1.push_back(vector<fake_block>());
        bl1[0].push_back(fill_fb((j + 1) * slen, border * slen, 0, fake_spike_down, invis, j + 1, 2 * border + H / slen, 0, 1));
        mapping[border][j + 1] = fake_spike_down;
        break;
    }
    case 2:
    {
        delete_trig(trig, j);
        endgame = true;
        break;
    }
    }
}