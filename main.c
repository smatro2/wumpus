/*
	Program 2: Wumpus, version 2 (dynamically allocated array)
	CS 211, UIC, Fall 2022
	System: Replit and CLion
	Author: Sivani Matroja

	Hunt the Wumpus is a classic text-based adventure game by Gregory Yob from 1972.
	The Wumpus lives in a completely dark cave of 20 rooms.  Each room has 3 tunnels leading to other rooms.
	This program implements this game, with super bats and arrows, using a dynamically allocated array.
*/

#include <stdio.h>
#include <stdbool.h>	// for the bool type in C
#include <stdlib.h>		// for srand
#include <ctype.h>		// for toupper()

// global constant
#define NUMBER_OF_ROOMS 20

// Used to more conveniently pass all game information between functions.
struct GameInfo {
    int moveNumber;  // Counts up from 1, incrementing for each move
    int personRoom;  // Room 1..20 the person currently is in
    int wumpusRoom;  // Room 1..20 the Wumpus is in
    int pitRoom1;    // Room 1..20 the first pit is in
    int pitRoom2;    // Room 1..20 the second pit is in
    int batRoom1;    // Room 1..20 the first super bats are in
    int batRoom2;    // Room 1..20 the second super bats are in
    int arrowRoom;   // Room 1..20 the arrow currently is in, -1 if arrow is with player
    int arrowRooms;  // 1-3 rooms entered by user
};



// Display cave function can display the cave for the user when prompted.
//--------------------------------------------------------------------------------
void displayCave()
{
    printf( "\n"
            "       ______18______             \n"
            "      /      |       \\           \n"
            "     /      _9__      \\          \n"
            "    /      /    \\      \\        \n"
            "   /      /      \\      \\       \n"
            "  17     8        10     19       \n"
            "  | \\   / \\      /  \\   / |    \n"
            "  |  \\ /   \\    /    \\ /  |    \n"
            "  |   7     1---2     11  |       \n"
            "  |   |    /     \\    |   |      \n"
            "  |   6----5     3---12   |       \n"
            "  |   |     \\   /     |   |      \n"
            "  |   \\       4      /    |      \n"
            "  |    \\      |     /     |      \n"
            "  \\     15---14---13     /       \n"
            "   \\   /            \\   /       \n"
            "    \\ /              \\ /        \n"
            "    16---------------20           \n"
            "\n");
}
// Display instructions function displays instructions for the user with prompted.
//--------------------------------------------------------------------------------
void displayInstructions()
{
    printf( "Hunt the Wumpus:                                             \n"
            "The Wumpus lives in a completely dark cave of 20 rooms. Each \n"
            "room has 3 tunnels leading to other rooms.                   \n"
            "                                                             \n"
            "Hazards:                                                     \n"
            "1. Two rooms have bottomless pits in them.  If you go there you fall and die.   \n"
            "2. Two other rooms have super-bats.  If you go there, the bats grab you and     \n"
            "   fly you to some random room, which could be troublesome.  Then those bats go \n"
            "   to a new room different from where they came from and from the other bats.   \n"
            "3. The Wumpus is not bothered by the pits or bats, as he has sucker feet and    \n"
            "   is too heavy for bats to lift.  Usually he is asleep.  Two things wake       \n"
            "    him up: Anytime you shoot an arrow, or you entering his room.  The Wumpus   \n"
            "    will move into the lowest-numbered adjacent room anytime you shoot an arrow.\n"
            "    When you move into the Wumpus' room, then he wakes and moves if he is in an \n"
            "    odd-numbered room, but stays still otherwise.  After that, if he is in your \n"
            "    room, he snaps your neck and you die!                                       \n"
            "                                                                                \n"
            "Moves:                                                                          \n"
            "On each move you can do the following, where input can be upper or lower-case:  \n"
            "1. Move into an adjacent room.  To move enter 'M' followed by a space and       \n"
            "   then a room number.                                                          \n"
            "2. Shoot your guided arrow through a list of up to three adjacent rooms, which  \n"
            "   you specify.  Your arrow ends up in the final room.                          \n"
            "   To shoot enter 'S' followed by the number of rooms (1..3), and then the      \n"
            "   list of the desired number (up to 3) of adjacent room numbers, separated     \n"
            "   by spaces. If an arrow can't go a direction because there is no connecting   \n"
            "   tunnel, it ricochets and moves to the lowest-numbered adjacent room and      \n"
            "   continues doing this until it has traveled the designated number of rooms.   \n"
            "   If the arrow hits the Wumpus, you win! If the arrow hits you, you lose. You  \n"
            "   automatically pick up the arrow if you go through a room with the arrow in   \n"
            "   it.                                                                          \n"
            "3. Enter 'R' to reset the person and hazard locations, useful for testing.      \n"
            "4. Enter 'C' to cheat and display current board positions.                      \n"
            "5. Enter 'D' to display this set of instructions.                               \n"
            "6. Enter 'P' to print the maze room layout.                                     \n"
            "7. Enter 'X' to exit the game.                                                  \n"
            "                                                                                \n"
            "Good luck!                                                                      \n"
            " \n\n");
}//end displayInstructions()


