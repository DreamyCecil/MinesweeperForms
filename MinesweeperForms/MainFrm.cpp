#include "MainFrm.h"

using namespace System;
using namespace System::Windows::Forms;

// Get tile index based on a position
int TileIndex(const int &iX, const int &iY) {
  return iY * FIELD_SIZE + iX;
};

// Get tile position based on an index
void TilePos(const int &iTile, int &iX, int &iY) {
  iX = (iTile % FIELD_SIZE);
  iY = floor(iTile / FIELD_SIZE);
};

// Disable a field tile
Void WinFormsCPPTemplate::MainFrm::DisableTile(Button ^butTile) {
  butTile->BackColor = Drawing::Color::LightGray;
  butTile->ForeColor = Drawing::Color::LightSlateGray;
  butTile->Enabled = false;
};

// Generate the field
void GenerateMineField(const int &iStartTile) {
  int ctMines = CT_MINES;
  int iTile = 0;
  bool bSecond = false;
  
  // Starting tiles
  int aiStartTiles[9] = {-1, -1, -1, -1, iStartTile, -1, -1, -1, -1};
  int iStartX = -1;
  int iStartY = -1;
  TilePos(iStartTile, iStartX, iStartY);

  // Fill neighboring starting tiles
  for (int iFillStartY = -1; iFillStartY <= 1; iFillStartY++) {
    for (int iFillStartX = -1; iFillStartX <= 1; iFillStartX++) {
      // Correct tile
      int iTileIndex = -1;

      switch (iFillStartY) {
        case -1: iTileIndex = 1 + iFillStartX; break; // Top
        case 0: iTileIndex = 4 + iFillStartX; break; // Center
        case 1: iTileIndex = 7 + iFillStartX; break; // Bottom
      }

      // If it's a valid tile
      if (ValidTile(iStartX+iFillStartX, iStartY+iFillStartY)) {
        int iCheckTile = TileIndex(iStartX+iFillStartX, iStartY+iFillStartY);
        aiStartTiles[iTileIndex] = iCheckTile;
      }
    }
  }

  int iChance = ceil(6.0f / float(CT_MINES) * 40.0f);

  while (ctMines > 0) {
    // Skip the starting tiles
    if (aiStartTiles[0] != iTile && aiStartTiles[1] != iTile && aiStartTiles[2] != iTile
     && aiStartTiles[3] != iTile && aiStartTiles[4] != iTile && aiStartTiles[5] != iTile
     && aiStartTiles[6] != iTile && aiStartTiles[7] != iTile && aiStartTiles[8] != iTile && !_abMines[iTile]) {
      // Force the rest of the mines on the second loop
      _abMines[iTile] = (bSecond ? true : (rand() % iChance == 0));

      if (_abMines[iTile]) {
        ctMines--;
      }
    }

    iTile += (bSecond ? 7 : 1);

    // Second loop
    if (iTile >= FIELD_SIZE*FIELD_SIZE) {
      bSecond = true;
    }

    iTile = iTile % (FIELD_SIZE*FIELD_SIZE);
  }
};

// Check for mines around the tile
int CheckMines(const int &iTile, int *piMine) {
  int iX = -1;
  int iY = -1;
  TilePos(iTile, iX, iY);

  int ctMines = 0;

  // Cycle through tiles around this tile
  for (int iCheckY = -1; iCheckY <= 1; iCheckY++) {
    for (int iCheckX = -1; iCheckX <= 1; iCheckX++) {
      // Center one is unimportant
      if (iCheckY == 0 && iCheckX == 0) {
        continue;
      }

      // Correct tile
      int iTileIndex = -1;

      switch (iCheckY) {
        case -1: iTileIndex = 1 + iCheckX; break; // Top
        case 0: iTileIndex = (iCheckX == -1) ? 3 : 4; break; // Center
        case 1: iTileIndex = 6 + iCheckX; break; // Bottom
      }

      // If it's a valid tile
      if (ValidTile(iX+iCheckX, iY+iCheckY)) {
        int iCheckTile = TileIndex(iX+iCheckX, iY+iCheckY);
        piMine[iTileIndex] = (!_abMines[iCheckTile] ? iCheckTile : -1);
        ctMines += _abMines[iCheckTile];
      }
    }
  }

  return ctMines;
};

