#include <exception>

#include "Board.hpp"
#include "BoardView.hpp"

int main()
{
	try
	{
		Board board;
		BoardView board_view(board);
		board_view.run();
	}
	catch (const std::exception& ex)
	{
		// logging
	}
	catch (...)
	{
		// logging unknown exception
	}
	return 0;
}