//--------------------------------------------------------------------------------
// Returns true if randomValue is already in array
int alreadyFound(int randomValue,      // New number we're checking
                 int randomNumbers[],  // Set of numbers previously found
                 int limit)            // How many numbers previously found
{
    int returnValue = false;

    // compare random value against all previously found values
    for( int i = 0; i<limit; i++) {
        if( randomValue == randomNumbers[i]) {
            returnValue = true;   // It is already there
            break;
        }
    }

    return returnValue;
}


//--------------------------------------------------------------------------------
// Fill array with unique random integers 1..20
void setUniqueValues(int randomNumbers[],   // Array of random numbers
                     int size)              // Size of random numbers array
{
    int randomValue;

    for( int i = 0; i<size; i++) {
        do randomValue = rand() % NUMBER_OF_ROOMS + 1;
        while (alreadyFound(randomValue, randomNumbers, i));
        randomNumbers[i] = randomValue;
    }
}


//--------------------------------------------------------------------------------
// Set the Wumpus, player, bats, pits and arrows in distinct random rooms
void initializeGame(struct GameInfo *gameInfo)   // All game settings variables
{
    // Array of 8 unique values 1..20, to be used in initializing cave hazards locations
    int randomNumbers[7];

    // Select some unique random values 1..20 to be used for 2 bats rooms, 2
    // pits rooms, Wumpus room, and initial player room
    setUniqueValues(randomNumbers, 7);
    // Use the unique random numbers to set initial locations of hazards, which
    //    should be non-overlapping.
    gameInfo->personRoom = randomNumbers[0];
    gameInfo->wumpusRoom = randomNumbers[1];
    gameInfo->pitRoom1 = randomNumbers[2];
    gameInfo->pitRoom2 = randomNumbers[3];
    gameInfo->batRoom1 = randomNumbers[4];
    gameInfo->batRoom2 = randomNumbers[5];
    gameInfo->arrowRoom = randomNumbers[6];

    gameInfo->moveNumber = 1; //allows for iteration to keep track of what move number the user is on.
}// end initializeBoard(...)


//--------------------------------------------------------------------------------
// Returns true if nextRoom is adjacent to current room, else returns false.
int roomIsAdjacent( int tunnels[3],      // Array of adjacent tunnels
                    int nextRoom)        // Desired room to move to
{
    return( tunnels[0] == nextRoom ||
            tunnels[1] == nextRoom ||
            tunnels[2] == nextRoom
    );
}


//--------------------------------------------------------------------------------
// Display where everything is on the board.
void displayCheatInfo(struct GameInfo gameInfo)        // Hazards location and game info
{
    printf( "Cheating! Game elements are in the following rooms: \n"
            "Player Wumpus Pit1 Pit2 Bats1 Bats2 Arrow  \n"
            "%4d %5d %6d %5d %5d %5d %5d \n\n",
            gameInfo.personRoom,
            gameInfo.wumpusRoom,
            gameInfo.pitRoom1,
            gameInfo.pitRoom2,
            gameInfo.batRoom1,
            gameInfo.batRoom2,
            gameInfo.arrowRoom
    );
}// end displayCheatInfo(...)