// Click on the tile
Void WinFormsCPPTemplate::MainFrm::FieldTileClick(Object ^sender, MouseEventArgs ^e) {
  Button ^butTile = (Button ^)sender;
  int iTile = Convert::ToInt32(butTile->Name);

  if (!butTile->Enabled || (_bGameOver && !_abMines[iTile])) {
    return;
  }

  switch (e->Button) {
    // Check the tile
    case Windows::Forms::MouseButtons::Right:
      if (!_bGameOver) {
        _abChecked[iTile] = !_abChecked[iTile];
        butTile->Text = _abChecked[iTile] ? "!" : "";
        butTile->BackColor = _abChecked[iTile] ? Drawing::Color::LightYellow : Drawing::Color::RoyalBlue;
      }
      return;

    // Open the tile
    case Windows::Forms::MouseButtons::Left:
      if (_abChecked[iTile]) {
        return;
      }
      break;

    // No action
    default: return;
  }

  DisableTile(butTile);

  // Generate the mine field
  if (!_bGenerated) {
    GenerateMineField(iTile);
    _bGenerated = true;
  }

  // Mine tile
  if (_abMines[iTile]) {
    butTile->Text = "X";
    butTile->BackColor = Drawing::Color::Red;

    bool bOver = _bGameOver;
    _bGameOver = true;

    // Open other mine tiles
    for (int iMineTile = 0; iMineTile < FIELD_SIZE*FIELD_SIZE; iMineTile++) {
      Button ^butMine = m_apTiles[iMineTile];

      if (butMine->Enabled && _abMines[iMineTile]) {
        FieldTileClick(butMine, e);
      }
    }

    if (!bOver) {
      MessageBoxA(0, "Better luck next time.", "You lost!", MB_ICONINFORMATION);
    }

    return;
  }

  int iMine[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
  int ctMines = CheckMines(iTile, iMine);

  if (ctMines > 0) {
    butTile->Text = Convert::ToString(ctMines);

    switch (ctMines) {
      case 1: butTile->BackColor = MINE_1; break;
      case 2: butTile->BackColor = MINE_2; break;
      case 3: butTile->BackColor = MINE_3; break;
      case 4: butTile->BackColor = MINE_4; break;
      case 5: butTile->BackColor = MINE_5; break;
      case 6: butTile->BackColor = MINE_6; break;
      case 7: butTile->BackColor = MINE_7; break;
      case 8: butTile->BackColor = MINE_8; break;
    }

  // Open neighboring tiles
  } else {
    for (int iTileClick = 0; iTileClick < 8; iTileClick++) {
      int iNextTile = iMine[iTileClick];

      if (iNextTile == -1) {
        continue;
      }

      Button ^butClick = m_apTiles[iNextTile];

      if (!_abChecked[iNextTile] && butClick->Enabled) {
        FieldTileClick(butClick, e);
      }
    }
  }
};

// Reset the mine field
Void WinFormsCPPTemplate::MainFrm::ResetMineField(Object ^sender, EventArgs ^e) {
  _bGenerated = false;
  _bGameOver = false;

  for (int i = 0; i < FIELD_SIZE*FIELD_SIZE; i++) {
    Button ^butTile = m_apTiles[i];

    butTile->BackColor = Drawing::Color::RoyalBlue;
    butTile->ForeColor = Drawing::Color::Black;
    butTile->Text = "";
    butTile->Enabled = true;

    _abMines[i] = false;
    _abChecked[i] = false;
  }
};

Void WinFormsCPPTemplate::MainFrm::TimerTick(Object ^sender, EventArgs ^e) {
  // Count checked mines
  int ctChecked = 0;
  for (int iChecked = 0; iChecked < FIELD_SIZE*FIELD_SIZE; iChecked++) {
    if (_abChecked[iChecked]) {
      ctChecked++;
    }
  }

  txtMines->Text = Convert::ToString(CT_MINES-ctChecked);
};

// Initialize minesweeper
Void WinFormsCPPTemplate::MainFrm::MainFrm_Load(Object ^sender, EventArgs ^e) {
  // Template tile
  butField->Name = Convert::ToString(0);
  m_apTiles[0] = butField;

  for (int i = 1; i < FIELD_SIZE*FIELD_SIZE; i++) {
    Button ^butTile = gcnew Button();

    // Tile style
    butTile->FlatStyle = FlatStyle::Flat;
    butTile->BackColor = Drawing::Color::RoyalBlue;
    butTile->Font = butField->Font;
    butTile->Size = Drawing::Size(24, 24);
    butTile->Text = "";
    butTile->TextAlign = butField->TextAlign;

    // Tile index
    butTile->Name = Convert::ToString(i);

    // Tile location
    int iX = 0;
    int iY = 0;
    TilePos(i, iX, iY);
    butTile->Location = Drawing::Point(24*iX + 24, 24*iY + 48);

    butTile->MouseUp += gcnew MouseEventHandler(this, &MainFrm::FieldTileClick);
    m_apTiles[i] = butTile;

    Controls->Add(butTile);
  }
};

[STAThreadAttribute]
void Main(array<String ^> ^args)
{
  Application::EnableVisualStyles();
  Application::SetCompatibleTextRenderingDefault(false);

  WinFormsCPPTemplate::MainFrm frm;
  Application::Run(%frm);
};