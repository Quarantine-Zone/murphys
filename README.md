# Initial Setup

Once you've pulled the repo, do the following.
1) Rebuild lighting (Build -> Build Lighting Only)
2) Add starter content (Add New -> Add Feature or Content Pack -> Content Packs -> Starter Content)
3) Add free "Modern City Downtown with Interiors Megapack (Urban Building/ Buildings)" pack from Unreal Engine Marketplace. 
4) Re-generate C++ solution (File -> Generate Visual Studio Project)


# Multiplayer

# Using Frameworks

## Minigame

To add a Minigame:

1) Create a new folder for the game in the minigames folder. (you can organize the content inside however you want)
2) Create a new level for the minigame
3) Create a userwidget blueprint for the game's arcade menu
4) Open the widget and do the following:
    - Go to the Class Details tab and set the parent class to ArcadeMenuBase
    - Add the blueprint node "event On Initialized"
    - From the new node add a "Set Level Path" node and asign it the value of the path to the new level for the minigame
    - Name the button that will launch the minigame "PlayButton" and name the button that will close the menu "CloseMenuButton". The ArcadeMenuBase class will bind these automatically to the correct functions.
5) Place a new instance of the Arcade Machine Blueprint into the lobby.
    - set the Menu Name variable on the arcade machine to the name of the menu widget for the minigame.
6) Inherrit your gamemode from MiniGameModeBase


## Scoreboard


The following explains the general steps to use the scoreboard class in your minigame. Currently, text color and size is not modifiable.


1) Create a new Blueprint for a User Widget (or copy an existing one), and change the base class to Scoreboard.
2) In the blueprint, during the Construct Event, call the following:
    - SetUseCharacter: Do you want to use a character (check) or pawn (unchecked)
    - AddColumn: Add a named column to the scoreboard 
    - Set ScoreboardWidth (default = 900). This should be the maximum size for a row.
    - Init: This will finally create the scoreboard based off the columns and players.
3) In the blueprint, override the GetColumnData function.
    - This function will pass either a MurphysBaseCharacter or MurphysBasePawn, depending on which you set, and an array of the column headers.
      - The character/pawn can be cast to an inherited one (for minigame specific functions)
    - You should loop through the column headers, and construct a new string array.
      - This new string array should be the row values (in order) as they appeared in the column array. Example: Columns: [Name, Ping] => Return array = [crazyscouter, 100]
      - I recommend doing a switch of all possible row values you added, and then calling functions on your character (such as Get Player Name or Get Player Ping - custom current ones).
    - The new row value array should be returned. 
4) Bind to the InputAction Scoreboard in your character, construct your new blueprint and add it to the viewport. Be sure to set a variable with the object reference so it can be removed on key release.     

**Note**: When designing the UI, at minimum the following elements are required:
    - HeadersContainer<Horizontal Box>: Where the headers will be placed
    - RowContainer<Scroll Box>: Where the created rows will be placed
