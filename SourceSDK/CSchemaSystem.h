#pragma once
#include <unordered_set>
#include <ranges>
#include "../Utils/Memory.h"
#include "../Utils/ProcessHandle.h"

constexpr size_t CLASS_DESCRIPTION_CONTAINERS_ARRAY_OFFSET = 0x580;
constexpr size_t CLASS_DESCRIPTION_CONTAINERS_ARRAY_SIZE = 0x28;
constexpr size_t CLASS_DESCRIPTION_CONTAINERS_ARRAY_MAX_INDEX = 256;
constexpr size_t CLASS_DESCRIPTION_CONTAINER_SIZE = 0x20;
constexpr size_t SCHEMA_CLASS_FIELD_DATA_SIZE = 0x20;

class CSchemaType_Builtin {
public:
	optional <string> type_name() const {
		const auto type_name_ptr = Memory::read_memory<uintptr_t>(this + 0x8);
		return !type_name_ptr ? nullopt : Memory::read_string(type_name_ptr.value());
	}
};

class SchemaClassFieldData_t {
public:
	optional <string> netvar_name() const {
		const auto netvar_name_ptr = Memory::read_memory<uintptr_t>(this);
		return !netvar_name_ptr ? nullopt : Memory::read_string(netvar_name_ptr.value());
	}

	CSchemaType_Builtin* m_type() const {
		return Memory::read_memory<CSchemaType_Builtin*>(this + 0x8).value_or(nullptr);
	}

	optional <int32_t> offset() const {
		return Memory::read_memory<int32_t>(this + 0x10);
	}

	bool is_netvar() const {
		return Memory::read_memory<int32_t>(this + 0x14).value_or(10) < 10;
	}
};

class ClassDescription;

class SchemaParentInfo {
public:
	ClassDescription* parent() const {
		return Memory::read_memory<ClassDescription*>(this + 0x8).value_or(nullptr);
	}
};

class ClassDescription {
public:
	optional <string> class_name() const {
		const auto class_name_ptr = Memory::read_memory<uintptr_t>(this + 0x8);
		return !class_name_ptr ? nullopt : Memory::read_string(class_name_ptr.value());
	}

	optional <uint32_t> class_size() const {
		return Memory::read_memory<uint32_t>(this + 0x18);
	}

	optional <uint32_t> members_size() const {
		return Memory::read_memory<uint32_t>(this + 0x1C);
	}

	optional <SchemaClassFieldData_t*> members_description(const size_t index) const {
		const auto schema_class_field_data_base = Memory::read_memory<uintptr_t>(this + 0x28);
		if (!schema_class_field_data_base || schema_class_field_data_base.value_or(0) == 0)
			return nullopt;

		const auto schema_class_field_data = schema_class_field_data_base.value() + SCHEMA_CLASS_FIELD_DATA_SIZE * index ;
		if (!Memory::is_valid_ptr(schema_class_field_data))
			return nullopt;

		return reinterpret_cast<SchemaClassFieldData_t*>(schema_class_field_data);
	}

	optional <SchemaParentInfo*> parent_info() const {
		return Memory::read_memory<SchemaParentInfo*>(this + 0x30);
	}
};

class ClassDescription_Container {
public:
	optional <ClassDescription*> class_description(const size_t index) const {
		auto class_description_ptr = reinterpret_cast<uintptr_t>(this) + CLASS_DESCRIPTION_CONTAINER_SIZE * index;
		return Memory::is_valid_ptr(class_description_ptr) ? Memory::read_memory<ClassDescription*>(class_description_ptr + 0x10) : nullopt;
	}
};

class CSchemaSystemTypeScope {
public:
	optional <string> scope_name() const {
		return Memory::read_string(this + 0x8);
	}

	optional <ClassDescription_Container*> class_description_container(const size_t index) const {
		if (index > CLASS_DESCRIPTION_CONTAINERS_ARRAY_MAX_INDEX)
			return nullopt;

		const auto container_base = (uintptr_t)this + CLASS_DESCRIPTION_CONTAINERS_ARRAY_OFFSET;
		const auto container_ptr = container_base + CLASS_DESCRIPTION_CONTAINERS_ARRAY_SIZE * index;
		if (!Memory::is_valid_ptr(container_ptr))
			return nullopt;

		return Memory::read_memory<ClassDescription_Container*>(container_ptr + 0x18);
	}
};

