#include "header.h"
#include <windows.h>
using namespace sf;
bool next = true, x_view = true, y_view = true, endgame = false;

int game::events()
{
    Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)
        {
            window.close();
            return 1;
        }
        else if (event.type == Event::LostFocus)
            while (!(window.pollEvent(event) && event.type == Event::GainedFocus));

        if (!cs_state && (event.type == Event::KeyPressed) && (event.key.code == Keyboard::M))
        {
            mute = !mute;
            if (mute)
            {
                mus.pause();
                spr_bl.setTextureRect(IntRect(204, 53, 12, 11));
            }
            else
            {
                if (i != n)
                {
                    change_mus();
                    mus.openFromFile(mus_name[mn]);
                }
                mus.play();
                spr_bl.setTextureRect(IntRect(192, 53, 12, 11));
            }
        }
    }
    return 0;
}

void game::draw_blocks(int k)
{
    int i, j;
    for (i = H / slen - 1; i > H / slen - 3; i--)
    {
        for (j = 0; j <= W / slen; j++)
        {
            if ((j < k) || (j > k + 5))
            {
                spr_bl.setPosition(j * slen - fr2, i * slen);
                window.draw(spr_bl);
            }
        }
    }
}
void game::cutsc()
{
    int k = W / slen + 1;
    double a;
    text.setCharacterSize(12);
    text.setPosition(3, H - slen / 2);
    text.setString("Press Esc to skip");
    while (k + 3 != W / slen / 2)
    {
        if (events() || Keyboard::isKeyPressed(Keyboard::Escape))
            return;
        window.clear();
        window.draw(spr_bgr);
        draw_blocks(k);
        fr2 += 0.15;
        if (fr2 >= slen)
        {
            fr2 = 0;
            k--;
        }

        spr_chr.setPosition(W / 2, H - 2 * slen - 1);
        if ((int)floor(fr1) % 2 == 0)
            spr_chr.move(-1, 0);
        window.draw(spr_chr);
        fr1 += 0.004;
        if (fr1 >= 7)
            fr1 = 3;
        spr_chr.setTextureRect(IntRect(floor(fr1) * 49, 0, 48, 67));

        window.draw(text);
        window.display();
    }

    for (a = 0; a < 3; a += 0.001)
    {
        if (events() || Keyboard::isKeyPressed(Keyboard::Escape))
            return;
        window.clear();
        window.draw(spr_bgr);
        draw_blocks(k);

        if (a < 1)
            spr_chr.setTextureRect(IntRect(14 * 49, 0, 48, 67));
        else if (a < 2)
            spr_chr.setTextureRect(IntRect(15 * 49, 0, 48, 67));
        else
            spr_chr.setTextureRect(IntRect(9 * 49, 0, 48, 67));
        window.draw(spr_chr);

        window.draw(text);
        window.display();
    }

    a = spr_chr.getPosition().y;
    spr_chr.setTextureRect(IntRect(8 * 49, 0, 48, 67));
    while (a < H + spr_chr.getGlobalBounds().height)
    {
        if (events() || Keyboard::isKeyPressed(Keyboard::Escape))
            return;
        window.clear();
        window.draw(spr_bgr);
        draw_blocks(k);

        spr_chr.setPosition(W / 2, a);
        a += 0.3;
        window.draw(spr_chr);

        window.draw(text);
        window.display();
    }
}
void game::end_cs()
{
    Clock clock, cl;
    double t = 0, t2 = 0;
    spr_chr.setColor(Color::Black);
    spr_chr.setTextureRect(IntRect(784, 0, 38, 67));
    spr_chr.setPosition(chr.get_x() + W / 2, chr.get_y() + 64);

    text.setCharacterSize(12);
    text.setPosition(spr_chr.getPosition().x + spr_chr.getGlobalBounds().width / 2, chr.get_y());
    text.setString("wake up.");

    while (t < 7)
    {
        cl.restart();
        if (events())
            return;
        t2 += cl.getElapsedTime().asSeconds();
        t = clock.getElapsedTime().asSeconds() - t2;

        chr.control(0, true);
        if (t < 1)
        {
            view.move(0.1, 0);
            window.setView(view);
        }

        window.clear();
        if (t < 5)
        {
            spr_bgr.setPosition(view.getCenter().x - W / 2, view.getCenter().y - H / 2);
            window.draw(spr_bgr);

            lvl[i - 1]->build(0, window, view);
            lvl[i - 1]->build_spec(0, window, chr.get_x(), chr.get_y());
            chr.draw(window);

            window.draw(spr_chr);

            if (t > 3)
                window.draw(text);
        }
        else
        {
            text.setPosition(view.getCenter().x - view.getSize().x / 2 + 3, view.getCenter().y + view.getSize().y / 2 - 15);
            text.setString("The end.");
            window.draw(text);
        }
        window.display();
    }
}

