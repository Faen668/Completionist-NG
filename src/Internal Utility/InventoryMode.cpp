#include "Serialization.hpp"
#include "Variables.hpp"
#include "InventoryMode.hpp"
#include "mainHUD.hpp"
#include "DKUtil/Utility.hpp"

namespace CInventoryEvents {

	//---------------------------------------------------
	//-- Event Functions --------------------------------
	//---------------------------------------------------

	void InventoryAPI::Register() {

		auto ui = RE::UI::GetSingleton();
		ui->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(InventoryAPI::GetSingleton()));
	}

	//---------------------------------------------------
	//-- Event Functions --------------------------------
	//---------------------------------------------------

	EventResult InventoryAPI::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event || !a_event->opening) { return EventResult::kContinue; }
		
		auto* const ui = RE::UI::GetSingleton();
		RE::ItemList* itemList{ nullptr };
		RE::GFxValue nullIconSetter{};

		if (a_event->menuName == RE::BarterMenu::MENU_NAME) {
			if (const auto* menu = static_cast<RE::BarterMenu*>(ui->GetMenu(a_event->menuName).get())) {
				
				menu->uiMovie->SetVariable("InventoryIconSetter", &nullIconSetter);
				itemList = menu->itemList;
			}
		}
		else if (a_event->menuName == RE::ContainerMenu::MENU_NAME) {
			if (const auto* menu = static_cast<RE::ContainerMenu*>(ui->GetMenu(a_event->menuName).get())) {
				
				menu->uiMovie->SetVariable("InventoryIconSetter", &nullIconSetter);
				itemList = menu->itemList;
			}
		}
		else if (a_event->menuName == RE::GiftMenu::MENU_NAME) {
			if (const auto* menu = static_cast<RE::GiftMenu*>(ui->GetMenu(a_event->menuName).get())) {
				
				menu->uiMovie->SetVariable("InventoryIconSetter", &nullIconSetter);
				itemList = menu->itemList;
			}
		}
		else if (a_event->menuName == RE::InventoryMenu::MENU_NAME) {
			if (const auto* menu = static_cast<RE::InventoryMenu*>(ui->GetMenu(a_event->menuName).get())) {
				
				menu->uiMovie->SetVariable("InventoryIconSetter", &nullIconSetter);
				itemList = menu->itemList;
			}
		}
		if (itemList) {
			for (auto* item : itemList->items) {
				Completionist_IconSetter::ProcessEntry(item);
			}
		}
		return EventResult::kContinue;
	}
}

namespace Completionist_IconSetter {
	using namespace CVariables;
	using namespace Completionist_MainHUD;

	enum class MaskedArmorSlot : std::uint32_t
	{
		kHelmet = 1,
		kBody = 2,
		kHand = 3,
		kFeet = 7,
		kShield = 9
	};

	enum class TextType
	{
		kAppend = 0,
		kPrepnd = 1,
		kWrapAround = 2
	};


	enum class IconType
	{
		kShowAll = 0,
		kCollectable = 1,
		kCollected = 2,
		kNone = 3
	};

	enum class ColorCode
	{
		kAV_FIRERESIST,
		kAV_ELECTRICRESIST,
		kAV_FROSTRESIST,
		kArmor,
		kLightArmor,
		kHeavyArmor,
		kWeapon,
		kAmmo,
		kPOTION_POISON,
		kPOTION_HEALTH,
		kPOTION_MAGICKA,
		kPOTION_STAMINA,
		kPOTION_FIRERESIST,
		kPOTION_ELECTRICRESIST,
		kPOTION_FROSTRESIST,
		kSoulGem,
		kSOULGEM_PETTY,
		kSOULGEM_LESSER,
		kSOULGEM_COMMON,
		kSOULGEM_GREATER,
		kSOULGEM_GRAND,
		kSOULGEM_AZURA,
		kMISC_GEM,
		kMISC_HIDE,
		kMISC_INGOT,
		kMISC_FIREWOOD,
		kMISC_GOLD,
		kMISC_LEATHER,
		kMISC_LEATHERSTRIPS,
		kMISC_CHILDRENSCLOTHES,
		kDEFAULT,
		kTorch
	};

