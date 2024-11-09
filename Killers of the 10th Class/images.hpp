//
//  images.hpp
//  Killers of the 10th Class
//
//  Created by ハイラル・ゼルダ on 26/10/2024.
//

#ifndef images_hpp
#define images_hpp

#include <stdio.h>
#include <string>
#include "raylib.h"
using namespace std;

namespace generals {
extern Texture back;
extern Texture L_Hospital;
extern Texture wcy;
extern Texture lcy;
}

namespace players {
extern Texture defaulted;
}

namespace cards {
extern Texture back;
extern Texture slash;
extern Texture suit0;
extern Texture suit1;
extern Texture suit2;
extern Texture suit3;
}

namespace uis {
extern Texture target_icon;
extern Texture lord_icon;
}

void LoadTextures();
void UnloadTextures();

#endif /* images_hpp */
