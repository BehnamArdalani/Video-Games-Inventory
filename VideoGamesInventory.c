/*
Project Name    : Video Game Project
Description     : This project manages video games information. User can load information from a file, 
				  add new information, search, update, or delete informations. This project written with functions and a good structure with
                  minimum required memory.
Author          : Behnam Ardalani (@BehnamArdalani)
Language        : C
Date            : Dec 19, 2021
Time            : 17:13:55
Version         : 1.5.3
*/

#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include <sys/time.h>

// ********  All Program Parameters:

#define MAX_VIDEO_GAMES 50
#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))
#define inttostring(input) #input

const char VideoType[7][13] = { "Action", "Idle", "Role-playing", "Simulation", "Sports", "Strategy", "Other" };
const char ESRB_Rating[6][5] = {"E", "E10+", "T", "M", "A", "RP" };
const char Platform[6][16] = {"PS", "Xbox", "Nintendo Switch", "PC", "Mobile", "Streaming" };
const char PlayerModes[3][14] = {"Single player", "Coop", "Competitive" };
const char GameModes[2][6] = {"Local", "Cloud"};

// ********  Structure and Global Variables:

typedef struct VideoGame_St {
    char sku[20];
    char name[50];
    char studio[50];
    bool type[LEN(VideoType)];
    int esrbRating;
    int metacriticRating;
    bool platform[LEN(Platform)];
    bool playerModes[LEN(PlayerModes)];
    bool gameModes[LEN(GameModes)];
    float sellPrice;
    bool digital;
} VideoGame;

VideoGame allvideogames[MAX_VIDEO_GAMES]  = { NULL };
int videogameCount = 0;

// ********  Functions:

void getVideoGame(VideoGame*,int);
void showVideoGame(VideoGame);
void showAllVideoGames();
int mainMenu();
int findVideoGame(VideoGame*);
void editVideoGame();
void writeToFile();
int readFromFile();
char *time_stamp();

// ********  Main Function:

int main(){

    printf("\n********   Welcome to the video games manager application   *******");


    do{
        switch(mainMenu()){
            case 1:
                {
                    char loadfile;
                    do{
                        printf("\nWARNING:\nIf you load data from the file, all information in the memory will be lost!\n");
                        printf("\nDo you want to load inventory data from allvideogames.data? (y/n) (default = n) ");
                        scanf("%c",&loadfile);
                        fflush(stdin);
                    }while(loadfile != 'y' && loadfile != 'n' && loadfile != '\n');
                    if(loadfile == 'y'){
                        int fileresult = readFromFile();
                    }
                    break;
                }
            case 2:
                {
                    if(videogameCount < MAX_VIDEO_GAMES){
                        getVideoGame(&allvideogames[videogameCount],0);
                        printf("\nThe video game information added to the inventory successfully.",allvideogames[videogameCount].name);
                        videogameCount++;
                    }else{
                        printf("\nThe inventory is full, you can not add any more information!");
                    }
                    break;
                }
            case 3:
                {
                    VideoGame videogames[MAX_VIDEO_GAMES] = { NULL };
                    int matchcount = findVideoGame(&videogames);
                    printf("\n\nMatched %d video games with your search.",matchcount);
                    break;
                }
            case 4:
                {
                    editVideoGame();
                    break;
                }
            case 5:
                {
                    showAllVideoGames();
                    break;
                }
            case 6:
                {
                    writeToFile();
                    printf("\n******   All video games saved to : allvideogames.data!");
                }
            case 7:
                {
                    printf("\n******   Thank you. See you later...   *******\n\n");

                    return 0;
                }
        }
    } while (1);

}

// ********  Other Functions:

