unsigned char O_SER = 5;
unsigned char O_RCLK = 6;
unsigned char O_SRCLK = 7;

unsigned int C[3] = {130, 262, 523};
  unsigned int Db[3] = {139, 277, 554};
  unsigned int D[3] = {147, 294, 587};
  unsigned int Eb[3] = {155, 331, 622};
  unsigned int E[3] = {165, 330, 659};
  unsigned int F[3] = {175, 349, 698};
  unsigned int Gb[3] = {185, 370, 392};
  unsigned int G[3] = {196, 392, 784};
  unsigned int Ab[3] = {208, 415, 831};
  unsigned int A[3] = {220, 440, 880};
  unsigned int Bb[3] = {233, 466, 932};
  unsigned int B[3] = {247, 494, 988};

unsigned char ColumnPin[8] = {4, 10, 11, 1, 13, 2, 6, 7};
unsigned char RowPin[8] =       {0, 5, 15, 3, 8, 14, 9, 12};
//入力状態を取得するときに使用するクラス
class Input{
  public:
    bool InputData[8] = {false, false, false, false, false, false, false, false,};
    bool InputData_old[8] = {false, false, false, false, false, false, false, false,};
    unsigned int IN_PUT_PIN[8] = {4, 3, 11, 2, 12, 9, 10, 8};
    bool cross_up_key = false;
    bool cross_left_key = false;
    bool cross_right_key = false;
    bool cross_down_key = false;

    bool side_left_key = false;
    bool side_right_key = false;
    bool select_up_key = false;
    bool select_down_key = false;