void game::menu()
{
    int i, j;
    if (Keyboard::isKeyPressed(Keyboard::Enter))
    {
        cutsc();
        start = false;
        cs_state = false;
        spr_bgr.setTextureRect(IntRect(0, 0, W, H));
        mus.play();
        spr_bl.setTextureRect(IntRect(192, 53, 12, 11));
        return;
    }
    window.clear();
    window.draw(spr_bgr);

    for (i = H / slen - 1; i > H / slen - 3; i--)
    {
        for (j = 0; j <= W / slen; j++)
        {
            spr_bl.setPosition(j * slen - fr2, i * slen);
            window.draw(spr_bl);
        }
    }
    fr2 += 0.15;
    if (fr2 >= slen)
        fr2 = 0;

    spr_chr.setPosition(W / 2, H - 2 * slen - 1);
    if ((int)floor(fr1) % 2 == 0)
        spr_chr.move(-1, 0);
    window.draw(spr_chr);
    fr1 += 0.004;
    if (fr1 >= 7)
        fr1 = 3;
    spr_chr.setTextureRect(IntRect(floor(fr1) * 49, 0, 48, 67));

    text.setPosition(W / 2 - 75, 2 * H / 3 + fr3);
    if (up)
        fr3 -= 0.015;
    else
        fr3 += 0.015;
    if (abs(fr3) >= 15)
        up = !up;
    window.draw(text);

    window.draw(spr_logo);

    window.display();
}

game::game() : i(0), k(0), mn(rand() % m), fr1(3), fr2(0), fr3(0), start(true), up(true), mute(false), cs_state(true)
{
    Image img_chr, img_bl, img_logo;
    window.create(VideoMode(W, H), "Trigger Pool", Style::Close);
    window.setPosition(Vector2i(250, 150));
    view.setSize(W, H);

    tex_bgr.loadFromFile("../../a/bgr.png");
    spr_bgr.setTexture(tex_bgr);
    spr_bgr.setTextureRect(IntRect(0, 2 * (H + 1), W, H));

    img_chr.loadFromFile("../../a/sp.png");
    img_chr.createMaskFromColor(Color(195, 195, 195));
    tex_chr.loadFromImage(img_chr);
    spr_chr.setTexture(tex_chr);
    spr_chr.setTextureRect(IntRect(49 * fr1, 0, 48, 67));
    spr_chr.setOrigin(24, 67);

    img_bl.loadFromFile("../../a/base.png");
    img_bl.createMaskFromColor(Color(195, 195, 195));
    tex_bl.loadFromImage(img_bl);
    spr_bl.setTexture(tex_bl);
    spr_bl.setTextureRect(IntRect(3 * slen, 0, slen, slen));

    img_logo.loadFromFile("../../a/spec.png");
    img_logo.createMaskFromColor(Color(195, 195, 195));
    tex_logo.loadFromImage(img_logo);
    spr_logo.setTexture(tex_logo);
    spr_logo.setTextureRect(IntRect(0, 0, 593, 257));
    spr_logo.setOrigin(296, 128);
    spr_logo.setPosition(W / 2, H / 3);

    font.loadFromFile("../../arial.ttf");
    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(Color::White);
    text.setString("Press Enter to start");

    mus.openFromFile(mus_name[mn]);
}

void game::change_mus()
{
    int mn2 = mn;
    while (mn == mn2)
        mn2 = rand() % m;
    mn = mn2;
}
void game::run()
{
    int trig;
    double t, vx, vy, x, y;
    bool f = true;
    Clock clock;

    while (window.isOpen())
    {
        t = clock.getElapsedTime().asMicroseconds();
        t /= 180;
        //t /= 300;
        events();
        if (start)
        {
            menu();
            continue;
        }
        if (endgame)
        {
            cs_state = true;
            end_cs();
            break;
        }

        if (!mute && (mus.getStatus() != Music::Status::Playing) && (i != n))
        {
            change_mus();
            mus.openFromFile(mus_name[mn]);
            mus.play();
        }

        if (chr.dead)
        {
            t = 0;
            if (k++ < 1200)
                chr.anim(5, 9);
            else
            {
                i--;
                next = true;
            }
        }
        if (next && (i < n))
        {
            window.clear();
            window.display();
            if (f && (i + 1 == n))
            {
                f = false;
                chr.to_black();
                lvl[i]->to_black();
                spr_bgr.setTextureRect(IntRect(0, H + 1, W, H));
                mus.openFromFile(mus_name[m]);
                mus.setLoop(true);
                if (!mute)
                    mus.play();
            }
            lvl[i]->restart();
            lvl[i++]->load();
            chr.restart();
            next = false;
            x_view = true;
            y_view = true;
            k = 0;
        }
        else
            chr.control(t, mute);
        if (x_view)
            vx = chr.get_x();
        if (y_view)
            vy = chr.get_y();
        x = chr.get_x();
        y = chr.get_y();
        if ((trig = chr.check_trigger(x, y)) > 0)
            lvl[i - 1]->trig_work(trig, floor(y / slen), floor(x / slen), vx, vy);

        set_view(view, vx, vy);
        window.setView(view);
        window.clear();

        spr_bgr.setPosition(view.getCenter().x - W / 2, view.getCenter().y - H / 2);
        window.draw(spr_bgr);

        lvl[i - 1]->build(t, window, view);
        lvl[i - 1]->build_spec(t, window, chr.get_x(), chr.get_y());
        chr.draw(window);

        spr_bl.setPosition(view.getCenter().x - view.getSize().x / 2 + 5, view.getCenter().y + view.getSize().y / 2 - spr_bl.getGlobalBounds().height - 5);
        window.draw(spr_bl);

        clock.restart();
        window.display();
    }
}

int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    srand(time(NULL));
    game game;
    game.run();
    return 0;
}