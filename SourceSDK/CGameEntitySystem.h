#pragma once
#include "../Utils/Memory.h"
#include "CEntityIdentity.h"

// Forward declaration
class CHandle;
class CEntityIdentity;
//

class SchemaName {
public:
	optional<string> name() const {
		const auto name_ptr = Memory::read_memory<uintptr_t>(this + 0x8);
		return !name_ptr ? nullopt : Memory::read_string(name_ptr.value());
	}
};

class CSchemaClassBinding {
public:
	optional<string> binary_name() const { // C_DOTA_Unit_Hero_AntiMage
		optional base = Memory::read_memory<SchemaName*>(this + 0x30);
		return !base ? nullopt : base.value()->name();
	}

	optional<string> class_name() const { // C_DOTA_BaseNPC_Hero
		optional base = Memory::read_memory<SchemaName*>(this + 0x38);
		return !base ? nullopt : base.value()->name();
	}
};

class CGameEntitySystem {
public:
	void dump_entities() const { // for testing purposes
		std::ofstream dump_file;
		dump_file.open("C:\\entity_dump.txt");
		int ents_count = 0;

		auto ident = this->first_identity();
		if (!ident)
			return;

		while (true) {
			ents_count++;

			const auto schema = ident->schema_class_binding();
			const auto internal_name = ident->internal_name();
			const auto entity_name = ident->entity_name();
			const auto binary_name = schema->binary_name();
			const auto class_name = schema->class_name();

			dump_file
				<< "internal_name: " << internal_name.value_or("empty")
				<< " | entity_name: " << entity_name.value_or("empty")
				<< " | binary_name: " << binary_name.value_or("empty")
				<< " | class_name: " << class_name.value_or("empty");

			if (ident->handle())
				dump_file << " | handle: " << std::hex << ident->handle().value().get() << " | index: " << ident->handle().value().to_index();

			dump_file << " -> [" << (void*)ident->base_entity() << "]\n";

			const auto next_ent = ident->m_pNext();
			if (!next_ent)
				break;

			ident = next_ent.value();
		}
		
		dump_file.close();
		printf("\n");
		LOG::INFO("dump_entities: done. Total: {}", ents_count);
	}

	optional<CBaseEntity*> find_by_name(const ENTITY_NAME_TYPE& name_type, const string& name_to_find) const {
		auto ident = this->first_identity();
		if (!ident)
			return nullopt;

		while (true) {
			switch (name_type) {
				case ENTITY_NAME_TYPE::INTERNAL_NAME: {
					const auto internal_name = ident->internal_name().value_or("");
					if (internal_name == name_to_find)
						return ident->base_entity();
					break;
				}
				case ENTITY_NAME_TYPE::ENTITY_NAME: {
					const auto entity_name = ident->entity_name().value_or("");
					if (entity_name == name_to_find)
						return ident->base_entity();
					break;
				}
				case ENTITY_NAME_TYPE::BINARY_NAME: {
					const auto schema = ident->schema_class_binding();
					const auto binary_name = schema->binary_name().value_or("");
					if (binary_name == name_to_find)
						return ident->base_entity();
					break;
				}
				case ENTITY_NAME_TYPE::CLASS_NAME: {
					const auto schema = ident->schema_class_binding();
					const auto class_name = schema->class_name().value_or("");
					if (class_name == name_to_find)
						return ident->base_entity();
					break;
				}
			}

			const auto next_ent = ident->m_pNext();
			if (!next_ent)
				break;

			ident = next_ent.value();
		}

		return nullopt;
	}

	template <typename T>
	std::vector<T*> find_vector_by_name(const ENTITY_NAME_TYPE& name_type, const string& name_to_find) const {
		std::vector<T*> found;

		auto ident = this->first_identity();

		while (ident) {
			switch (name_type) {
				case ENTITY_NAME_TYPE::INTERNAL_NAME: {
					const auto internal_name = ident->internal_name().value_or("");
					if (internal_name == name_to_find)
						found.push_back(reinterpret_cast<T*>(ident->base_entity()));
					break;
				}
				case ENTITY_NAME_TYPE::ENTITY_NAME: {
					const auto entity_name = ident->entity_name().value_or("");
					if (entity_name == name_to_find)
						found.push_back(reinterpret_cast<T*>(ident->base_entity()));
					break;
				}
				case ENTITY_NAME_TYPE::BINARY_NAME: {
					const auto schema = ident->schema_class_binding();
					const auto binary_name = schema->binary_name().value_or("");
					if (binary_name == name_to_find)
						found.push_back(reinterpret_cast<T*>(ident->base_entity()));
					break;
				}
				case ENTITY_NAME_TYPE::CLASS_NAME: {
					const auto schema = ident->schema_class_binding();
					const auto class_name = schema->class_name().value_or("");
					if (class_name == name_to_find)
						found.push_back(reinterpret_cast<T*>(ident->base_entity()));
					break;
				}
			}

			ident = ident->m_pNext().value_or(nullptr);
		}

		return found;
	}

	optional<CBaseEntity*> find_by_index(uint32_t index, bool hero_only = false) const {
		for (auto ident = this->first_identity(); ident; ident = ident->m_pNext().value_or(nullptr)) {
			if (hero_only) {
				const auto schema = ident->schema_class_binding();
				if (!schema || schema->class_name().value_or("") != "C_DOTA_BaseNPC_Hero")
					continue;
			}

			if (ident->handle() && ident->handle().value().to_index() == index)
				return ident->base_entity();
		}

		return nullopt;
	}


	optional <CBaseEntity*> find_by_handle(const CHandle handle, bool hero_only = false) const {
		return find_by_index(handle.to_index(), hero_only);
	}

	CEntityIdentity* first_identity() const {
		return Memory::read_memory<CEntityIdentity*>(this + 0x210).value_or(nullptr);
	}
};