class CSchemaSystem {
public:
	optional <CSchemaSystemTypeScope*> type_scope(const string& scope_name) const {
		const auto scopes_list = Memory::read_memory<uintptr_t>(this + 0x190);
		if (!scopes_list)
			return nullopt;

		for (size_t i = 0; i < 18; i++) {
			const auto current_scope = Memory::read_memory<CSchemaSystemTypeScope*>(scopes_list.value() + i * 8);
			if (!current_scope)
				continue;

			const auto current_name = current_scope.value()->scope_name();
			if (!current_name || current_name.value_or(scope_name) != scope_name)
				continue;

			return current_scope;
		}

		return nullopt;
	}

	optional <CSchemaSystemTypeScope*> type_scope(const size_t scope_index) const {
		if (scope_index > 19)
			return nullopt;

		const auto scopes_list = Memory::read_memory<uintptr_t>(this + 0x190);
		if (!scopes_list)
			return nullopt;

		return Memory::read_memory<CSchemaSystemTypeScope*>(scopes_list.value() + scope_index * 8);
	}

	std::stringstream iterate_netvars(const string& class_name, const ClassDescription* class_description, const bool dump_to_file) const {
		std::stringstream dump_content;

		for (size_t i = 0; auto members_description = class_description->members_description(i); ++i) {
			if (!members_description || !members_description.value()->is_netvar())
				break;

			const auto netvar_name = members_description.value()->netvar_name();
			const auto offset = members_description.value()->offset();
			const auto m_type = members_description.value()->m_type()->type_name();

			if (netvar_name && offset.value_or(0) != 0) {
				g_netvars[class_name][netvar_name.value()] = offset.value();
				if (dump_to_file)
					dump_content << netvar_name.value() << " | " << std::hex << offset.value() << " | " << m_type.value_or("unknow") << "\n";
			}
		}

		return dump_content;
	}

	size_t dump_netvars(const string& scope_name, const bool dump_to_file = false) const {
		const auto scope = this->type_scope(scope_name);
		if (!scope)
			return 0;

		if (dump_to_file)
			std::filesystem::create_directories("C:\\netvars\\");

		std::unordered_set<string> processed_classes;

		size_t container_index = 0;
		while (true) {
			const auto class_description_container = scope.value()->class_description_container(container_index);
			if (!class_description_container)
				break;

			size_t class_description_index = 0;
			auto class_description = class_description_container.value()->class_description(class_description_index);
			while (class_description) {
				const auto class_name = class_description.value()->class_name();
				if (!class_name)
					break;

				std::vector<std::pair<string, ClassDescription*>> class_hierarchy;
				auto current_class = class_description.value();
				while (current_class) {
					const auto current_class_name = current_class->class_name();
					if (!current_class_name)
						break;
					class_hierarchy.insert(class_hierarchy.begin(), { current_class_name.value(), current_class });
					current_class = current_class->parent_info() ? current_class->parent_info().value()->parent() : nullptr;
				}

				for (const auto& [name, desc] : class_hierarchy) {
					if (processed_classes.contains(name))
						continue;

					std::stringstream dump_content;
					dump_content << "[Hierarchy]\n";
					for (const auto& [hier_name, _] : class_hierarchy)
						dump_content << hier_name << " -> ";
					dump_content.seekp(-4, std::ios_base::end);
					dump_content << "\n\n";

					dump_content << "[" << name << "]\n";
					dump_content << iterate_netvars(name, desc, dump_to_file).rdbuf();
					processed_classes.insert(name);

					if (dump_to_file && dump_content.tellp() != std::streampos(0) && dump_content.str().find("|") != string::npos) {
						string filename = "C:\\netvars\\" + name + ".txt";
						std::ofstream dump_file(filename);
						dump_file << dump_content.rdbuf();
						dump_file.close();
					}
				}

				class_description_index++;
				class_description = class_description_container.value()->class_description(class_description_index);
			}
			container_index++;
		}

		size_t netvar_count = 0;
		for (const auto& [class_name, netvar_map] : g_netvars) {
			netvar_count += netvar_map.size();
		}

		return netvar_count;
	}

	size_t dump_netvars(const size_t scope_index, const bool dump_to_file) const {
		const auto scope = this->type_scope(scope_index);
		if (!scope)
			return 0;

		const auto scope_name = scope.value()->scope_name();
		if (!scope_name)
			return 0;

		return dump_netvars(scope_name.value(), dump_to_file);
	}

	template<typename T>
	optional <uintptr_t> get_netvar(const T& addr, const string& class_name, const string& netvar_name) const {
		if (auto it_class = g_netvars.find(class_name); it_class != g_netvars.end()) {
			if (auto it_var = it_class->second.find(netvar_name); it_var != it_class->second.end())
				return reinterpret_cast<uintptr_t>(addr) + it_var->second;
		}
		return nullopt;
	}

	static inline std::unordered_map<string, std::unordered_map<std::string, int32_t>> g_netvars;
};