	constexpr std::pair<std::uint32_t, std::uint32_t> ColorTbl[32] = {
	{ 13055542u, 10027059u },
	{ 16776960u, 16776960u },
	{ 2096127u, 24495u },
	{ 15587975u, 2763306u },
	{ 7692288u, 4284041241u },
	{ 7042437u, 4282139988u },
	{ 10790335u, 2763306u },
	{ 11050636u, 2763306u },
	{ 11337907u, 7602301u },
	{ 14364275u, 10027059u },
	{ 3055579u, 24495u },
	{ 5364526u, 4278217984u },
	{ 13055542u, 10027059u },
	{ 15379200u, 4285613824u },
	{ 2096127u, 4278212446u },
	{ 14934271u, 24495u },
	{ 14144767u, 24495u },
	{ 12630783u, 24495u },
	{ 11248639u, 24495u },
	{ 9735164u, 24495u },
	{ 7694847u, 24495u },
	{ 7694847u, 24495u },
	{ 16756945u, 4285739088u },
	{ 14398318u, 4284041241u },
	{ 8553090u, 4282139988u },
	{ 11050636u, 2763306u },
	{ 13421619u, 16766720u },
	{ 12225827u, 4284041241u },
	{ 12225827u, 4284041241u },
	{ 15587975u, 2763306u },
	{ 0u, 0u },
	{ 14388228u, 0u },
	};

#define ICON_EXTEND_INVOKE(Category) Process##Category
#define ICON_EXTEND(Category, Item)                                                \
	{                                                                              \
		DEBUG("!Processing [{}] {}({}) | {:X}", #Category, a_item->data.GetName(), \
			a_item->data.GetCount(), a_item->data.objDesc->object->GetFormID());   \
		itemName = a_item->data.GetName();                                         \
		ICON_EXTEND_INVOKE(Category)                                               \
		(Item);                                                                    \
		Item->obj.SetMember("text", { itemName });                                 \
		Item->obj.SetMember("iconLabel", { iconLabel });                           \
		if (iconColor) {                                                           \
			Item->obj.SetMember("iconColor", { iconColor });                       \
		}                                                                          \
		itemName.clear();                                                          \
		iconLabel = "default_misc";                                                \
		iconColor = 0x0;                                                           \
		DEBUG("+Processed [{}] {}({}) | {:X}", #Category, a_item->data.GetName(),  \
			a_item->data.GetCount(), a_item->data.objDesc->object->GetFormID());   \
	}
#define CastAs(var, type) auto* var = std::bit_cast<RE::type*>(a_item->data.objDesc->object)

	//---------------------------------------------------
	//-- Icon Setter (Inventory Mode) -------------------
	//---------------------------------------------------

	static std::string itemName{};
	static std::string iconLabel{ "default_misc" };
	static std::uint32_t iconColor{ 0 };

	//---------------------------------------------------
	//-- Icon Setter (Inventory Mode) -------------------
	//---------------------------------------------------

	constexpr inline std::uint32_t Color(ColorCode a_color)
	{
		return V_DearDiary ? ColorTbl[std::to_underlying(a_color)].second : ColorTbl[std::to_underlying(a_color)].first;
	}

	//---------------------------------------------------
	//-- Icon Setter (Inventory Mode) -------------------
	//---------------------------------------------------

