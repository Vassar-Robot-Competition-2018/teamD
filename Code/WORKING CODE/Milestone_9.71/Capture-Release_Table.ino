// set capture-release variables based on Home

/*void Cap_Rel_Table() {
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
}*/

void Cap_Rel_Table() {
  Neutral_1 += Home;
  if (Neutral_1 > 4) {
    Neutral_1 -= 4;
  }
  else Enemy += Home;
  if (Enemy > 4) {
    Enemy -= - 4;
  }
  Neutral_2 += Home;
  if (Neutral_2 > 4) {
    Neutral_2 -= 4;
  }
}
