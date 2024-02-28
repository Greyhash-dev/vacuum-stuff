int lamps[10] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40};
int buttons[10] = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41};
int relay_board[6] = {43, 45, 47, 49, 51, 53};
int relay_board_bindings[6] = {4, 4, 1, 5, 3, 2};
bool relay_board_active_high[6] = {1, 1, 1, 1, 1, 0};
bool state[5] = {0, 0, 0, 0, 0};

void setup() {
  Serial.begin(9600);
  for(int i : relay_board)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i,1);
  }
  for(int i : lamps)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i,1);
    delay(50);
  }
  for(int i : buttons)
  {
    pinMode(i, INPUT_PULLUP);
  }
  delay(200);
  int cnt = 0;
  for(int i : lamps)
  {
    digitalWrite(i,0);
    if((cnt & 0x01) != 0)
      digitalWrite(i,1);
    cnt ++;
  }
}

void loop() {
  int cnt = 0;
  for(int i : buttons)
  {
    if(digitalRead(i) == 0)
    {
      int e = cnt / 2;
      if((cnt & 0x01) == 0)
      {
        digitalWrite(lamps[cnt],1);
        digitalWrite(lamps[cnt+1],0);
        state[e] = 1;
        
      }
      else
      {
        digitalWrite(lamps[cnt-1],0);
        digitalWrite(lamps[cnt],1);
        //digitalWrite(relay_board[e],1);
        state[e] = 0;
      }
      for(bool s : state)
        Serial.print(s);
      Serial.println();
    }
    cnt++;
  }
  for(int e = 0; e < sizeof(state) / sizeof(state[0]); e++)
    for(int i = 0; i < sizeof(relay_board) / sizeof(relay_board[0]); i++)
      if(relay_board_bindings[i] == e+1)
        digitalWrite(relay_board[i], !(relay_board_active_high[i] xor state[e]));
}
