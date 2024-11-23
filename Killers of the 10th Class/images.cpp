//
//  images.cpp
//  Killers of the 10th Class
//
//  Created by ハイラル・ゼルダ on 26/10/2024.
//

#include "images.hpp"

// 定义纹理
namespace generals {
Texture back;
Texture wcy;
Texture lcy;
Texture Trunchbull;
Texture Matilda;
}

namespace players {
Texture defaulted;
}

namespace cards {
Texture back;
Texture slash;
Texture peach;
Texture suit0;
Texture suit1;
Texture suit2;
Texture suit3;
}

namespace uis {
Texture target_icon;
Texture lord_icon;
}

void load_a_texture(const char* path,Texture* target){
    Image the_image = LoadImage(path);
    *target = LoadTextureFromImage(the_image);
    UnloadImage(the_image);
}

void LoadTextures() {
    load_a_texture("resource/generals/Generals_back.png", &generals::back);
    load_a_texture("resource/default_player.png", &players::defaulted);
    load_a_texture("resource/cards/card_back.png", &cards::back);
    load_a_texture("resource/cards/slash.png", &cards::slash);
    load_a_texture("resource/cards/peach.png", &cards::peach);
    load_a_texture("resource/suit0.png", &cards::suit0);
    load_a_texture("resource/suit1.png", &cards::suit1);
    load_a_texture("resource/suit2.png", &cards::suit2);
    load_a_texture("resource/suit3.png", &cards::suit3);
    load_a_texture("resource/generals/wcy.png", &generals::wcy);
    load_a_texture("resource/generals/lcy.png", &generals::lcy);
    load_a_texture("resource/generals/Miss.Trunchbull.png",
                   &generals::Trunchbull);
    load_a_texture("resource/generals/Matilda.png", &generals::Matilda);
    load_a_texture("resource/target.png", &uis::target_icon);
    load_a_texture("resource/lord.png", &uis::lord_icon);
}
