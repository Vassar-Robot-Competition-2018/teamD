// set capture-release variables based on Home
void Cap_Table() {
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

void Rel_Table() {
  if ((CW_relQ + releaseQ) > 4) {
    CW_relQ += (releaseQ - 4);
  }
  else CW_relQ += releaseQ;

  if ((Diag_relQ + releaseQ) > 4) {
    Diag_relQ += (releaseQ - 4);
  }
  else Diag_relQ += releaseQ;

  if ((CCW_relQ + releaseQ) > 4) {
    CCW_relQ += (releaseQ - 4);
  }
  else CCW_relQ += releaseQ;
}