void getVideoGame(VideoGame *videogame,int optmode){ // optmode : { 0 -> insert , 1 -> edit }

    printf("\n\n********   Adding a new video game   *******");

    printf("\nPlease enter the name (String 50): ");
    fgets(videogame->name,sizeof(videogame->name),stdin);
    fflush(stdin);

    printf("Please enter the studio (String 50): ");
    fgets(videogame->studio,sizeof(videogame->studio),stdin);
    fflush(stdin);

    if(optmode==0)
        strcpy(videogame->sku,time_stamp());

    printf("Please enter the game types: (default answer = n)\n");
    for(int i=0; i < LEN(VideoType); i++){
        char gType;
        do{
            printf("    \\--- %s type? (n/y): ",VideoType[i]);
            scanf("%c",&gType);
            fflush(stdin);
        }while(gType != 'y' && gType != 'n' && gType != '\n');

        videogame->type[i]= ((gType == 'y') ? 1 : 0);
    }

    int esRating = 0;
    do{
        printf("\nESRB rating list:\n");
        for(int i=0; i < LEN(ESRB_Rating); i++){
            printf("\t%d.%s" ,i + 1,ESRB_Rating[i]);
        }
        printf("\nPlease enter one Id of the above list: ");
        scanf("%d",&esRating);
        fflush(stdin);
    } while (esRating > LEN(ESRB_Rating) || esRating < 0);
    videogame->esrbRating= esRating - 1;
    fflush(stdin);

    do{
        printf("\nPlease enter the Metacritic rating (0..100): ");
        scanf("%d",&videogame->metacriticRating);
        fflush(stdin);
    }while(videogame->metacriticRating < 0 || videogame->metacriticRating > 100);

    printf("\nPlease enter the platform: (default answer = n)\n");
    for(int i=0; i < LEN(Platform); i++){
        char platform;
        do{
            printf("    \\--- %s platform? (n/y): ",Platform[i]);
            scanf("%c",&platform);
            fflush(stdin);
        }while(platform != 'y' && platform != 'n' && platform != '\n');
        videogame->platform[i]= ((platform == 'y') ? 1 : 0);
    }

    printf("\nPlease enter the player modes: (default answer = n)\n");
    for(int i=0; i < LEN(PlayerModes); i++){
        char playerMode;
        do{
            printf("    \\--- %s player mode? (n/y): ",PlayerModes[i]);
            scanf("%c",&playerMode);
            fflush(stdin);
        }while(playerMode != 'y' && playerMode != 'n' && playerMode != '\n');
        videogame->playerModes[i]= ((playerMode == 'y') ? 1 : 0);
    }

    printf("\nPlease enter the game modes: (default answer = n)\n");
    for(int i=0; i < LEN(GameModes); i++){
        char gameMode;
        do{
            printf("    \\--- %s game mode? (n/y): ",GameModes[i]);
            scanf("%c",&gameMode);
            fflush(stdin);
        }while(gameMode != 'y' && gameMode != 'n' && gameMode != '\n');

        videogame->gameModes[i]= ((gameMode == 'y') ? 1 : 0);
    }

    do{
        printf("\nPlease enter the sell price: ");
        scanf("%f",&videogame->sellPrice);
        fflush(stdin);
    }while(videogame->sellPrice < 0);

    char downloadable;
    do{
        printf("\nDigital version (downloadable)? (y/n) (default answer = n) ");
        scanf("%c",&downloadable);
        fflush(stdin);
    }while(downloadable != 'y' && downloadable != 'n' && downloadable != '\n');

    videogame->digital = ((downloadable == 'y') ? 1 : 0);
}

