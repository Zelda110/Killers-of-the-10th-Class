//
//  game.hpp
//  Killers of the 10th Class
//
//  Created by ハイラル・ゼルダ on 20/10/2024.
//

#ifndef game_hpp
#define game_hpp

#include <stdio.h>
#include <vector>
#include <any>
#include <iostream>
#include <ctime>
#include <random>

#include "images.hpp"
using namespace std;

extern bool game_stopped;//游戏是否结束

//关于游戏基本逻辑的变量、函数和类

class Game;
extern Game* MAINGAME;

class Card{
public:
    Card(int suit,int number,string name,int type,string distribe,string quotation,Texture* texture):suit(suit),number(number),name(name),type(type),texture(texture){};
    string name;
    Texture* texture;//牌面纹理
    int number;//点数 0-13
    int suit;
    int type;
    string get_info();
};

class Skill{//技能类
public:
    Skill(string name,int type,int situation,void (*use)(Game*,Skill*),bool (*judge)(Game*,int),vector<vector<int>> (*client)(Game*,int)):name(name),type(type),situation(situation),use(use),judge(judge),client(client){
    };
    string name;
    int type;//类型 -1特殊 0基本牌技 1锦囊牌技 2装备牌技 3武将技能
    int situation;//使用场景 0自己回合 1他人回合 2响应阶段 3其他
    int using_player;//使用的玩家(玩家号)
    void (*use)(Game*,Skill*);//技能实现
    bool (*judge)(Game*,int);//判断能否使用
    vector<vector<int>> (*client)(Game*,int);//客户端指令 用于选择目标等 返回{target_players target_cards target_positions args}
    int used_time=0;//使用次数
    vector<Card> based_card;//基于的卡牌
    vector<int> target_players;//目标(玩家号)
    vector<int> target_cards;//目标(卡牌) {玩家号 位置号 下标}
    vector<int> target_positions;//目标(位置) {玩家号 位置号} 位置号 0手牌 1判定区 2武器 3防具 4宝物 5马+ 6马-
    vector<int> args;//参数
    void perform(int using_player,vector<int> target_players,vector<int> target_cards,vector<int> target_positions,vector<int> args);//使用技能
};

class General{
public:
    General(string name,Texture* card,int max_health,vector<Skill> skills={}):name(name),card(card),max_health(max_health),skills(skills){
    };
    string name;
    Texture* card;//武将牌纹理
    vector<Skill> skills;//技能列表
    int max_health;//体力上限
    vector<int> subject;
};

//所有武将
extern General wcy;
extern General lcy;
extern General Trunchbull;
extern General Matilda;

class Player{
public:
    Player(string id,General general):id(id),general(general){};
    string id;//游戏id
    int index;//出牌顺序序号
    General general;//武将
    vector<Card> cards;//手牌
    vector<Card> judgment_area;//判定区
    vector<Card> weapon;//武器
    vector<Card> armour;//防具
    vector<Card> treasure;//宝物
    vector<Card> horse_plus;//马+
    vector<Card> horse_minus;//马-
    int health;//体力
    int max_health;//体力上限
    int team;//阵营
    vector<Skill> skills;//技能
};

class Game{//一局游戏的类
public:
    Game(int player_number):player_number(player_number){};
    void start();
    bool starting_finished=false;
    void generate_cards();//生成卡牌
    void check_cards(int type);//查看卡牌 0牌堆 1废牌堆
    void give_initing_cards();//发初始卡牌
    unsigned int random_seed;//随机数种子
    int player_number;
    vector<Player> players;
    vector<int> playing_players;//正在游玩的玩家
    vector<int> begining_players;//开始时玩家
    int this_player;//本机扮演的玩家
    int current_stage;//当前阶段 0准备 1判定 2摸牌 3出牌 4弃牌 5结束
    int current_player;//当前玩家(在playing_players中下标)
    vector<Card> card_deck;//牌堆
    vector<Card> used_deck;//废牌堆
    void execute_code(string name,vector<int> args);//执行指令
    
    void pass();//结束当前阶段
};

//事件处理函数
void game_start();//游戏开始
void round_start();//一轮开始
void turn_start();//回合开始
void turn_finish();//回合结束
void stage_start();//阶段开始
void stage_finish();//阶段结束
void stage_skip();//阶段跳过

//基础游戏行为
void health_decrease();//扣减体力
void health_increase();//体力提升
void move_card(vector<Card>* from,vector<Card>* end,int from_index=0,int end_index=-1);//移动卡牌

#endif /* game_hpp */
