#pragma once
#include <string>
#include <vector>
#include <iostream>

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

class Character;
class Inventory;
class IItem;

enum class TextAlign { Left, Center, Right };
struct Vec2 { int x; int y; };
struct Box { int startX; int startY; int endX; int endY; };

class UIManager {
public:
    void Initialize();
    std::string ShowCharacterGeneration();
    int GetTitleResult();
    
    void ShowMainFrame();
    void UpdateStat(Character* character);
    
    void UpdateScene(bool isCombat = false, std::string monsterName = "");
    
    std::vector<std::string> LogMessages;
    void PrintLog(const std::string& message = "");
    void UpdateInventory(Inventory* inven);
    void ShowShop(Character* character);

private:
    int HandleMenuInput(int& selectedIndex, int maxMenu);
    int ShowMenuAt(Vec2 at, const std::vector<std::string>& menuList, int step = 2, bool isVertical = true);
    int ShowMenuAlign(int startX, int endX, int y, const std::vector<std::string>& menuList, int step = 2, TextAlign textAlign = TextAlign::Left);
    void PrintTextAt(int x, int y, const std::string& text);
    int GetDisplayWidth(const std::string& text);
    void PrintTextAlign(int startX, int endX, int y, const std::string& text, TextAlign align);
    std::string RepeatString(const std::string& str, int count);
    void DrawBox(int startX, int startY, int endX, int endY);
    void Gotoxy(int x, int y);
    void DrawTitleMenu();
    
    void DrawScenePanel();
    void DrawStatPanel();
    void DrawInventoryPanel();
    void DrawLogPanel();

private:
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
    int EndLogX = ConsoleWidth - 1;
    int EndLogY = ConsoleHeight - 1;
    
