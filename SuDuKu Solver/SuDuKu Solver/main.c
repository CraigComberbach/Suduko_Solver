#include <stdio.h>
#include <stdlib.h>

/*
Before optimizations to the validity function it takes 40.592 seconds to run test code
After simple optimizations to Row and Column Checks it is now 23.877 seconds to run test code
After removal of redundant check during a failed attempt, it is now 19.819 seconds to run test code
After configuring the compiler for maximum speed optimization, it is now 15.219 seconds to run test code
*/

int SudokuTime(int grid[9][9]);
int validity(int grid[9][9]);
void initialize(int grid[9][9]);
void printGrid(int grid[9][9]);
void userInput(int grid[9][9]);
int digitConverter(char digit);

void test(int grid[9][9]);

//Global variable
long int count;

int main()
{
    int grid[9][9];
	char	path;

    //Initialize variable
    count = 0;
    initialize(grid);

    //Accept user I/P for starting parameters of grid
	printf("Enter custom puzzle (Y/N): ");
	while(path = toupper(getche()), (path != 'Y') && (path != 'N'));
	if(path == 'Y')
		userInput(grid);
	else
		test(grid);

    printGrid(grid);

    if(SudokuTime(grid) == 1)
    {
        printGrid(grid);
        printf("\n\nSolved in %ld steps\n", count);
    }
    else
    {
        printGrid(grid);
        printf("Sorry try again %ld", count);
    }

    return 0;
}

int SudokuTime(int grid[9][9])
{
    int x,
        y,
        testNumber;

    ++count; //How many steps it took to solve. I guess this could be a rough measure of how complicated the puzzle was?

	//Find the cell that this function will work on
    for(y = 0; y < 9; ++y)
    {
        for(x = 0; x < 9; ++x)
        {
            if(grid[x][y] == 0)
                break;
        }

        if(grid[x][y] == 0)
            break;
    }

//Find a number that fits and move on to the next cell
    for(testNumber = 1; testNumber <= 9; ++testNumber)
    {
        grid[x][y] = testNumber;
        if(validity(grid))//If it is valid, prepare to move onto the next tile
        {
            if((count % 10000) == 0)
            {
                printGrid(grid);
                printf("\nCurrent iteration = %ld", count);
            }

            if((x == 8) && (y == 8))//If its the last available cell then collapse the recursion chain
                return 1;
            else if(SudokuTime(grid))//If the next tile is valid, break out of the function
                return 1;
            else
                continue;//Really just a statement to fluff out the code
        }
        else if((x == 8) && (y == 8) && (testNumber == 9))//Unable to solve the puzzle
        {
            grid[x][y] = 0;
            printf("\nSorry the puzzle as you entered it was not solvable.\n\n\nBetter luck next time!\n");
            return 1;
        }
    }

    grid[x][y] = 0;//Process failed, reset to 0 and prepare to backtrack

    return 0;//Safety net, if unsure, always default to back up and try again!
    //Also, without this return statement, the compiler throws a warning my way :(
}

int validity(int grid[9][9])
{
    int x,
        y,
        testx,
        testy,
        startx,
        starty,
        loop;

    //Check rows for validity
    for(y = 0; y < 9; ++y)
        for(x = 0; x < 9; ++x)
            for(testx = 8; testx > x; --testx)
                if((testx != x) && (grid[testx][y] == grid[x][y]) && (grid[testx][y] != 0))//If the value is repeated in this row (Excluding the test point and empty squares)
                {
                    return 0;//This is not valid
                }

    //Check columns for validity
    for(y = 0; y < 9; ++y)
        for(x = 0; x < 9; ++x)
            for(testy = 8; testy > y; --testy)
                if((testy != y) && (grid[x][testy] == grid[x][y]) && (grid[x][testy] != 0))//If the value is repeated in this row (Excluding the testy point and empty squares)
                {
                    return 0;//This is not valid
                }

    //check 3x3 squares
    for(y = 0; y < 9; ++y)
        for(x = 0; x < 9; ++x)
        {
            if((x >= 0) && (x <= 2))
                startx = 0;
            else if((x >= 3) && (x <= 5))
                startx = 3;
            else if((x >= 6) && (x <= 8))
                startx = 6;

            if((y >= 0) && (y <= 2))
                starty = 0;
            else if((y >= 3) && (y <= 5))
                starty = 3;
            else if((y >= 6) && (y <= 8))
                starty = 6;

            for(testy = 0; testy < 3; ++testy)
                for(testx = 0; testx < 3; ++testx)
                    if(!(((starty + testy) == y) && ((startx + testx) == x)) && (grid[startx + testx][starty + testy] == grid[x][y]) && (grid[startx + testx][starty + testy] != 0))//If the value is repeated in this box (Excluding the test point and empty squares)
                    {
                        return 0;//This is not valid
                    }
        }

    return 1;
}

