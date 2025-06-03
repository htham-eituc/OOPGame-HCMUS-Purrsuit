#include "Item.h"
#include "Map.h"

Item::Item(const std::string& name, const SDL_Rect& rect, int gid)
    : name(name), rect(rect), gid(gid) {}

void Item::render(SDL_Renderer* renderer, const std::vector<Tileset>& tilesets) {
    if (collected || gid == 0) return;

    const Tileset* ts = nullptr;
    int localID = 0;

    for (int i = static_cast<int>(tilesets.size()) - 1; i >= 0; --i) {
        if (gid >= tilesets[i].firstgid) {
            ts = &tilesets[i];
            localID = gid - tilesets[i].firstgid;
            break;
        }
    }

    if (!ts || !ts->texture) return;

    SDL_Rect srcRect = {
        (localID % ts->columns) * ts->tileWidth,
        (localID / ts->columns) * ts->tileHeight,
        ts->tileWidth,
        ts->tileHeight
    };

    SDL_RenderCopy(renderer, ts->texture, &srcRect, &rect);
}

SDL_Rect Item::getBounds() const {
    return rect;
}