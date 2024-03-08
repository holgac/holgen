enum LandscapeType {
    Land;
    Sea;
    River;
    Mountain;
}

struct GridCell {
    s32 x;
    s32 y;
    LandscapeType landscape;
}