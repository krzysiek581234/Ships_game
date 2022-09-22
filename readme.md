Ps.  That was my first 'big project in c' so it was not written well.
# Game of Ships
The aim of the project is to write a simple game Battleship. Then extend it, so it will become
interesting. Consult https://en.wikipedia.org/wiki/Battleship_(game) for an overview. The rules are
modified a bit so the progress made on basic version is not lost when the game is extended.

### Score:
![Alt text](score.png?raw=true "score.png")

## 1. Implementing the logic of base game:
<br>
All of the following functionalities should be implemented to obtain any points for the project. <br><br>
<ol>
<li>[1pt] The board is of fixed size 21 x 10  numbered from 0 to 20 in Y dimension and from 0 to 9
in X dimension. There are 2 players: A and B. First starts player A by default.
By default each of the player has exactly 1 ship of size 5, 2 ships of size 4, 3 ships of size 3, 4
ships of size 2; unless otherwise specified. The classes of ships are denoted by [CAR]RIER,
[BAT]TLESHIP, [CRU]ISER, [DES]TROYER. The numbers of ships can be either default or
specified. For simplicity assume that the number of ships of given type is bounded by 10.</li>

<li>[1pt] There is a possibility of placing the ships by the players. A can put ships in rows 0...9; B
can put ships in rows 11...20. More complicated rules (i.e. about siding ships) are not required
yet.</li>
<li>[1pt] The players can make moves, one after another, the only possible move is to either place
the ships or shot one shot at a given position. The number of shoots does not depend on the
number of ships present, each player can shoot once, wherever she want. The moves of the
players in the consecutive turns are given by [playerX] command.</li>
<li>[1pt] The game can detect which players wins. The checking of victory condition (the opposing
player has no healthy ship fragments left) should be done at the end of the turn. </li>
<li>[1pt] The game can print the state of the game. This is done by PRINT x command with x = 0.
The printing should simply print a rectangle 21 x 10 with + marking a present ship and x
marking a destroyed part of a ship. Assume that Y axis "grows down" and X axis "grows right".
The required validation is given in the last chapter. </li>
</ol>

## 2. Implementing the extended logic.
<p>To implement the extended logic all the basic functionalities should be present. Observe, some of the
functionalities are dependent on others. E.g. it will be hard to execute and verify the commands
produced by A.I if printing the state of the game does not work. Or it will be hard to save the state of
the game if SHIP is not implemented. Some, can be implemented only partially if others are not present
e.g. extended printing.</p>
<ol>
<li>[1.5pt] The board can be of arbitrary size. The player starting positions can be bounded into
arbitrary rectangle. There can be specified reefs, i.e. forbidden fields on the map and no ship
under any circumstances can be present on such a field. The number of reefs specified can be
quite large, but bounded by the size of board. Also, some of the ships of the players can be
preallocated. Please make the size and allocation of the board truly dynamic.</li>
<li>[1.5pt] A player can make an order to move a ship. In each round each of the players can make
any ship rotate, or move one field in its direction. Each ship except the carrier can move up to 3
times in a round. Carrier can move only up to 2 times. However, due to the fact that the ships
are ships, they cannot rotate in place, and, for simplicity, are always going forward 1 field.
Hence, in essence, after rotation the back of the ship is exactly one field further than the head
was in the direction pointed before the rotation. Assume that the ships during a move cannot
collide with reefs etc, despite the fact that the move is physically unfeasible.
E.g.  for a destroyer changing course to the right twice. <br>

![Alt text](ships1.png?raw=true "ships1.png")
</li>
<li>[1.5pt] Each of the ships contain 2 sections: cannon as a field after head, and engine at it's back.
The size of ship determines the range of the cannon. For carrier it is infinite, for others it is the
size of the ship calculated for the field containing the cannon, the field is not included.
The formula to calculate distance between points p1 = (y,x) and p2 = (y’,x’) is as follows:
dist(p1, p2) = sqrt(|y-y’|2 + |x -x’|2)
In essence, if there is a ship, where cannon is at position p1 then a field p2 is in the range if dist
(p1, p2) is less than or equal to length of the ship.
If the engine is damaged the ship cannot move. If the cannon is damaged the ship cannot shoot.
For each of the ships the player can specify in each round the number of shoots equal to the size
of the ship.</li>
<li>[1.5pt] Each of the ships contains additional section, radar at it's head. The radar provides an
information about positions of other ships, i.e. if there is a opposing ship in the range of the
radar. If the radar is damaged, then the range of the radar is 1; otherwise it is the length of the
ship. A field is in range if the distance (the same formula as for the cannon) between the field
and the radar is at most the range of the radar. Of course, a ship knows it's position always.
Also, carriers can send spy planes (in number up to equal to the number of shoots - 5), the
planes revel presence of enemy ships in 3x3 square each. Sending a planes count as shooting.
Printing from player's perspective should be implemented.</li>
<li>[1pt] Extended printing, in addition to the previous point the following things should be printed:
<ol>
    <li>engine as %</li>
    <li>cannon as !</li>
    <li>radar as @</li>
    </ol>
