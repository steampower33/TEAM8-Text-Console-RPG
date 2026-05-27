#pragma once
#include <string>
#include <vector>
#include <map>

#define NOMINMAX // ★ windows.h가 min, max 매크로를 생성하지 못하게 차단
#include <windows.h>
#include <conio.h>
#include <sstream>
#include <iomanip>

#define KEY_UP    72
#define KEY_DOWN  80
#define KEY_LEFT  75
#define KEY_RIGHT 77
#define KEY_ENTER 13
#define KEY_ESC   27

namespace Color 
{
const std::string RESET   = "\033[0m";
    
const std::string RED     = "\033[38;5;124m";
const std::string ORANGE     = "\033[38;5;208m";

}

class Character;
class Inventory;
class IItem;

enum class TextAlign { Left, Center, Right };

struct Vec2
{
    int x;
    int y;
};

struct Box
{
    int startX;
    int startY;
    int endX;
    int endY;
};

// https://oualator.com/calculate/ascii-art-generator/
// Width 50
// Brightness 100 
// Contrast 29
// Sharpness 10
// Invert Colors True

class UIManager
{
public:
    void Initialize();
    std::string ShowCharacterGeneration();
    int GetTitleResult();

    void ShowMainFrame();
    void UpdateStat(Character* character);

    void ClearScenePanel();
    void UpdateScene(
        bool isCombat, std::string monsterName, int playerOffset = 0, 
        int monsterOffset = 0, bool isMonsterDead = false, bool isPlayerAttacked = false, bool isMonsterAttacked = false);
    void AnimateStrike(
        bool isPlayerAttacking, std::string monsterName, 
        bool isMonsterDead = false, bool isPlayerAttacked = false, bool isMonsterAttacked = false);
    
    std::vector<std::string> LogMessages;
    void PrintLog(const std::string& message = "", int delay = 300);
    void UpdateInventory(Inventory* inven);
    void ShowShop(Character* character);
    void ShowPlayerDead();
    void ShowEndingCredit();

    std::map<std::string, int> killList;
    void UpdateKillList(std::string monsterName = "");
    std::string RepeatString(const std::string& str, int count);

    int HandleMenuInput(int& selectedIndex, int maxMenu);
    int ShowMenuAt(Vec2 at,
                   const std::vector<std::string>& menuList,
                   int step = 2,
                   bool isVertical = true);
    int ShowMenuAlign(int startX,
                      int endX,
                      int y,
                      const std::vector<std::string>& menuList,
                      int step = 2,
                      TextAlign textAlign = TextAlign::Left);
    void PrintTextAt(int x, int y, const std::string& text);
    int GetDisplayWidth(const std::string& text);
    void PrintTextAlign(int startX,
                        int endX,
                        int y,
                        const std::string& text,
                        TextAlign align = TextAlign::Left);
    void DrawBox(int startX, int startY, int endX, int endY);
    void Gotoxy(int x, int y);
    void DrawTitleMenu();

    void DrawScenePanel();
    void DrawStatPanel();
    void DrawInventoryPanel();
    void DrawLogPanel();
    void DrawKillListPanel();

    int ConsoleWidth = 150;
    int ConsoleHeight = 40;

    int StartSceneX = 0;
    int StartSceneY = 0;
    int EndSceneX = 110;
    int EndSceneY = 25;

    int StartStatX = EndSceneX + 1;
    int StartStatY = StartSceneY;
    int EndStatX = ConsoleWidth - 1;
    int EndStatY = 9;

    int StartInventoryX = StartStatX;
    int StartInventoryY = EndStatY + 1;
    int EndInventoryX = ConsoleWidth - 1;
    int EndInventoryY = EndSceneY;

    int StartLogX = StartSceneX;
    int StartLogY = EndSceneY + 1;
    int EndLogX = EndSceneX;
    int EndLogY = ConsoleHeight - 1;