void showVideoGame(VideoGame videogame){

    printf("\n\n********   Showing video game details   *******");

    printf("\n   The unique Id (SKU):\t\t");
    printf("%s",videogame.sku);

    printf("   The name:\t\t\t");
    printf("%s",videogame.name);

    printf("   The studio:\t\t\t");
    printf("%s",videogame.studio);

    printf("   The type:\t\t\t");
    for(int i=0 ; i < LEN(VideoType); i++){
        if(videogame.type[i])
            printf("%s\t",VideoType[i]);
    }

    printf("\n   The ESRB rating:\t\t");
    printf("%s\t",ESRB_Rating[videogame.esrbRating]);

    printf("\n   The Metacritic rating:\t");
    printf("%d\n",videogame.metacriticRating);

    printf("   The platform:\t\t");
    for(int i=0 ; i < LEN(Platform); i++){
        if(videogame.platform[i])
            printf("%s\t",Platform[i]);
    }


    printf("\n   The player modes:\t\t");
    for(int i=0 ; i < LEN(PlayerModes); i++){
        if(videogame.playerModes[i])
            printf("%s\t",PlayerModes[i]);
    }

    printf("\n   The game modes:\t\t");
    for(int i=0 ; i < LEN(GameModes); i++){
        if(videogame.gameModes[i])
            printf("%s\t",GameModes[i]);
    }

    printf("\n   The sell price:\t\t");
    printf("%f",videogame.sellPrice);

    printf("\n   Digital version (Downloadable): ");
    printf((videogame.digital == 0) ? ("No") : ("Yes"));


}

void showAllVideoGames(){
    printf("\n******   All video games in the inventory   *******");
    for(int i = 0; i < videogameCount; i ++){
        showVideoGame(allvideogames[i]);
    }

}

int mainMenu() {
    int selectedoption = 0;
    do{
        printf("\n\n********   Main Menu:   *******");
        printf("\n   1 - Load the inventory from file");
        printf("\n   2 - Populate the inventory");
        printf("\n   3 - Search for a video game");
        printf("\n   4 - Edit a video game information");
        printf("\n   5 - Display all video games");
        printf("\n   6 - Save and Exit the system");
        printf("\n   7 - Exit the system without save");
        printf("\nPlease select one option : ");
        scanf("%d",&selectedoption);
        fflush(stdin);
    } while (selectedoption > 7 || selectedoption < 1);
    fflush(stdin);

    return selectedoption;
}

