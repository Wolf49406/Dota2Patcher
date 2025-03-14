#include "Hacks.h"
#include "../SourceSDK/CDOTAPlayerController.h"
#include "../SourceSDK/CGameEntitySystem.h"
#include "../SourceSDK/CBaseEntity.h"

bool Hacks::find_local_hero() {
	const auto hero = vmt.entity_system->find_by_handle(GameData::local_player->assigned_hero_handle(), true);
	if (!hero)
		return false;

	GameData::local_hero = hero.value();

	return true;
}

bool Hacks::find_local_player() {
	std::vector<C_DOTAPlayerController*> players = vmt.entity_system->find_vector_by_name<C_DOTAPlayerController>(ENTITY_NAME_TYPE::ENTITY_NAME, "dota_player_controller");

	for (const auto player : players) {
		if (player->is_local_player()) {
			GameData::local_player = player;
			return true;
		}
	}

	return false;
}