//--------------------------------------------------------------------------------
// Display room number and hazards detected
void displayRoomInfo( struct GameInfo gameInfo,       // Hazards location and game info
                        int **Rooms)                  // Rooms and associated tunnels
{
    // Retrieve player's current room number and display it
    int currentRoom = gameInfo.personRoom;
    printf("You are in room %d. ", currentRoom);

    // Retrieve index values of+ all 3 adjacent rooms

    int room1 = Rooms[currentRoom][0];
    int room2 = Rooms[currentRoom][1];
    int room3 = Rooms[currentRoom][2];

    // Display hazard detection message if Wumpus is in an adjacent room
    int wumpusRoom = gameInfo.wumpusRoom;
    if( room1 == wumpusRoom || room2 == wumpusRoom || room3 == wumpusRoom) {
        printf("You smell a stench. ");
    }

    // Display hazard detection message if a pit is in an adjacent room
    int pit1Room = gameInfo.pitRoom1;
    int pit2Room = gameInfo.pitRoom2;
    if( room1 == pit1Room || room1 == pit2Room ||
        room2 == pit1Room || room2 == pit2Room ||
        room3 == pit1Room || room3 == pit2Room
            ) {
        printf("You feel a draft. ");
    }
    // Display hazard detection message if a bat is in an adjacent room
    int bat1Room = gameInfo.batRoom1;
    int bat2Room = gameInfo.batRoom2;
    if( room1 == bat1Room || room1 == bat2Room ||
        room2 == bat1Room || room2 == bat2Room ||
        room3 == bat1Room || room3 == bat2Room
            ) {
        printf("You hear rustling of bat wings.");
    }
    printf("\n\n");
}//end displayRoomInfo(...)


//--------------------------------------------------------------------------------
// If the player just moved into a room with a pit, the person dies.
// If the person just moved into the Wumpus room, then if the room number is odd
// the Wumpus moves to a random adjacent room.
void checkForHazards(
        struct GameInfo *gameInfo, // Hazards location and game info
        bool *personIsAlive,       // Person is alive, but could die depending on the hazards
        int **Rooms)               // Rooms within the game with associated tunnels
{
    // retrieve the room the person is in
    int personRoom = gameInfo->personRoom;

    // Check for the Wumpus
    if( personRoom == gameInfo->wumpusRoom) {
        // To make it easier to test, in this version of the program the Wumpus always
        // moves if it is currently in an odd-numbered room, and it moves into the
        // lowest-numbered adjacent room.  In the version that is more fun to play
        // (but harder to test), the Wumpus has a 75% chance of moving, and a 25%
        // chance of staying and killing you.  The "more fun" code is commented out below.

        // Wumpus is there. 75% change of Wumpus moving, 25% chance of it killing you
        // Generate a random number 1..100
        // if(  (rand() % 100) < 75) {
        if( gameInfo->wumpusRoom %2 == 1) {
            // You got lucky and the Wumpus moves away
            printf( "You hear a slithering sound, as the Wumpus slips away. \n"
                    "Whew, that was close! \n");
            // Choose a random adjacent room for the Wumpus to go into
            // gameInfo.wumpusRoom = rooms[ personRoom][ rand() % 3];
            gameInfo->wumpusRoom = Rooms[personRoom][ 0];  // Choose the lowest-numbered adjacent room
        }
        else {
            // Wumpus kills you
            printf(	"You briefly feel a slimy tentacled arm as your neck is snapped. \n"
                       "It is over.\n");
            *personIsAlive = false;
            return;
        }
    }

    // Check whether there is a pit
    if( personRoom == gameInfo->pitRoom1 || personRoom == gameInfo->pitRoom2) {
        // Person falls into pit
        printf("Aaaaaaaaahhhhhh....   \n");
        printf("    You fall into a pit and die. \n");
        *personIsAlive = false;
        return;
    }

    // Check whether there are bats
    if( gameInfo->personRoom == gameInfo->batRoom1 || gameInfo->personRoom == gameInfo->batRoom2) {
        int temp = gameInfo->personRoom;
        int tempBat1 = gameInfo->batRoom1;
        int tempBat2 = gameInfo->batRoom2;
        while(temp == gameInfo->personRoom || gameInfo->personRoom == gameInfo->batRoom2 || gameInfo->personRoom == gameInfo->batRoom1){
            gameInfo->personRoom = rand() % NUMBER_OF_ROOMS + 1;
        }
        if (temp == gameInfo->batRoom1){
            while(gameInfo->batRoom1 == gameInfo->batRoom2 || tempBat1 == gameInfo->batRoom1){
                gameInfo->batRoom1 = rand() % NUMBER_OF_ROOMS + 1;
            }
        }
        if (temp == gameInfo->batRoom2){
            while(gameInfo->batRoom1 == gameInfo->batRoom2|| tempBat2 == gameInfo->batRoom2){
                gameInfo->batRoom2 = rand() % NUMBER_OF_ROOMS + 1;
            }
        }
        // Person teleported by bats
        printf("Woah... you're flying! \n");
        printf("You've just been transported by bats to room %d.\n", gameInfo->personRoom);
    }

}//end checkForHazards(...)