int findVideoGame(VideoGame *videogames){

    int matchcount = 0;

    int selectedSearch = 0;
    do{
        printf("\n\n********   Search Menu   *******");
        printf("\n  1. Search by SKU or Name or Studio");
        printf("\n  2. Search by Game Type");
        printf("\n  3. Search by ESRB Rating");
        printf("\n  4. Search by Metacritic Rating");
        printf("\n  5. Search by Platform");
        printf("\n  6. Search by Player Modes");
        printf("\n  7. Search by Game Modes");
        printf("\n  8. Search by Sell Price");
        printf("\n  9. Search by downloadable");
        printf("\n  0. Back to Main Menu");
        printf("\nPlease select one of above options : ");
        fflush(stdin);
        scanf("%d",&selectedSearch);
        fflush(stdin);
    } while (selectedSearch > 9 || selectedSearch < 0);

    switch(selectedSearch){
        case 1:
            {
                printf("\nSearch for (enter a text or a number) ? ");
                char searchtext[50];
                fgets(searchtext,50,stdin);
                fflush(stdin);

                matchcount = 0;
                for (int i = 0; i < videogameCount; i++) {

                    if(strstr(allvideogames[i].sku,searchtext) > 0 ||
                       strstr(allvideogames[i].name,searchtext) > 0 ||
                       strstr(allvideogames[i].studio,searchtext) > 0){
                           videogames[matchcount++] = allvideogames[i];
                           showVideoGame(allvideogames[i]);
                       }
                }

                break;
            }
        case 2:
            {
                printf("\nAll video game types : ");
                for(int i = 0; i < LEN(VideoType); i++){
                    printf("%d-%s\t",i+1,VideoType[i]);
                }
                int searchtypeid = 0;
                do{
                    printf("\nPlease enter the id of video type : ");
                    scanf("%d",&searchtypeid);
                    fflush(stdin);
                }while(searchtypeid < 1 && searchtypeid > LEN(VideoType));

                matchcount = 0;
                for (int i = 0; i < videogameCount; i++) {
                    if(allvideogames[i].type[searchtypeid-1]){
                           videogames[matchcount++] = allvideogames[i];
                           showVideoGame(allvideogames[i]);
                       }
                }

                break;
            }
        case 3:
            {
                printf("\nAll video game ESRB ratings : ");
                for(int i = 0; i < LEN(ESRB_Rating); i++){
                    printf("%d-%s\t",i+1,ESRB_Rating[i]);
                }
                int searchesrbid = 0;
                do{
                    printf("\nPlease enter the id of ESRB rating : ");
                    scanf("%d",&searchesrbid);
                    fflush(stdin);
                }while(searchesrbid < 1 && searchesrbid > LEN(ESRB_Rating));

                matchcount = 0;
                for (int i = 0; i < videogameCount; i++) {
                    if(allvideogames[i].esrbRating == searchesrbid){
                           videogames[matchcount++] = allvideogames[i];
                           showVideoGame(allvideogames[i]);
                       }
                }

                break;
            }
        case 4:
            {
                printf("\nSearch for Metacritic Rating (0..100) ");
                int searchmetarate_min;
                int searchmetarate_max;
                do{
                    printf("\nPlease enter the lowest rate (zero or higher) : ");
                    scanf("%d",&searchmetarate_min);
                    fflush(stdin);
                }while(searchmetarate_min < 0 && searchmetarate_min > 100);

                do{
                    printf("\nPlease enter the highest rate (100 or lower) : ");
                    scanf("%d",&searchmetarate_max);
                    fflush(stdin);
                }while(searchmetarate_max < 0 && searchmetarate_max > 100);

                matchcount = 0;
                for (int i = 0; i < videogameCount; i++) {
                    if(allvideogames[i].metacriticRating >= searchmetarate_min &&
                       allvideogames[i].metacriticRating <= searchmetarate_max){
                           videogames[matchcount++] = allvideogames[i];
                           showVideoGame(allvideogames[i]);
                       }
                }

                break;
            }
        case 5:
            {
                printf("\nAll video game platforms : ");
                for(int i = 0; i < LEN(Platform); i++){
                    printf("%d-%s\t",i+1,Platform[i]);
                }
                int searchplatformid = 0;
                do{
                    printf("\nPlease enter the id of platform : ");
                    scanf("%d",&searchplatformid);
                    fflush(stdin);
                }while(searchplatformid < 0 && searchplatformid > LEN(Platform));
                matchcount = 0;
                for (int i = 0; i < videogameCount; i++) {
                    if(allvideogames[i].platform[searchplatformid-1]){
                           videogames[matchcount++] = allvideogames[i];
                           showVideoGame(allvideogames[i]);
                       }
                }

                break;
            }
        case 6:
            {
                printf("\nAll video game player modes : ");
                for(int i = 0; i < LEN(PlayerModes); i++){
                    printf("%d-%s\t",i+1,PlayerModes[i]);
                }
                int searchplayerid = 0;
                do{
                    printf("\nPlease enter the id of player mode : ");
                    scanf("%d",&searchplayerid);
                    fflush(stdin);
                }while(searchplayerid < 0 && searchplayerid > LEN(PlayerModes));
                matchcount = 0;
                for (int i = 0; i < videogameCount; i++) {
                    if(allvideogames[i].playerModes[searchplayerid-1]){
                           videogames[matchcount++] = allvideogames[i];
                           showVideoGame(allvideogames[i]);
                       }
                }

                break;
            }
        case 7:
            {
                printf("\nAll video game, game modes : ");
                for(int i = 0; i < LEN(GameModes); i++){
                    printf("%d-%s\t",i+1,GameModes[i]);
                }
                int searchgameid = 0;
                do{
                    printf("\nPlease enter the id of game mode : ");
                    scanf("%d",&searchgameid);
                    fflush(stdin);
                }while(searchgameid < 0 && searchgameid > LEN(GameModes));
                matchcount = 0;
                for (int i = 0; i < videogameCount; i++) {
                    if(allvideogames[i].playerModes[searchgameid-1]){
                           videogames[matchcount++] = allvideogames[i];
                           showVideoGame(allvideogames[i]);
                       }
                }

                break;
            }
        case 8:
            {
                printf("\nSearch for sell price (amount) ");
                float searchprice_min;
                float searchprice_max;
                do{
                    printf("\nPlease enter the lowest rate (0.00 or higher) : ");
                    scanf("%f",&searchprice_min);
                    fflush(stdin);
                }while(searchprice_min < 0.0);

                printf("\nPlease enter the highest rate : ");
                scanf("%d",&searchprice_max);
                fflush(stdin);

                matchcount = 0;
                for (int i = 0; i < videogameCount; i++) {
                    if(allvideogames[i].sellPrice >= searchprice_min &&
                       allvideogames[i].sellPrice <= searchprice_max){
                           videogames[matchcount++] = allvideogames[i];
                           showVideoGame(allvideogames[i]);
                       }
                }

                break;
            }
        case 9:
            {
                char searchdownloadable;
                do{
                    printf("\nDownloadable games ? (y/n) (default = n)");
                    scanf("%c",&searchdownloadable);
                    fflush(stdin);
                }while(searchdownloadable != 'y' && searchdownloadable != 'n' && searchdownloadable != '\n');

                matchcount = 0;
                for (int i = 0; i < videogameCount; i++) {
                    if(allvideogames[i].digital == (searchdownloadable=='y')?(1):(0)){
                           videogames[matchcount++] = allvideogames[i];
                           showVideoGame(allvideogames[i]);
                       }
                }

                break;
            }
        case 0:
            break;
    }

    return matchcount;

}

