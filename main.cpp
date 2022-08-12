#include <iostream>
#include <SDL.h>
#include <time.h>
#include <string>
using namespace std;

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 750;

const int recSize = 10;
const int MarrSize = 90;
int Uarr[90];
int Carr[90];

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool complete = false;

bool init()
{
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Couldn't initialize SDL.\nSDL Error: " << SDL_GetError();
        success = false;
    }
    else
    {
        if(!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")))
            cout << "Warning: Linear Texture Filtering not enabled.\n";

        window = SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(window == NULL)
        {
            cout << "Couldn't create window.\nSDL Error: " << SDL_GetError();
            success = false;
        }
        else if(renderer == NULL)
        {
            cout << "Couldn't create renderer.\nSDL Error: " << SDL_GetError();
            success = false;
        }
    }

    return success;
}


void close()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}


void visualize(int x=-1, int y=-1, int z=-1)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    int j = 0;
    for(int i=0; i<=SCREEN_WIDTH-recSize; i+=recSize)
    {
        SDL_PumpEvents();

        SDL_Rect rect = {i, 0, recSize, Carr[j]};
        if(complete)
        {
            SDL_SetRenderDrawColor(renderer, 8, 255, 8, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        else if(j==x || j==z)
        {
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else if(j == y)
        {
            SDL_SetRenderDrawColor(renderer, 255, 69, 0, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 170, 183, 184, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        j++;
    }
    SDL_RenderPresent(renderer);
}


void randomizedArray()
{
    unsigned int seed = (unsigned)time(NULL);
    srand(seed);
    for(int i=0; i<MarrSize; i++)
    {
        int random = rand()%(SCREEN_HEIGHT);
        Uarr[i] = random;
    }
}
void loadArray()
{
    memcpy(Carr, Uarr, sizeof(int)*MarrSize);
}


void bubbleSort()
{
    for(int i=0; i<MarrSize-1; i++)
    {
        for(int j=0; j<MarrSize-1-i; j++)
        {
            if(Carr[j+1]<Carr[j])
            {
                int temp=Carr[j];
                Carr[j]=Carr[j+1];
                Carr[j+1]=temp;

                visualize(j+1, j, MarrSize-i);
            }
            SDL_Delay(1);
        }
    }
}

void insertionSort()
{
    for(int i=1; i<MarrSize; i++)
    {
        int j=i-1;
        int temp=Carr[i];
        while(j>=0 && Carr[j]>temp)
        {
            Carr[j+1]=Carr[j];
            j--;

            visualize(i, j+1);
            SDL_Delay(5);
        }
        Carr[j+1]=temp;
    }
}

void selectionSort()
{
    int minIndex;
    for(int i=0;i<MarrSize-1;i++)
    {
        minIndex=i;
        for(int j=i+1;j<MarrSize;j++)
        {
            if(Carr[j]<Carr[minIndex])
            {
                minIndex=j;
                visualize(i, minIndex);
            }
            SDL_Delay(1);
        }
        int temp=Carr[i];
        Carr[i]=Carr[minIndex];
        Carr[minIndex]=temp;
    }
}


void execute()
{
    if(!init())
        cout << "SDL Initialization Failed\n";
    else
    {
        randomizedArray();
        loadArray();

        SDL_Event eve;
        bool quit = false;

        while(!quit)
        {
            while(SDL_PollEvent(&eve) != 0)
            {
                if(eve.type == SDL_QUIT)
                {
                    quit = true;
                    complete = false;
                }
                else if(eve.type == SDL_KEYDOWN) {
                    switch (eve.key.keysym.sym) {
                        case (SDLK_q):
                            quit = true;
                            complete = false;
                            cout << "\nExiting Sorting Visualizer.\n";
                            break;
                        case (SDLK_0):
                            randomizedArray();
                            complete = false;
                            loadArray();
                            cout << "\nNew Random List Generated.\n";
                            break;
                        case (SDLK_1):
                            loadArray();
                            cout << "\nSelection Sort Started.\n";
                            complete = false;
                            selectionSort();
                            complete = true;
                            cout << "\nSelection Sort Complete.\n";
                            break;
                        case (SDLK_2):
                            loadArray();
                            cout << "\nInsertion Sort Started.\n";
                            complete = false;
                            insertionSort();
                            complete = true;
                            cout << "\nInsertion Sort Complete.\n";
                            break;
                        case (SDLK_3):
                            loadArray();
                            cout << "\nBubble Sort Started.\n";
                            complete = false;
                            bubbleSort();
                            complete = true;
                            cout << "\nBubble Sort Complete.\n";
                            break;
                    }
                }
            }
            visualize();
        }
        close();
    }
}


bool controls()
{
    cout <<"WARNING: Giving repetitive commands may cause latency and the visualizer may behave unexpectedly. Please give a new command only after the current command's execution is done.\n\n"
         <<"Available Controls inside Sorting Visualizer:-\n"
         <<"    Use 0 to Generate a different randomized list.\n"
         <<"    Use 1 to start Selection Sort Algorithm.\n"
         <<"    Use 2 to start Insertion Sort Algorithm.\n"
         <<"    Use 3 to start Bubble Sort Algorithm.\n"
         <<"    Use q to exit out of Sorting Visualizer\n\n"
         <<"PRESS ENTER TO START SORTING VISUALIZER...\n\n"
         <<"Or type -1 and press ENTER to quit the program.";

    string s;
    getline(cin, s);
    if(s=="-1")
    {
        return false;
    }
    //else if(s=="\n")
    //{
    //    return true;
    //}
    return true;
}


void intro()
{
    cout<<"==============================Sorting Visualizer==============================\n\n"
        <<"Visualization of some different sorting algorithms. A sorting algorithm is an algorithm that puts the elements of a list in a certain order.\n"
        <<"In this implementation of sorting visualizer, we'll be looking at some of these sorting algorithms and visually comprehend their working.\n"
        <<"The sorting algorithms covered here are Selection Sort, Insertion Sort, Bubble Sort.\n"
        <<"The list size is fixed to 90 elements. You can randomize the list and select any type of sorting algorithm to call on the list from the given options. Here, all sorting algorithms will sort the elements in ascending order.\n\n"
        <<"Press ENTER to show controls...";

    string s;
    getline(cin, s);
    if(s=="\n")
    {
        return;
    }
}


int main()
{
    intro();

    while(true)
    {
        cout << '\n';
        if(controls())
            execute();
        else
        {
            cout << "\nExiting Program.\n";
            break;
        }
    }
    return 0;
}