void initialize(int grid[9][9])
{
    int x,
        y;

    for(y = 0; y < 10; ++y)
        for(x = 0; x < 10; ++x)
            grid[x][y] = 0;

    return;
}

void printGrid(int grid[9][9])
{
    int x,
        y;

    system("cls");

    for(y = 0; y < 9; ++y)
    {
        for(x = 0; x < 25; ++x)
            if((y % 3) == 0)
                printf("-");

        printf("\n");

        for(x = 0; x < 9; ++x)
        {
            if((x % 3) == 0)
                printf("| ");

            if(grid[x][y] == 0)
                printf("  ");
            else if(grid[x][y] == 10)
                printf("X ");
            else
                printf("%d ", grid[x][y]);
        }

        printf("|\n");
    }

    for(x = 0; x < 25; ++x)
        printf("-");

 return;
}

void userInput(int grid[9][9])
{
    int x,
        y,
        data;

    for(y = 0; y < 9; ++y)
        for(x = 0; x < 9; ++x)
        {
            grid[x][y] = data = 10;//10 is used to tell the print function to print an X, it also ensures that the following loop will also always be entered
            printGrid(grid);//Print the grid, including the current location marker

            while(!((data >= 0) && (data <= 9)))
            {
                //get and clean the user input, note it does not require the enter key to be hit (++Bonus)
                data = digitConverter(getch());
            }

            grid[x][y] = data;//Assign the coveted user input into its most holy of resting places
        }

        if(validity(grid) == 0)
        {
            printf("Try again!%c\n", 0x7);
            initialize(grid);
            userInput(grid);
        }

    return;
}

int digitConverter(char digit)
{
    int temp;
    switch(digit)
    {
        case '0':
            temp = 0;
            break;
        case '1':
            temp = 1;
            break;
        case '2':
            temp = 2;
            break;
        case '3':
            temp = 3;
            break;
        case '4':
            temp = 4;
            break;
        case '5':
            temp = 5;
            break;
        case '6':
            temp = 6;
            break;
        case '7':
            temp = 7;
            break;
        case '8':
            temp = 8;
            break;
        case '9':
            temp = 9;
            break;
        default:
            temp = 10;
            break;
    }

    return temp;
}

void test(int grid[9][9])
{
    char i[] = {'0','0','7','1','0','0','0','0','4','0','6','0','0','2','0','1','0','0','0','0','5','0','0','0','8','6','0','5','0','0','0','4','0','9','2','0','7','0','0','0','0','0','0','0','5','0','4','9','0','8','0','0','0','3','0','9','6','0','0','0','5','0','0','0','0','1','0','3','0','0','9','0','4','0','0','0','0','6','2','0','0'};
    int x,
        y,
        z = 0;

    for(y = 0; y < 9; ++y)
        for(x = 0; x < 9; ++x)
        {
            grid[x][y] = digitConverter(i[z]);
            ++z;
        }
/*
    SudokuTime(grid);
    if(count == 420471)
        for(x = 0; x < 3; ++x)
            putchar(0x7);
*/
    return;
}