void editVideoGame(){
    printf("\n\n********   Edit Menu   *******");
    printf("\nBefore editing video games, you should find them...");
    VideoGame editedvideogames[MAX_VIDEO_GAMES] = { NULL };

    int searchcount = findVideoGame(&editedvideogames);

    printf("\n\nThere is/are %d row/s to edit.",searchcount);

    if(searchcount > 0){
        for(int i = 0; i < searchcount; i++){
            printf("\n\nEditing row %d : ",i + 1);

            for(int j = 0; j < videogameCount; j++){
                if(!strcmp(allvideogames[j].sku, editedvideogames[i].sku)){
                    int editmode;
                    do{
                            printf("\nEdit modes: ");
                            printf("\n  1. Edit all information of the video game");
//                            printf("\n  2. Edit the SKU");
                            printf("\n  2. Edit the name");
                            printf("\n  3. Edit the studio");
                            printf("\n  4. Edit the type");
                            printf("\n  5. Edit the ESRB rating");
                            printf("\n  6. Edit the Metacritic rating");
                            printf("\n  7. Edit the platform");
                            printf("\n  8. Edit the player modes");
                            printf("\n  9.Edit the game modes");
                            printf("\n  10.Edit the sell price");
                            printf("\n  11.Edit the digital");
                            printf("\n  12.Cancel editing this row");
                            printf("\nPlease enter the edit mode id : ");
                            scanf("%d",&editmode);
                            fflush(stdin);
                    }while(editmode < 0 || editmode > 13);

                    switch(editmode){
                    case 1 :
                        {
                            getVideoGame(&allvideogames[j],1);
                            printf("\nThis row edited successfully.");
                            break;
                        }
/*                    case 2:
                        {
                            printf("\nPlease enter the unique Id (String 16): ");
                            fgets(allvideogames[j].sku,sizeof(allvideogames[j].sku),stdin);
                            fflush(stdin);
                            printf("\nThis row edited successfully.");
                            break;
                        }
*/
                    case 2:
                        {
                            printf("Please enter the name (String 50): ");
                            fgets(allvideogames[j].name,sizeof(allvideogames[j].name),stdin);
                            fflush(stdin);
                            printf("\nThis row edited successfully.");
                            break;
                        }
                    case 3:
                        {
                            printf("Please enter the studio (String 50): ");
                            fgets(allvideogames[j].studio,sizeof(allvideogames[j].studio),stdin);
                            fflush(stdin);
                            printf("\nThis row edited successfully.");
                            break;
                        }
                    case 4:
                        {
                            printf("Please enter the game types: (default answer = n)\n");
                            for(int i=0; i < LEN(VideoType); i++){
                                char gType;
                                do{
                                    printf("    \\--- %s type? (n/y): ",VideoType[i]);
                                    scanf("%c",&gType);
                                    fflush(stdin);
                                }while(gType != 'y' && gType != 'n' && gType != '\n');

                                allvideogames[j].type[i]= ((gType == 'y') ? 1 : 0);
                            }
                            printf("\nThis row edited successfully.");
                            break;
                        }
                    case 5:
                        {
                            int esRating = 0;
                            do{
                                printf("\nESRB rating list:\n");
                                for(int i=0; i < LEN(ESRB_Rating); i++){
                                    printf("\t%d.%s" ,i + 1,ESRB_Rating[i]);
                                }
                                printf("\nPlease enter one Id of the above list: ");
                                scanf("%d",&esRating);
                                fflush(stdin);
                            } while (esRating > LEN(ESRB_Rating) && esRating < 0);
                            allvideogames[j].esrbRating= esRating - 1;
                            fflush(stdin);
                            printf("\nThis row edited successfully.");
                            break;
                        }
                    case 6:
                        {
                            do{
                                printf("\nPlease enter the Metacritic rating (0..100): ");
                                scanf("%d",&allvideogames[j].metacriticRating);
                                fflush(stdin);
                            }while(allvideogames[j].metacriticRating < 0 || allvideogames[j].metacriticRating > 100);
                            printf("\nThis row edited successfully.");
                            break;
                        }
                    case 7:
                        {
                            printf("\nPlease enter the platform: (default answer = n)\n");
                            for(int i=0; i < LEN(Platform); i++){
                                char platform;
                                do{
                                    printf("    \\--- %s platform? (n/y): ",Platform[i]);
                                    scanf("%c",&platform);
                                    fflush(stdin);
                                }while(platform != 'y' && platform != 'n' && platform != '\n');
                                allvideogames[j].platform[i]= ((platform == 'y') ? 1 : 0);
                            }
                            printf("\nThis row edited successfully.");
                            break;
                        }
                    case 8:
                        {
                            printf("\nPlease enter the player modes: (default answer = n)\n");
                            for(int i=0; i < LEN(PlayerModes); i++){
                                char playerMode;
                                do{
                                    printf("    \\--- %s player mode? (n/y): ",PlayerModes[i]);
                                    scanf("%c",&playerMode);
                                    fflush(stdin);
                                }while(playerMode != 'y' && playerMode != 'n' && playerMode != '\n');
                                allvideogames[j].playerModes[i]= ((playerMode == 'y') ? 1 : 0);
                            }
                            printf("\nThis row edited successfully.");
                            break;
                        }
                    case 9:
                        {
                            printf("\nPlease enter the game modes: (default answer = n)\n");
                            for(int i=0; i < LEN(GameModes); i++){
                                char gameMode;
                                do{
                                    printf("    \\--- %s game mode? (n/y): ",GameModes[i]);
                                    scanf("%c",&gameMode);
                                    fflush(stdin);
                                }while(gameMode != 'y' && gameMode != 'n' && gameMode != '\n');

                                allvideogames[j].gameModes[i]= ((gameMode == 'y') ? 1 : 0);
                            }
                            printf("\nThis row edited successfully.");
                            break;
                        }
                    case 10:
                        {
                            printf("\nPlease enter the sell price: ");
                            scanf("%f",&allvideogames[j].sellPrice);
                            fflush(stdin);
                            printf("\nThis row edited successfully.");
                            break;
                        }
                    case 11:
                        {

                            char downloadable;
                            do{
                                printf("\nDigital version (downloadable)? (y/n) (default answer = n) ");
                                scanf("%c",&downloadable);
                                fflush(stdin);
                            }while(downloadable != 'y' && downloadable != 'n' && downloadable != '\n');

                            allvideogames[j].digital = ((downloadable == 'y') ? 1 : 0);
                            printf("\nThis row edited successfully.");
                            break;
                        }
                    case 12:
                        {
                            printf("\nEditing this row was canceled.");
                            break;
                        }
                    }
                }
            }
        }
    }
}

