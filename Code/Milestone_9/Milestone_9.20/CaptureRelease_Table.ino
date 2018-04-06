// set capture-release variables based on Home

void Cap_Rel_Table() {
  if ((Neutral_1 + Home) > 5) {
    Neutral_1 += (Home - 4);
  }
  else Neutral_1 += Home;

  if ((Enemy + Home) > 5) {
    Enemy += (Home - 4);
  }
  else Enemy += Home;

  if ((Neutral_2 + Home) > 5) {
    Neutral_2 += (Home - 4);
  }
  else Neutral_2 += Home;
}
