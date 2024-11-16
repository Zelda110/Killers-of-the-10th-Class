//
//  game.cpp
//  Killers of the 10th Class
//
//  Created by ハイラル・ゼルダ on 20/10/2024.
//

#include "game.hpp"

bool game_stopped=false;

Game* MAINGAME=nullptr;

General L_Hospital("甄姬",&generals::L_Hospital,3);
General wcy("王成悦",&generals::wcy,3);
General lcy("刘宸驿",&generals::lcy,4);

string Card::get_info(){
    return to_string(suit)+" "+to_string(number)+" "+name+" "+to_string(type);
}

void Skill::perform(int using_player,vector<int> target_players,vector<int> target_cards,vector<int> target_positions,vector<int> args){
    this->using_player=using_player;
    this->target_players=target_players;
    this->target_cards=target_cards;
    this->target_positions=target_positions;
    this->args=args;
    use(MAINGAME, this);
}

void Game::check_cards(int type){
    if (type==0) {
        for (int i=0; i<card_deck.size(); i++) {
            cout<<card_deck[i].get_info()<<endl;
        }
    }
    else{
        for (int i=0; i<used_deck.size(); i++) {
            cout<<used_deck[i].get_info()<<endl;
        }
    }
}

void Game::generate_cards(){
    for (int suit=0; suit<=3; suit++) {
        for (int num=1; num<=13; num++) {
            card_deck.push_back(Card(suit,num,"杀",0,"","",&cards::slash));
        }
    }
    for (int suit=0; suit<=3; suit++) {
        for (int num=1; num<=13; num++) {
            card_deck.push_back(Card(suit,num,"桃",0,"","",&cards::peach));
        }
    }
    cout<<"Card deck generated."<<endl;
    check_cards(0);
}

void Game::give_initing_cards(){
    for (int i=0; i<players.size(); i++) {
        for (int m=0; m<4; m++) {
            give_card(&card_deck, &players[i].cards);
        }
        cout<<"Initing cards given."<<endl;
    }
}

void Game::pass(){
    current_stage++;
    if (current_stage>5) {
        current_stage%=5;
        current_player++;
        if (current_player>=playing_players.size()) {
            current_player%=playing_players.size();
        }
    }
}

void give_card(vector<Card>* from,vector<Card>* end,int from_index,int end_index){
    if (from_index<0) {
        from_index+=from->size();
    }
    if (end_index<0) {
        end_index+=end->size()+1;
    }
    end->insert(end->begin()+end_index, *(from->begin()+from_index));
    from->erase(from->begin()+from_index);
}

void hurt(Game* maingame,int player_index,int number){
    maingame->players[player_index].health-=number;
}

namespace skills_exes {

void slash(Game* maingame,Skill* skill){
    ;
}

void peach(Game* maingame,Skill* skill){
    maingame->players[skill->targets[0]].health++;
}

}

vector<Skill> basic_card_skills={
    Skill("桃",0,3,skills_exes::peach),
};

void Game::start(){
    random_seed=time(0);
    this_player=2;//待定
    players.clear();
    generate_cards();//生成牌堆
    //初始化玩家
    for (int i=0; i<player_number; i++) {
        players.push_back(Player("player"+to_string(i),wcy));
        playing_players.push_back(i);
    }
    srand(random_seed);
    shuffle(playing_players.begin(),
            playing_players.end(),
            default_random_engine(random_seed));//打乱玩家顺序
    shuffle(card_deck.begin(),
            card_deck.end(),
            default_random_engine(random_seed));//打乱牌堆顺序
    begining_players=playing_players;
    //发初始手牌
    give_initing_cards();
    cout<<"player_list:";
    for (int i=0; i<playing_players.size(); i++) {
        cout << playing_players[i];
    }
    cout<<endl;
    //初始化玩家生命值
    for (int i=0; i<players.size(); i++) {
        int h=players[i].general.max_health;
        players[i].max_health=h;
        players[i].health=h;
    }
    
    //初始化游戏状态机
    current_player=0;
    current_stage=0;
    
    //初始化玩家技能
    for (int i=0; i<players.size(); i++) {
        for (int j=0; j<basic_card_skills.size(); j++) {
            players[i].skills.push_back(basic_card_skills[j]);
        }
    }
    
    cout<<"Game started."<<endl;
    starting_finished=true;
    
    //游戏主循环
    while (!game_stopped) {
        Player* current_player_ptr=&players[playing_players[current_player]];
        switch (current_stage) {
            case 0://准备阶段
                pass();
                break;
            case 1://判定阶段
                pass();
                break;
            case 2://摸牌阶段
                for (int m=0; m<2; m++) {
                    give_card(&card_deck, &players[playing_players[current_player]].cards);
                }
                for (int i=0; i<current_player_ptr->skills.size(); i++) {
                    current_player_ptr->skills[i].perform(playing_players[current_player], {playing_players[current_player]}, {});
                }
                pass();
                break;
            case 3://出牌阶段
                pass();
                break;
            case 4://弃牌阶段
                pass();
                break;
            case 5://结束阶段
                pass();
                break;
            default:
                break;
        }
        WaitTime(0.1);
    }
    return;
}
