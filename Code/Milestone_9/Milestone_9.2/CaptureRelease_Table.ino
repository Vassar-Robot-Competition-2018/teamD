// set capture-release variables based on Home

void Cap_Rel_Table() {
  if ((Neutral_1 + Home) > 4) {
    Neutral_1 += (Home - 4);
  }
  else Neutral_1 += Home;

  if ((Enemy + Home) > 4) {
    Enemy += (Home - 4);
  }
  else Enemy += Home;

  if ((Neutral_2 + Home) > 4) {
    Neutral_2 += (Home - 4);
  }
  else Neutral_2 += Home;
}

/*void Cap_Rel_Table (){
  if(Home == 4){ //Blue
    Neutral_1 = 1; //Red
    Enemy = 3; //Green
    Neutral_2 = 2; //Yellow
  }
  if(Home == 1){ //Red
    Neutral_1 = 3; //Green
    Enemy = 2; //Yellow
    Neutral_2 = 4; //Blue
  }
  if(Home == 2){ //Yellow
    Neutral_1 = 4; //Blue
    Enemy = 1; //Red
    Neutral_2 = 3; //Green
  }
  if(Home == 3){ //Green
    Neutral_1 = 2; //yellow
    Enemy = 4; //Blue
    Neutral_2 = 1; //Red
  }
} */

