#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int num_line;
int num_col;
int** tabl;
const int numb_of_digits_max = 3;
const char displays[] = ".oxH";

/*Init function to setup the puissance4 table*/
void init_tabl(int num_line, int num_col){
  int i, j;
  tabl = malloc(num_line* sizeof(int*));
  for (i=0; i<num_line; i++){
    tabl[i] = malloc(num_col * sizeof(int*));
  }

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


/*puts a token in the given column at the lowest line possible, which is returned*/
int place_token(int player, int column){
  int line_to_place_on = search_lowest_available(column);
  tabl[line_to_place_on][column] = player;
  return line_to_place_on;
}
/*counts the number of successive token in the direction given starting from the position given*/
int numb_successive_token_in_direction(int player, int start_line, int start_col, int direc_line, int direc_col){
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

/* for each axis (vertical, horizontal, diagonals), verifies the number of tokens aligned with the token of given coords and returns the highest value */
int numb_tokens_aligned(int player, int line, int col){
  int array_number_tokens_aligned[4];
  int numb_max_of_alignment = 0;
  int i;

  array_number_tokens_aligned[0] = numb_successive_token_in_direction(player, line, col, 1, 0) + numb_successive_token_in_direction(player, line, col, -1, 0); /* veritcal axis*/
  array_number_tokens_aligned[1] = numb_successive_token_in_direction(player, line, col, 0, 1) + numb_successive_token_in_direction(player, line, col, 0, -1); /* horizontal axis*/
  array_number_tokens_aligned[2] = numb_successive_token_in_direction(player, line, col, -1, 1) + numb_successive_token_in_direction(player, line, col, 1, -1); /*diagonal from bottom left to top right*/
  array_number_tokens_aligned[3] = numb_successive_token_in_direction(player, line, col, -1, -1) + numb_successive_token_in_direction(player, line, col, 1, 1); /* diagonal from top left to bottom right*/

  for (i=0; i<4; i++){
    if (array_number_tokens_aligned[i] > numb_max_of_alignment){
      numb_max_of_alignment = array_number_tokens_aligned[i];
    }
  }

  return numb_max_of_alignment;
}

/*tests if there is a 4-token alignement from a certain position*/
bool victory_test(int player, int line, int col){
  if(numb_tokens_aligned(player, line, col) >= 3){
    return true;
  }
  else{
    return false;
  }
  
}

/* tests if placing a token in a certain column will lead to losing next turn and returns the answer as a bool*/
bool move_leads_to_lose(int player, int line, int col){
  return (victory_test(player, line-1, col));
}

/*switches between the different players*/
void change_player(int* player){
  *player = *player == 1 ? 2 : 1;
}

bool player_turn(int tokens_placed, int actual_player){
  print_tabl();
  int actual_turn = tokens_placed/2 +1;
  printf("\nTURN NUMBER %d\n", actual_turn);
  int col_chosen = ask_column(actual_player);
  int line_token_placed = place_token(actual_player, col_chosen);
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
void player_versus_player(int num_line, int num_col){
  int max_tokens = num_col * num_line;
  int tokens_placed = 0;
  int actual_player = 1;
  bool victory = false;
  init_tabl(num_line, num_col);
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
    change_player(&actual_player);
  }
  if (victory == false){
    printf("Draw : no one managed to align 4 tokens !\n");
  }
}

/* makes the bot play one turn */
bool bot_turn(){
  int i;
  int max_numb_tokens_aligned = 0;
  int col_max_token_aligned;
  int line_max_token_aligned;
  int line;
  bool already_played = false;
  bool victory = false;
  for(i=0; i<num_col; i++){
    line = search_lowest_available(i);
    if(victory_test(2, line,i)==true && search_lowest_available(i) != -1 && already_played == false){
      place_token(2,i);
      already_played=true;
      victory = true;
    }
    if(victory_test(1, line,i)==true && search_lowest_available(i) != -1 && already_played == false){
      place_token(2,i);
      already_played=true;
    }
    if (numb_tokens_aligned(2, line, i) > max_numb_tokens_aligned){
      col_max_token_aligned = i;
      line_max_token_aligned = line;
      max_numb_tokens_aligned = numb_tokens_aligned(2, line, i);
    }
  }

  if (max_numb_tokens_aligned != 0 && move_leads_to_lose(1, line_max_token_aligned, col_max_token_aligned) != true && already_played == false && search_lowest_available(col_max_token_aligned) != -1){
    place_token(2, col_max_token_aligned);
    already_played = true;
  }

  if(already_played == false){
    srand(time(NULL));
    int random_col;
    int random_line;
    bool good_placement;
    do{
      good_placement = false;
      random_col = rand() % 7;
      random_line = search_lowest_available(random_col);
      if (move_leads_to_lose(1, random_line, random_col) == false && random_line != -1){
        good_placement = true;
        place_token(2, random_col);
      }
    } while (good_placement == false); 
  }

  return victory;
}

/* plays a game of puissance 4 with 1 human player against a bot*/
void player_versus_ia(int num_line, int num_col){
  int max_tokens = num_col * num_line;
  int tokens_placed = 0;
  int actual_turn;
  int actual_player_type = 1;
  bool victory = false;
  init_tabl(num_line, num_col);
  printf("Welcome to Puissance4 (Player versus IA)!  \nGame size is %dx%d\n", num_line, num_col);

  while(victory == false && tokens_placed < max_tokens){
    actual_turn = tokens_placed/2 +1;
    if (actual_player_type == 1){
      victory = player_turn(tokens_placed, actual_player_type);
      tokens_placed++;
      change_player(&actual_player_type);
    }
    else {
      victory = bot_turn();
      tokens_placed++;
      if (victory == true){
        print_tabl();
        printf("The bot won in %d turns !! \n", actual_turn);
      }
      
      change_player(&actual_player_type);
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
  int verif_scan;
  int max_num_column = 100;

  printf("Welcome to the puissance 4 game !\n\nPlease select the mode you want to play :\n 1 for basic Player 1v1, \n 2 for a Player 1v1 with custom number of lines/columns, \n 3 for Player vs IA \n 4 to exit\n");
  do{
    printf("Enter you choice : ");
    verif_scan = scanf("%d", &gamemode_chosen);
    correct_input = true;
    if (verif_scan == 0){
      buffer_drain();
      printf("\nYou didn't enter a number, please try again !\n");
      correct_input = false;
    }
    else
      if (gamemode_chosen < 1 || gamemode_chosen > 4){
      printf("\nPlease enter a correct number\n");
      correct_input = false;
    }
  }
    while(correct_input == false);
  if (gamemode_chosen == 1){
    num_line = 6;
    num_col = 7;
    player_versus_player(num_line, num_col);
  }
  
  else if (gamemode_chosen == 2){
    printf("\nPlease enter the number of line you want : ");
    do {
      verif_scan = scanf("%d", &num_line);
      correct_input = true;
      if (verif_scan == 0){
        buffer_drain();
        printf("\nYou didn't enter a number, please try again : ");
        correct_input = false;
      }

    } while (correct_input == false);

    printf("\nPlease enter the number of column you want (max 100) : ");
    do {
      verif_scan = scanf("%d", &num_col);
      correct_input = true;
      if (verif_scan == 0){
        buffer_drain();
        printf("\nYou didn't enter a number, please try again : ");
        correct_input = false;
      }

      else 
        if(num_col < 1 || num_col > max_num_column){
          printf("\n You didn't enter a number between 1 and %d, please try again : ", max_num_column);
          correct_input = false;
        }
    } while (correct_input == false);

    player_versus_player(num_line, num_col);

  }
  

  else if (gamemode_chosen == 3){
    num_line = 6;
    num_col = 7;
    player_versus_ia(num_line, num_col);
  }
  

  else if(gamemode_chosen == 4){
    printf("\nGoodbye !\n\n");
    return 0;
  }
}
