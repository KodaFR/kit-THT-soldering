#pragma once

#include "kodalogo.h"
#include "egg1.h"
#include "egg2.h"
#include "egg3.h"
#include "egg4.h"
#include "experiment.h"
#include "food.h"
#include "health.h"
#include "learn.h"
#include "money.h"
#include "pet.h"
#include "play.h"
#include "skills.h"
#include "stats.h"
#include "activity_experiment.h"
#include "activity_learn.h"
#include "dead_pet.h"
#include "need_food.h"
#include "need_interrogation.h"
#include "need_play.h"
#include "play_both.h"
#include "play_left.h"
#include "play_right.h"

// Array of all bitmaps for convenience.
const int epd_bitmap_allArray_LEN = 23;
const unsigned char* epd_bitmap_allArray[23] = {
	logo_bitmap,
	egg1_bitmap,
	egg2_bitmap,
	egg3_bitmap,
	egg4_bitmap,
	experiment_menu_bitmap,
	food_menu_bitmap,
	health_menu_bitmap,
	learn_menu_bitmap,
	money_menu_bitmap,
	pet_menu_bitmap,
	play_menu_bitmap,
	skills_menu_bitmap,
	stats_menu_bitmap,
	activity_experiment_bitmap,
	activity_learn_bitmap,
	dead_pet_bitmap,
	need_food_bitmap,
	need_interrogation_bitmap,
	need_play_bitmap,
	play_both_bitmap,
	play_left_bitmap,
	play_right_bitmap,
};