    bool side_key_ = false;
    bool change = false;
    Input(){
      for (unsigned int i=0; i<8; i++){
        pinMode( IN_PUT_PIN[8] , INPUT );
      }
    }
    //入力状態を取得する。
    void update(){
      for (unsigned int i=0; i<8; i++){
        InputData[i] = (digitalRead(IN_PUT_PIN[i]));
      }
      side_left_key = InputData[0];
      cross_up_key = InputData[1];
      cross_right_key = InputData[2];
      cross_left_key = InputData[3];
      cross_down_key = InputData[4];
      select_up_key = InputData[5];
      select_down_key = InputData[6];
      side_right_key = InputData[7];
    }
    // 入力状態に変化があった場合Trueを返すfe
    bool change_input(bool not_return_all_zero = false){
    for (unsigned int i=0; i<8; i++){
      if (InputData_old[i] != InputData[i]){
        for (unsigned int j=0; j<8; j++){
          InputData_old[j] = InputData[j];
        }
        if(not_return_all_zero == true){
          for (unsigned int j=0; j<8; j++){
            if(InputData_old[j] == true){
              return true;
            }
          }
          return false;
        }
        return true;
      }
    }
    return false;
}
};
//製作者用
class ShiftRegister {
public:
    bool light_map[8][8];
    bool light_line[64];
    ShiftRegister(){
      
      init();
    }
    void init(){
      for(int i = 0; i < sizeof(light_map) / sizeof(light_map[0]); i++){
        for (int j = 0; j < sizeof(light_map[0]) / sizeof(light_map[0][0]); j++){
          light_map[i][j] = false;
        }
      }
      digitalWrite(O_RCLK, LOW);
      for(int i = 0; i < 64;i++){
        digitalWrite(O_SER, LOW);
        //クロックパルスを立ち上げる
    	  digitalWrite(O_SRCLK, HIGH);
    	  digitalWrite(O_SRCLK, LOW);
        //-----------------------------------------
      }
      digitalWrite(O_RCLK, HIGH);
    }
    void get_map(int shift_num , unsigned char map[][16]){
      if (shift_num == 0){
        for (int i = 0; i < 8 ; i++){
          for (int j = 0; j < 8; j++){
            if(map[i][j] != 0){
              light_map[i][j] = true;
            }
            else {
              light_map[i][j] = false;
            }
          }
        }
      }
      if (shift_num == 1){
        for (int i = 0; i < 8 ; i++){
          for (int j = 8; j < 16; j++){
            if(map[i][j] != 0){
              light_map[i][j - 8] = true;
            }
            else {
              light_map[i][j - 8] = false;
            }
          }
        }
      }
      if (shift_num == 3){
        for (int i = 8; i < 16 ; i++){
          for (int j = 0; j < 8; j++){
            if(map[i][j] != 0){
              light_map[i - 8][j] = true;
            }
            else {
              light_map[i - 8][j] = false;
            }
          }
        }
      }
      if (shift_num == 2){
        for (int i = 8; i < 16 ; i++){
          for (int j = 8; j < 16; j++){
            if(map[i][j] != 0){
              light_map[i - 8][j - 8] = true;
            }
            else {
              light_map[i - 8][j - 8] = false;
            }
          }
        }
      }
    }
    void get_line(int raw , unsigned char map[][16]){
      for (int shift_num = 0; shift_num < 4; shift_num++){
        for (int i = 0; i < sizeof(RowPin) / sizeof(RowPin[0]); i++){
          light_line[RowPin[i] + 16 * shift_num] = true;
        }
      }
      for (int shift_num = 0; shift_num < 4; shift_num++){
        for (int i = 0; i < sizeof(ColumnPin) / sizeof(ColumnPin[0]); i++){
          light_line[ColumnPin[i] + 16 * shift_num] = false;
        }
      }

      for(int r = 0; r < 4; r++){
        get_map(r, map);
        for (int j = 0; j < sizeof(light_map[0]) / sizeof(light_map[0][0]); j++){
          if ( light_map[raw][j] == true){
            light_line[ColumnPin[j] + 16 * r] = true;
            light_line[RowPin[raw] + 16 * r] = false;
          }
        }      
      }
    }
    void LightUp(unsigned char map[][16]){
      for (int r = 0; r < 8; r++){
      get_line(r, map);
      digitalWrite(O_RCLK, LOW);
      for(int c = 63; c != -1;c--){
        if (light_line[c] == true){
          digitalWrite(O_SER, HIGH);
        }
        else{
          digitalWrite(O_SER, LOW);
        }
        //クロックパルスを立ち上げる
        digitalWrite(O_SRCLK, HIGH);
        digitalWrite(O_SRCLK, LOW);
        //-----------------------------------------
      }
      digitalWrite(O_RCLK, HIGH);
    }
  }
};
class DotMatrix {
public:
    unsigned char map[16][16];
    ShiftRegister shift_register;
    int count = 0;
    DotMatrix(){
      pinMode( O_SER,  OUTPUT );
      pinMode( O_RCLK, OUTPUT );
      pinMode( O_SRCLK, OUTPUT );
      map_init();
      Serial.begin(9600);
    }
    void update(){
    }
    void draw(){
      count += 1;
      if (count % 5 == 0){
        for(unsigned char i = 0; i < sizeof(map) / sizeof(map[0]); i++){
          for (unsigned char j = 0; j < sizeof(map[0]) / sizeof(map[0][0]); j++){
            if(map[i][j] == 2){
              map[i][j] = 0;
            }
          }
        }
      }
      if (count % 2 == 0){
        for(unsigned char i = 0; i < sizeof(map) / sizeof(map[0]); i++){
          for (unsigned char j = 0; j < sizeof(map[0]) / sizeof(map[0][0]); j++){
            if(map[i][j] == 3){
              map[i][j] = 0;
            }
          }
        }
      }
      shift_register.LightUp(map);
    }
    void clear(){
      for(unsigned char i = 0; i < sizeof(map) / sizeof(map[0]); i++){
        for (unsigned char j = 0; j < sizeof(map[0]) / sizeof(map[0][0]); j++){
          map[i][j] = 0;
        }
      }
    }
    void map_init(){
      for(unsigned char i = 0; i < sizeof(map) / sizeof(map[0]); i++){
        for (unsigned char j = 0; j < sizeof(map[0]) / sizeof(map[0][0]); j++){
          map[i][j] = 0;
        }
      }
    }
    void map_debug_view(){
      for(unsigned char i = 0; i < sizeof(map) / sizeof(map[0]); i++){
        for (unsigned char j = 0; j < sizeof(map[0]) / sizeof(map[0][0]); j++){
          if (map[i][j] == true){
            Serial.write("1, ");
          }
          else if (map[i][j] == false){
            Serial.write("0, ");
          }
          else{
            Serial.write("N, ");
          }
        }
        Serial.println();
      }
    }
};



Input input;
DotMatrix matrix;



