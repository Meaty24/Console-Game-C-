Hello, thank you for choosing Dungeon Explorer :)

Dungeon Explorer is a 2D game that utilizes an algorithm to 
procedurally generate randomized mazes. All the room layouts
were pre-built, but their location and quantity constantly changes.

Currently the game works under a few conditions:
1. It has to be opened and ran through an IDE that is able to run C++ v11
or higher
2. The Leaderboard.txt has to be in the same directory as the .cpp file

The controls of the game are simple - WASD to move the character (denoted as P),
Space to skip a turn (makes the enemies move without the player moving),
H to open help menu, Q to quit.

The game is a roguelike dungeon crawler. The objective is to reach the end without
losing as many maps as possible and obtaining the highest score possible. At the
end of the run, whenever the player decides to quit, the score is automatically
saved to the leaderboard and it is updated to reflect their score. Each move the
player makes lowers the final amount of points, so beating the game with the least
amount of moves and the highest amount of points results in a higher place in the
leaderboard. Points are awarded for moving through rooms, as well as reaching the
final cell.

The board is limited and has intermediary walls. The player can never go through
or into walls. Bumping into moving enemies ends the run and displays the amount
of points the player would have saved if they had survived up to that point.

The game is in early development. No notable bugs have been found by the author.
In case any of these are encountered, reach out to me using my email:
mitko356@abv.bg

If you need any help navigating, read the on-screen text or press H while in-game
to review the Help screen, which has a detailed description about every element
within the game board.

Enjoy! :)