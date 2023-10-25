#include <C:\Users\macar\Documents\Arduino Script\ard_game_new\ard-game-lib.ino>
// 実行環境によって変える必要があります
class FroggerGame{
  public:
    
};
class Frogger{
  public:
    PlayerXY player = PlayerXY(0, 0, 5);
    // update処理
    Timer update = Timer();
    // 死亡時のタイマー
    Timer death = Timer();
    CrossEffect effect = CrossEffect(0,0);
    int difficulty = 1;
    bool visible = true;
    bool do_handle_event = true;
    unsigned char map[16][16];
    bool game = true;
    Frogger(){
      linemap_reset();
      linemap_create(0);
    }
    void handle_event(){
      if(do_handle_event == true){
        if(input.cross_up_key == true){
          player.x -= 1;
        }
        else if(input.cross_right_key == true){
          player.x += 1;
        }
        else if(input.cross_left_key == true){
          player.y -= 1;
        }
        else if(input.cross_down_key == true){
          player.y += 1;
        }
        if (player.x < 0){
          player.x = 0;
        }
        if(player.x > 15){
          player.y = 15;
        }
        if(player.y < 0){
          player.y = 0;
        }
        if(player.y > 15){
          player.y = 15;
        }
      }
    }

  void update(){
    effect.update();
    // deathアニメーション終了
    if(effect.effect.update.get_cycle_finish() == true){
      update.visible = true;
      do_handle_event = true;
    }
    if(update.Do(10) == true){
      if(hit_player() == true){
        death_player();
        effect.play(5, player.x, player.y);
        update.visible = false;
      }
    }
  }
  /// フロッガーが死亡したとき
  void death_player(){
      player.h -= 1;
      do_handle_event = false;
      if(player.h == 0){
        game_over();
      }
      else{

      }
  }
  /// ゲーム終了
  void game_over(){
    game = false;
  }
  bool game_stute(){
    return game;
  }
  // フロッガーが物体に衝突したとき
  bool hit_player(){
    if(map[player.y][player.x] == 1){
      return true;
    }
    return false;
  }
  // マップをリセットする all 0
  void linemap_reset(){
    for(int i = 0; i < 16; i++){
      for(int j = 0; j < 16; j++){
        map[i][j] = 0;
      }
    }
  }

  // マップを生成する
  void linemap_create(int create_map_difficulty = 0){
    if(create_map_difficulty == 0){
      for(int i = 10; i < 15; i++){
        InsertLine(i, 4);
      }
      for(int i = 2; i < 9; i++){
        InsertLine(i, 5);
      }
    }
    if(create_map_difficulty == 1){
      for(int i = 10; i < 15; i++){
        InsertLine(i, 8);
      }
      for(int i = 2; i < 9; i++){
        InsertLine(i, 7);
      }
    }
    if(create_map_difficulty == 2){
      for(int i = 10; i < 15; i++){
        InsertLine(i, 12);
      }
      for(int i = 2; i < 9; i++){
        InsertLine(i, 10);
      }
    }
    if(create_map_difficulty == 3){
      for(int i = 10; i < 15; i++){
        InsertLine(i, 16);
      }
      for(int i = 2; i < 9; i++){
        InsertLine(i, 15);
      }
    }
  }
  // map_rawの行にobs_num分の障害物を並べる
  void InsertLine(int map_raw, int obs_num){
    for(int i = 0; i < 16; i++){
      map[map_raw][i] = 0;
    }
    int insert_index = random(0, 16);
    for(int i = 0; i < obs_num; i++){
      map[map_raw][insert_index] = 1;
      insert_index += 1;
      if(insert_index > 15){
        insert_index = 0;
      }
    }
  }

};
class PlayerXY{
    public:
      int x = 0;
      int y = 0;
      int h = 5;
      PlayerXY(int arg_x, int arg_y, int arg_h){
        x = arg_x;
        y = arg_y;
        h = arg_h;
      }
  }