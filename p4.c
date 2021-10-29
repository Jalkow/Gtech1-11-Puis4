#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define num_line 6
#define num_col 100
int tabl[num_line][num_col];
const int numb_of_digits_max = 3;
const char displays[] = ".oxH";

/*Init function to setup the puissance4 table*/
void init_tabl(void){
  int i, j;
  for(i=0; i<num_line; i++){
    for(j=0; j<num_col; j++){
      tabl[i][j] = 0;
    }
  }
}

void affichage_vertical(int numb){
    const int numb_of_line = log10(numb) + 1;
    int empty_space_number = 0;
    int number_until_new_digit;
    int i, j;
    
    int digits_array[numb_of_digits_max];
    int actual_number;
    int actual_digit;
    int digit_to_print;
    int compteur;
    int compteur_nombres;
    
    for(i=0; i<numb_of_line; i++){
        printf("\n");
        number_until_new_digit = 9;
        compteur_nombres = 0;
        digit_to_print = 0;
        for(compteur = 0; compteur < i; compteur++){
            number_until_new_digit = number_until_new_digit * 10;
        }

        for(j=0; j<(empty_space_number); j++){
            printf("  ");
        }
        


        for(j=empty_space_number; j<numb; j++){
            actual_number = j+1;
            compteur = 0;

            while (actual_number != 0){
                actual_digit = actual_number % 10;

                digits_array[compteur] = actual_digit;
                compteur++;

                actual_number = actual_number / 10;
            }

            printf("%d ", digits_array[digit_to_print]);
            compteur_nombres++;

            if (compteur_nombres % number_until_new_digit == 0){
                digit_to_print++;
                number_until_new_digit = number_until_new_digit * 10;
                compteur_nombres = 0;
            }
            
        }
        if (empty_space_number == 0){
            empty_space_number = 9;
        }
        else{
            empty_space_number = empty_space_number * 10 + 9;
        }

        
    }
}


/*Displays the table*/
void print_tabl(void){
  int i, j;
  for (i=0; i<num_col*2; i++){
    printf("-");
  }
  for(i=0; i<num_line; i++){
      printf("\n");                                                                                                       
      for(j=0; j<num_col; j++){                                                                                             
        printf("%c ", displays[tabl[i][j]]);
    }
  }
  printf("\n");
  for (i=0; i<num_col*2; i++){
    printf("-");
  }
  affichage_vertical(num_col);
  printf("\n");
}

/* search the lowest line where a token can be put in the given column and returns it's number*/
int search_lowest_available(int column){
  int line;
  for (line = 0; line<num_line; line++) {
    if (tabl[line][column] != 0){
      return line-1;
    }
  }
  return num_line-1;
}

/*empties the scan buffer, mostly used when an incorrect value is written by the player*/
void buffer_drain (void){
  int c = getchar();
  while (c != '\n' && c != EOF){
    c = getchar();
  }
}

/*ask the column to play*/
int ask_column(int player){
  int col_chosen;
  bool correct_input;
  bool skip_next_test = false;
  printf("Player %d (Symbol %c), please enter a column number between 1 and %d : ", player, displays[player], num_col);
  do{
    int verif_scan = scanf("%d", &col_chosen);
    correct_input = true;
    if (verif_scan == 0){
      buffer_drain();
      printf("You didn't write a number, please try again : ");
      correct_input = false;
    }
    
    else {
      if(col_chosen < 1 || col_chosen > num_col){
        printf("The value you have written isn't a number between 1 and %d, please retry : ", num_col);
        correct_input = false;
        skip_next_test = true;
      }
    }
    
      if(search_lowest_available(col_chosen-1) <= -1 && skip_next_test == false){
        printf("The column you have chosen is full, please try another one : ");
        correct_input = false;
        skip_next_test = false;
      }
  }
    while (correct_input == false);
  
  return col_chosen-1;
}


/*puts a token in the given column at the lowest line possible*/
void place_token(int player, int column){
  int line_to_place_on = search_lowest_available(column);
  tabl[line_to_place_on][column] = player;
}
/*counts the number of successive token in the direction given starting from the position given*/
int numb_successive_token(int player, int start_line, int start_col, int direc_line, int direc_col){
  int numb_tokens = 0;
  int actual_line = start_line + direc_line;
  int actual_col = start_col + direc_col;

  while (actual_line >= 0 && actual_line < num_line && actual_col >= 0 && actual_col < num_col){

    if(tabl[actual_line][actual_col] != player){
      return numb_tokens;
    }
    numb_tokens++;
    actual_line = actual_line + direc_line;
    actual_col = actual_col + direc_col;
  }
  return numb_tokens;
}

