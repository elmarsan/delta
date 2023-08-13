#pragma once

struct Animation
{
    int index;
    int frames;
    int speed;

    Animation(int i, int frames, int speed): index(i), frames(frames), speed(speed) {}
};