    int StartKillX = EndLogX + 1;
    int StartKillY = StartLogY;
    int EndKillX = ConsoleWidth - 1;
    int EndKillY = ConsoleHeight - 1;

private:
    std::vector<std::string> warriorIdle = {
        R"_EOL(                                               @@ )_EOL",
        R"_EOL(                                              @ * )_EOL",
        R"_EOL(                                              =-@ )_EOL",
        R"_EOL(                                              ##  )_EOL",
        R"_EOL(                   @##@                      @:*  )_EOL",
        R"_EOL(                 @*----#@                    #-@  )_EOL",
        R"_EOL(                @--------@                  @.#   )_EOL",
        R"_EOL(                @:-(*-*)-#                  +-*   )_EOL",
        R"_EOL(                *----^---#                  #-@   )_EOL",
        R"_EOL(                %--------*                 @:-%   )_EOL",
        R"_EOL(             @*%*:-------# @@              @-#    )_EOL",
        R"_EOL(         @*##---:---------#--###+@        @ -#    )_EOL",
        R"_EOL(       @ ------------------------+        =--@    )_EOL",
        R"_EOL(       .--------------------------@       %-#     )_EOL",
        R"_EOL(      @:--------------------------:@     @:-*     )_EOL",
        R"_EOL(       #--------------------------*      @:-@     )_EOL",
        R"_EOL(       %--------------------------#      +-#      )_EOL",
        R"_EOL(     @+----- -----------------:----#*@+*@--*      )_EOL",
        R"_EOL(    @.-----.@ ---------------+%:-----.%:---+@@@   )_EOL",
        R"_EOL(    @:----:@ #--------------# @ :-----:----*      )_EOL",
        R"_EOL(    @:---:@  %--------------#  @@*#####.---@      )_EOL",
        R"_EOL(   @.----*  @.---------------@         @.%@       )_EOL",
        R"_EOL(   @:---:@ @ -----------------@        +#         )_EOL",
        R"_EOL(   @.---@  @:-----------------#       @:+         )_EOL",
    };

    std::vector<std::string> warriorBattle = {
        R"_EOL(      @@+*@                                       )_EOL",
        R"_EOL(       @%.+@                                      )_EOL",
        R"_EOL(         @@ *%+                                   )_EOL",
        R"_EOL(           @% =#@                                 )_EOL",
        R"_EOL(             @=--#@    @*+@                       )_EOL",
        R"_EOL(               @*#-@   %--:##@                    )_EOL",
        R"_EOL(                  @%+*#-------#@                  )_EOL",
        R"_EOL(                    @*:----(*)-#                  )_EOL",
        R"_EOL(                     +---------@                 )_EOL",
        R"_EOL(                   @**:-----\===@                 )_EOL",
        R"_EOL(                 @+.-:---------@                 )_EOL",
        R"_EOL(               @*.---------==-::*%:@#@            )_EOL",
        R"_EOL(               %--------------@@+-=--*            )_EOL",
        R"_EOL(               +-------------#  *:----#@          )_EOL",
        R"_EOL(               @.-------------##:@:---:**@        )_EOL",
        R"_EOL(               @+:---------------:--=+-:--#@      )_EOL",
        R"_EOL(                =:---------------------- #--@     )_EOL",
        R"_EOL(                @=:------------------:@=@ @@@     )_EOL",
        R"_EOL(                 #--------------:::##@            )_EOL",
        R"_EOL(                 @.------------*@@@               )_EOL",
        R"_EOL(                 *------------%                   )_EOL",
        R"_EOL(               @+--------------@                  )_EOL",
        R"_EOL(               *----------------@                 )_EOL",
        R"_EOL(               #-----------------*@               )_EOL",
    };

    std::vector<std::string> goblin = {
        R"_EOL(                  @######@    @%**@              )_EOL",
        R"_EOL(                @*#--------####::#@               )_EOL",
        R"_EOL(              @*:-------------:%@                 )_EOL",
        R"_EOL(              @:--*)----------@                   )_EOL",
        R"_EOL(             @---------------=--#@                 )_EOL",
        R"_EOL(  -ㅡㅡㅡ      @%@/------::--------###*@            )_EOL",
        R"_EOL( /-:--=#\        +:::#*@#-------------*@          )_EOL",
        R"_EOL( |-:----|\       @@@@   *--------------:*@        )_EOL",
        R"_EOL(  \+.:---#\             #-------------:.-:*@      )_EOL",
        R"_EOL(    \% :---\           @.--:.--------:=@=---@     )_EOL",
        R"_EOL(     \#.:--|          *--.@@:--------* @=--#     )_EOL",
        R"_EOL(      \%::-|       @#--:@ %:--------#  #:-#     )_EOL",
        R"_EOL(       =\@@\-###@#%*:: #*@%.----------@ @:-#     )_EOL",
        R"_EOL(         \@@ %:---::###@@  *------------%  @:-@    )_EOL",
        R"_EOL(           \@%*#.-@     @#:------------@ @.---@   )_EOL",
        R"_EOL(                @ :@   @:-------------@ @ %:--@   )_EOL",
        R"_EOL(                 @@  @::---:----------@ @=@ :*@   )_EOL",
        R"_EOL(                      :--*:@@@@.------@  @:@@     )_EOL",
        R"_EOL(                     @:--:@    @*.----*@          )_EOL",
        R"_EOL(                      @.--*      @**::::#@        )_EOL",
        R"_EOL(                       @.-%        @@%@.-##*@     )_EOL",
        R"_EOL(                      @:---@           @:---@     )_EOL",
        R"_EOL(                 @**#*..#:.*@         @:--:@      )_EOL",
        R"_EOL(                 @#@%#@@ @@         @@**#*@       )_EOL",
    };


