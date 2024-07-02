#include "header.h"
using namespace std;
using namespace sf;
void set_view(View& view, double x, double y)
{
    int N = level::mapping.size(), M = level::mapping[0].size();
    double vx = x - slen / 2, vy = y - slen / 2;
    if (vx < W / 2 + slen)
        vx = W / 2 + slen;
    else if (vx > M * slen - W / 2 - slen)
        vx = M * slen - W / 2 - slen;

    if (vy < H / 2 + (border + 1) * slen)
        vy = H / 2 + (border + 1) * slen;
    else if (vy > N * slen - H / 2 - (border + 1) * slen)
        vy = N * slen - H / 2 - (border + 1) * slen;
    view.setCenter(vx, vy);
}



vector<vector<int>> level::mapping;
vector<enemy*> level::en;
bool level::chp = false;
level::level(string map_file, string texture_file) : map_file(map_file)
{
    Image img_base, img;
    img_base.loadFromFile("../../a/base.png");
    img_base.createMaskFromColor(Color(195, 195, 195));
    texture_base.loadFromImage(img_base);
    sprite_base.setTexture(texture_base);

    img.loadFromFile(texture_file);
    img.createMaskFromColor(Color(195, 195, 195));
    texture.loadFromImage(img);
    sprite.setTexture(texture);
}
void level::to_black()
{
    sprite.setColor(Color::Black);
    sprite_base.setColor(Color::Black);
}

void level::fill_row(int k, int a, int x)
{
    int i;
    mapping.push_back(vector<int>());
    mapping[k].push_back(invis);
    for (i = 0; i < x; i++)
        mapping[k].push_back(a);
    mapping[k].push_back(invis);
}
void level::load()
{
    int i, j, k = 0;
    Color col;
    Image img_map;
    img_map.loadFromFile(map_file);
    mapping.clear();

    for (i = en.size() - 1; i >= 0; i--)
        delete en[i];
    en.clear();

    fill_row(k++, invis, img_map.getSize().x);
    for (i = 0; i < border; i++, k++)
        fill_row(k, 0, img_map.getSize().x);

    for (i = 0; i < img_map.getSize().y; i++, k++)
    {
        mapping.push_back(vector<int>());
        mapping[k].push_back(invis);
        for (j = 0; j < img_map.getSize().x; j++)
        {
            col = img_map.getPixel(j, i);
            switch (col.r)
            {
            case 0: mapping[k].push_back(invis); break;
            case 5: mapping[k].push_back(fake_regular); break;
            case 50: mapping[k].push_back(regular); break;
            case 75: mapping[k].push_back(semi_sol); break;
            case 100: mapping[k].push_back(start_trig);
                if (!chp)
                {
                    character::start_i = k + 1;
                    character::start_j = j + 1;
                }
                break;
            case 105: mapping[k].push_back(door); break;
            case 110: mapping[k].push_back(fake_door); break;
            case 150: mapping[k].push_back(spike_up); break;
            case 155: mapping[k].push_back(spike_down); break;
            case 200: mapping[k].push_back(killbox); break;
            case 205: mapping[k].push_back(0);
                en.push_back(new en1((j + 1) * slen, k * slen));
                break;
            case 255:
            {
                if (col.g == 255)
                {
                    if (col.b == 255)
                        mapping[k].push_back(0);
                    else
                        mapping[k].push_back(trigger + col.b);
                }
                else
                {
                    if (col.b == 0)
                        mapping[k].push_back(regular + 1 + col.g);
                    else if (col.b == 255)
                        mapping[k].push_back(background - col.g);
                    else
                        mapping[k].push_back(0);
                }
                break;
            }
            default: mapping[k].push_back(0);
            }
        }
        mapping[k].push_back(invis);
    }

    fill_row(k++, 0, img_map.getSize().x);
    fill_row(k++, killbox, img_map.getSize().x);
    for (i = 0; i < border - 2; i++, k++)
        fill_row(k, 0, img_map.getSize().x);
    fill_row(k, invis, img_map.getSize().x);
}