void writeToFile(){
    FILE *backup = fopen("allvideogames.data", "w");

    for(int i = 0; i < videogameCount; i++){

        allvideogames[i].sku[strcspn(allvideogames[i].sku, "\n")] = 0;
        fputs(allvideogames[i].sku, backup);
        fputs("\t", backup);

        allvideogames[i].name[strcspn(allvideogames[i].name, "\n")] = 0;
        fputs(allvideogames[i].name, backup);
        fputs("\t", backup);

        allvideogames[i].studio[strcspn(allvideogames[i].studio, "\n")] = 0;
        fputs(allvideogames[i].studio, backup);
        fputs("\t", backup);

        for(int j = 0; j < LEN(VideoType); j++){
            fprintf(backup , "%d" , allvideogames[i].type[j]);
        }
        fputs("\t", backup);

        fprintf(backup, "%d\t", allvideogames[i].esrbRating);

        fprintf(backup , "%d\t", allvideogames[i].metacriticRating);

        for(int j = 0; j < LEN(Platform); j++){
            fprintf(backup, "%d", allvideogames[i].platform[j]);
        }
        fputs("\t", backup);

        for(int j = 0; j < LEN(PlayerModes); j++){
            fprintf(backup, "%d", allvideogames[i].playerModes[j]);
        }
        fputs("\t", backup);

        for(int j = 0; j < LEN(GameModes); j++){
            fprintf(backup, "%d", allvideogames[i].gameModes[j]);
        }
        fputs("\t", backup);

        fprintf(backup, "%f\t", allvideogames[i].sellPrice);

        fprintf(backup, "%d", allvideogames[i].digital);

        fprintf(backup, "\n");

    }

    fclose(backup);

}