If the cannon and engine are taking the same field, then engine should be printed. Also, the
numbers of rows and columns should be printed, e.g. as:

    
    00000000001 
    01234567890 
    00           
    01           
    02 # @!++%   
    03           
    04 #% 
    05 @ 
    Extended printing is also called by PRINT x command, with x = 1.
</li>
<li>
[1pt] Printing (Saving) the state of the game. The program should be able to print commands
which allows to reconstruct the state of the game. Hence, there should be an option that
program prints commands that fully specify the state of the game after the execution. The
program should use no players command - it can only use [state] commands.
To give an example of desire output:
[state] <br>
BOARD_SIZE 21 10 <br>
NEXT_PLAYER A <br>
INIT_POSITION A 0 0 9 9 <br>
SET_FLEET A 1 1 0 1 <br>
SHIP A 5 9 N 0 CAR 11111 <br>
SHIP A 9 4 W 0 BAT 1011 <br>
SHIP A 9 0 W 0 DES 11 <br>
INIT_POSITION B 11 0 20 9 <br>
SET_FLEET B 0 1 1 0 <br>
SHIP B 11 4 E 0 BAT 1111 <br>
SHIP B 11 7 W 0 CRU 011 <br>
[state] <br>
Observe: no [player] command is used. The state of the game – what is the size of the board,
where the players can place their ships, which player should go next, what are the positions and
states of player’s ships, what fleets they have – is preserved.</li>
</ol>

## 3. Comands to set up the game.

### Basic:
1. PRINT x
Print the state of the game. For x = 0 it is the basic version. The known fields should be
printed as either: ‘+’, ‘x’, ‘ ’. ‘+’ means that the field is occupied by a ship. 'x' means that
there is a destroyed fragment at the position. ' ' means that the position is empty. Also, print
a line containing an information how many undestroyed parts of ships remain.
2. SET_FLEET P a1 a2 a3 a4
The number of ships of consecutive classes (CARRIER ... DESTROYER) for the player P
are a1 ... a4. If the command is not specified the default numbers 1, 2, 3, 4 apply.
3. NEXT_PLAYER P
Sets an information that P should make move next

### Extended:
1. BOARD_SIZE y x
Set the board size to y times x
2. INIT_POSITION P y1 x1 y2 x2
The player P can place ships in rectangle given by (y1, x1) (left upper) and (y2, x2) (right
lower). It means that the player can place ships in [y1, y2] in Y dimension and in [x1, x2] in
X dimension - the edges are included.
3. REEF y x
There is a reef at position (y,x). It should be printed as #.
4. SHIP P y x D i C a1...al
There is a ship of player P at position (y,x) in the direction D(N/W/S/E) i-th of class C
(CAR/BAT/CRU/DES). The state of the segments are a1 … al, where l is the length of the
ship and 1 means that the part is not destroyed, 0 that it was destroyed. This command is
unaffected by starting positions requirements. Keep in mind that there are no spaces
between a1 and a2, …, al-1 and al.
5. EXTENDED_SHIPS
Turn the extended logic of ships. I.e. the ships are composed of the 2 sections, etc. Turning
this on disables the default SHOOT operations by players.
6. SET_AI_PLAYER P
The player P becomes an A.I.
7. PRINT x
The printing with x = 1 should follows the advanced rules.
8. SRAND x
Set the seed of chosen random number generator to x. Please use this seed in your program
to make the executions consistent.
9. SAVE
Save the commands that allows to recreate the state of the game. Please, save even the 

## 4. Comands to play the game.

### Basic Logic:
1. PLACE_SHIP y x D i C
Place the i-th ship of class C at (y,x) with direction equal to D.
2. SHOOT y x
Shoot at the field (y,x). Shooting can only start if all the ships were placed.
### Extended Logic:
1. MOVE i C x
Move the i-th ship of class C x ([F]orward, [L]eft, [R]ight)
2. SHOOT i C y x
Let one of the shoots of i-th class ship be at (y,x).
3. SPY i y x
Send a spy plane from i-th carrier at the position (y,x).
4. PRINT x
Print the state of the game as seen by the player. That is, print the state as seen using radars
and spy planes. The reefs are known every time. If a field is empty print ' '. If a field
contains a visible part of ship print it. If a field is unknown print '?'. For x = 0 use basic
printing. For x = 1 advanced one.
### Advanced Logic:
1. SHOOT y x
Shoot at the field (y,x).
2. SPY y x
Send a spy plane at (y,x).