void level::view_borders(int& i, int& j, int& n, int& m, View view)
{
    int N = mapping.size(), M = mapping[0].size();
    i = (view.getCenter().y - H / 2) / slen - 1;
    if (i < 0)
        i = 0;

    n = (view.getCenter().y + H / 2) / slen + 1;
    if (n > N)
        n = N;

    j = (view.getCenter().x - W / 2) / slen - 1;
    if (j < 0)
        j = 0;

    m = (view.getCenter().x + W / 2) / slen + 1;
    if (m > M)
        m = M;
}
void level::build(double t, RenderWindow& window, View view)
{
    int i, j, i2, j2, k, n, m;
    view_borders(i, j, n, m, view);
    i2 = i;
    j2 = j;

    for (; i < n; i++)
    {
        for (j = j2; j < m; j++)
        {
            switch (mapping[i][j])
            {
            case regular: case fake_regular: sprite_base.setTextureRect(IntRect(3 * slen, 0, slen, slen)); break;
            case semi_sol: sprite_base.setTextureRect(IntRect(slen, slen, slen, slen)); break;
            case spike_up: sprite_base.setTextureRect(IntRect(0, slen, slen, slen)); break;
            case spike_down: sprite_base.setTextureRect(IntRect(0, 2 * slen, slen, -slen)); break;
            case door: case fake_door: case fake_door2:
                sprite_base.setTextureRect(IntRect(4 * slen, 0, slen, 2 * slen));
                sprite_base.setPosition(j * slen, (i - 1) * slen);
                window.draw(sprite_base);
                continue;
            case lever: sprite_base.setTextureRect(IntRect(2 * slen, slen, slen, slen)); break;
            case lever_used: sprite_base.setTextureRect(IntRect(3 * slen, slen, slen, slen)); break;
            case checkpoint:
                sprite_base.setTextureRect(IntRect(5 * slen, 0, slen, 2 * slen));
                sprite_base.setPosition(j * slen, (i - 1) * slen);
                window.draw(sprite_base);
                continue;
            default: continue;
            }
            sprite_base.setPosition(j * slen, i * slen);
            window.draw(sprite_base);
        }
    }

    for (k = 0; k < en.size(); k++)
    {
        if (en[k]->shown)
            en[k]->move(t);
        if ((en[k]->get_x() >= (j2 - 1) * slen) && (en[k]->get_x() < (m + 1) * slen) &&
            (en[k]->get_y() >= (i2 - 1) * slen) && (en[k]->get_y() < (n + 1) * slen))
        {
            en[k]->shown = true;
            en[k]->draw(window);
        }
    }
}

void level::delete_trig(int trig, int j, bool h)
{
    int i, n;
    if (!h)
        n = mapping.size();
    else
        n = mapping[0].size();
    for (i = 0; i < n; i++)
    {
        if (!h && mapping[i][j] == trig)
            mapping[i][j] = 0;
        else if (h && mapping[j][i] == trig)
            mapping[j][i] = 0;
    }
}

fake_block level::fill_fb(double x, double y, int prev, int curr, int fin, int dest_x, int dest_y, double vx, double vy)
{
    fake_block a;
    a.x = x;
    a.y = y;
    a.prev = prev;
    a.curr = curr;
    a.fin = fin;
    a.dest_x = dest_x;
    a.dest_y = dest_y;
    a.vx = vx;
    a.vy = vy;
    return a;
}