	void ProcessWeapon(RE::ItemList::Item* a_item)
	{
		static constexpr const char* const WEAPTYPE_ICON[13] = {
			"default_weapon",
			"weapon_sword",
			"weapon_dagger",
			"weapon_waraxe",
			"weapon_mace",
			"weapon_greatsword",
			"weapon_battleaxe",
			"weapon_bow",
			"weapon_staff",
			"weapon_crossbow",
			"weapon_pickaxe",
			"weapon_woodaxe",
			"weapon_hammer",
		};

		CastAs(weapon, TESObjectWEAP);

		// use DKU version for range iterators
		using WEAP = enum class RE::WEAPON_TYPE;
		DKUtil::model::enumeration<WEAP, std::uint32_t> types = weapon->GetWeaponType();

		auto idx = 0;
		for (const WEAP type : types.value_range(WEAP::kOneHandSword, WEAP::kCrossbow)) {
			if (types == type && !idx) {
				idx = std::to_underlying(type);
				break;
			}
		}

		if (weapon->HasKeywordString("WeapTypeWarhammer")) {
			idx = 12;
		}

		if (!idx) {
			if (weapon->GetFormID() == 0x0E3C16 ||
				weapon->GetFormID() == 0x06A707 ||
				weapon->GetFormID() == 0x1019D4 ||
				RE::stl::string::icontains(weapon->GetFormEditorID(), "pickaxe")) {
				idx = 10;
			}
			else if (weapon->GetFormID() == 0x02F2F4 ||
				weapon->GetFormID() == 0x0AE086 ||
				RE::stl::string::icontains(weapon->GetFormEditorID(), "wood")) {
				idx = 11;
			}
		}

		iconLabel = WEAPTYPE_ICON[idx];
		iconColor = Color(ColorCode::kWeapon);

		if (TextnTagsAPI::ItemIsCollectable(weapon)) {
			ApplyTextModifications(itemName);
		}

		RE::stl::enumeration<IconType> icon = static_cast<IconType>(V_IconChoice);

		if (icon == IconType::kNone) { return; }

		if (icon == IconType::kShowAll || icon == IconType::kCollectable) {
			if (TextnTagsAPI::ItemIsCollectable(weapon) && V_Weapons_Enabled_New) {
				iconColor = V_Weapons_Colour_New;
				return;
			}
		}

		if (icon == IconType::kShowAll || icon == IconType::kCollected) {
			if (TextnTagsAPI::ItemIsCollected(weapon) && V_Weapons_Enabled_Found) {
				iconColor = V_Weapons_Colour_Found;
				return;
			}
		}
	}

	//---------------------------------------------------
	//-- Icon Setter (Inventory Mode) -------------------
	//---------------------------------------------------

	void ProcessSoulGem(RE::ItemList::Item* a_item)
	{
		static constexpr const char* const SOULGEM_ICON[8] = {
			"soulgem_empty",
			"soulgem_full",
			"soulgem_partial",
			"soulgem_grandempty",
			"soulgem_grandfull",
			"soulgem_grandpartial",
			"soulgem_azura",
			"misc_soulgem",
		};

		CastAs(soulgem, TESSoulGem);

		auto level = soulgem->GetMaximumCapacity();
		auto filled = a_item->data.objDesc->GetSoulLevel();

		DEBUG("LEVEL {} FILLED {}", std::to_underlying(level), std::to_underlying(filled));

		auto idx = level >= RE::SOUL_LEVEL::kGreater ? 3 : 0;
		if (filled != RE::SOUL_LEVEL::kNone) {
			idx += filled > level ? 1 : 2;
		}

		if (soulgem->GetFormID() == 0x063B29 || soulgem->GetFormID() == 0x063B27) {
			idx = 6;
		}

		iconLabel = SOULGEM_ICON[idx];
		iconColor = Color(std::bit_cast<ColorCode>(std::to_underlying(ColorCode::kSoulGem) + std::to_underlying(level)));

		if (TextnTagsAPI::ItemIsCollectable(soulgem)) {
			ApplyTextModifications(itemName);
		}

		RE::stl::enumeration<IconType> icon = static_cast<IconType>(V_IconChoice);

		if (icon == IconType::kNone) { return; }

		if (icon == IconType::kShowAll || icon == IconType::kCollectable) {
			if (TextnTagsAPI::ItemIsCollectable(soulgem) && V_Other_Enabled_New) {
				iconColor = V_Other_Colour_New;
				return;
			}
		}

		if (icon == IconType::kShowAll || icon == IconType::kCollected) {
			if (TextnTagsAPI::ItemIsCollected(soulgem) && V_Other_Enabled_Found) {
				iconColor = V_Other_Colour_Found;
				return;
			}
		}
	}

	//---------------------------------------------------
	//-- Icon Setter (Inventory Mode) -------------------
	//---------------------------------------------------

