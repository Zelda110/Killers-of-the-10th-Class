//
//  main.cpp
//  Killers of the 10th Class
//
//  Created by ハイラル・ゼルダ on 20/10/2024.
//

#include <iostream>
#include <thread>
#include "raylib.h"

#include "game.hpp"
#include "images.hpp"
using namespace std;

int width=800;
int height=450;
double scale=0.5;//相对标准尺寸(1600,900)的缩放

vector<string> NUMBERS={"0","A","2","3","4","5","6","7","8","9","10","J","Q","K"};//牌面数字

double PLAYERS_ELLIPSE(double x){//玩家排列的椭圆
    double a=250000;
    double b=80000;
    double rx=(x-width/2)/scale;
    try {
        if (-sqrt(b*(1-rx*rx/a))*scale+height/2>0) {
            return -sqrt(b*(1-rx*rx/a))*scale+height/2;
        }
        return height/2;
    } catch (...) {
        return height/2;
    }
}

void window_change_size(){
    width=GetScreenWidth();
    height=GetScreenHeight();
    scale=min((double)width/1600, (double)height/900);
}

//绘制卡牌
void draw_card(double x,double y,double s,bool is_backed,Card* card=NULL){
    if (is_backed) {
        DrawTextureEx(cards::back,
                      Vector2(x-210*scale*s,y-286*scale*s),0,scale*s,
                      WHITE);
    }
    else{
        DrawTextureEx(*card->texture,
                      Vector2(x-210*scale*s,y-286*scale*s),
                      0,
                      scale*s,
                      WHITE);
        switch (card->suit) {
            case 0:
                DrawTextureEx(cards::suit0,
                              Vector2(x-170*scale*s,y-256*scale*s),
                              0,
                              scale*s,
                              WHITE);
                DrawText(NUMBERS[card->number].c_str(),
                         x-155*scale*s,
                         y-206*scale*s,
                         50*scale*s,
                         RED);
                break;
                
            case 1:
                DrawTextureEx(cards::suit1,
                              Vector2(x-170*scale*s,y-256*scale*s),
                              0,
                              scale*s,
                              WHITE);
                DrawText(NUMBERS[card->number].c_str(),
                         x-155*scale*s,
                         y-206*scale*s,
                         50*scale*s,
                         BLACK);
                break;
                
            case 2:
                DrawTextureEx(cards::suit2,
                              Vector2(x-170*scale*s,y-256*scale*s),
                              0,
                              scale*s,
                              WHITE);
                DrawText(NUMBERS[card->number].c_str(),
                         x-155*scale*s,
                         y-206*scale*s,
                         50*scale*s,
                         BLACK);
                break;
                
            case 3:
                DrawTextureEx(cards::suit3,
                              Vector2(x-170*scale*s,y-256*scale*s),
                              0,
                              scale*s,
                              WHITE);
                DrawText(NUMBERS[card->number].c_str(),
                         x-155*scale*s,
                         y-206*scale*s,
                         50*scale*s,
                         RED);
                break;
            default:
                break;
        }
    }
}

