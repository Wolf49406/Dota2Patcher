#pragma once

enum class DOTA_GAME_STATE {
    INVALID = -1,
    INIT,
    WAIT_FOR_PLAYERS_TO_LOAD,
    HERO_SELECTION,
    STRATEGY_TIME,
    PRE_GAME,
    GAME_IN_PROGRESS,
    POST_GAME,
    DISCONNECT,
    TEAM_SHOWCASE,
    CUSTOM_GAME_SETUP,
    WAIT_FOR_MAP_TO_LOAD,
    SCENARIO_SETUP,
    PLAYER_DRAFT,
    LAST
};

enum class ENTITY_NAME_TYPE {
    INTERNAL_NAME,
    ENTITY_NAME,
    BINARY_NAME,
    CLASS_NAME
};

enum class PATCH_TYPE {
    JE = 0x74,
    JNE = 0x75,
    TEST = 0x85,
    JMP = 0xEB,
    CUSTOM = 0x0
};

enum class ASM_TYPE {
    LEA, // 3, 7
    CALL // 2, 6
};

enum class DOTA_WEATHER {
    WEATHER_DEFAULT,
    WEATHER_SNOW,
    WEATHER_RAIN,
    WEATHER_MOONBEAM,
    WEATHER_PESTILENCE,
    WEATHER_HARVEST,
    WEATHER_SIROCCO,
    WEATHER_SPRING,
    WEATHER_ASH,
    WEATHER_AURORA
};

enum class DOTA_RIVER {
    RIVER_DEFAULT,
    RIVER_OIL,
    RIVER_DRY,
    RIVER_SLIME,
    RIVER_CHROME,
    RIVER_ELECTRIC,
    RIVER_POTION,
    RIVER_BLOOD
};

