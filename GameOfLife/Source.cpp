#include <iostream>
using namespace std;

#include "ConsoleEngine.h"

class GameOfLife : public olcConsoleGameEngine
{
public:
	GameOfLife()
	{
		m_sAppName = L"Game Of Life";
	}

private:
	int* m_output;
	int* m_state;



protected:
	// Called by olcConsoleGameEngine
	virtual bool OnUserCreate()
	{
		m_output = new int[ScreenWidth() * ScreenHeight()];
		m_state = new int[ScreenWidth() * ScreenHeight()];

		memset(m_output, 0, ScreenWidth() * ScreenHeight() * sizeof(int));
		memset(m_state, 0, ScreenWidth() * ScreenHeight() * sizeof(int));
		

		for (int i = 0; i < ScreenWidth()*ScreenHeight(); i++)
			m_state[i] = rand() % 2;

		auto set = [&](int x, int y, wstring s)
		{
			int p = 0;
			for (auto c : s)
			{
				m_state[y*ScreenWidth() + x + p] = c == L'#' ? 1 : 0;
				p++;
			}
		};

		/*
		// R-Pentomino
		set(80, 50, L"  ## ");
		set(80, 51, L" ##  ");
		set(80, 52, L"  #  ");

		// Gosper Glider Gun
		set(60, 45, L"........................#............");
		set(60, 46, L"......................#.#............");
		set(60, 47, L"............##......##............##.");
		set(60, 48, L"...........#...#....##............##.");
		set(60, 49, L"##........#.....#...##...............");
		set(60, 50, L"##........#...#.##....#.#............");
		set(60, 51, L"..........#.....#.......#............");
		set(60, 52, L"...........#...#.....................");
		set(60, 53, L"............##.......................");*/
		

		// Infinite Growth
		//set(65, 50, L"########.#####...###......#######.#####");

		/*
		set(80, 33, L"....#.....#....");
		set(80, 34, L"....#.....#....");
		set(80, 35, L"....##...##....");
		set(80, 36, L"...............");
		set(80, 37, L"###..##.##..###");
		set(80, 38, L"..#.#.#.#.#.#..");
		set(80, 39, L"....##...##....");
		set(80, 50, L"...............");
		set(80, 61, L"....##...##....");
		set(80, 62, L"..#.#.#.#.#.#..");
		set(80, 63, L"###..##.##..###");
		set(80, 64, L"...............");
		set(80, 65, L"....##...##....");
		set(80, 66, L"....#.....#....");
		set(80, 67, L"....#.....#....");

		set(12, 33, L"....#.....#....");
		set(12, 34, L"....#.....#....");
		set(12, 35, L"....##...##....");
		set(12, 36, L"...............");
		set(12, 37, L"###..##.##..###");
		set(12, 38, L"..#.#.#.#.#.#..");
		set(12, 39, L"....##...##....");
		set(12, 50, L"...............");
		set(12, 61, L"....##...##....");
		set(12, 62, L"..#.#.#.#.#.#..");
		set(12, 63, L"###..##.##..###");
		set(12, 64, L"...............");
		set(12, 65, L"....##...##....");
		set(12, 66, L"....#.....#....");
		set(12, 67, L"....#.....#....");
			 
		set(120, 3, L"....#.....#....");
		set(120, 4, L"....#.....#....");
		set(120, 5, L"....##...##....");
		set(120, 6, L"...............");
		set(120, 7, L"###..##.##..###");
		set(120, 8, L"..#.#.#.#.#.#..");
		set(120, 9, L"....##...##....");
		set(120, 20, L"...............");
		set(120, 31, L"....##...##....");
		set(120, 32, L"..#.#.#.#.#.#..");
		set(120, 33, L"###..##.##..###");
		set(120, 34, L"...............");
		set(120, 35, L"....##...##....");
		set(120, 36, L"....#.....#....");
		set(120, 37, L"....#.....#....");*/

		
		

		return true;
	}



	// Called by olcConsoleGameEngine
	virtual bool OnUserUpdate(float fElapsedTime)
	{
		this_thread::sleep_for(1ms);

		//if (!m_keys[VK_SPACE].bReleased)
			//return true;

		auto cell = [&](int x, int y)
		{
			if (x >= ScreenWidth())
			x = x - ScreenWidth();
			if (x < 0)
			x = x + ScreenWidth();

			if (y >= ScreenHeight())
			y = y - ScreenHeight();
			if (y < 0)
			y = y + ScreenHeight();

			return m_output[y * ScreenWidth() + x];
		};

		// Store output state
		for (int i = 0; i < ScreenWidth()*ScreenHeight(); i++)
			m_output[i] = m_state[i];

		for (int x = 0; x < ScreenWidth() ; x++)
		{ 
			for (int y = 0; y < ScreenHeight() ; y++)
			{
				// The secret of artificial life =================================================
				int nNeighbours = cell(x - 1, y - 1) + cell(x - 0, y - 1) + cell(x + 1, y - 1) +
					cell(x - 1, y + 0) + 0 + cell(x + 1, y + 0) +
					cell(x - 1, y + 1) + cell(x + 0, y + 1) + cell(x + 1, y + 1);

				if (cell(x, y) == 1)
					m_state[y*ScreenWidth() + x] = nNeighbours == 2 || nNeighbours == 3;
				else
					m_state[y*ScreenWidth() + x] = nNeighbours == 3;
				// ===============================================================================


				if (cell(x, y) == 1)
					Draw(x, y, PIXEL_SOLID, FG_WHITE);
				else
					Draw(x, y, PIXEL_SOLID, FG_BLACK);

				
			}
		}

		return true;
	}
};


int main()
{
	// Seed random number generator
	srand(clock());

	// Use olcConsoleGameEngine derived app
	GameOfLife game;
	game.ConstructConsole(160, 100, 8, 8);
	game.Start();

	return 0;
}