class Timer{
public:
    class Count{
        public:
        int increase = 0;
        int first = 0;
        int value = 0;
        Count(int a_increase, int a_first){
            increase = a_increase;
            first = a_first;
            value = a_first;
        }
        bool Do(int goal){
            value += increase;
            if (value % goal == 0){
                return true;
            }
            return false;
        }
        void Reset(){
            int value = 0;
        }
    };
    String type = "cycle";
    bool finish_cycle = false;
    bool visible = true;
    int cycle_time = 0;
    // DoをなんかいTrueで返すか
    int cycle_finish_time = 0;
    Count count = Count(1, 0);
    // type : timeの時は、cycle_finish_timeを記述してください
    Timer(bool a_visible = true, String a_type = "cycle", int a_cycle_finish_time = 0){
        visible = a_visible;
        type = a_type;
        cycle_finish_time = a_cycle_finish_time;
    }
    bool Do(int goal){
        if(visible == true){
            if(count.Do(goal) == true){
                cycle_time += 1;
                if(cycle_finish_time < cycle_time && type == "time"){
                    Reset(false);
                    finish_cycle = true;
                }
                return true;
            }
        }
        return false;
    }
    bool get_cycle_finish(){
        if(finish_cycle == true){
            finish_cycle = false;
            return true;
        }
        return false;
    }
    void Reset(bool change_visible){
        count.Reset();
        finish_cycle = false;
        cycle_time = 0;
        visible = change_visible;
    }
};


class Sound{
  public:
  class Melody{
    int MS;
    int ST;
    int SI;
    int VT;
    public:
        Melody(unsigned int musical_scale, unsigned int sound_time, unsigned int sound_index, int value_timing){
            MS = musical_scale;
            ST = sound_time;
            SI = sound_index;
            VT = value_timing;
        }
        void play(int value){
            if(SI == 1 && value == VT){
                tone(A7, MS,ST);
            }
        }
  };
  
  int pin = 0;
  int value = 0;
    Sound(int sound_pin){
      pin = sound_pin;
    }
    void Play(unsigned int musical_scale, unsigned int sound_time, unsigned int sound_index){
        if(sound_index == 1){
            tone(A7, musical_scale,sound_time);
        }
        if(sound_index == 2){
            tone(A6, musical_scale,sound_time);
        }
    }
    void MusicPlay(){

        value += 1;
    }
};


// これはeffectクラスたちの属性的構造体です
class Effect{
    public:
    Timer update = Timer(true, "time", 8);
    bool visible = true;
    Effect(){
        return ;
    }
};
class EffectPoint{
    public:
        int x = 0;
        int y = 0;
        int effect_point_type = 3;
        bool visible = false;
        EffectPoint(int arg_x, int arg_y){
            x = arg_x;
            y = arg_y;
        }
        void overflow_checker(){
            if (x < 0 || x > 15){
              x = 0;
              visible = false;
            }
            if (y < 0 || y > 15){
              y = 0;
              visible = false;
            }
        }
        void draw(){
            if(visible == true) matrix.map[y][x] = effect_point_type;
        }
    };

class CrossEffect{
    public:
    Effect effect = Effect();
    EffectPoint EP_LU;
    EffectPoint EP_LD;
    EffectPoint EP_RU;
    EffectPoint EP_RD;
    CrossEffect(int arg_x, int arg_y){
        EP_LU = EffectPoint(arg_x, arg_y);
        EP_LD = EffectPoint(arg_x, arg_y);
        EP_RU = EffectPoint(arg_x, arg_y);
        EP_RD = EffectPoint(arg_x, arg_y);
    }
    void update(){
        if(effect.update.Do(5) == true){
            EP_LU.y -= 1;
            EP_LU.x -= 1;

            EP_LD.y += 1;
            EP_LD.x -= 1;

            EP_RU.y -= 1;
            EP_RU.x += 1;

            EP_RD.y += 1;
            EP_RD.x += 1;
            if(effect.update.finish_cycle == true){
                EP_LD.visible = EP_LU.visible = EP_RD.visible = EP_RU.visible = false;
            }
        }
    }
    // effectを再生します、update関数を常に実行している状態にしてください。また、終了の合図はeffect.update.get_cycle_finish()で確認してください
    void play(int playtime, int arg_x, int arg_y){
        effect.update.Reset(true);
        effect.update.cycle_finish_time = playtime;
        effect.update.type = "time";
        EP_LD.x = EP_LU.x = EP_RD.x = EP_RU.x = arg_x;
        EP_LD.y = EP_LU.y = EP_RD.y = EP_RU.y = arg_y;
        EP_LD.visible = EP_LU.visible = EP_RD.visible = EP_RU.visible = true;
    }
};