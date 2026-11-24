
enum class UserInput {
    NONE,
    LEFT, 
    RIGHT, 
    BOTH,
    ALL,
    PRESS,
    LONG_PRESS,
};

enum class AppState{
    STATE_BOOT,
    STATE_ERROR, //un module n'est pas detecté (deffectueux, débranché etc...)
    STATE_MENU_EGG,
    STATE_MENU_PET,
    STATE_MENU_DEAD,


    STATE_MENU_HEALTH,
    STATE_MENU_SKILLS,
    STATE_MENU_STATS_PET,
    STATE_MENU_MONEY,

    STATE_MENU_EAT,
    STATE_MENU_PLAY,
    STATE_MENU_LEARN,
    STATE_MENU_EXPERIMENT,


    STATE_STATS_HEALTH,
    STATE_STATS_SKILLS,
    STATE_STATS_PET,
    STATE_STATS_MONEY,

    STATE_ACTIVITY_EAT,
    STATE_ACTIVITY_LEARN,
    STATE_ACTIVITY_EXPERIMENT,
    STATE_ACTIVITY_PLAY,

};

enum EggState {
    NEW,
    CRACK1,
    CRACK2,
    CRACK3,
    HATCHED //Il n'y a plus d'oeuf
};

void renderUI();
void stateManager();
extern UserInput lastInput;