int readFromFile(){
    FILE *restore = fopen("allvideogames.data", "r");

    char sku[255];
    char name[255];
    char studio[255];
    char type[255];
    int  esrbRating;
    int  metacriticRating;
    char platform[255];
    char playerModes[255];
    char gameModes[255];
    float sellPrice;
    int digital;

    while(fscanf(restore, "%s\t%s\t%s\t%s\t%d\t%d\t%s\t%s\t%s\t%f\t%d\n",&sku, &name, &studio , &type ,&esrbRating ,&metacriticRating ,&platform ,&playerModes ,&gameModes ,&sellPrice ,&digital ) != EOF)
    {
        strcpy(allvideogames[videogameCount].sku,strcat(sku,"\n"));
        strcpy(allvideogames[videogameCount].name,strcat(name,"\n"));
        strcpy(allvideogames[videogameCount].studio,strcat(studio,"\n"));
        for(int i = 0; i < LEN(allvideogames[videogameCount].type); i++){
            allvideogames[videogameCount].type[i] = type[i] - '0';
        }
        allvideogames[videogameCount].esrbRating = esrbRating;
        allvideogames[videogameCount].metacriticRating = metacriticRating;
        for(int i = 0; i < LEN(platform); i++){
            allvideogames[videogameCount].platform[i] = platform[i] - '0';
        }
        for(int i = 0; i < LEN(playerModes); i++){
            allvideogames[videogameCount].playerModes[i] = playerModes[i] - '0';
        }
        for(int i = 0; i < LEN(gameModes); i++){
            allvideogames[videogameCount].gameModes[i] = gameModes[i] - '0';
        }
        allvideogames[videogameCount].sellPrice = sellPrice;
        allvideogames[videogameCount].digital = digital;

        videogameCount++;
    }

    printf("\nTotal loaded video games from file : %d", videogameCount);

    fclose(restore);

}

char *time_stamp(){
    char *timestamp = (char *)malloc(sizeof(char) * 16);
    time_t ltime;
    ltime=time(NULL);
    struct tm *tm;
    tm=localtime(&ltime);
    sprintf(timestamp,"%04d%02d%02d%02d%02d%02d\n", tm->tm_year+1900, tm->tm_mon,
        tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
return timestamp;
}