    std::vector<std::string> slime = {
        R"_EOL(                     @##*%@                       )_EOL",
        R"_EOL(                 @**@:----:##*@                   )_EOL",
        R"_EOL(               @*=@% #---------#@                 )_EOL",
        R"_EOL(              @--@ *#------------@                )_EOL",
        R"_EOL(             @:---#---------------@               )_EOL",
        R"_EOL(             *--+-----------------*               )_EOL",
        R"_EOL(             #--------------------=@              )_EOL",
        R"_EOL(            @----------------------*              )_EOL",
        R"_EOL(           @------------------.-+=-=@             )_EOL",
        R"_EOL(          @----:-------------:@.-----#@           )_EOL",
        R"_EOL(       @##----+*--------------.=------:%@         )_EOL",
        R"_EOL(    @*#-----=--:-------------------------##@#@@   )_EOL",
        R"_EOL(  @ :::::-----------=---------------------::.*@   )_EOL",
        R"_EOL(   @#@%@++=:-#--------------=---.#=--=++=+@@@     )_EOL",
        R"_EOL(        @##%@ @%####.... ####@*+@ @#.. ..@        )_EOL",
        R"_EOL(                    @@@@@           @@@@@         )_EOL",
    };

    std::vector<std::string> orc = {
        R"_EOL(                            @*%@                  )_EOL",
        R"_EOL(                          @#----@                 )_EOL",
        R"_EOL(                        @#-------#%               )_EOL",
        R"_EOL(                         #-(*--*))-.@             )_EOL",
        R"_EOL(                       @#-==-^-+--.*#@            )_EOL",
        R"_EOL(                   @+##--------------:##%@        )_EOL",
        R"_EOL(        @###@     @:---------------------=@       )_EOL",
        R"_EOL(     @**----:+#   *------------------------@      )_EOL",
        R"_EOL(     *-----:*@@   #-------------------------@     )_EOL",
        R"_EOL(    @-----:.*+ %*#-------------------------:+@    )_EOL",
        R"_EOL(-#*#-----:*@  :-----------------------%*----:+@   )_EOL",
        R"_EOL( @#------:=@  @.-------=-------------@  *----:.@  )_EOL",
        R"_EOL(   @# ----::*@@ @....%#--------------@ @------*@  )_EOL",
        R"_EOL(     @+*%%*##+%@ @@@@  *-------------* @=-----@   )_EOL",
        R"_EOL(                      @---------------#------ @   )_EOL",
        R"_EOL(                     @---------------------:-@    )_EOL",
        R"_EOL(                    @---------------------:*@     )_EOL",
        R"_EOL(                   @=--------------------#@@      )_EOL",
        R"_EOL(                   #--------------------@         )_EOL",
        R"_EOL(                   #-----: .@@@@%:------@         )_EOL",
        R"_EOL(                  @-----=:@@    @@.:----*         )_EOL",
        R"_EOL(                  #------@        %+-----#@       )_EOL",
        R"_EOL(                  #-----:.@       %=------+       )_EOL",
        R"_EOL(                  @-----::@        @:----:-@      )_EOL",
        R"_EOL(                   #----:@         @+----=@       )_EOL",
        R"_EOL(                  @----- @           #---- @      )_EOL",
        R"_EOL(                @#---: *@@           #----- @     )_EOL",
        R"_EOL(                @####@@              @#%%%#@@     )_EOL",
    };