	void ProcessMisc(RE::ItemList::Item* a_item)
	{
		static constexpr const std::pair<const char* const,
			std::pair<const char* const, const ColorCode>>
			MISC_ICON[8] = {
				{ "BYOHAdoptionClothes",
					{ "clothing_body", ColorCode::kMISC_CHILDRENSCLOTHES } },
				{ "VendorItemDaedricArtifact",
					{ "misc_artifact", ColorCode::kDEFAULT } },
				{ "VendorItemGem",
					{ "misc_gem", ColorCode::kMISC_GEM } },
				{ "VendorItemAnimalHide",
					{ "misc_leather", ColorCode::kMISC_HIDE } },
				{ "VendorItemAnimalPart",
					{ "misc_remains", ColorCode::kDEFAULT } },
				{ "VendorItemOreIngot",
					{ "misc_ingot", ColorCode::kMISC_INGOT } },
				{ "VendorItemClutter",
					{ "misc_clutter", ColorCode::kDEFAULT } },
				{ "VendorItemFirewood",
					{ "misc_wood", ColorCode::kMISC_FIREWOOD } },
		};

		CastAs(misc, TESObjectMISC);

		iconLabel = "default_misc";

		switch (misc->GetFormID()) {
		case 0x00000A:
		{
			iconLabel = "misc_lockpick";
			break;
		}
		case 0x0DB5D2:
		{
			iconLabel = "misc_leather";
			iconColor = Color(ColorCode::kMISC_LEATHER);
			break;
		}
		case 0x0800E4:
		{
			iconLabel = "misc_strips";
			iconColor = Color(ColorCode::kMISC_LEATHERSTRIPS);
			break;
		}
		case 0x06851E:
		{
			iconLabel = "misc_gem";
			iconColor = Color(ColorCode::kMISC_GEM);
			break;
		}
		case 0x01CB34:
		{
			iconLabel = "armor_ring";
			break;
		}
		default:
		{
			// for C.O.I.N support
			if (misc->pickupSound &&  // [ItmGoldUp]
				misc->pickupSound->GetFormID() == 0x03E952) {
				iconLabel = "misc_gold";
				iconColor = Color(ColorCode::kMISC_GOLD);
				break;
			}

			auto set = false;
			for (auto& [keyword, icon] : MISC_ICON) {
				if (misc->ContainsKeywordString(keyword)) {
					iconLabel = icon.first;
					iconColor = Color(icon.second);
					break;
				}
			}

			if (set) {
				break;
			}

			if (RE::stl::string::icontains(misc->model, "RuinsDragonClaw")) {
				iconLabel = "misc_dragonclaw";
			}
			else if (RE::stl::string::icontains(misc->GetName(), "skull")) {
				iconLabel = "misc_remains";
			}
			else if (RE::stl::string::icontains(misc->GetName(), "token")) {
				iconLabel = "misc_artifact";
			}

			break;
		}
		}

		if (TextnTagsAPI::ItemIsCollectable(misc)) {
			ApplyTextModifications(itemName);
		}

		RE::stl::enumeration<IconType> icon = static_cast<IconType>(V_IconChoice);

		if (icon == IconType::kNone) { return; }

		if (icon == IconType::kShowAll || icon == IconType::kCollectable) {
			if (TextnTagsAPI::ItemIsCollectable(misc) && V_Other_Enabled_New) {
				iconColor = V_Other_Colour_New;
				return;
			}
		}

		if (icon == IconType::kShowAll || icon == IconType::kCollected) {
			if (TextnTagsAPI::ItemIsCollected(misc) && V_Other_Enabled_Found) {
				iconColor = V_Other_Colour_Found;
				return;
			}
		}
	}

	//---------------------------------------------------
	//-- Icon Setter (Inventory Mode) -------------------
	//---------------------------------------------------

	void ProcessLight(RE::ItemList::Item* a_item)
	{
		CastAs(light, TESObjectLIGH);

		iconLabel = "misc_torch";
		iconColor = Color(ColorCode::kTorch);

		//
	}

	//---------------------------------------------------
	//-- Icon Setter (Inventory Mode) -------------------
	//---------------------------------------------------

