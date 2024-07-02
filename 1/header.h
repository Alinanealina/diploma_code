#pragma once
#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
const int W = 672, H = 576, slen = 32, border = 5, n = 6, m = 4;
enum blocks {
    background = -6,
    fake_door = -5,
    door = -4,
    fake_spike_down = -3,
    fake_spike_up = -2,
    fake_regular = -1,

    empty_space = 0,
    semi_sol = 1,
    invis = 2,
    regular = 3,
    moving_fake = 4,
    moving_fake2 = 5,

    spike_up = 260,
    spike_down = 261,
    killbox = 262,
    trigger = 263,

    fake_door2 = 513,
    start_trig = 514,
    lever = 515,
    lever_used = 516,
    checkpoint = 517
};
struct fake_block { double x; double y; int prev; int curr; int dest_x; int fin; int dest_y; double vx; double vy; };
extern bool next, x_view, y_view, endgame;

void set_view(sf::View& view, double x, double y);

class character {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::RectangleShape rect;
    sf::SoundBuffer sbuf;
    sf::Sound sound;
    double a, jh, x, y, sx, sy, spr_sx, spr_sy, frame;
    bool ground, jump, right, mute;
    void collision_x(int i, int j, double& dx);
    void collision_fall(int i, int j, double& dy);
    void collision_rise(int i, int j, double& dy);
    void fall_check(int i, int j);
    bool is_solid(int i, int j);
    bool check_block(int i, int j, int a);
    bool death_check(int i, int j);
public:
    character();
    bool dead;
    static double start_i, start_j;
    double get_x();
    double get_y();
    void restart();
    void control(double t, bool mute);
    int check_trigger(double& x, double& y);
    void to_black();
    void anim(int i, int k);
    void draw(sf::RenderWindow& window);
};

class enemy {
protected:
    sf::Texture tex;
    sf::Sprite sprite;
    int sx, sy;
    double a, vx, vy;
    bool border_check();
    virtual void anim() = 0;
public:
    enemy();
    bool shown;
    double get_x();
    double get_y();
    sf::FloatRect get_rect();
    void draw(sf::RenderWindow& window);
    virtual void move(double t);
};
class en1 : public enemy {
private:
    void anim();
public:
    en1(double x, double y);
    void move(double t);
};
class en2 : public enemy {
private:
    void anim();
public:
    en2(double x, double y, double vy);
};
class en3 : public enemy {
private:
    void anim();
public:
    en3(double x, double y, double vx, double vy);
};
class en4 : public enemy {
private:
    void anim();
public:
    en4(double x, double y);
};

class level {
private:
    sf::Texture texture_base, texture;
    sf::Sprite sprite;
    std::string map_file;
    void fill_row(int k, int a, int x);
protected:
    sf::Sprite sprite_base;
    std::vector<std::vector<fake_block>> bl1, bl2;
    void view_borders(int& i, int& j, int& n, int& m, sf::View view);
    void delete_trig(int trig, int j, bool h = false);
    fake_block fill_fb(double x, double y, int prev, int curr, int fin, int dest_x, int dest_y, double vx, double vy);
    void move_fakes1(std::vector<std::vector<fake_block>>& reg, sf::RenderWindow& window, double t);
    void move_fakes2(std::vector<std::vector<fake_block>>& reg, sf::RenderWindow& window, double t);
    void start_fakes(double t, sf::RenderWindow& window);
    void set_checkpoint(int i, int j);
public:
    level(std::string map_file, std::string texture_file);
    static std::vector<std::vector<int>> mapping;
    static std::vector<enemy*> en;
    static bool chp;
    void to_black();
    void load();
    void build(double t, sf::RenderWindow& window, sf::View view);
    virtual void trig_work(int trig, int i, int j, double& vx, double& vy) = 0;
    virtual void build_spec(double t, sf::RenderWindow& window, double x, double y) = 0;
    virtual void restart() = 0;
};

class level0 : public level {
private:
    sf::Texture tex;
    sf::Sprite spr;
    sf::Font font;
    sf::Text text;
    bool up;
    double fr;
public:
    level0();
    void restart();
    void build_spec(double t, sf::RenderWindow& window, double x, double y);
    void trig_work(int trig, int i, int j, double& vx, double& vy);
};
class level1 : public level {
public:
    level1();
    void restart();
    void build_spec(double t, sf::RenderWindow& window, double x, double y);
    void trig_work(int trig, int i, int j, double& vx, double& vy);
};
class level2 : public level {
private:
    sf::Texture tex;
    sf::Sprite spr;
    sf::RectangleShape rect;
    double spr_sx, spr_sy;
public:
    level2();
    void restart();
    void build_spec(double t, sf::RenderWindow& window, double x, double y);
    void trig_work(int trig, int i, int j, double& vx, double& vy);
};
class level3 : public level {
private:
    int ind, p;
    bool f, g, h;
    sf::Clock cl1, cl2;
public:
    level3();
    void restart();
    void build_spec(double t, sf::RenderWindow& window, double x, double y);
    void trig_work(int trig, int i, int j, double& vx, double& vy);
};
class level4 : public level {
public:
    level4();
    void restart();
    void build_spec(double t, sf::RenderWindow& window, double x, double y);
    void trig_work(int trig, int i, int j, double& vx, double& vy);
};
class level5 : public level {
public:
    level5();
    void restart();
    void build_spec(double t, sf::RenderWindow& window, double x, double y);
    void trig_work(int trig, int i, int j, double& vx, double& vy);
};

class game {
private:
    character chr;
    level* lvl[n] = {
        new level0(),
        new level1(),
        new level2(),
        new level3(),
        new level4(),
        new level5(),
    };
    std::string mus_name[m + 1] = {
        "../../bgm/Jeremy_Black_Blast_From_The_Past.wav",
        "../../bgm/Jeremy_Korpas_The_Sewars.wav",
        "../../bgm/NoMBe_Colour_Of_Your_Face.wav",
        "../../bgm/Ofshane_Back_To_The_Future.wav",
        "../../bgm/wind.wav"
    };
    sf::View view;
    sf::RenderWindow window;
    sf::Texture tex_bgr, tex_chr, tex_bl, tex_logo;
    sf::Sprite spr_bgr, spr_chr, spr_bl, spr_logo;
    sf::Font font;
    sf::Text text;
    sf::Music mus;
    int i, k, mn;
    double fr1, fr2, fr3;
    bool start, up, mute, cs_state;
    int events();
    void draw_blocks(int k);
    void cutsc();
    void end_cs();
    void menu();
    void change_mus();
public:
    game();
    void run();
};