    std::vector<std::string> warrior = {
        R"_EOL(        $$                                          )_EOL",
        R"_EOL(       $$ $$                                        )_EOL",
        R"_EOL(        $$   $$                                     )_EOL",
        R"_EOL(          $$$  $$                                   )_EOL",
        R"_EOL(             $$  $$                                 )_EOL",
        R"_EOL(               $$  @$     $$$$                      )_EOL",
        R"_EOL(                 $$  Q$  $    $$                    )_EOL",
        R"_EOL(                   $$   $  ^    $$                  )_EOL",
        R"_EOL(                     $$*  :.      $                 )_EOL",
        R"_EOL(                       $ I`    .  $                 )_EOL",
        R"_EOL(                     $$$  ' ^I!!  $$                )_EOL",
        R"_EOL(                   $$    ^.^   ,:$                  )_EOL",
        R"_EOL(                 $$  ;|i:. ^ $$   n$$$$$$           )_EOL",
        R"_EOL(                 $  -..    ^   $$$    i             )_EOL",
        R"_EOL(                 $ ]"       ^?   $   >  $$          )_EOL",
        R"_EOL(                 $ ,+         I $$Q$ "   $$$$       )_EOL",
        R"_EOL(                 $$ f.        ,       $$     $      )_EOL",
        R"_EOL(                  $ ]>         :>>([+`        $     )_EOL",
        R"_EOL(                  $. -`        ^!|cQ}     $$$$$$    )_EOL",
        R"_EOL(                   $ .:       ^      L$$$           )_EOL",
        R"_EOL(                   $  ,       " $$$$$               )_EOL",
        R"_EOL(                  $  !"       " $                   )_EOL",
        R"_EOL(                 $$ !:.       .  $                  )_EOL",
        R"_EOL(                 $  <.         .  $$                )_EOL",
    };
    
    std::vector<std::string> goblin = {
        R"_EOL(                   $$$$$$$$    $$] $              )_EOL",
        R"_EOL(                $ $        $$$$  $$               )_EOL",
        R"_EOL(              $$    +^...^     $$                 )_EOL",
        R"_EOL(              $  }].      `   $                   )_EOL",
        R"_EOL(             $  $  +.`-rr-" $  $$                 )_EOL",
        R"_EOL(   $$$$      $$$ $ k-.    }      $$$$$            )_EOL",
        R"_EOL( $$    $$            $"$$ `^.` .      $           )_EOL",
        R"_EOL( $  $J. $        $$$$     )`    .   `  $$$        )_EOL",
        R"_EOL(  $   '+ $$             $ |tc}:     >     $$      )_EOL",
        R"_EOL(    $$     $           $ :    !:    ~ $$    $     )_EOL",
        R"_EOL(      $$    $          $ : $$  :    _ C $   $     )_EOL",
        R"_EOL(        $$   $       $$   $ $ _:    ^ $  $  $     )_EOL",
        R"_EOL(          $$  $$$$$$    $$$$  >     `  $ $  $     )_EOL",
        R"_EOL(            $      $$$$$  $  >      '~ $  $  $    )_EOL",
        R"_EOL(            $$$$  $     $$  ?`      ^?  $$    $   )_EOL",
        R"_EOL(                $  $   $  n0]'`I;, `I{ $$ $ L ~   )_EOL",
        R"_EOL(                 $$  $       nx< ~I,    $$$  L$   )_EOL",
        R"_EOL(                       $ $ $      )  $$  $ $$     )_EOL",
        R"_EOL(                     $  : $ $$ $$ I?< $           )_EOL",
        R"_EOL(                      $   %  $$ $$$    J$         )_EOL",
        R"_EOL(                       $  $        $$$$  $$$$     )_EOL",
        R"_EOL(                      $  > $           $ ~  $     )_EOL",
        R"_EOL(                 $  $   $  %$         $    $      )_EOL",
        R"_EOL(                 $|$$$$$ $$         $$ $$$$       )_EOL",
    };
    
    std::vector<std::string> slime = {
        R"_EOL(                      $$$$$                       )_EOL",
        R"_EOL(                 $  $c     $$$$                   )_EOL",
        R"_EOL(               $ $$$ $ r-]:    $$                 )_EOL",
        R"_EOL(              $  $  o     . 'l   $                )_EOL",
        R"_EOL(             $    $   ^..     `>  $               )_EOL",
        R"_EOL(                $   .          ';                 )_EOL",
        R"_EOL(             $    ;'           !1 o$              )_EOL",
        R"_EOL(            $  i!i'         `-t                   )_EOL",
        R"_EOL(           $  }|]^.        `}   $$ $$             )_EOL",
        R"_EOL(          $       ..       `  $      $$           )_EOL",
        R"_EOL(       $$$    $$ + ;:"     `]  $ :rC   $$         )_EOL",
        R"_EOL(    $$$    a$    I    ^   'I<("  ,``+d,  $$$$$$   )_EOL",
        R"_EOL(  $            x+  }$ '  `    -1`             $   )_EOL",
        R"_EOL(   $$$$$$${  $       fm0+   $    $| '$$$$$$$$     )_EOL",
        R"_EOL(        $$$$$ $$$$$$     $$$$$&|$ $$     $        )_EOL",
        R"_EOL(                    $$$$$           $$$$$         )_EOL",
    };
    
    std::vector<std::string> orc = {
        R"_EOL(                            $$$$                  )_EOL",
        R"_EOL(                          $$    $                 )_EOL",
        R"_EOL(                        $$  .?u  BW               )_EOL",
        R"_EOL(                         )        $               )_EOL",
        R"_EOL(                       $$ $$ $ $o  $$$            )_EOL",
        R"_EOL(                    $$$          >"   $$$$        )_EOL",
        R"_EOL(        $$$$$     $$    `:lll;-_~^.,[;   o$       )_EOL",
        R"_EOL(     $$$          $  "'.              '~,  $      )_EOL",
        R"_EOL(     $   [Z $$$   $ .....           `ll;l_  $     )_EOL",
        R"_EOL(    $   +      $$$  .   ..         "+    :{ $$    )_EOL",
        R"_EOL(_$z$  ' f $$  C    ';:];  .       `!  $m  <  $$   )_EOL",
        R"_EOL( $$   )<C $$  $   0Q   r[ .       ,  $  $ .+   $  )_EOL",
        R"_EOL(   $$        $$ $    $$   .       ,  $ $  ,,<     )_EOL",
        R"_EOL(     $$B$$$$$ $$ $$$$  a `        `!   $" !? *p   )_EOL",
        R"_EOL(                      $  .         `'.$   \   $   )_EOL",
        R"_EOL(                     $   .          .   i}   $    )_EOL",
        R"_EOL(                    $  `.            `:_   $$     )_EOL",
        R"_EOL(                   $  ..  ..   :ll`  ".  $$  $ $$ )_EOL",
        R"_EOL(                   *  ",}nLc{}/    ` !  $    $ $$ )_EOL",
        R"_EOL(                   $  :'        $$ ]>i  $         )_EOL",
        R"_EOL(                  $   ! c$$$$ $$     ]  u         )_EOL",
        R"_EOL(                  $  .!  $   $$   $$ -,  $$       )_EOL",
        R"_EOL(                  $   ,)  $       $  c:;          )_EOL",
        R"_EOL(                  $   ^x  $           Ii"  $      )_EOL",
        R"_EOL(                   %  ,| $         $$i i $$       )_EOL",
        R"_EOL(                  $l;ut  $           q t{  $      )_EOL",
        R"_EOL(                $-     ~$$           $      $     )_EOL",
        R"_EOL(                $q$$$$$              $$$$$$$$     )_EOL",
    };
    
    std::vector<std::string> troll = {
        R"_EOL(                                   $$             )_EOL",
        R"_EOL(                              $$$$$  $$$          )_EOL",
        R"_EOL(                            $$     f>   $         )_EOL",
        R"_EOL(                      $$$$$$  Ifv,:  $ $          )_EOL",
        R"_EOL(                    $a       i    $ $             )_EOL",
        R"_EOL(                 $q$    ''' : ($ $                )_EOL",
        R"_EOL(          $$ $$$     `      :!     $              )_EOL",
        R"_EOL(      $$$$  $   Y$ |         :}\u|  $$$$          )_EOL",
        R"_EOL(     $     `  $ [  i             ._.    [$        )_EOL",
        R"_EOL( $}k    I ~  $ $ ;_'                 ':" $        )_EOL",
        R"_EOL( $b@$ $   _ $ w  ^                     ~   $$     )_EOL",
        R"_EOL(       $  ~   " `l[[l                   >i        )_EOL",
        R"_EOL(       $ ^ ~]]^l(    l              .,;,' _ $     )_EOL",
        R"_EOL(       $  ; i1?   $$ !             .    ^'`  $$   )_EOL",
        R"_EOL(        $      l$$   !             .  $  : `  W   )_EOL",
        R"_EOL(         $$${$$   $                ' @ $ l  . $   )_EOL",
        R"_EOL(                  v ^ !            .< $   . , $   )_EOL",
        R"_EOL(                  $                 u $ $ . ] $   )_EOL",
        R"_EOL(                   $                \ $ $  ./ $   )_EOL",
        R"_EOL(                  $  !              l  $ | .f $   )_EOL",
        R"_EOL(                  $ ^        ..     [      :+ $   )_EOL",
        R"_EOL(                 $  ^ ~fCCf)(}! .  I  $  $ +"  $  )_EOL",
        R"_EOL(                 r ! ~         I'  l $ |$}   +$   )_EOL",
        R"_EOL(                $  ~ ~ $$$$ $$   .~:  ($ $$$$     )_EOL",
        R"_EOL(                 $ l'~' $  $   $$  ; I  $         )_EOL",
        R"_EOL(                  } + Q $        $ $  -  $ $      )_EOL",
        R"_EOL(                  $ -IX $           $ ;`,  $      )_EOL",
        R"_EOL(                $$       $                $       )_EOL",
        R"_EOL(              $$$$$$8$$$ $         $$$$$$$        )_EOL",
    };
};

