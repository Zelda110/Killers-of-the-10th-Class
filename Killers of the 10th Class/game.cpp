//
//  game.cpp
//  Killers of the 10th Class
//
//  Created by ハイラル・ゼルダ on 20/10/2024.
//

#include "game.hpp"

General L_Hospital("甄姬",&generals::L_Hospital,3);
General wcy("王成悦",&generals::wcy,3);
General lcy("刘宸驿",&generals::lcy,4);

string Card::get_info(){
    return to_string(suit)+" "+to_string(number)+" "+name+" "+to_string(type);
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
    
    cout<<"Game started."<<endl;
    starting_finished=true;
    
    while (!WindowShouldClose()) {
        pass();
        WaitTime(0.01);
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
