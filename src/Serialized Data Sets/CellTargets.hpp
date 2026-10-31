#pragma once

namespace Serialization
{
	// Tracker slots and the location marker, kept in the co-save
	struct CompletionistCellTargets final : public ISerializable
	{
		struct Target
		{
			RE::FormID  form{ 0 };
			std::string name;
			std::string type;
		};

		// Saved as one byte, older saves have 0 and 1
		enum class Kind : std::uint8_t
		{
			kMark,	 // cell scanner marker, ends when its cell unloads
			kPin,	 // pinned item, placed while in the player's cell
			kFind,	 // NPC or placed item from a page, wherever it is
			kTravel	 // map marker from a page
		};

		struct Tracked
		{
			RE::FormID   form{ 0 };
			RE::FormID   ref{ 0 };	// item, or the container / NPC holding it
			std::string  name;
			std::string  type;	// located from a page: the objective's translation key, when not the usual one
			Kind         kind{ Kind::kMark };
			std::int32_t slot{ -1 };	// -1 for a pin not in this cell
			bool         active{ true };

			[[nodiscard]] bool Pinned() const noexcept { return kind == Kind::kPin; }
			// Tracked from a page, not tied to the loaded cells
			[[nodiscard]] bool Located() const noexcept { return kind == Kind::kFind || kind == Kind::kTravel; }
		};

		std::vector<Tracked> tracked;
		Target               location;	// the old single location marker, read from older saves only

		virtual void Save(SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept override
		{
			const auto count = static_cast<std::uint32_t>(tracked.size());
			if (!a_intfc->WriteRecordData(&count, sizeof(count))) {
				ERROR("Failed to write serialized form data: {}", a_name);
				return;
			}
			for (const auto& item : tracked) {
				const auto         kind = static_cast<std::uint8_t>(item.kind);
				const std::uint8_t active = item.active ? 1 : 0;
				if (!a_intfc->WriteRecordData(&item.form, sizeof(item.form)) || !a_intfc->WriteRecordData(&item.ref, sizeof(item.ref)) ||
					!WriteText(a_intfc, item.name) || !WriteText(a_intfc, item.type) || !a_intfc->WriteRecordData(&kind, sizeof(kind)) ||
					!a_intfc->WriteRecordData(&item.slot, sizeof(item.slot)) || !a_intfc->WriteRecordData(&active, sizeof(active))) {
					ERROR("Failed to write serialized form data: {}", a_name);
					return;
				}
			}
			const Target none;
			if (!a_intfc->WriteRecordData(&none.form, sizeof(none.form)) || !WriteText(a_intfc, none.name) || !WriteText(a_intfc, none.type)) {
				ERROR("Failed to write serialized form data: {}", a_name);
				return;
			}
			INFO("Saved {} to co-save ({} tracked)", a_name, tracked.size());
		}

		virtual void Load(SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept override
		{
			tracked.clear();
			location = {};
			std::uint32_t count = 0;
			if (!a_intfc->ReadRecordData(&count, sizeof(count)) || count > 1024) {
				ERROR("Failed to read serialized form data: {}", a_name);
				return;
			}
			const auto resolve = [&](RE::FormID a_form) {
				RE::FormID resolved = 0;
				return a_form && a_intfc->ResolveFormID(a_form, resolved) ? resolved : 0;
			};
			for (std::uint32_t i = 0; i < count; ++i) {
				Tracked      item;
				std::uint8_t kind = 0;
				std::uint8_t active = 1;
				if (!a_intfc->ReadRecordData(&item.form, sizeof(item.form)) || !a_intfc->ReadRecordData(&item.ref, sizeof(item.ref)) ||
					!ReadText(a_intfc, item.name) || !ReadText(a_intfc, item.type) || !a_intfc->ReadRecordData(&kind, sizeof(kind)) ||
					!a_intfc->ReadRecordData(&item.slot, sizeof(item.slot)) || !a_intfc->ReadRecordData(&active, sizeof(active))) {
					ERROR("Failed to read serialized form data: {}", a_name);
					tracked.clear();
					return;
				}
				item.form = resolve(item.form);
				item.ref = resolve(item.ref);
				item.kind = kind <= static_cast<std::uint8_t>(Kind::kTravel) ? static_cast<Kind>(kind) : Kind::kMark;
				item.active = active != 0;
				if (item.form) {
					tracked.push_back(std::move(item));
				}
			}
			RE::FormID form = 0;
			if (!a_intfc->ReadRecordData(&form, sizeof(form)) || !ReadText(a_intfc, location.name) || !ReadText(a_intfc, location.type)) {
				ERROR("Failed to read serialized form data: {}", a_name);
				location = {};
				return;
			}
			location.form = resolve(form);
			INFO("Loaded SKSE co-save {} ({} tracked, location {:08X})", a_name, tracked.size(), location.form);
		}

		virtual void Revert([[maybe_unused]] SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept override
		{
			INFO("Reverting {} from co-save", a_name);
			tracked.clear();
			location = {};
		}

	private:
		static bool WriteText(SKSE::SerializationInterface* a_intfc, const std::string& a_text)
		{
			const std::size_t size = a_text.size();
			return a_intfc->WriteRecordData(&size, sizeof(size)) && (!size || a_intfc->WriteRecordData(a_text.data(), static_cast<std::uint32_t>(size)));
		}

		static bool ReadText(SKSE::SerializationInterface* a_intfc, std::string& a_text)
		{
			std::size_t size = 0;
			if (!a_intfc->ReadRecordData(&size, sizeof(size)) || size > 4096) {
				return false;
			}
			a_text.resize(size);
			return !size || a_intfc->ReadRecordData(a_text.data(), static_cast<std::uint32_t>(size));
		}
	};
}
