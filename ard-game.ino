#include <C:\Users\macar\Documents\Arduino Script\ard_game_new\ard-game-lib.ino>
// 実行環境によって変える必要があります

class Chrono{
  public:
  int b[3] = {A7, A6, 0};
  bool visible = false;
  Music M = Music(b, 3200, 2); //375
  Chrono(bool arg_visible = true){
    visible = arg_visible;
  }
  void Add_s(MLD array[8]){
    for( int i = 0; i < 8; i++){
      M.sound1[i] = array[i];
    }
  }
  void Do(){
    if(visible == true){
      if (M.cycle_time == 0) { MLD a[8] = {MLD( 0  ), MLD( 0  ), MLD( 0   ), MLD( 0   ), MLD(Ab[2]), MLD(Eb[2]), MLD(Bb[1]), MLD(A[1])};Add_s(a);}
      if (M.cycle_time == 1) { MLD a[8] = {MLD(D[2]), MLD(G[2]), MLD(Gb[2]), MLD(Db[2]), MLD(Ab[1]), MLD(G[1]), MLD(C[2]), MLD(F[2])};Add_s(a);} 
      if (M.cycle_time == 2) { MLD a[8] = {MLD(E[2], 1000), MLD( 0  ), MLD( 0  ), MLD( 0   ), MLD( 0   ), MLD( 0  ), MLD( 0   ), MLD( 0   )};Add_s(a);} 
      if (M.cycle_time == 3) { MLD a[8] = {MLD(0), MLD( 0  ), MLD( 0  ), MLD( 0   ), MLD( 0   ), MLD( 0  ), MLD( 0   ), MLD( 0   )};Add_s(a);}      
      M.Do();
    }
  }
  void Reset(){
    M.Reset();
    visible = true;
  }
  void Stop(){
    M.Reset();
    visible = false;
  }
  void Play(){
    visible = true;
  }
};

int select_game_index = 0;
class PlayerXY{
    public:
      int x = 7;
      int y = 15;
      int h = 5;
      PlayerXY(int arg_x, int arg_y, int arg_h){
        x = arg_x;
        y = arg_y;
        h = arg_h;
      }
  };
class LineData{
  public:
  int speed = 10;
  int direction = -1;
  LineData(){
    if(random(0, 2) == 1){
      direction = 1;
    }
    speed = random(8, 13);
  }
};
class Frogger{
  public:
    PlayerXY player = PlayerXY(7, 15, 5);
    // update処理
    Timer update_t = Timer();
    Timer goal_sound = Timer(false);
    // 死亡時のタイマー
    Timer death = Timer();
    Timer stage_clear_animation = Timer(false);
    LineData line_data[11] = {LineData(),LineData(), LineData(),LineData(), LineData(),LineData(), LineData(),LineData(), LineData(),LineData(), LineData()};
    CrossEffect effect = CrossEffect(0,0);
    int difficulty = 1;
    bool visible = true;
    bool do_handle_event = true;
    unsigned char map[16][16];
    bool game = true;
    bool goal1 = false;
    bool goal2 = false;
    bool goal3 = false;
    Frogger(){
      linemap_reset();
      linemap_create(0);
    }
    void handle_event(){
      if(do_handle_event == true){
        if(input.change_input() == true){
          if(input.cross_up_key == true){
            player.y -= 1;
          }
          else if(input.cross_right_key == true){
            player.x += 1;
          }
          else if(input.cross_left_key == true){
            player.x -= 1;
          }
          else if(input.cross_down_key == true){
            player.y += 1;
          }
          if (player.x < 0){
            player.x = 0;
          }
          if(player.x > 15){
            player.x = 15;
          }
          if(player.y < 0){
            player.y = 0;
          }
          if(player.y > 15){
            player.y = 15;
          }
          if(goal1 == true && player.y == 0 && player.x > 0 && player.x < 5){
            player.y = 1;
          }
          if(goal2 == true && player.y == 0 && player.x > 5 && player.x < 10){
            player.y = 1;
          }
          if(goal3 == true && player.y == 0 && player.x > 10 && player.x < 15){
            player.y = 1;
          }
        }
      }
    }

