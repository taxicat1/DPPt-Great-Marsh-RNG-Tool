#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include "marsh_poke.h"
#include "mersenne_twister.h"


static uint32_t arng(uint32_t x) {
	return (x * 1812433253) + 1;
}

static uint16_t lotto_number(uint32_t groupseed) {
	return ((groupseed * 1103515245) + 12345) >> 16;
}

typedef struct {
	uint32_t      seed;
	int           vsync;
	int           frame;
	uint32_t      groupseed;
	uint16_t      lotto;
	game_details  game;
} seed_details; 

static void print_seed_search_result(const seed_details* details) {
	printf("Seed: %08x\n", details->seed);
	printf("Delay: %u\n", details->vsync);
	printf("Frame: %u ", details->frame+1);
	
	int double_taps = (details->frame / 12) - 1;
	if (double_taps < 0) {
		double_taps = 0;
	}
	
	int coin_flips = details->frame - (double_taps * 12);
	
	switch (double_taps) {
		case 0:
			printf("(%u coin flips; do not open Happiness Checker)\n", coin_flips);
			break;
		
		case 1:
			printf("(%u coin flips; open Happiness Checker but do not double tap)\n", coin_flips);
			break;
		
		case 2:
			printf("(%u coin flips; 1 Happiness Checker double tap)\n", coin_flips);
			break;
		
		default:
			printf("(%u coin flips; %u Happiness Checker double taps)\n", coin_flips, double_taps-1);
			break;
	}
	
	printf("\n");
	printf("Lotto number: %u\n", details->lotto);
	print_groupseed_marsh_pokes(details->groupseed, details->game);
}

static void find_good_seed(int max_advances,
                           int min_delay, int max_delay,
                           game_details game,
                           uint16_t tid,
                           marsh_poke_map target_pokes) {
	
	int backup_found = 0;
	seed_details backup_seed;
	
	int min_advances = 10;
	
	for (uint32_t month_day_minute_second = 0x01000000; month_day_minute_second != 0x00000000; month_day_minute_second += 0x01000000) {
		for (uint32_t hour = 0x00000000; hour < (24*0x00010000); hour += 0x00010000) {
			for (uint32_t vsync = min_delay; vsync < max_delay+99; vsync++) {
				
				uint32_t seed = month_day_minute_second + hour + vsync;
				mt_srand(seed);
				
				for (int frame = min_advances; frame < min_advances+max_advances+1; frame++) {
					uint32_t mt_out = mt_read(frame);
					uint32_t groupseed = arng(arng(mt_out));
					
					// Lotto number check
					uint16_t lotto = lotto_number(groupseed);
					if (lotto != tid && backup_found) {
						continue;
					}
					
					// Marsh pokes check
					marsh_poke_map marsh_pokes = groupseed_to_marsh_poke_map(groupseed, game);
					if ((marsh_pokes & target_pokes) != target_pokes) {
						continue;
					}
					
					// Save this seed if checks pass
					seed_details current_seed = (seed_details){
						.seed      = seed,
						.vsync     = vsync,
						.frame     = frame,
						.groupseed = groupseed,
						.lotto     = lotto,
						.game      = game
					};
					
					// Check is this is a backup or a match
					if (lotto == tid) {
						printf("Found!\n\n");
						print_seed_search_result(&current_seed);
						return;
					} else {
						backup_seed = current_seed;
						backup_found = 1;
					}
				}
				
			}
		}
	}
	
	printf("Not found! Try increasing the delay or frames, a different TID, or change your target Pokes\n");
	if (backup_found) {
		printf("Here is a seed which has the requested Great Marsh Pokes, but does not match the Lotto:\n\n");
		print_seed_search_result(&backup_seed);
	}
	
	return;
}


static int read_input_int(const char* prompt, int min, int max) {
	int result;
	do {
		printf("%s ", prompt);
		errno = 0;
		
		//char* line = NULL;
		//size_t line_len = 0;
		//getline(&line, &line_len, stdin);
		//result = strtol(line, NULL, 10);
		//free(line);
		
		// getline() alternative:
		// - 16 chars is more than enough to read a max sized int (strtol() is undefined for so large anyway)
		// - After reading string, check if the last char is a line break
		// - If it is not, then we need to keep reading to clear the pending input until we find a line break
		char intbuf[16];
		if (fgets(intbuf, 16, stdin) == NULL) {
			fprintf(stderr, "Input error!\n");
			exit(1);
		}
		
		int len = strlen(intbuf);
		if (len > 0 && intbuf[len-1] != '\n') {
			int end;
			do {
				end = fgetc(stdin);
			} while (end != EOF && end != '\n');
		}
		
		result = strtol(intbuf, NULL, 10);
	} while (result < min || result > max || errno != 0);
	
	return result;
}

int main(void) {
	printf("Which describes your game and progress?\n");
	printf("1) D/P, pre-National Dex\n");
	printf("2) D/P, post-National Dex\n");
	printf("3) Pt, pre-National Dex\n");
	printf("4) Pt, post-National Dex\n");
	printf("\n");
	
	int game_input = read_input_int("Enter the number (1-4):", 1, 4);
	game_details game = (game_details)(game_input - 1);
	
	printf("\n");
	printf("These Pokes are available in the Great Marsh as dailies:\n");
	int num_options = print_marsh_poke_options(game);
	printf("\n");
	
	int num_manip_input = read_input_int("How many different Pokes do you want to manip? (1-6):", 1, 6);
	
	marsh_poke_map target_pokes = 0;
	for (int i = 0; i < num_manip_input; i++) {
		char prompt[99];
		snprintf(prompt, 99, "Enter the number of target Poke #%i (%i-%i):", i+1, 1, num_options);
		
		int option_input = read_input_int(prompt, 1, num_options);
		
		marsh_poke target_poke = get_marsh_poke_by_number(option_input, game);
		target_pokes |= target_poke;
	}
	
	printf("\n");
	
	int tid = read_input_int("Enter your trainer ID (or other target Lotto number):", 0, 65535);
	
	printf("\n");
	
	int min_delay = read_input_int("Enter the min delay to search:", 1, 65535);
	int max_delay = read_input_int("Enter the max delay to search:", min_delay, 65535);
	int max_advances = read_input_int("Enter the max number of extra RNG frames (coin flips/double taps) to search (0-613):", 0, 613);
	
	printf("\n");
	printf("Searching...\n");
	find_good_seed(max_advances, min_delay, max_delay, game, tid, target_pokes);
	
	printf("\n\n");
	printf("Press Enter to exit\n");
	getchar();
	
	return 0;
}
