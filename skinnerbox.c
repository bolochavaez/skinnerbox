#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>


#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BOLD    "\033[1m"
#define RESET   "\033[0m"
#define BLINK	"\033[5m"

typedef struct {
int rolls;
int points;
int nh_range;
int hit_range;
int temp_hit_range;
int temp_nh_range;
int nh_bonus;

} roller;

void upgrade_menu(roller * p_state){
	char input = ' ';
	printf("upgrades: 1. multi roll 2. hit bonus 3. near hit range   4. near hit range grower");
	input = getchar();
	if (input == '1' && p_state->points >= 100 ){
	p_state->rolls += 1;
	p_state->points -= 100;
	} else if (input == '2' && p_state->points >= 100 ){
		p_state->hit_range += 1;
		p_state->points -= 100;
	} else if (input == '3' && p_state->points >= 10 ){
		p_state->nh_range += 1;
		p_state->points -= 10;
	} else if (input == '4' && p_state->points >= 300 ){
		p_state->nh_bonus = 1;
		p_state->points -= 300;
	}


}

int main(){
	srand(time(NULL));
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	char input = ' ';
	int number;
	int target = 10;
	roller p_state = {1, 500, 5, 0,0,0,0};
	while(input != 'q'){
		printf("points %d target %d \n", p_state.points, target);
		input = getchar();

		if(input == 'u') upgrade_menu(&p_state);
		else {
			p_state.points += 1;
			for (int i =0; i < p_state.rolls; i++) {
				number = ((rand() % target) + p_state.hit_range + p_state.temp_hit_range) % target;
				if (number == 0 ){ 
					printf(BOLD GREEN BLINK "HIT!: %d"RESET "\n", target ); 
					sleep(1);
					target = target * 2;
					p_state.temp_hit_range = 0;
					p_state.temp_nh_range = 0;
				}
				else if(number >= target - (p_state.nh_range + p_state.temp_nh_range)) {
					printf( YELLOW "near hit: %d"RESET "\n", number);
					if(p_state.nh_bonus) p_state.temp_nh_range *= 2;
				} else printf( RED "miss: %d"RESET "\n", number);
				
			}
		}
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