enum class DOTA_HERO_ID {
    npc_dota_hero_antimage = 1,
    npc_dota_hero_axe = 2,
    npc_dota_hero_bane = 3,
    npc_dota_hero_bloodseeker = 4,
    npc_dota_hero_crystal_maiden = 5,
    npc_dota_hero_drow_ranger = 6,
    npc_dota_hero_earthshaker = 7,
    npc_dota_hero_juggernaut = 8,
    npc_dota_hero_mirana = 9,
    npc_dota_hero_morphling = 10,
    npc_dota_hero_nevermore = 11,
    npc_dota_hero_phantom_lancer = 12,
    npc_dota_hero_puck = 13,
    npc_dota_hero_pudge = 14,
    npc_dota_hero_razor = 15,
    npc_dota_hero_sand_king = 16,
    npc_dota_hero_storm_spirit = 17,
    npc_dota_hero_sven = 18,
    npc_dota_hero_tiny = 19,
    npc_dota_hero_vengefulspirit = 20,
    npc_dota_hero_windrunner = 21,
    npc_dota_hero_zuus = 22,
    npc_dota_hero_kunkka = 23,
    npc_dota_hero_lina = 25,
    npc_dota_hero_lion = 26,
    npc_dota_hero_shadow_shaman = 27,
    npc_dota_hero_slardar = 28,
    npc_dota_hero_tidehunter = 29,
    npc_dota_hero_witch_doctor = 30,
    npc_dota_hero_lich = 31,
    npc_dota_hero_riki = 32,
    npc_dota_hero_enigma = 33,
    npc_dota_hero_tinker = 34,
    npc_dota_hero_sniper = 35,
    npc_dota_hero_necrolyte = 36,
    npc_dota_hero_warlock = 37,
    npc_dota_hero_beastmaster = 38,
    npc_dota_hero_queenofpain = 39,
    npc_dota_hero_venomancer = 40,
    npc_dota_hero_faceless_void = 41,
    npc_dota_hero_skeleton_king = 42,
    npc_dota_hero_death_prophet = 43,
    npc_dota_hero_phantom_assassin = 44,
    npc_dota_hero_pugna = 45,
    npc_dota_hero_templar_assassin = 46,
    npc_dota_hero_viper = 47,
    npc_dota_hero_luna = 48,
    npc_dota_hero_dragon_knight = 49,
    npc_dota_hero_dazzle = 50,
    npc_dota_hero_rattletrap = 51,
    npc_dota_hero_leshrac = 52,
    npc_dota_hero_furion = 53,
    npc_dota_hero_life_stealer = 54,
    npc_dota_hero_dark_seer = 55,
    npc_dota_hero_clinkz = 56,
    npc_dota_hero_omniknight = 57,
    npc_dota_hero_enchantress = 58,
    npc_dota_hero_huskar = 59,
    npc_dota_hero_night_stalker = 60,
    npc_dota_hero_broodmother = 61,
    npc_dota_hero_bounty_hunter = 62,
    npc_dota_hero_weaver = 63,
    npc_dota_hero_jakiro = 64,
    npc_dota_hero_batrider = 65,
    npc_dota_hero_chen = 66,
    npc_dota_hero_spectre = 67,
    npc_dota_hero_ancient_apparition = 68,
    npc_dota_hero_doom_bringer = 69,
    npc_dota_hero_ursa = 70,
    npc_dota_hero_spirit_breaker = 71,
    npc_dota_hero_gyrocopter = 72,
    npc_dota_hero_alchemist = 73,
    npc_dota_hero_invoker = 74,
    npc_dota_hero_silencer = 75,
    npc_dota_hero_obsidian_destroyer = 76,
    npc_dota_hero_lycan = 77,
    npc_dota_hero_brewmaster = 78,
    npc_dota_hero_shadow_demon = 79,
    npc_dota_hero_lone_druid = 80,
    npc_dota_hero_chaos_knight = 81,
    npc_dota_hero_meepo = 82,
    npc_dota_hero_treant = 83,
    npc_dota_hero_ogre_magi = 84,
    npc_dota_hero_undying = 85,
    npc_dota_hero_rubick = 86,
    npc_dota_hero_disruptor = 87,
    npc_dota_hero_nyx_assassin = 88,
    npc_dota_hero_naga_siren = 89,
    npc_dota_hero_keeper_of_the_light = 90,
    npc_dota_hero_wisp = 91,
    npc_dota_hero_visage = 92,
    npc_dota_hero_slark = 93,
    npc_dota_hero_medusa = 94,
    npc_dota_hero_troll_warlord = 95,
    npc_dota_hero_centaur = 96,
    npc_dota_hero_magnataur = 97,
    npc_dota_hero_shredder = 98,
    npc_dota_hero_bristleback = 99,
    npc_dota_hero_tusk = 100,
    npc_dota_hero_skywrath_mage = 101,
    npc_dota_hero_abaddon = 102,
    npc_dota_hero_elder_titan = 103,
    npc_dota_hero_legion_commander = 104,
    npc_dota_hero_techies = 105,
    npc_dota_hero_ember_spirit = 106,
    npc_dota_hero_earth_spirit = 107,
    npc_dota_hero_abyssal_underlord = 108,
    npc_dota_hero_terrorblade = 109,
    npc_dota_hero_phoenix = 110,
    npc_dota_hero_oracle = 111,
    npc_dota_hero_winter_wyvern = 112,
    npc_dota_hero_arc_warden = 113,
    npc_dota_hero_monkey_king = 114,
    npc_dota_hero_pangolier = 120,
    npc_dota_hero_grimstroke = 121,
    npc_dota_hero_hoodwink = 123,
    npc_dota_hero_void_spirit = 126,
    npc_dota_hero_snapfire = 128,
    npc_dota_hero_mars = 129,
    npc_dota_hero_ringmaster = 131,
    npc_dota_hero_dawnbreaker = 135,
    npc_dota_hero_marci = 136,
    npc_dota_hero_primal_beast = 137,
    npc_dota_hero_muerta = 138,
    npc_dota_hero_kez = 145
};