	void ProcessKey(RE::ItemList::Item* a_item)
	{
		CastAs(key, TESKey);

		iconLabel = "default_key";

		if (TextnTagsAPI::ItemIsCollectable(key)) {
			ApplyTextModifications(itemName);
		}

		RE::stl::enumeration<IconType> icon = static_cast<IconType>(V_IconChoice);

		if (icon == IconType::kNone) { return; }

		if (icon == IconType::kShowAll || icon == IconType::kCollectable) {
			if (TextnTagsAPI::ItemIsCollectable(key) && V_Other_Enabled_New) {
				iconColor = V_Other_Colour_New;
				return;
			}
		}

		if (icon == IconType::kShowAll || icon == IconType::kCollected) {
			if (TextnTagsAPI::ItemIsCollected(key) && V_Other_Enabled_Found) {
				iconColor = V_Other_Colour_Found;
				return;
			}
		}
	}

	//---------------------------------------------------
	//-- Icon Setter (Inventory Mode) -------------------
	//---------------------------------------------------

	void ProcessIngredient(RE::ItemList::Item* a_item)
	{
		CastAs(ingredient, IngredientItem);

		iconLabel = "default_ingredient";

		if (TextnTagsAPI::ItemIsCollectable(ingredient)) {
			ApplyTextModifications(itemName);
		}

		RE::stl::enumeration<IconType> icon = static_cast<IconType>(V_IconChoice);

		if (icon == IconType::kNone) { return; }

		if (icon == IconType::kShowAll || icon == IconType::kCollectable) {
			if (TextnTagsAPI::ItemIsCollectable(ingredient) && V_Alchemy_Enabled_New) {
				iconColor = V_Alchemy_Colour_New;
				return;
			}
		}

		if (icon == IconType::kShowAll || icon == IconType::kCollected) {
			if (TextnTagsAPI::ItemIsCollected(ingredient) && V_Alchemy_Enabled_Found) {
				iconColor = V_Alchemy_Colour_Found;
				return;
			}
		}
	}

	//---------------------------------------------------
	//-- Icon Setter (Inventory Mode) -------------------
	//---------------------------------------------------

	void ProcessBook(RE::ItemList::Item* a_item)
	{
		CastAs(book, TESObjectBOOK);

		if (book->IsNote() || book->HasKeywordString("VendorItemRecipe")) {
			iconLabel = "book_recipe";
		}
		else if (book->HasKeywordString("VendorItemSpellTome")) {
			iconLabel = "book_tome";
		}
		else {
			iconLabel = "default_book";
		}

		if (TextnTagsAPI::ItemIsCollectable(book)) {
			ApplyTextModifications(itemName);
		}

		RE::stl::enumeration<IconType> icon = static_cast<IconType>(V_IconChoice);

		if (icon == IconType::kNone) { return; }

		if (icon == IconType::kShowAll || icon == IconType::kCollectable) {
			if (TextnTagsAPI::ItemIsCollectable(book) && V_Books_Enabled_New) {
				iconColor = V_Books_Colour_New;
				return;
			}
		}

		if (icon == IconType::kShowAll || icon == IconType::kCollected) {
			if (TextnTagsAPI::ItemIsCollected(book) && V_Books_Enabled_Found) {
				iconColor = V_Books_Colour_Found;
				return;
			}
		}
	}

	//---------------------------------------------------
	//-- Icon Setter (Inventory Mode) -------------------
	//---------------------------------------------------

