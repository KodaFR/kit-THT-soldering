
enum class UserInput {
    LEFT, 
    RIGHT, 
    PRESS, 
};

enum class AppState{
    STATE_BOOT,
    STATE_ERROR, //un module n'est pas detecté (deffectueux, débranché etc...)
    STATE_MENU_OEUF,
    STATE_MENU_OK,
    STATE_MENU_DEAD,

    STATE_STATS_HEALTH,
    STATE_STATS_SKILLS,
    STATE_STATS_PET,
    STATE_STATS_MONEY,

    STATE_LOADING_EAT,
    STATE_LOADING_PLAY,
    STATE_LOADING_LEARN,
    STATE_LOADING_EXPERIMENT,

    STATE_ACTIVITY_EAT,
    STATE_ACTIVITY_LEARN,
    STATE_ACTIVITY_EXPERIMENT,
    STATE_ACTIVITY_PLAY,

};

void renderUI();
extern UserInput lastInput;