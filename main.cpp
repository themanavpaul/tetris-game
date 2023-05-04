//Tetris Games For C++ Project (Minor)
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
using namespace sf;

const int M = 20;	//M is the number of rows
const int N = 10;   //N is the number of columns.

int field[M][N] = {0};

struct Point
{int x,y;} a[4], b[4];

int figures[7][4] =
{
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};
//current position of a tetromino is valid or not
//squares of the tetris are outside/overlap the game field
bool check()
{
   for (int i=0;i<4;i++)
      if (a[i].x<0 || a[i].x>=N || a[i].y>=M) return 0;
      else if (field[a[i].y][a[i].x]) return 0;

   return 1;
};


int main()
{
    srand(time(0));   //seeds the random number generator with the current time  

    RenderWindow window(VideoMode(320, 480), "The Game!");
	// texture objects that will be used to store images
    Texture t1,t2,t3,t4;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/background.png");
    t3.loadFromFile("images/frame.png");
	t4.loadFromFile("images/gameover.png");
    Sprite s(t1), background(t2), frame(t3), gameover(t4); // two-dimensional bitmap or animation
    
    ///-----------Load sound files-----------////
	sf::SoundBuffer lineClearBuffer;
	lineClearBuffer.loadFromFile("images/lineclear.wav");
	sf::Sound lineClearSound;
	lineClearSound.setBuffer(lineClearBuffer);
	
	sf::SoundBuffer gameoverBuffer;
	gameoverBuffer.loadFromFile("images/gameover.wav");
	sf::Sound gameoverSound;
	gameoverSound.setBuffer(gameoverBuffer);


    int dx=0; bool rotate=0; int colorNum=1;
    float timer=0,delay=0.3; // track time

    Clock clock; //elapsed time 
    
	////-----------Play background music-----------////
	sf::Music music;
	music.openFromFile("images/musicfile.wav");
	music.setLoop(true);
	music.play();

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed)
              if (e.key.code==Keyboard::Up) rotate=true;
              else if (e.key.code==Keyboard::Left) dx=-1;
              else if (e.key.code==Keyboard::Right) dx=1;
        }

    if (Keyboard::isKeyPressed(Keyboard::Down)) delay=0.05;

    ////----------- <- Move -> -----------///
    for (int i=0;i<4;i++)  { b[i]=a[i]; a[i].x+=dx; }
    if (!check()) for (int i=0;i<4;i++) a[i]=b[i];
    
    // TEST FAILED #09
//    // Check if any blocks in the top row are occupied
//	for (int j = 0; j < N-1; j++) {
//	    if (field[0][j]) {
//	        window.close(); // Close the game window
//	        return 0; // Exit the program
//	    }
//	}

	//-----------Game Over TITLE-----------//
	for (int col = 0; col < N-1; col++) {
    if (field[0][col]) {
        Texture t4;
    t4.loadFromFile("images/gameover.png");
    Sprite gameover(t4);
    gameover.setPosition(0, 0);
    window.draw(gameover);
    window.display();
    music.stop(); // stop background music
    gameoverSound.play();
    sf::sleep(sf::seconds(5)); // Pause the program for 5 seconds
    window.close(); // Close the game window
    return 0; // Exit the program
    }
}


    //-----------Rotate-----------//
    if (rotate)
      {
        Point p = a[1]; //center of rotation
        //// Loop through each block in the piece
        for (int i=0;i<4;i++)
          {
          	//// Get the relative x and y distance from the center of rotation
            int x = a[i].y-p.y;
            int y = a[i].x-p.x;
            //// Apply the rotation to the block
            a[i].x = p.x - x;
            a[i].y = p.y + y;
           }
           //// If the rotation results in an invalid position, revert the piece back to its previous position
          if (!check())
			{
			    for (int i = 0; i < 4; i++)
			    {
			        a[i] = b[i];
			    }
			}
      }

    //--------Tick------------//////
    if (timer>delay)  //// If the timer is greater than the delay
      {
      	// // Move the current block down by one row
        for (int i=0;i<4;i++) { b[i]=a[i]; a[i].y+=1; }
		//// If the move is not valid (block collides with existing blocks)		
        if (!check())
        {		///// Place the block on the field (add its color to the corresponding cells)
         for (int i=0;i<4;i++) field[b[i].y][b[i].x]=colorNum;
		//// Set a new random color and figure for the next block
         colorNum=1+rand()%7;
         int n=rand()%7;
         for (int i=0;i<4;i++)
           {  //// Set the coordinates of the new block based on the selected figure
            a[i].x = figures[n][i] % 2;
            a[i].y = figures[n][i] / 2;
           }
        }
		// Reset the timer
         timer=0;
      }

    //-----------check lines-----------////
    int k=M-1;
    for (int i=M-1;i>0;i--)
    {
        int count=0;	//// Count the number of filled cells in the ro
        for (int j=0;j<N;j++)  /// Iterate through each column in the row
        {
            if (field[i][j]) count++;	/// If the cell is not empty, increment count
            field[k][j]=field[i][j];	//// Move the row down by copying it to the k-th row
        }
        if (count<N) k--;	//// If the row is not full, move the k pointer down by one
         else {
            // Play sound when line is cleared
            lineClearSound.play();
        }
    }
		dx=0; // Reset horizontal movement
		rotate=0; // Reset rotation flag
		delay=0.3; // Reset the delay between ticks
    
    


    ///------------draw------------////
    window.clear(Color::White);    //Clears the window with a white color and draws the background image.
    window.draw(background);
          //Loops through the game field matrix, where each cell represents a square on the game screen
    for (int i=0;i<M;i++)
     for (int j=0;j<N;j++)
       {
         if (field[i][j]==0) continue;	//If a cell is empty, continue to the next cell
         //If a cell has a block, sets the texture of the sprite s to the appropriate block color and draws it on the game screen.
         s.setTextureRect(IntRect(field[i][j]*18,0,18,18));	
         s.setPosition(j*18,i*18);
         s.move(28,31); //offset
         window.draw(s);
       }
	////Loops through the current piece's array a of four blocks.
    for (int i=0;i<4;i++)
      {
      	//Sets the texture of the sprite s to the current block's color and draws it on the game screen
        s.setTextureRect(IntRect(colorNum*18,0,18,18));
        s.setPosition(a[i].x*18,a[i].y*18);		
        s.move(28,31); //offset
        window.draw(s);
      }
	//Draws the game frame and displays everything on the window
    window.draw(frame);
    window.display();
    }

    return 0;
}