	void ProcessArmor(RE::ItemList::Item* a_item)
	{
		static constexpr const char* const ARMOSLOT_ICON[14] = {
			"head",
			"head",
			"body",
			"hands",
			"forearms",
			"amulet",
			"ring",
			"feet",
			"calves",
			"shield",
			"body",
			"head",
			"circlet",
			"head",
		};

		CastAs(armor, TESObjectARMO);

		// use DKU version for range iterators
		using ARMO = enum class RE::BIPED_MODEL::BipedObjectSlot;
		DKUtil::model::enumeration<ARMO, std::uint32_t> slots = armor->GetSlotMask();

		switch (armor->GetArmorType()) {
		case RE::BIPED_MODEL::ArmorType::kLightArmor:
		{
			iconLabel = "lightarmor_";
			iconColor = Color(ColorCode::kLightArmor);
			break;
		}
		case RE::BIPED_MODEL::ArmorType::kHeavyArmor:
		{
			iconLabel = "armor_";
			iconColor = Color(ColorCode::kHeavyArmor);
			break;
		}
		case RE::BIPED_MODEL::ArmorType::kClothing:
		{
			iconLabel = "clothing_";
			iconColor = Color(ColorCode::kArmor);
			break;
		}
		default:
			break;
		}

		auto set = false;
		for (const ARMO slot : slots.flag_range(ARMO::kHead, ARMO::kEars)) {
			if (slots.any(slot) && !set) {
				iconLabel += ARMOSLOT_ICON[slots.index_of(slot)];
				set = true;
				break;
			}
		}

		if (slots.any(ARMO::kAmulet, ARMO::kRing, ARMO::kAmulet)) {
			RE::stl::string::replace_first_instance(iconLabel, "light", "");
			RE::stl::string::replace_first_instance(iconLabel, "clothing", "armor");
			set = true;
		}

		if (!set) {
			iconLabel = "default_armor";
		}

		if (TextnTagsAPI::ItemIsCollectable(armor)) {
			ApplyTextModifications(itemName);
		}

		RE::stl::enumeration<IconType> icon = static_cast<IconType>(V_IconChoice);

		if (icon == IconType::kNone) { return; }

		if (icon == IconType::kShowAll || icon == IconType::kCollectable) {
			if (TextnTagsAPI::ItemIsCollectable(armor) && V_Armor_Enabled_New) {
				iconColor = V_Armor_Colour_New;
				return;
			}
		}

		if (icon == IconType::kShowAll || icon == IconType::kCollected) {
			if (TextnTagsAPI::ItemIsCollected(armor) && V_Armor_Enabled_Found) {
				iconColor = V_Armor_Colour_Found;
				return;
			}
		}
	}

	//---------------------------------------------------
	//-- Icon Setter (Inventory Mode) -------------------
	//---------------------------------------------------

	void ProcessAmmo(RE::ItemList::Item* a_item)
	{
		CastAs(ammo, TESAmmo);

		iconLabel = ammo->IsBolt() ? "weapon_bolt" : "weapon_arrow";
		iconColor = Color(ColorCode::kAmmo);

		if (TextnTagsAPI::ItemIsCollectable(ammo)) {
			ApplyTextModifications(itemName);
		}

		RE::stl::enumeration<IconType> icon = static_cast<IconType>(V_IconChoice);

		if (icon == IconType::kNone) { return; }

		if (icon == IconType::kShowAll || icon == IconType::kCollectable) {
			if (TextnTagsAPI::ItemIsCollectable(ammo) && V_Ammo_Enabled_New) {
				iconColor = V_Ammo_Colour_New;
				return;
			}
		}

		if (icon == IconType::kShowAll || icon == IconType::kCollected) {
			if (TextnTagsAPI::ItemIsCollected(ammo) && V_Ammo_Enabled_Found) {
				iconColor = V_Ammo_Colour_Found;
				return;
			}
		}
	}

	//---------------------------------------------------
	//-- Icon Setter (Inventory Mode) -------------------
	//---------------------------------------------------