//--------------------------------------------------------------------------------
// Prompt for and reset the positions of the game hazards and the person's location,
// useful for testing.  No error checking is done on these values.
void resetPositions(struct GameInfo *theGameInfo)
{
    printf("Enter the room locations (1..20) for player, wumpus, pit1, pit2, bats1, bats2, and arrow: \n\n");
    // In the scanf below note that we put the space at the beginning of the scanf so
    // that any newline left over from a previous input is not read in and used as
    // the next move. Another option is having getchar() after the scanf() statement.
    scanf("%d %d %d %d %d %d %d", &theGameInfo->personRoom, &theGameInfo->wumpusRoom,
          &theGameInfo->pitRoom1, &theGameInfo->pitRoom2, &theGameInfo->batRoom1, &theGameInfo->batRoom2, &theGameInfo->arrowRoom);
}

void shootArrow1(struct GameInfo *gameInfo, // Hazards location and game info
                 bool *personIsAlive,       // Person is alive, but could die depending on the hazards
                 bool *wumpusIsAlive,       // Wumpus is alive, but could die depending on the hazards
                 int **Rooms,               // Rooms within the game with associated tunnels
                 int *room1){               // Room where user has indicated where they would like the arrow would go
    //Checking if room is adjacent
    if(roomIsAdjacent(Rooms[gameInfo->personRoom], *room1)){
        gameInfo->arrowRoom = *room1;
        // If Wumpus is in the room with the arrow, will indicate that user has won.
        if (gameInfo->wumpusRoom == gameInfo->arrowRoom){
            printf( "Wumpus has just been pierced by your deadly arrow! \n"
                    "Congratulations on your victory, you awesome hunter you.\n");
            *wumpusIsAlive = false;
            return;
        }
        // If Player is in the room with the arrow, will indicate that user has died and lost.
        if (gameInfo->personRoom == gameInfo->arrowRoom){
            printf( "You just shot yourself.  \n"
                    "Maybe Darwin was right.  You're dead.\n");
            *personIsAlive = false;
            return;
        }
    }
    // If the room is not adjacent, branch will be entered
    else{
        printf("Room %d is not adjacent.  Arrow ricochets...\n", *room1);
        gameInfo->arrowRoom = gameInfo->personRoom;
        // Arrow enters lowest number room after ricochet.
        gameInfo->arrowRoom = Rooms[gameInfo->arrowRoom][0];
        // If ricochet hits the Wumpus, the user will win accidentally.
        if (gameInfo->wumpusRoom == gameInfo->arrowRoom) {
            printf( "Your arrow ricochet killed the Wumpus, you lucky dog!\n"
                    "Accidental victory, but still you win!\n");
            *wumpusIsAlive = false;
            return;
        }
        // If ricochet hits the Player, the Player will die and the user will lose
        else if (gameInfo->personRoom == gameInfo->arrowRoom) {
            printf( "You just shot yourself, and are dying.\n"
                    "It didn't take long, you're dead.\n");
            *personIsAlive = false;
            return;
        }
    }
    // When arrow is shot, Wumpus will move to lowest numbered adjacent room.
    gameInfo->wumpusRoom = Rooms[gameInfo->wumpusRoom][0];
}
void shootArrow2(struct GameInfo *gameInfo, // Hazards location and game info
                 bool *personIsAlive,       // Person is alive, but could die depending on the hazards
                 bool *wumpusIsAlive,       // Wumpus is alive, but could die depending on the hazards
                 int **Rooms,               // Rooms within the game with associated tunnels
                 int *room1,                // Room where user has indicated where they would like the first arrow to go
                 int *room2){               // Room where user has indicated where they would like the second arrow to go

    //Checking if room is adjacent
    if(roomIsAdjacent(Rooms[gameInfo->personRoom], *room1)){
        gameInfo->arrowRoom = *room1;
        // If Wumpus is in the room with the first arrow, will indicate that user has won.
        if (gameInfo->wumpusRoom == gameInfo->arrowRoom){
            printf( "Wumpus has just been pierced by your deadly arrow! \n"
                    "Congratulations on your victory, you awesome hunter you.\n");
            *wumpusIsAlive = false;
            return;
        }
        // If Player is in the room with the first arrow, will indicate that user has died and lost.
        if (gameInfo->personRoom == gameInfo->arrowRoom){
            printf( "You just shot yourself.  \n"
                    "Maybe Darwin was right.  You're dead.\n");
            *personIsAlive = false;
            return;
        }
    }
    // If the room is not adjacent, branch will be entered
    else{
        printf("Room %d is not adjacent.  Arrow ricochets...\n", *room1);
        gameInfo->arrowRoom = gameInfo->personRoom;
        // First arrow enters lowest number room after ricochet.
        gameInfo->arrowRoom = Rooms[gameInfo->arrowRoom][0];
        // If ricochet hits the Wumpus, the user will win accidentally.
        if (gameInfo->wumpusRoom == gameInfo->arrowRoom) {
            printf( "Your arrow ricochet killed the Wumpus, you lucky dog!\n"
                    "Accidental victory, but still you win!\n");
            *wumpusIsAlive = false;
            return;
        }
        // If ricochet hits the Player, the Player will die and the user will lose
        else if (gameInfo->personRoom == gameInfo->arrowRoom) {
            printf( "You just shot yourself, and are dying.\n"
                    "It didn't take long, you're dead.\n");
            *personIsAlive = false;
            return;
        }
    }
    //Checking if first arrow room is adjacent to second arrow room.
    if(roomIsAdjacent(Rooms[gameInfo->arrowRoom], *room2)){
        gameInfo->arrowRoom = *room2;
        // If Wumpus is in the room with the second arrow, will indicate that user has won.
        if (gameInfo->wumpusRoom == gameInfo->arrowRoom){
            printf( "Wumpus has just been pierced by your deadly arrow! \n"
                    "Congratulations on your victory, you awesome hunter you.\n");
            *wumpusIsAlive = false;
            return;
        }
        // If Player is in the room with the second arrow, will indicate that user has died and lost.
        if (gameInfo->personRoom == gameInfo->arrowRoom){
            printf( "You just shot yourself.  \n"
                    "Maybe Darwin was right.  You're dead.\n");
            *personIsAlive = false;
            return;
        }
    }
    // If the room is not adjacent, branch will be entered
    else{
        printf("Room %d is not adjacent.  Arrow ricochets...\n", *room2);
        gameInfo->arrowRoom = *room1;
        // Second arrow enters lowest number room after ricochet.
        gameInfo->arrowRoom = Rooms[gameInfo->arrowRoom][0];
        // If ricochet hits the Wumpus, the user will win accidentally.
        if (gameInfo->wumpusRoom == gameInfo->arrowRoom) {
            printf( "Your arrow ricochet killed the Wumpus, you lucky dog!\n"
                    "Accidental victory, but still you win!\n");
            *wumpusIsAlive = false;
            return;
        }
        // If ricochet hits the Player, the Player will die and the user will lose
        else if (gameInfo->personRoom == gameInfo->arrowRoom) {
            printf( "You just shot yourself, and are dying.\n"
                    "It didn't take long, you're dead.\n");
            *personIsAlive = false;
            return;
        }
    }
    // When arrow is shot, Wumpus will move to lowest numbered adjacent room.
    gameInfo->wumpusRoom = Rooms[gameInfo->wumpusRoom][0];
}
void shootArrow3(struct GameInfo *gameInfo, // Hazards location and game info
                 bool *personIsAlive,       // Person is alive, but could die depending on the hazards
                 bool *wumpusIsAlive,       // Wumpus is alive, but could die depending on the hazards
                 int **Rooms,               // Rooms within the game with associated tunnels
                 int *room1,                // Room where user has indicated where they would like the first arrow to go
                 int *room2,                // Room where user has indicated where they would like the second arrow to go
                 int *room3){               // Room where user has indicated where they would like the third arrow to go
    //Checking if room is adjacent
    if(roomIsAdjacent(Rooms[gameInfo->personRoom], *room1)){
        gameInfo->arrowRoom = *room1;
        // If Wumpus is in the room with the first arrow, will indicate that user has won.
        if (gameInfo->wumpusRoom == gameInfo->arrowRoom){
            printf( "Wumpus has just been pierced by your deadly arrow! \n"
                    "Congratulations on your victory, you awesome hunter you.\n");
            *wumpusIsAlive = false;
            return;
        }
        // If Player is in the room with the first arrow, will indicate that user has died and lost.
        if (gameInfo->personRoom == gameInfo->arrowRoom){
            printf( "You just shot yourself.  \n"
                    "Maybe Darwin was right.  You're dead.\n");
            *personIsAlive = false;
            return;
        }
    }
    // If the room is not adjacent, branch will be entered
    else{
        printf("Room %d is not adjacent.  Arrow ricochets...\n",*room1);
        // Arrow enters lowest number room after ricochet.
        gameInfo->arrowRoom = Rooms[gameInfo->personRoom][0];
        // If ricochet hits the Wumpus, the user will win accidentally.
        if (gameInfo->wumpusRoom == gameInfo->arrowRoom) {
            printf( "Your arrow ricochet killed the Wumpus, you lucky dog!\n"
                    "Accidental victory, but still you win!\n");
            *wumpusIsAlive = false;
            return;
        }
        // If ricochet hits the Player, the Player will die and the user will lose
        else if (gameInfo->personRoom == gameInfo->arrowRoom) {
            printf( "You just shot yourself, and are dying.\n"
                    "It didn't take long, you're dead.\n");
            *personIsAlive = false;
            return;
        }
    }
    //Checking if first arrow room is adjacent to second arrow room
    if(roomIsAdjacent(Rooms[gameInfo->arrowRoom], *room2)){
        gameInfo->arrowRoom = *room2;
        // If Wumpus is in the room with the second arrow, will indicate that user has won.
        if (gameInfo->wumpusRoom == gameInfo->arrowRoom){
            printf( "Wumpus has just been pierced by your deadly arrow! \n"
                    "Congratulations on your victory, you awesome hunter you.\n");
            *wumpusIsAlive = false;
            return;
        }
        // If Player is in the room with the second arrow, will indicate that user has died and lost.
        if (gameInfo->personRoom == gameInfo->arrowRoom){
            printf( "You just shot yourself.  \n"
                    "Maybe Darwin was right.  You're dead.\n");
            *personIsAlive = false;
            return;
        }
    }
    // If the room is not adjacent, branch will be entered
    else{
        printf("Room %d is not adjacent.  Arrow ricochets...\n",*room2);
        // Second arrow enters lowest number room after ricochet.
        gameInfo->arrowRoom = Rooms[gameInfo->arrowRoom][0];
        // If ricochet hits the Wumpus, the user will win accidentally.
        if (gameInfo->wumpusRoom == gameInfo->arrowRoom) {
            printf( "Your arrow ricochet killed the Wumpus, you lucky dog!\n"
                    "Accidental victory, but still you win!\n");
            *wumpusIsAlive = false;
            return;
        }
        // If ricochet hits the Player, the Player will die and the user will lose
        else if (gameInfo->personRoom == gameInfo->arrowRoom) {
            printf( "You just shot yourself, and are dying.\n"
                    "It didn't take long, you're dead.\n");
            *personIsAlive = false;
            return;
        }
    }
    //Checking if second arrow room is adjacent to third arrow room
    if(roomIsAdjacent(Rooms[gameInfo->arrowRoom], *room3)){
        gameInfo->arrowRoom = *room3;
        // If Wumpus is in the room with the third arrow, will indicate that user has won.
        if (gameInfo->wumpusRoom == gameInfo->arrowRoom){
            printf( "Wumpus has just been pierced by your deadly arrow! \n"
                    "Congratulations on your victory, you awesome hunter you.\n");
            *wumpusIsAlive = false;
            return;
        }
    // If Player is in the room with the third arrow, will indicate that user has died and lost.
        if (gameInfo->personRoom == gameInfo->arrowRoom){
            printf( "You just shot yourself.  \n"
                    "Maybe Darwin was right.  You're dead.\n");
            *personIsAlive = false;
            return;
        }
    }
    // If the room is not adjacent, branch will be entered
    else{
        printf("Room %d is not adjacent.  Arrow ricochets...\n",*room3);
        // Arrow enters lowest number room after ricochet.
        gameInfo->arrowRoom = Rooms[gameInfo->arrowRoom][0];
        // If ricochet hits the Wumpus, the user will win accidentally
        if (gameInfo->wumpusRoom == gameInfo->arrowRoom) {
            printf( "Your arrow ricochet killed the Wumpus, you lucky dog!\n"
                    "Accidental victory, but still you win!\n");
            *wumpusIsAlive = false;
            return;
        }
        // If ricochet hits the Player, the Player will die and the user will lose
        else if (gameInfo->personRoom == gameInfo->arrowRoom) {
            printf( "You just shot yourself, and are dying.\n"
                    "It didn't take long, you're dead.\n");
            *personIsAlive = false;
            return;
        }
    }
    // When arrow is shot, Wumpus will move to lowest numbered adjacent room.
    gameInfo->wumpusRoom = Rooms[gameInfo->wumpusRoom][0];
}