    std::vector<std::string> troll = {
        R"_EOL(                                   @@             )_EOL",
        R"_EOL(                              @####:.##@          )_EOL",
        R"_EOL(                           @*#--------- @         )_EOL",
        R"_EOL(                      @####:--------:#*@          )_EOL",
        R"_EOL(                    @#---------.- #*@             )_EOL",
        R"_EOL(                 @*#----------===@. @             )_EOL",
        R"_EOL(          @@ @##*@-@---(*---*).**-*==@             )_EOL",
        R"_EOL(     @+###--#--@@ @---------------=--:#@          )_EOL",
        R"_EOL(   @*@-------@  @:-------\/-------------@          )_EOL",
        R"_EOL( @@@@ @-----@  @------------------------:@        )_EOL",
        R"_EOL( @#@  @----:@:@--------------------------:@       )_EOL",
        R"_EOL(       #----:------------------------------@      )_EOL",
        R"_EOL(       #------------------------------------@     )_EOL",
        R"_EOL(       @---------.@+-------------------------#@   )_EOL",
        R"_EOL(        @-----:*#@ *------------------#-------@@  )_EOL",
        R"_EOL(         @*#*@@   @------------------#  %------#  )_EOL",
        R"_EOL(                  *------------------@  @------*  )_EOL",
        R"_EOL(                  @------------------.@  #-----#  )_EOL",
        R"_EOL(                   #------------------@  @----:@  )_EOL",
        R"_EOL(                  @-------------------@  @.-----# )_EOL",
        R"_EOL(                  %-------------------@  @------@ )_EOL",
        R"_EOL(                 @-------------------:@  @-=----@@)_EOL",
        R"_EOL(                 *-----:.:@@@:-------@  @@=---*@@ )_EOL",
        R"_EOL(                @:----:=@@   @@+------@  @@@@@@   )_EOL",
        R"_EOL(                 @:----.@      @#:-----@          )_EOL",
        R"_EOL(                  *----:@        @*#-----@@@      )_EOL",
        R"_EOL(                  @:---:@           @----- @      )_EOL",
        R"_EOL(                @#:-----.@          *----.@       )_EOL",
        R"_EOL(              @@####*%#*=@         @####*@        )_EOL",
    };

    std::vector<std::string> boss = {
        R"_EOL(                               @@                 )_EOL",
        R"_EOL(                    #@@        @-#@               )_EOL",
        R"_EOL(   @*@+%           @*@          @- @              )_EOL",
        R"_EOL( @#--.+@           *=@ @#@ @%*#@::  %             )_EOL",
        R"_EOL(@-----: @@         #.:#--.@:---::=@.#             )_EOL",
        R"_EOL(@-------.@        @#%:-(*---*)--===-.*             )_EOL",
        R"_EOL(@%#------:@      @-:--------------.@+    @+@      )_EOL",
        R"_EOL(@@ -------%       @ -:---\/-------- @   @--%      )_EOL",
        R"_EOL( @@@ -----:@       @-----:=-------=.@ @*---*      )_EOL",
        R"_EOL(    @#*----%       %-------+--------* #----#      )_EOL",
        R"_EOL(       @#:-.@     @---=-:------------#-----.@     )_EOL",
        R"_EOL(         @*# @  @#---:::#:-----------------#      )_EOL",
        R"_EOL(            @ @#-=-- @-*:------+=--=-------#      )_EOL",
        R"_EOL(             #:----+@+.--:----------:-------#@    )_EOL",
        R"_EOL(             #----# ===-:+==-------:#:--------*@  )_EOL",
        R"_EOL(             @##+: @ -:--=:------==.:--------:+#@ )_EOL",
        R"_EOL(                @*%=-----:+-------.@:------:.:.=@ )_EOL",
        R"_EOL(                    *---:-:---------:-----=+@@    )_EOL",
        R"_EOL(                   @---:@ *:--=:----+:--.*#@      )_EOL",
        R"_EOL(                   @#.---# --=-+---- %%*@         )_EOL",
        R"_EOL(                     @.- @@=:@%*--- @             )_EOL",
        R"_EOL(                @*#*@.--@       #-:*@             )_EOL",
        R"_EOL(             @##--==-----@   ##*=-:+@             )_EOL",
    };
};


extern UIManager ui;