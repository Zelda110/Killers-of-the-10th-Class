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
    Card(int suit,int number,string name,int type,string distribe,string quotation,Texture* texture):suit(suit),number(number),name(name),type(type),distribe(distribe),quotation(quotation),texture(texture){};
    string name;
    string distribe;//牌的介绍
    string quotation;//牌底的名言
    Texture* texture;//牌面纹理
    int number;//点数 0-13
    int suit;//花色 -1特殊 0红桃 1黑桃 2梅花 3方块
    int type;//牌型 -1特殊 0普通 1锦囊 2装备 3武器 4宝物 5马+ 6马-
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
    vector<int> subject;//学科 0神 1数学 2物理 3化学 4生物 5信息 6英语
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
    int team;//阵营 0老常 1班委 2学生 3班长
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
    
    void pass();//跳过当前阶段
};

void give_card(vector<Card>* from,vector<Card>* end,int from_index=0,int end_index=-1);

void hurt(Game* maingame,int player_index,int number);

namespace skills_exes {

//游戏逻辑
//跳过此阶段
void pass(Game* maingame,Skill* skill);//arg格式:{}

//基本牌
//杀
void slash(Game* maingame,Skill* skill);//arg格式:{伤害数,响应能力(可/否)0/1,属性(普通/火焰/雷电)0/1/2}
//桃
void peach(Game* maingame,Skill* skill);//arg格式:{回复量}

}

namespace skills_clients {

//游戏逻辑
//跳过此阶段
vector<vector<int>> pass(Game* maingame,int using_player);

//基本牌
//桃
vector<vector<int>> peach(Game* maingame,int using_player);

}

namespace skills_judges {

//游戏逻辑
//跳过此阶段
bool pass(Game* maingame,int using_player);

}

extern vector<Skill> game_logic_skills;//所有游戏逻辑技能
extern vector<Skill> basic_card_skills;//所有基本牌技能

#endif /* game_hpp */