//--------------------------------------------------------------------------------
int main(void) {
    struct GameInfo gameInfo;   // Used to more easily pass game info variables around
    bool personIsAlive = true;  // Used in checking for end of game
    bool wumpusIsAlive = true;  // Used in checking for end of game
    char typeOfMove;            // Used to handle user input letter
    int nextRoom;               // User input of destination room number, used on a 'M' type move
    int **Rooms;                // Rooms within the game with associated tunnels

    // Using malloc() to allocated space for rooms.
    Rooms = (int**)malloc(21 * sizeof(int*)); // 21 rows allocated
    for(int i = 0; i < 21; ++i){
        Rooms[i] = malloc(3 * sizeof(int));   // 3 columns allocated
    }
    //Lines 602-684 attach rooms numbers with attached tunnels in ascending order
    Rooms[0][0] = 0;        // Creating empty row 0 to account for off by 1 index
    Rooms[0][1] = 0;        // Creating empty row 0 to account for off by 1 index
    Rooms[0][2] = 0;        // Creating empty row 0 to account for off by 1 index

    Rooms[1][0] = 2;
    Rooms[1][1] = 5;
    Rooms[1][2] = 8;

    Rooms[2][0] = 1;
    Rooms[2][1] = 3;
    Rooms[2][2] = 10;

    Rooms[3][0] = 2;
    Rooms[3][1] = 4;
    Rooms[3][2] = 12;

    Rooms[4][0] = 3;
    Rooms[4][1] = 5;
    Rooms[4][2] = 14;

    Rooms[5][0] = 1;
    Rooms[5][1] = 4;
    Rooms[5][2] = 6;

    Rooms[6][0] = 5;
    Rooms[6][1] = 7;
    Rooms[6][2] = 15;

    Rooms[7][0] = 6;
    Rooms[7][1] = 8;
    Rooms[7][2] = 17;

    Rooms[8][0] = 1;
    Rooms[8][1] = 7;
    Rooms[8][2] = 9;

    Rooms[9][0] = 8;
    Rooms[9][1] = 10;
    Rooms[9][2] = 18;

    Rooms[10][0] = 2;
    Rooms[10][1] = 9;
    Rooms[10][2] = 11;

    Rooms[11][0] = 10;
    Rooms[11][1] = 12;
    Rooms[11][2] = 19;

    Rooms[12][0] = 3;
    Rooms[12][1] = 11;
    Rooms[12][2] = 13;

    Rooms[13][0] = 12;
    Rooms[13][1] = 14;
    Rooms[13][2] = 20;

    Rooms[14][0] = 4;
    Rooms[14][1] = 13;
    Rooms[14][2] = 15;

    Rooms[15][0] = 6;
    Rooms[15][1] = 14;
    Rooms[15][2] = 16;

    Rooms[16][0] = 15;
    Rooms[16][1] = 17;
    Rooms[16][2] = 20;

    Rooms[17][0] = 7;
    Rooms[17][1] = 16;
    Rooms[17][2] = 18;

    Rooms[18][0] = 9;
    Rooms[18][1] = 17;
    Rooms[18][2] = 19;

    Rooms[19][0] = 11;
    Rooms[19][1] = 18;
    Rooms[19][2] = 20;

    Rooms[20][0] = 13;
    Rooms[20][1] = 16;
    Rooms[20][2] = 19;

    // Seed the random number generator.  Change seed to time(0) to change output each time.
    //srand(time(1));
    srand(0);

    // Set random initial values for person, Wumpus, bats and pits
    initializeGame( &gameInfo);

    // Main playing loop.  Break when player dies, or player kills Wumpus
    while (personIsAlive && wumpusIsAlive) {

        // Display current room information: Room number, hazards detected
        displayRoomInfo(gameInfo,&*Rooms);

        // Prompt for and handle move
        printf("%d. Enter your move (or 'D' for directions): ", gameInfo.moveNumber);
        // Note the extra space in the scanf below between the opening quote and the %c.
        //    This skips leading white space in the input so that the newline left over from
        //    a previous move is not read in and used as the current move.  An alternative
        //    is using getchar() after the scanf() statement.
        scanf(" %c", &typeOfMove);

        typeOfMove = toupper(typeOfMove);  // Make uppercase to facilitate checking

        // Check all types of user input and handle them.  This uses if-else-if code
        //   rather than switch-case, so that we can take advantage of break and continue.
        if( typeOfMove == 'D') {
            displayCave();
            displayInstructions();
            continue;       // Loop back up to re prompt for the same move
        }
        else if( typeOfMove == 'P') {
            // To assist with play, display the cave layout
            displayCave();
            continue;       // Loop back up to re prompt for the same move
        }
        else if( typeOfMove == 'M') {
            // Move to an adjacent room,
            // Note the extra space in the scanf below between the opening quote and the %c.
            //    This skips leading white space in the input so that the newline left over from
            //    a previous move is not read in and used as the current move.  An alternative
            //    is using getchar() after the scanf() statement.
            scanf(" %d", &nextRoom);

            if( roomIsAdjacent( Rooms[gameInfo.personRoom], nextRoom)) {
                gameInfo.personRoom = nextRoom;        // move to a new room
                // Check if pit or wumpus is present in this new room
                checkForHazards(&gameInfo, &personIsAlive,&*Rooms);
                if (gameInfo.arrowRoom == gameInfo.personRoom){
                    printf("Congratulations, you found the arrow and can once again shoot.\n");
                    gameInfo.arrowRoom = -1;
                }
            }
            else {
                printf("Invalid move.  Please retry. \n");
                continue;        // Doesn't count as a move, so retry same move.
            }
        }
        else if( typeOfMove == 'S'){
            // Initializing local variables to aid in use of arrow shoot functions.
            int arrowRoom1 = 0;
            int arrowRoom2 = 0;
            int arrowRoom3 = 0;
            int arrowRoomNum = 0;
            // Branch will be entered if player has arrow to shoot.
            if(gameInfo.arrowRoom == -1){
                //Receiving user input for # of rooms into which they want to shoot.
                printf("Enter the number of rooms (1..3) into which you want to shoot, followed by the rooms themselves: ");
                scanf("%d",&arrowRoomNum);
                //If user wants to shoot arrow into 1 room, branch will be entered.
                if(arrowRoomNum == 1){
                    //Scanning for rooms themselves.
                    scanf("%d",&arrowRoom1);
                    shootArrow1(&gameInfo, &personIsAlive,&wumpusIsAlive,&*Rooms,&arrowRoom1);
                }
                //If user wants to shoot arrow into 2 rooms, branch will be entered.
                else if(arrowRoomNum == 2){
                    //Scanning for rooms themselves.
                    scanf("%d %d",&arrowRoom1,&arrowRoom2);
                    //Shooting arrow.
                    shootArrow2(&gameInfo, &personIsAlive,&wumpusIsAlive,&*Rooms,&arrowRoom1,&arrowRoom2);
                }
                //If user wants to shoot arrow into 2 rooms, branch will be entered.
                else if(arrowRoomNum == 3){
                    //Scanning for rooms themselves.
                    scanf("%d %d %d",&arrowRoom1,&arrowRoom2,&arrowRoom3);
                    //Shooting arrow.
                    shootArrow3(&gameInfo, &personIsAlive,&wumpusIsAlive,&*Rooms,&arrowRoom1,&arrowRoom2,&arrowRoom3);
                }
                //If user enters more than 3 rooms, their input will be defaulted to 3 rooms.
                else{
                    printf("Sorry, the max number of rooms is 3.  Setting that value to 3.");
                    scanf("%d %d %d",&arrowRoom1,&arrowRoom2,&arrowRoom3);
                    //Shooting arrow.
                    shootArrow3(&gameInfo, &personIsAlive,&wumpusIsAlive,&*Rooms,&arrowRoom1,&arrowRoom2,&arrowRoom3);
                }
            }
            // If arrow is not with player, prompt is shown to user.
            else{
                printf("Sorry, you can't shoot an arrow you don't have.  Go find it.\n");
            }
        }
        else if( typeOfMove == 'C') {
            // Display Cheat information
            displayCheatInfo(gameInfo);
            continue;        // Doesn't count as a move, so retry same move.
        }
        else if( typeOfMove == 'R') {
            // Specify resetting the hazards and person positions, useful for testing
            resetPositions( &gameInfo);
            continue;        // Doesn't count as a move, so retry same move.
        }
        // Below guess function was used in version 1. In our case we are trying to shoot and kill the
        //   Wumpus. This section is commented out to allow for function use to still be available if uncommented.
        /*else if( typeOfMove == 'G') { //delete this when done
            // Prompt for room number guess and see if user has found the Wumpus
            int wumpusRoomGuess = 0;
            printf("Enter room (1..20) you think Wumpus is in: ");
            scanf(" %d", &wumpusRoomGuess);
            // See if user guess was correct, for a win or loss.
            if( gameInfo.wumpusRoom == wumpusRoomGuess) {
                printf("You won!\n");
            }
            else{
                printf("You lost.\n");
            }
            break;
        }*/
        else if( typeOfMove == 'X') {
            // Exit program
            personIsAlive = false;   // Indicate person is dead as a way to exit playing loop
            break;
        }

        // Increment the move number
        gameInfo.moveNumber = gameInfo.moveNumber + 1;

    }//end while(personIsAlive && wumpusIsAlive)

    printf("\nExiting Program ...\n");

    return 0;
}