void level::move_fakes1(vector<vector<fake_block>>& reg, RenderWindow& window, double t)
{
    int i, j, k = 0, l = 0;
    double x, y;
    for (i = 0; i < reg.size(); i++)
    {
        for (j = 0; j < reg[i].size(); j++, l++)
        {
            if ((floor(reg[i][j].y / slen) == reg[i][j].dest_y) && (floor(reg[i][j].x / slen) == reg[i][j].dest_x))
            {
                k++;
                mapping[floor(reg[i][j].y / slen)][floor(reg[i][j].x / slen)] = reg[i][j].fin;
                continue;
            }
            reg[i][j].x += reg[i][j].vx * t;
            reg[i][j].y += reg[i][j].vy * t;

            if ((reg[i][j].curr == moving_fake) || (reg[i][j].curr == moving_fake2))
                sprite_base.setTextureRect(IntRect(3 * slen, 0, slen, slen));
            else if (reg[i][j].curr == fake_spike_up)
                sprite_base.setTextureRect(IntRect(0, slen, slen, slen));
            else if (reg[i][j].curr == fake_spike_down)
                sprite_base.setTextureRect(IntRect(0, 2 * slen, slen, -slen));
            sprite_base.setPosition(reg[i][j].x, reg[i][j].y);
            window.draw(sprite_base);
        }
    }
    if (k == l)
        reg.clear();
    for (i = 0; i < reg.size(); i++)
    {
        for (j = 0; j < reg[i].size(); j++)
        {
            x = reg[i][j].x - reg[i][j].vx * t;
            y = reg[i][j].y - reg[i][j].vy * t;
            if ((mapping[floor(reg[i][j].y / slen)][floor(reg[i][j].x / slen)] != reg[i][j].fin) &&
                (mapping[floor(y / slen)][floor(x / slen)] != mapping[floor(reg[i][j].y / slen)][floor(reg[i][j].x / slen)]))
            {
                mapping[floor(y / slen)][floor(x / slen)] = reg[i][j].prev;
                reg[i][j].prev = mapping[floor(reg[i][j].y / slen)][floor(reg[i][j].x / slen)];
                mapping[floor(reg[i][j].y / slen)][floor(reg[i][j].x / slen)] = reg[i][j].curr;
            }
        }
    }
}
void level::move_fakes2(vector<vector<fake_block>>& reg, RenderWindow& window, double t)
{
    int i, j, k = 0, l = 0;
    double x, y;
    for (i = reg.size() - 1; i >= 0; i--)
    {
        for (j = reg[i].size() - 1; j >= 0; j--, l++)
        {
            if ((floor(reg[i][j].y / slen) == reg[i][j].dest_y) && (floor(reg[i][j].x / slen) == reg[i][j].dest_x))
            {
                k++;
                mapping[floor(reg[i][j].y / slen)][floor(reg[i][j].x / slen)] = reg[i][j].fin;
                continue;
            }
            reg[i][j].x += reg[i][j].vx * t;
            reg[i][j].y += reg[i][j].vy * t;

            if ((reg[i][j].curr == moving_fake) || (reg[i][j].curr == moving_fake2))
                sprite_base.setTextureRect(IntRect(3 * slen, 0, slen, slen));
            else if (reg[i][j].curr == fake_spike_up)
                sprite_base.setTextureRect(IntRect(0, slen, slen, slen));
            else if (reg[i][j].curr == fake_spike_down)
                sprite_base.setTextureRect(IntRect(0, 2 * slen, slen, -slen));
            sprite_base.setPosition(reg[i][j].x, reg[i][j].y);
            window.draw(sprite_base);
        }
    }
    if (k == l)
        reg.clear();
    for (i = reg.size() - 1; i >= 0; i--)
    {
        for (j = reg[i].size() - 1; j >= 0; j--)
        {
            x = reg[i][j].x - reg[i][j].vx * t;
            y = reg[i][j].y - reg[i][j].vy * t;
            if ((mapping[floor(reg[i][j].y / slen)][floor(reg[i][j].x / slen)] != reg[i][j].fin) &&
                (mapping[floor(y / slen)][floor(x / slen)] != mapping[floor(reg[i][j].y / slen)][floor(reg[i][j].x / slen)]))
            {
                mapping[floor(y / slen)][floor(x / slen)] = reg[i][j].prev;
                reg[i][j].prev = mapping[floor(reg[i][j].y / slen)][floor(reg[i][j].x / slen)];
                mapping[floor(reg[i][j].y / slen)][floor(reg[i][j].x / slen)] = reg[i][j].curr;
            }
        }
    }
}
void level::start_fakes(double t, RenderWindow& window)
{
    if (bl1.size() > 0)
        move_fakes1(bl1, window, t);
    if (bl2.size() > 0)
        move_fakes2(bl2, window, t);
}

void level::set_checkpoint(int i, int j)
{
    mapping[i][j] = 0;
    character::start_i = i + 1;
    character::start_j = j;
    chp = true;
}