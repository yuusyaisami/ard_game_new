#include <C:\Users\豊橋工科高生徒00601\Documents\C++File\ard_game_new\ard-game-lib.ino>
SnakeGame snake_game;
Chrono chrono;
Timer select_response_time;
FroggerGame frogger_game;
int select_now = 0;
void setup(){

}
class OS{
  public:
  String select_game_name = "snake_game"; //進行中のゲーム
  OS(){
  
  }
  void Start(String game_type = "snake_game"){
    select_game_name = game_type;
    if(game_type == "snake_game"){
      snake_game = SnakeGame();
      select_game_index = 0;
    }
    if(game_type == "frogger_game"){
      frogger_game = FroggerGame();
      select_game_index = 1;
    }
    if(game_type == "Chrono"){
      chrono = Chrono();
      select_game_index = 2;
    }
  }
  void Stop(){
    select_game_index = -1;
    snake_game = SnakeGame(false);
    chrono = Chrono(false);
    frogger_game = FroggerGame(false);
  }
};
OS os = OS();
void loop(){
  input.update();
  if(input.side_left_key == true){
    os.Stop();
  }
    if(select_game_index == 0){
        snake_game.handle_event();

        snake_game.update();

        snake_game.draw();

        matrix.draw();

        matrix.clear();
    }
    else if(select_game_index == 1){
        frogger_game.handle_event();

        frogger_game.update();

        frogger_game.draw();

        matrix.draw();

        matrix.clear();
    }
    else if(select_game_index == 2){
      chrono.Do();
      if(input.side_right_key == true){
        chrono.Reset();
      }
    }
    
    else{
        input.update();
        if(input.change_input(true) == true){
            if(input.cross_right_key == true){
              select_now += 1;
            }
            else if(input.cross_left_key == true){
              if (select_now > 0){
               select_now -= 1; 
              }
            }
            else if(input.select_up_key == true){
              if(select_now == 0){ //スネークゲーム
                os.Start("snake_game");
              }
              if(select_now == 1){
                os.Start("frogger_game");
              }
              if(select_now == 2){
                os.Start("Chrono");
              }
            }
        }
        matrix.map[1][1 + 1 * select_now] = 1;
        matrix.draw();
        matrix.clear();
    }
}