/*tests if there is a 4-token alignement from a certain position*/
bool victory_test(int player, int line, int col){
  if(numb_successive_token(player, line, col, 1, 0) + numb_successive_token(player, line, col, -1, 0) >= 3 /* veritcal axis*/
  || numb_successive_token(player, line, col, 0, 1) + numb_successive_token(player, line, col, 0, -1) >= 3 /* horizontal axis*/
  || numb_successive_token(player, line, col, -1, 1) + numb_successive_token(player, line, col, 1, -1) >= 3 /*diagonal from bottom left to top right*/
  || numb_successive_token(player, line, col, -1, -1) + numb_successive_token(player, line, col, 1, 1) >= 3) /* diagonal from top left to bottom right*/ 
  {
    return true;
  }
  return false;
}

/*switches between the different players*/
int change_player(int player){
  return player == 1 ? 2 : 1;
}

bool player_turn(int tokens_placed, int actual_player){
  print_tabl();
  int actual_turn = tokens_placed/2 +1;
  printf("\nTURN NUMBER %d\n", actual_turn);
  int col_chosen = ask_column(actual_player);
  int line_token_placed = search_lowest_available(col_chosen);
  place_token(actual_player, col_chosen);
  if (victory_test(actual_player, line_token_placed, col_chosen)){
    print_tabl();
    printf("Player %d won in %d turns, congratulations !! \n", actual_player, actual_turn);
    return true;
  }
  else {
    return false;
  }
}


/* plays a game of puissance 4 with multiple human players */
void player_versus_player(void){
  int max_tokens = num_col * num_line;
  int tokens_placed = 0;
  int actual_player = 1;
  bool victory = false;
  init_tabl();
  printf("Welcome to Puissance4 (Player versus Player)!  \nGame size is %dx%d\n", num_line, num_col);

  while(victory == false && tokens_placed < max_tokens){
    print_tabl();
    int actual_turn = tokens_placed/2 +1;
    printf("\nTURN NUMBER %d\n", actual_turn);
    int col_chosen = ask_column(actual_player);
    int line_token_placed = search_lowest_available(col_chosen);
    place_token(actual_player, col_chosen);
    if (victory_test(actual_player, line_token_placed, col_chosen)){
      print_tabl();
      printf("Player %d won in %d turns, congratulations !! \n", actual_player, actual_turn);
      victory = true;
    }

    tokens_placed++;
    actual_player = change_player(actual_player);
  }
  if (victory == false){
    printf("Draw : no one managed to align 4 tokens !\n");
  }
}

bool bot_turn(int bot_number){
  int i;
}

/* plays a game of puissance 4 with 1 human player against bots */
void player_versus_ia(int numb_of_bot){
  int max_tokens = num_col * num_line;
  int tokens_placed = 0;
  int actual_player_type = 1;
  bool victory = false;
  init_tabl();
  printf("Welcome to Puissance4 (Player versus IA)!  \nGame size is %dx%d\n", num_line, num_col);

  while(victory == false && tokens_placed < max_tokens){
    if (actual_player_type == 1){
      victory = player_turn(tokens_placed, actual_player_type);
      tokens_placed++;
      actual_player_type = change_player(actual_player_type);
    }
    else {
      int bot_number;
      for(bot_number=2; bot_number<numb_of_bot+2; bot_number++) /* starting from 2 so that the bots tokens displays aren't the same as the player's which is displays[1]*/  
      {
        bot_turn(bot_number);
        tokens_placed++;
        
      actual_player_type = change_player(actual_player_type);
      }
      
    }
  }
  if (victory == false){
    printf("Draw : no one managed to align 4 tokens !\n");
  }
}


/*Main founction of the game*/
int main(void){
  int gamemode_chosen;
  bool correct_input; 
  printf("Welcome to the puissance 4 game, please write the number of the mode you want to play \n(write 1 for Player 1v1, 2 for Player vs IAs or 3 to exit) : ");
  do{
    int verif_scan = scanf("%d", &gamemode_chosen);
    correct_input = true;
    if (verif_scan == 0){
      buffer_drain();
      printf("You didn't write a number, please try again : ");
      correct_input = false;
    }
    else
      if (gamemode_chosen < 1 || gamemode_chosen > 3){
      printf("Please write a correct number, please try again \n(1 for Player 1v1, 2 for Player vs IAs or 3 to exit) : ");
      correct_input = false;
    }
  }
    while(correct_input == false);
  if (gamemode_chosen == 1){
    player_versus_player();
  }
  
  else if (gamemode_chosen == 2){
    int numb_of_bots;
    printf("\nPlease insert the number of bot you want to play against (from 1 to 3)");
    scanf("%d", numb_of_bots);
    player_versus_ia(numb_of_bots);
  }
  
  else if(gamemode_chosen == 3){
    printf("\nGoodbye !\n\n");
    return 0;
  }
}
