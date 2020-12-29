/* Copyright (c) 2020 Dreamy Cecil 
This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as published by
the Free Software Foundation


This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

#pragma once

#include <cstdlib>
#include <string>
#include <math.h>
#include <windows.h>

// Field size in both directions
#define FIELD_SIZE 16
// Amount of mines
#define CT_MINES 40

#define MINE_1 Drawing::Color::LightBlue;
#define MINE_2 Drawing::Color::LimeGreen;
#define MINE_3 Drawing::Color::Coral;
#define MINE_4 Drawing::Color::LightCyan;
#define MINE_5 Drawing::Color::PaleVioletRed;
#define MINE_6 Drawing::Color::SandyBrown;
#define MINE_7 Drawing::Color::Coral;
#define MINE_8 Drawing::Color::Coral;

extern bool _abMines[FIELD_SIZE*FIELD_SIZE] = {};
extern bool _abChecked[FIELD_SIZE*FIELD_SIZE] = {};
extern bool _bGenerated = false;
extern bool _bGameOver = false;

inline int TileIndex(const int &iX, const int &iY);
void TilePos(const int &iTile, int &iX, int &iY);

void GenerateMineField(const int &iStartTile);
int CheckMines(const int &iTile, int *piMine);

inline bool ValidTile(const int &iX, const int &iY) {
  return (iX >= 0 && iX < FIELD_SIZE && iY >= 0 && iY < FIELD_SIZE);
};

// Windows Forms
namespace WinFormsCPPTemplate {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MainFrm : public System::Windows::Forms::Form {
	public:
    array<Button ^> ^m_apTiles;

		MainFrm(void) {
			InitializeComponent();

      for (int iMine = 0; iMine < FIELD_SIZE*FIELD_SIZE; iMine++) {
        _abMines[iMine] = false;
        _abChecked[iMine] = false;
      }

      m_apTiles = gcnew array<Button ^>(FIELD_SIZE*FIELD_SIZE);
		};

	protected:
		~MainFrm() {
			if (components) {
				delete components;
			}
		};

  private: System::Windows::Forms::Button^  butField;
  private: System::Windows::Forms::Button^  butReset;
  private: System::Windows::Forms::Label^  txtMines;
  private: System::Windows::Forms::Timer^  tmrLoop;
  private: System::ComponentModel::IContainer^  components;

	private:

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
      this->components = (gcnew System::ComponentModel::Container());
      this->butField = (gcnew System::Windows::Forms::Button());
      this->butReset = (gcnew System::Windows::Forms::Button());
      this->txtMines = (gcnew System::Windows::Forms::Label());
      this->tmrLoop = (gcnew System::Windows::Forms::Timer(this->components));
      this->SuspendLayout();
      // 
      // butField
      // 
      this->butField->BackColor = System::Drawing::Color::RoyalBlue;
      this->butField->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
      this->butField->Font = (gcnew System::Drawing::Font(L"Consolas", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
      this->butField->Location = System::Drawing::Point(24, 48);
      this->butField->Name = L"butField";
      this->butField->Size = System::Drawing::Size(24, 24);
      this->butField->TabIndex = 0;
      this->butField->TextAlign = System::Drawing::ContentAlignment::TopCenter;
      this->butField->UseVisualStyleBackColor = false;
      this->butField->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainFrm::FieldTileClick);
      // 
      // butReset
      // 
      this->butReset->Location = System::Drawing::Point(345, 12);
      this->butReset->Name = L"butReset";
      this->butReset->Size = System::Drawing::Size(75, 23);
      this->butReset->TabIndex = 1;
      this->butReset->Text = L"Reset";
      this->butReset->UseVisualStyleBackColor = true;
      this->butReset->Click += gcnew System::EventHandler(this, &MainFrm::ResetMineField);
      // 
      // txtMines
      // 
      this->txtMines->AutoSize = true;
      this->txtMines->Font = (gcnew System::Drawing::Font(L"Impact", 20.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
      this->txtMines->ForeColor = System::Drawing::Color::Black;
      this->txtMines->Location = System::Drawing::Point(18, 9);
      this->txtMines->Name = L"txtMines";
      this->txtMines->Size = System::Drawing::Size(29, 34);
      this->txtMines->TabIndex = 2;
      this->txtMines->Text = L"0";
      // 
      // tmrLoop
      // 
      this->tmrLoop->Enabled = true;
      this->tmrLoop->Interval = 10;
      this->tmrLoop->Tick += gcnew System::EventHandler(this, &MainFrm::TimerTick);
      // 
      // MainFrm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->BackColor = System::Drawing::SystemColors::AppWorkspace;
      this->ClientSize = System::Drawing::Size(432, 458);
      this->Controls->Add(this->txtMines);
      this->Controls->Add(this->butReset);
      this->Controls->Add(this->butField);
      this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
      this->MaximizeBox = false;
      this->Name = L"MainFrm";
      this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
      this->Text = L"Minesweeper";
      this->Load += gcnew System::EventHandler(this, &MainFrm::MainFrm_Load);
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  
  Void MainFrm_Load(Object ^sender, EventArgs ^e);
  Void FieldTileClick(Object ^sender, MouseEventArgs ^e);
  Void DisableTile(Button ^butTile);
  Void ResetMineField(Object ^sender, EventArgs ^e);
  Void TimerTick(Object ^sender, EventArgs ^e);
};
}