  void update(){
    effect.update();
    // deathアニメーション終了
    if(effect.effect.update.get_cycle_finish() == true){
      Sound1.Play(A[0], 60);
      update_t.visible = true;
      do_handle_event = true;
      player.x = 7;
      player.y = 15;
    }
    if(goal_sound.Do(5) == true){
      Serial.println(String(goal_sound.cycle_time));
      if(goal_sound.cycle_time == 1) Sound1.Play(E[1], 60);
      if(goal_sound.cycle_time == 2) Sound1.Play(G[1], 60);
      if(goal_sound.cycle_time == 3) Sound1.Play(Bb[1], 60);
      if(goal_sound.cycle_time == 4) Sound1.Play(G[1], 60);
      if(goal_sound.cycle_time == 5) goal_sound.Reset(false);

    }
    if(stage_clear_animation.Do(5) == true){
      map[0][stage_clear_animation.cycle_time - 1] = 0;
      if(stage_clear_animation.cycle_time == 1)Sound1.Play(Ab[1], 60);
      if(stage_clear_animation.cycle_time == 2)Sound1.Play(Gb[1], 60);
      if(stage_clear_animation.cycle_time == 3) Sound1.Play(Ab[1], 60);
      if(stage_clear_animation.cycle_time == 4) Sound1.Play(Db[1], 60);
      if(stage_clear_animation.cycle_time == 5) Sound1.Play(Eb[1], 60);
      if(stage_clear_animation.cycle_time == 6) Sound1.Play(A[1], 60);
      if(stage_clear_animation.cycle_time == 7) Sound1.Play(Ab[1], 60);
      if(stage_clear_animation.cycle_time == 8) Sound1.Play(Gb[1], 60);
      if(stage_clear_animation.cycle_time == 9) Sound1.Play(Eb[1], 60);
      if(stage_clear_animation.cycle_time == 10) Sound1.Play(Bb[1], 60);
      if(stage_clear_animation.cycle_time == 11) Sound1.Play(Gb[1], 60);
      if(stage_clear_animation.cycle_time == 12) Sound1.Play(Ab[1], 60);
      if(stage_clear_animation.cycle_time == 13) Sound1.Play(Eb[1], 60);
      if(stage_clear_animation.cycle_time == 14) Sound1.Play(Bb[1], 60);
      if(stage_clear_animation.cycle_time == 15) Sound1.Play(Ab[1], 60);
      if(stage_clear_animation.cycle_time == 16) Sound1.Play(Bb[1], 60);
      if(stage_clear_animation.cycle_time == 16){
        stage_clear_animation.Reset(false);
        do_handle_event = true;
        stage_clear();
      }
    }
    if(update_t.Do(10) == true){
      if(hit_player() == true){
        death_player();
        effect.play(3, player.x, player.y);
        update_t.visible = false;
      }
      if(goal1 == true && goal2 == true && goal3 == true){
        stage_clear_animation.Reset(true);
        do_handle_event = false;
        goal1 = false;
      }
    }
    if(do_handle_event == true){
      Shift_obs();
    }
    goal_player();
  }
  void draw(){
    for(int i = 0; i < 16; i++){
      for(int j = 0; j < 16; j++){
        if(map[i][j] == 1){
          matrix.map[i][j] = 1;
        }
      }
    }
    matrix.map[player.y][player.x] = 2;
    effect.draw();
  }

