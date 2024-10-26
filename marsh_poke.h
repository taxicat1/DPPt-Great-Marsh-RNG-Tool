#ifndef MARSH_POKE_H
#define MARSH_POKE_H

#include <stdint.h>

typedef enum marsh_poke {
	MARSH_POKE_CROAGUNK   = (1 << 0),
	MARSH_POKE_SKORUPI    = (1 << 1),
	MARSH_POKE_CARNIVINE  = (1 << 2),
	MARSH_POKE_GOLDUCK    = (1 << 3),
	MARSH_POKE_ROSELIA    = (1 << 4),
	MARSH_POKE_STARAVIA   = (1 << 5),
	MARSH_POKE_MARILL     = (1 << 6),
	MARSH_POKE_AZURILL    = (1 << 7),
	MARSH_POKE_WOOPER     = (1 << 8),
	MARSH_POKE_QUAGSIRE   = (1 << 9),
	MARSH_POKE_BIDOOF     = (1 << 10),
	MARSH_POKE_BIBAREL    = (1 << 11),
	MARSH_POKE_TOXICROAK  = (1 << 12),
	MARSH_POKE_DRAPION    = (1 << 13),
	MARSH_POKE_EXEGGCUTE  = (1 << 14),
	MARSH_POKE_YANMA      = (1 << 15),
	MARSH_POKE_SHROOMISH  = (1 << 16),
	MARSH_POKE_PARAS      = (1 << 17),
	MARSH_POKE_KANGASKHAN = (1 << 18),
	MARSH_POKE_GULPIN     = (1 << 19),
	MARSH_POKE_TROPIUS    = (1 << 20),
	MARSH_POKE_TANGELA    = (1 << 21),
	MARSH_POKE_KECLEON    = (1 << 22),
	MARSH_POKE_END        = (1 << 23)
} marsh_poke;

typedef enum game_details {
	DP_NO_NAT_DEX = 0,
	DP_NAT_DEX    = 1,
	PT_NO_NAT_DEX = 2,
	PT_NAT_DEX    = 3
} game_details;

typedef uint32_t marsh_poke_map;

marsh_poke_map groupseed_to_marsh_poke_map(uint32_t groupseed, game_details game);
void print_groupseed_marsh_pokes(uint32_t groupseed, game_details game);
unsigned int print_marsh_poke_options(game_details game);
marsh_poke get_marsh_poke_by_number(unsigned int num, game_details game);

#endif
