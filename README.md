# Cooperative Solitaire #
Goal of the game:
-
This game is played by two people working together and sometimes against each other in order to **get rid of all their cards**.  
In general, whoever gets rid of their cards first wins the game.  

From now on let us denote the two players by *A* and *B*.
- If player *A* started the game and finishes while player *B* cannot in the same round, player *A* wins and player *B* loses.  

- If player *A* started the game and finishes while player *B* can also finish in the same round , it is a tie.

**Preparation phase**:
-
- Both players get a full deck of french cards (No Jokers).
- They each need to shuffle them and distribute the following number of cards to the appropriate decks:
  - 13 cards go to their primary deck.
  - 1 card goes to each helper deck on the side (4/player).
  - Rest of cards are put to the secondary deck.

Now the table should look like this:
  
![Basic_layout_for_starters](https://github.com/user-attachments/assets/a2d2f20c-d747-4ced-b03c-a27216138348)

**Gameplay**
-
The players on their turn must check their possible moves and make the one which has the highest priority.

Placing rules:
- The main deck must be built up from ace to king from the same symbol (spades,clubs,hearts,diamonds), meaning an ace can always be placed into one of the empty main decks and if the king is placed onto the main deck, it is considered finished.
- If a helper deck is empty any card can be placed there, otherwise the card to be placed has to have a value exactly one less than the top of the helper deck and of the opposite colour(red/black).
- If a player wants to place a card onto the opponent's pile (top of opponents primary or throwaway deck), the card in question has to be of the same colour(red/black) and its value must be adjacent to the card of the opponent's deck. (eg. A spades of 7 can be placed onto the opponent's deck (primary or throwaway if the top card is black and has a value of 8 or 6)

The priority of moves are as follows:
- *1.* Fill the main decks:
  - Which card to take?
    - Take a card from the top of a helper deck
    - Take a card from the primary deck
    - Take a card from the secondary deck
- *2.* Fill the helper decks :
  - Which card to take?
    - Either take a card from the primary deck *OR* pick a helper deck that has only one card and move that card on top of the other helper deck. Then move the primary to the open place of the helper deck.
    - Take a card from the secondary deck
- *3.* (*Optional*) Fill the primary or throwaway deck of the opponent.
  - Which card to take?
    - Either take a card from the primary deck and place it on top of the valid deck of th opponent *OR* pick a helper deck that has only one card and move that card on top of the valid deck of the opponent and place the top card of the primary deck to the place of the helper deck. 
    - Take a card from the secondary deck and place it on the valid deck of the opponent.

If a player cannot perform any of these moves, they must put the top card of their secondary deck to their throwaway deck and the other player's turn starts. 
In case the throwaway deck runs out of cards and their throwaway deck still has cards, take the throwaway deck, flip it(The order of the cards must remain the same, as in the lastly thrown card has to be at the bottom of the deck) and it will be the new secondary deck of the player. 

**Logging Documentation**
-
Logger class added that takes care of log calls. In *logger.h* it is possible to change the logging symbols, that define a clear boundary between different log calls.
At the start of the game, the stats of each player is logged,
 - (overall_throw_tendency_player_1, primary_throw_tendency_player_1, throwaway_throw_tendency_player_1)
 - (overall_throw_tendency_player_2, primary_throw_tendency_player_2, throwaway_throw_tendency_player_2)
   
Then the starting player index:
 - starting player index
After that the *state of the game* is given in the following format:
 - log_table_symbol
 - (Player_1_primary_top_card,Player_1_secondary_top_card,Player_1_throwaway_top_card)
 - (Player_2_primary_top_card,Player_2_secondary_top_card,Player_2_throwaway_top_card)
 - Main Decks
 - Helper decks top cards
 - log_table_symbol

Then the game start and the starter player starts their moves. The *log_current_player_rounds* symbol marks the start and end of a player's round. The current player index is also logged after the start symbol, divided by a comma.
 - *log_current_player_rounds*, current_player_index
 - snapshots of game in the format of *state of the game*, described above
 - *log_current_player_rounds*
Then the next player's moves are logged in the same way, marked by the same format except that the current_player_index is obviously, different.

On the last line of the logging file, the result of the game is logged. 