	void ProcessAlchemy(RE::ItemList::Item* a_item)
	{
		CastAs(alchemy, AlchemyItem);

		iconLabel = "default_potion";

		if (alchemy->IsFood()) {
			if (alchemy->data.consumptionSound &&  // [SNDR:ITMPotionUse]
				alchemy->data.consumptionSound->GetFormID() == 0x0B6435) {
				iconLabel = "food_wine";
			}
			else {
				iconLabel = "default_food";
			}
		}
		else if (alchemy->IsPoison()) {
			iconLabel = "potion_poison";
			iconColor = Color(ColorCode::kPOTION_POISON);
		}
		else {
			auto* effect = alchemy->GetCostliestEffectItem();
			if (effect && effect->baseEffect) {

				switch (static_cast<RE::ActorValue>(effect->baseEffect->data.primaryAV)) {
				case RE::ActorValue::kHealth:
					[[fallthrough]];
				case RE::ActorValue::kHealRate:
					[[fallthrough]];
				case RE::ActorValue::kHealRateMult:
				{
					iconLabel = "potion_health";
					iconColor = Color(ColorCode::kPOTION_HEALTH);
					break;
				}
				case RE::ActorValue::kMagicka:
					[[fallthrough]];
				case RE::ActorValue::kMagickaRate:
					[[fallthrough]];
				case RE::ActorValue::kMagickaRateMult:
				{
					iconLabel = "potion_magic";
					iconColor = Color(ColorCode::kPOTION_MAGICKA);
					break;
				}
				case RE::ActorValue::kStamina:
					[[fallthrough]];
				case RE::ActorValue::KStaminaRate:
					[[fallthrough]];
				case RE::ActorValue::kStaminaRateMult:
				{
					iconLabel = "potion_stam";
					iconColor = Color(ColorCode::kPOTION_STAMINA);
					break;
				}
				case RE::ActorValue::kResistFire:
				{
					iconLabel = "potion_fire";
					iconColor = Color(ColorCode::kPOTION_FIRERESIST);
					break;
				}
				case RE::ActorValue::kResistShock:
				{
					iconLabel = "potion_shock";
					iconColor = Color(ColorCode::kPOTION_ELECTRICRESIST);
					break;
				}
				case RE::ActorValue::kResistFrost:
				{
					iconLabel = "potion_frost";
					iconColor = Color(ColorCode::kPOTION_FROSTRESIST);
					break;
				}
				default:
					break;
				}
			}
		}

		if (TextnTagsAPI::ItemIsCollectable(alchemy)) {
			ApplyTextModifications(itemName);
		}

		RE::stl::enumeration<IconType> icon = static_cast<IconType>(V_IconChoice);

		if (icon == IconType::kNone) { return; }

		if (icon == IconType::kShowAll || icon == IconType::kCollectable) {
			if (TextnTagsAPI::ItemIsCollectable(alchemy) && V_Alchemy_Enabled_New) {
				iconColor = V_Alchemy_Colour_New;
				return;
			}
		}

		if (icon == IconType::kShowAll || icon == IconType::kCollected) {
			if (TextnTagsAPI::ItemIsCollected(alchemy) && V_Alchemy_Enabled_Found) {
				iconColor = V_Alchemy_Colour_Found;
				return;
			}
		}
	}

	//---------------------------------------------------
	//-- Icon Setter (Inventory Mode) -------------------
	//---------------------------------------------------

	void ProcessEntry(RE::ItemList::Item* a_item)
	{
		switch (a_item->data.objDesc->object->GetFormType()) {
		case RE::FormType::AlchemyItem:
		{
			ICON_EXTEND(Alchemy, a_item);
			break;
		}
		case RE::FormType::Ammo:
		{
			ICON_EXTEND(Ammo, a_item);
			break;
		}
		case RE::FormType::Armor:
		{
			ICON_EXTEND(Armor, a_item);
			break;
		}
		case RE::FormType::Book:
		{
			ICON_EXTEND(Book, a_item);
			break;
		}
		case RE::FormType::Ingredient:
		{
			ICON_EXTEND(Ingredient, a_item);
			break;
		}
		case RE::FormType::KeyMaster:
		{
			ICON_EXTEND(Key, a_item);
			break;
		}
		case RE::FormType::Light:
		{
			ICON_EXTEND(Light, a_item);
			break;
		}
		case RE::FormType::Misc:
		{
			ICON_EXTEND(Misc, a_item);
			break;
		}
		case RE::FormType::SoulGem:
		{
			ICON_EXTEND(SoulGem, a_item);
			break;
		}
		case RE::FormType::Weapon:
		{
			ICON_EXTEND(Weapon, a_item);
			break;
		}
		default:
			break;
		}
	}

	void ApplyTextModifications(std::string& a_name) {

		switch (static_cast<TextType>(V_TextChoice)) {
		case TextType::kAppend: {
			a_name = a_name + " ***";
			break;
		}
		case TextType::kPrepnd: {
			a_name = "*** " + a_name;
			break;
		}
		case TextType::kWrapAround: {
			a_name = "*** " + a_name + " ***";
			break;
		}
		default:
			break;
		}
	}
}  // namespace IconExtender