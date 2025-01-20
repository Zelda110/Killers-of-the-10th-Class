//
//  game.cpp
//  Killers of the 10th Class
//
//  Created by ハイラル・ゼルダ on 20/10/2024.
//

#include "game.hpp"

bool game_stopped=false;

Game* MAINGAME=nullptr;

General wcy("王成悦",&generals::wcy,3);
General lcy("刘宸驿",&generals::lcy,5);
General Trunchbull("Miss.Trunchbull",&generals::Trunchbull,32);
General Matilda("Matilda",&generals::Matilda,2);

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
            card_deck
                .push_back(Card(suit,num,"大师之剑",0,"","",&cards::MasterSword));
        }
    }
    for (int suit=0; suit<=3; suit++) {
        for (int num=1; num<=13; num++) {
            card_deck.push_back(Card(suit,num,"加多宝",0,"","",&cards::JDB));
        }
    }
    cout<<"Card deck generated."<<endl;
    check_cards(0);
}

void move_card(vector<Card>* from,vector<Card>* end,int from_index,int end_index){
    if (from_index<0) {
        from_index+=from->size();
    }
    if (end_index<0) {
        end_index+=end->size()+1;
    }
    end->insert(end->begin()+end_index, *(from->begin()+from_index));
    from->erase(from->begin()+from_index);
}

void Game::give_initing_cards(){
    for (int i=0; i<players.size(); i++) {
        for (int m=0; m<4; m++) {
            move_card(&card_deck, &players[i].cards);
        }
        cout<<"Initing cards given."<<endl;
    }
}

void Game::start(){
    random_seed=time(0);
    this_player=2;//待定
    players.clear();
    generate_cards();//生成牌堆
    //初始化玩家
    for (int i=0; i<player_number; i++) {
        players.push_back(Player("player"+to_string(i),Matilda));
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
        ;
    }
    
    cout<<"Game started."<<endl;
    starting_finished=true;
    
    //游戏主循环
    while (!game_stopped) {
        Player* current_player_ptr=&players[playing_players[current_player]];
        switch (current_stage) {
            case 0://准备阶段
                break;
            case 1://判定阶段
                break;
            case 2://摸牌阶段
                for (int m=0; m<2; m++) {
                    move_card(&card_deck, &players[playing_players[current_player]].cards);
                }
                break;
            case 3://出牌阶段
                break;
            case 4://弃牌阶段
                break;
            case 5://结束阶段
                break;
            default:
                break;
        }
        WaitTime(0.1);
    }
    return;
}