  // フロッガーが死亡したとき
  void death_player(){
      player.h -= 1;
      do_handle_event = false;
      Sound1.Play(C[1], 60);
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
  void stage_clear(){
    goal1 = goal2 = goal3 = false;
    difficulty += 1;
    linemap_reset();
    linemap_create(difficulty);
  }
  void goal_player(){
    for(int i = 1; i < 5; i++){
      if(player.x == i && player.y == 0){
        goal_sound.Reset(true);
        goal1 = true;
        player.x = 7;
        player.y = 15;
        map[0][1] = map[0][2] = map[0][3] = map[0][4] = 1;
      } 
    }
    for(int i = 6; i < 10; i++){
      if(player.x == i && player.y == 0){
        goal_sound.Reset(true);
        goal2 = true;
        player.x = 7;
        player.y = 15;
        map[0][6] = map[0][7] = map[0][8] = map[0][9] = 1;
      } 
    }
    for(int i = 11; i < 15; i++){
      if(player.x == i && player.y == 0){
        goal_sound.Reset(true);
        goal3 = true;
        player.x = 7;
        player.y = 15;
        map[0][11] = map[0][12] = map[0][13] = map[0][14] = 1;
      } 
    }
  }
  // マップをリセットする all 0
  void linemap_reset(){
    for(int i = 0; i < 16; i++){
      for(int j = 0; j < 16; j++){
        map[i][j] = 0;
      }
    }
  }
  int c_s = 0;
  void Shift_obs(){
    c_s++;
    int LineData_index = 0;
    for(int i = 12; i < 15; i++){
      Shift_obs_line(i, LineData_index);
      LineData_index++;
    }
    for(int i = 1; i < 9; i++){
      Shift_obs_line(i, LineData_index);
      LineData_index++;
    }
  }
  void Shift_obs_line(int raw, int linedata_index){
    if(c_s % line_data[linedata_index].speed == 0){
      if(line_data[linedata_index].direction == -1){
        int end_true = false;
        if(map[raw][15] == 1){
          end_true = true;
        }
        for (int i = 14; i != -1; i--){
          map[raw][i + 1] = map[raw][i];
        }
        if(end_true == true){
          map[raw][0] = true;
        }
        else{
          map[raw][0] = false;
        }
      }
      if(line_data[linedata_index].direction == 1){
        int end_true = false;
        if(map[raw][0] == 1){
          end_true = true;
        }
        for (int i = 1; i != 16; i++){
          map[raw][i - 1] = map[raw][i];
        }
        if(end_true == true){
          map[raw][15] = true;
        }
        else{
          map[raw][15] = false;
        }
      }
    }
  }
  // マップを生成する
  void linemap_create(int create_map_difficulty = 0){
    map[0][0] = map[0][5] = 1;
    map[0][10] = map[0][15] = 1;
    if(create_map_difficulty == 0){
      for(int i = 12; i < 15; i++){
        InsertLine(i, 3);
      }
      for(int i = 1; i < 9; i++){
        InsertLine(i, 4);
      }
    }
    if(create_map_difficulty == 1){
      for(int i = 12; i < 15; i++){
        InsertLine(i, 5);
      }
      for(int i = 1; i < 9; i++){
        InsertLine(i, 6);
      }
    }
    if(create_map_difficulty == 2){
      for(int i = 12; i < 15; i++){
        InsertLine(i, 7);
      }
      for(int i = 1; i < 9; i++){
        InsertLine(i, 8);
      }
    }
    if(create_map_difficulty == 3){
      for(int i = 12; i < 15; i++){
        InsertLine(i, 9);
      }
      for(int i = 1; i < 9; i++){
        InsertLine(i, 10);
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
class FroggerGame{
  public:
  bool visible = true;
  Frogger frogger = Frogger();
  FroggerGame(bool arg_visible = true){
    visible = arg_visible;
  } 
  void handle_event(){
    if(visible == true) frogger.handle_event();
  }
  void update(){
    if(visible == true) frogger.update();
  }
  void draw(){
    if(visible == true) frogger.draw();
  }
};




class Tail{
      public:
      unsigned char x;
      unsigned char y;
      bool visible = false;
    };
    class Food{
    public:
    unsigned char x;
    unsigned char y;
    bool visible = false;
  };
//game classes
class Snake{
  public:
  String direction = "right"; //進行方向
  int speed = 6;//数値が小さいほど遅い
  int x; //スネーク自身の座標 
  int y;
  unsigned char random_x, random_y;
  unsigned int tail_len = 0;
  Tail tail_array[256];
  bool visible = true; //自身を描写するか
  bool gameover = false; //ゲーム終了
  Food food;
  Timer death;
  int death_x = 0;
  int death_y = 0;
  Timer move;
  Snake(){
    Serial.begin(9600);
    for(unsigned int i = 0; i < 256; i++){
      tail_array[i].visible = false;
    }
    int rx, ry;
    create_random();
    food.x = random_x;
    food.y = random_y;
  }
  void handle_event(){
    if(input.cross_up_key == true){
      direction = "up";
    }
    else if(input.cross_right_key == true){
      direction = "right";
    }
    else if(input.cross_left_key == true){
      direction = "left";
    }
    else if(input.cross_down_key == true){
      direction = "down";
    }
  }
  void update(){
    if(visible == true){
      if(move.Do(speed) == true){
        if(direction == "up"){
          add_tail(x, y); //移動前の座標を挿入する
          y = y - 1;
          if (y < 0){
            y = 15; //マップ外だった場合、そこから対称の地点に移動する
          }
        }
        if(direction == "right"){
          add_tail(x, y);
          x = x + 1;
          if (x > 15){
            x = 0;
          }
        }
        if (direction == "left"){
          add_tail(x, y);
          x = x - 1;
          if (x < 0){
            x = 15;
          }
        }
        if (direction == "down"){
          add_tail(x, y);
          y = y + 1;
          if (y > 15){
            y = 0;
          }
        }
        if(hit_tail() == true){
          visible = false;
        }
        hit_food();
      }
    }
    else{
      if(death.Do(3) == true){
        if (tail_array[death.cycle_time - 1].visible == true){
          tail_array[death.cycle_time - 1].x = death_x;
          tail_array[death.cycle_time - 1].y = death_y;
          death_x++;
          if(death_x > 15){
            death_y++;
            death_x = 0;
          }
          
          if(death.cycle_time % 2 == 0) Sound1.Play(C[1], 50);
          if(death.cycle_time % 2 == 1) Sound1.Play(B[0], 50);
        }
        else {
          Sound1.Play(A[0], 500);
          gameover = true;
          food.x = death_x;
          food.y = death_y;
        }
      }
    }
  }
  bool hit_food(){
    if (x == food.x && y == food.y){
      for(unsigned int i = 0; i < 256; i++){
        if(tail_array[i].visible == false){
          tail_array[i].visible = true;
          break;
        }
      }
      Sound1.Play(E[1], 100);
      //Sound2.Play(F[1], 50);
      create_random();
      food.x = random_x;
      food.y = random_y;
        return true;
      }
    return false;
  }
  void add_tail(unsigned int arg_x, unsigned int arg_y){
    array_shift();
    tail_array[0].x = arg_x;
    tail_array[0].y = arg_y;
  }
  void draw(){
    if(visible == true){
       matrix.map[y][x] = 1;
    }
    matrix.map[food.y][food.x] = 2;
    
    for(unsigned int i = 0; i < 256; i++){
      if(tail_array[i].visible == true){
        matrix.map[tail_array[i].y][tail_array[i].x] = 1;
      }
    }

  }
  void create_random(){
    bool cant_break = false;
    while(true){
      int r_x = random(0, 16);
      int r_y = random(0, 16);
      if(r_x == x && r_y == y){
        cant_break = true;
      }
      for(unsigned int i = 0; i < 256; i++){
        if(tail_array[i].visible == true){
          if(r_x == tail_array[i].x && r_y == tail_array[i].y){
            cant_break = true;
          }
        }
      }
      if(cant_break == false){
        random_x = r_x;
        random_y = r_y;
        break;
      }
      else{
        cant_break = false;
      }
    }
  }
  bool hit_tail(){
    for(unsigned int i = 0; i < 256; i++){
      if(tail_array[i].visible == true){
        if(x == tail_array[i].x && y == tail_array[i].y){
          return true;
        }
      }
    }
    return false;
  }
  void array_shift(){
    for(int i = (sizeof(tail_array) / sizeof(tail_array[0]) - 1); i != 0 ; i--){
      tail_array[i].x = tail_array[i - 1].x;
      tail_array[i].y = tail_array[i - 1].y;
    }
  }
};
class SnakeGame {
  public:
    Snake snake;
    bool visible = true;
    SnakeGame(bool arg_visible = true){
      visible = arg_visible;
    }
    void handle_event(){
      if(visible == true){
        input.update();
        if(input.side_right_key == true){
          snake = Snake();
        }
        if(snake.gameover == false){
          snake.handle_event();
        }
      }
    }
    void update(){
      if(visible == true){
        if(snake.gameover == false){
          snake.update();
        }
      }
    }
    void draw(){
      if(visible == true){
        snake.draw();
      }
    }
};