void ingame_display(Game* main_game){
    //游戏内的绘制函数
    
    if (IsWindowResized()) {
        window_change_size();
    }
    
    ClearBackground(Color(200,220,230));
    DrawText(to_string(GetFPS()).c_str(), 0, 0, 48*scale, BLACK);
    DrawText(to_string(scale).c_str(), 0, 40*scale, 48*scale, BLACK);
    
    //测试用
//    for (int i=0; i<width; i++) {
//        DrawLineEx(Vector2(i, PLAYERS_ELLIPSE(i)),
//                   Vector2(i+1,PLAYERS_ELLIPSE(i+1)),5*scale,
//                   BLACK);
//    }
    int this_number=distance(main_game->begining_players.begin(),
                             find(main_game->begining_players.begin(),
                                  main_game->begining_players.end(),
                                  main_game->this_player));//查找自己在初始序列中的位置
    int start_number = 0;
    //找到自己的下家的序号
    for (int i=0; i<main_game->player_number-1; i++) {
        int index=(i+this_number+1)%main_game->player_number;
        if (find(main_game->playing_players.begin(),
                 main_game->playing_players.end(),
                 main_game->begining_players[index])!=main_game->playing_players.end()) {
            start_number=distance(main_game->playing_players.begin(),
                                  find(main_game->playing_players.begin(),
                                           main_game->playing_players.end(),
                                           main_game->begining_players[index]));
            break;
        }
    }
    int playing_list_length=main_game->playing_players.size();
    if (find(main_game->playing_players.begin(), main_game->playing_players.end(), main_game->this_player)!=main_game->playing_players.end()) {
        playing_list_length--;
    }
    //渲染其他玩家
    for (int i=0; i<playing_list_length; i++) {
        double base_x=width/2+(1000/(playing_list_length-1)*i-500)*scale;
        double base_y=PLAYERS_ELLIPSE(base_x);
        int m=(start_number+i)%main_game->playing_players.size();
        Player* now_player=&main_game->players[main_game->playing_players[m]];
//        DrawTextureEx(players::defaulted,
//                      Vector2(base_x-100*scale,base_y),0,scale*0.8,
//                    WHITE);
        //玩家名
        DrawText(now_player->id.c_str(),
                 base_x-30*scale-MeasureText(now_player->id.c_str(),
                                             24*scale)/2,
                 base_y-10*scale-24*scale,
                 24*scale,
                 BLACK);
        //武将牌
        DrawTextureEx(*now_player->general.card,
                      Vector2(base_x-90*scale, base_y),0,scale*0.2*0.56,
                    WHITE);
        //当前角色图标
        if (m==main_game->current_player) {
            DrawTextureEx(uis::target_icon,
                          Vector2(base_x-100*scale,base_y-10*scale),
                          0,
                          scale*0.5, WHITE);
        }
        //生命值
        DrawText((to_string(now_player->health)+"/"+to_string(now_player->max_health)).c_str(),
                 base_x-30*scale-MeasureText((to_string(now_player->health)+"/"+to_string(now_player->max_health)).c_str(),
                                             24*scale)/2,
                 base_y+104.4*scale+24*scale,
                 24*scale,
                 BLACK);
        //手牌
        for (int n=0; n<now_player->cards.size(); n++) {
            DrawTextureEx(cards::back,
                          Vector2(base_x+(-10+3*n)*scale,base_y),0,scale*0.2,
                        WHITE);
        }
        //手牌数
        DrawText(to_string(now_player->cards.size()).c_str(),base_x+20
                 *scale,base_y+30*scale,
                 60*scale,
                 WHITE);
        //主公标记
        if (m==0) {
            DrawTextureEx(uis::lord_icon,
                          Vector2(base_x-25*scale,base_y-5*scale),
                          0,
                          scale*0.3, WHITE);
        }
    }
    
    //渲染自己的界面
    Player* self_player=&main_game->players[main_game->this_player];//自己玩家
    //武将牌
    DrawTextureEx(*self_player->general.card,
                  Vector2(width-200*scale,height-240*scale),
                  0,
                  scale*0.4*0.56,
                  WHITE);
    //当前角色图标
    if (main_game->playing_players[main_game->current_player]==main_game->this_player) {
        DrawTextureEx(uis::target_icon,
                      Vector2(width-210*scale,height-250*scale),
                      0,
                      scale*0.5, WHITE);
    }
    //主公标记
    if (main_game->this_player==main_game->playing_players[0]) {
        DrawTextureEx(uis::lord_icon,
                      Vector2(width-65*scale,height-250*scale),
                      0,
                      scale*0.4, WHITE);
    }
    //生命值
    DrawText((to_string(self_player->health)+"/"+to_string(self_player->max_health)).c_str(),
             width-118*scale-scale-MeasureText((to_string(self_player->health)+"/"+to_string(self_player->max_health)).c_str(),
                                         24*scale)/2,
             height-240*scale-50 *scale,
             24*scale,
             BLACK);
    //玩家名
    DrawText(self_player->id.c_str(),
             width-118*scale-MeasureText(main_game->players[main_game->this_player].id.c_str(),
                                         24*scale)/2,
             height-240*scale-24*scale,
             24*scale,
             BLACK);
    //手牌
    double base_x=width/2-(88*main_game->players[main_game->this_player].cards.size()-4)*scale;
    for (int i=0; i<main_game->players[main_game->this_player].cards.size(); i++) {
        draw_card(base_x+(176*i+84)*scale, height-125.6*scale, 0.4, false,&main_game->players[main_game->this_player].cards[i]);
    }
    
    //处理技能相关
    //寻找所有可用技能
    vector<Skill*> usable_skills;
    for (int i=0; i<self_player->skills.size(); i++) {
        if (self_player->skills[i].judge(main_game,main_game->this_player)) {
            usable_skills.push_back(&self_player->skills[i]);
        }
    }
    //渲染技能名字
    int skill_number=usable_skills.size();
    for (int i=0; i<skill_number; i++) {
        base_x=width/2-(skill_number-1)*50*scale+50*scale*i;
        DrawText(usable_skills[i]->name.c_str(),
                 base_x-MeasureText(usable_skills[i]->name.c_str(), 24*scale),
                 height-250*scale-24*scale,
                 24*scale,
                 BLACK);
    }
}

int main(){
    InitWindow(width, height, "Killers of the 10th Class");//初始化窗口
    SetTraceLogLevel(LOG_WARNING);
    SetTargetFPS(0);//无限帧率
    SetWindowMinSize(width, height);//最小尺寸
    SetExitKey(0);//不用键盘关闭窗口
    SetWindowState(FLAG_WINDOW_RESIZABLE|FLAG_WINDOW_ALWAYS_RUN);
    window_change_size();
    
    LoadTextures();
    
    
    
    Game maingame(5);
    MAINGAME=&maingame;
    thread game_logic(bind(&Game::start, &maingame));
    
    while (!maingame.starting_finished) {
        ;
    }
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        //键盘轮询
        
        //更改观察对象(调试用)
        if (IsKeyPressed(KEY_ZERO)) {
            maingame.this_player=0;
        }
        else if (IsKeyPressed(KEY_ONE)) {
            maingame.this_player=1;
        }
        else if (IsKeyPressed(KEY_TWO)) {
            maingame.this_player=2;
        }
        else if (IsKeyPressed(KEY_THREE)) {
            maingame.this_player=3;
        }
        else if (IsKeyPressed(KEY_FOUR)) {
            maingame.this_player=4;
        }
        
        ingame_display(&maingame);
        
        EndDrawing();
    }
    game_stopped=true;
    game_logic.join();
    CloseWindow();
}
