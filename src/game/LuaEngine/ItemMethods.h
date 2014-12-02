/*
* Copyright (C) 2010 - 2014 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef ITEMMETHODS_H
#define ITEMMETHODS_H

namespace LuaItem
{
    /* BOOLEAN */
    int IsSoulBound(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->IsSoulBound());
        return 1;
    }

#if (!defined(TBC) && !defined(CLASSIC))
    int IsBoundAccountWide(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->IsBoundAccountWide());
        return 1;
    }
#endif

    int IsBoundByEnchant(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->IsBoundByEnchant());
        return 1;
    }

    int IsNotBoundToPlayer(Eluna* E, Item* item)
    {
        Player* player = Eluna::CHECKOBJ<Player>(E->L, 2);

        Eluna::Push(E->L, item->IsBindedNotWith(player));
        return 1;
    }

    int IsLocked(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->IsLocked());
        return 1;
    }

    int IsBag(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->IsBag());
        return 1;
    }

#ifndef CLASSIC
    int IsCurrencyToken(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->IsCurrencyToken());
        return 1;
    }
#endif

    int IsNotEmptyBag(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->IsNotEmptyBag());
        return 1;
    }

    int IsBroken(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->IsBroken());
        return 1;
    }

    int CanBeTraded(Eluna* E, Item* item)
    {
#if (defined(TBC) || defined(CLASSIC))
        Eluna::Push(E->L, item->CanBeTraded());
#else
        bool mail = Eluna::CHECKVAL<bool>(E->L, 2, false);
        Eluna::Push(E->L, item->CanBeTraded(mail));
#endif
        return 1;
    }

    int IsInTrade(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->IsInTrade());
        return 1;
    }

    int IsInBag(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->IsInBag());
        return 1;
    }

    int IsEquipped(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->IsEquipped());
        return 1;
    }

    int HasQuest(Eluna* E, Item* item)
    {
        uint32 quest = Eluna::CHECKVAL<uint32>(E->L, 2);
#ifndef TRINITY
        Eluna::Push(E->L, item->HasQuest(quest));
#else
        Eluna::Push(E->L, item->hasQuest(quest));
#endif
        return 1;
    }

    int IsPotion(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->IsPotion());
        return 1;
    }

#ifndef CATA
    int IsWeaponVellum(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->IsWeaponVellum());
        return 1;
    }

    int IsArmorVellum(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->IsArmorVellum());
        return 1;
    }
#endif

    int IsConjuredConsumable(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->IsConjuredConsumable());
        return 1;
    }

    /*
     * int IsRefundExpired(Eluna* E, Item* item)// TODO: Implement core support
     * {
     *     Eluna::Push(E->L, item->IsRefundExpired());
     *     return 1;
     * }
     */

    /* GETTERS */

    /**
     * Returns the chat link of the [Item]
     *
     * <pre>
     * enum LocaleConstant
     * {
     *     LOCALE_enUS = 0,
     *     LOCALE_koKR = 1,
     *     LOCALE_frFR = 2,
     *     LOCALE_deDE = 3,
     *     LOCALE_zhCN = 4,
     *     LOCALE_zhTW = 5,
     *     LOCALE_esES = 6,
     *     LOCALE_esMX = 7,
     *     LOCALE_ruRU = 8
     * };
     * </pre>
     *
     * @param [LocaleConstant] locale = DEFAULT_LOCALE : locale to return the [Item]'s name in
     * @return string itemLink
     */
    int GetItemLink(Eluna* E, Item* item)
    {
        uint8 locale = Eluna::CHECKVAL<uint8>(E->L, 2, DEFAULT_LOCALE);
        if (locale >= TOTAL_LOCALES)
            return luaL_argerror(E->L, 2, "valid LocaleConstant expected");

        const ItemTemplate* temp = item->GetTemplate();
        std::string name = temp->Name1;
        if (ItemLocale const* il = eObjectMgr->GetItemLocale(temp->ItemId))
            ObjectMgr::GetLocaleString(il->Name, locale, name);

#ifndef CLASSIC
        if (int32 itemRandPropId = item->GetItemRandomPropertyId())
        {
#ifdef CATA
            char* suffix = NULL;
#else
            char* const* suffix = NULL;
#endif
            if (itemRandPropId < 0)
            {
                const ItemRandomSuffixEntry* itemRandEntry = sItemRandomSuffixStore.LookupEntry(-item->GetItemRandomPropertyId());
                if (itemRandEntry)
                    suffix = itemRandEntry->nameSuffix;
            }
            else
            {
                const ItemRandomPropertiesEntry* itemRandEntry = sItemRandomPropertiesStore.LookupEntry(item->GetItemRandomPropertyId());
                if (itemRandEntry)
                    suffix = itemRandEntry->nameSuffix;
            }
            if (suffix)
            {
                //std::string test(suffix[(name != temp->Name1) ? loc_idx : DEFAULT_LOCALE]);
                //if (!test.empty())
                //{
                name += ' ';
                name += suffix[(name != temp->Name1) ? locale : DEFAULT_LOCALE];
                /*}*/
            }
        }
#endif

        std::ostringstream oss;
        oss << "|c" << std::hex << ItemQualityColors[temp->Quality] << std::dec <<
            "|Hitem:" << temp->ItemId << ":" <<
            item->GetEnchantmentId(PERM_ENCHANTMENT_SLOT) << ":" <<
#ifndef CLASSIC
            item->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT) << ":" <<
            item->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_2) << ":" <<
            item->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_3) << ":" <<
            item->GetEnchantmentId(BONUS_ENCHANTMENT_SLOT) << ":" <<
#endif
            item->GetItemRandomPropertyId() << ":" << item->GetItemSuffixFactor() << ":" <<
            (uint32)item->GetOwner()->getLevel() << "|h[" << name << "]|h|r";

        Eluna::Push(E->L, oss.str());
        return 1;
    }

    int GetOwnerGUID(Eluna* E, Item* item)
    {
#ifndef TRINITY
        Eluna::Push(E->L, item->GetOwnerGuid());
#else
        Eluna::Push(E->L, item->GetOwnerGUID());
#endif
        return 1;
    }

    int GetOwner(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetOwner());
        return 1;
    }

    int GetCount(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetCount());
        return 1;
    }

    int GetMaxStackCount(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetMaxStackCount());
        return 1;
    }

    int GetSlot(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetSlot());
        return 1;
    }

    int GetBagSlot(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetBagSlot());
        return 1;
    }

    int GetEnchantmentId(Eluna* E, Item* item)
    {
        uint32 enchant_slot = Eluna::CHECKVAL<uint32>(E->L, 2);

        if (enchant_slot >= MAX_INSPECTED_ENCHANTMENT_SLOT)
            return luaL_argerror(E->L, 2, "valid EnchantmentSlot expected");

        Eluna::Push(E->L, item->GetEnchantmentId(EnchantmentSlot(enchant_slot)));
        return 1;
    }

    int GetSpellId(Eluna* E, Item* item)
    {
        uint32 index = Eluna::CHECKVAL<uint32>(E->L, 2);
        if (index >= MAX_ITEM_PROTO_SPELLS)
            return luaL_argerror(E->L, 2, "valid SpellIndex expected");

        Eluna::Push(E->L, item->GetTemplate()->Spells[index].SpellId);
        return 1;
    }

    int GetSpellTrigger(Eluna* E, Item* item)
    {
        uint32 index = Eluna::CHECKVAL<uint32>(E->L, 2);
        if (index >= MAX_ITEM_PROTO_SPELLS)
            return luaL_argerror(E->L, 2, "valid SpellIndex expected");

        Eluna::Push(E->L, item->GetTemplate()->Spells[index].SpellTrigger);
        return 1;
    }

    int GetClass(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetTemplate()->Class);
        return 1;
    }

    int GetSubClass(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetTemplate()->SubClass);
        return 1;
    }

    int GetName(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetTemplate()->Name1);
        return 1;
    }

    int GetDisplayId(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetTemplate()->DisplayInfoID);
        return 1;
    }

    int GetQuality(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetTemplate()->Quality);
        return 1;
    }

    int GetBuyCount(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetTemplate()->BuyCount);
        return 1;
    }

    int GetBuyPrice(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetTemplate()->BuyPrice);
        return 1;
    }

    int GetSellPrice(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetTemplate()->SellPrice);
        return 1;
    }

    int GetInventoryType(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetTemplate()->InventoryType);
        return 1;
    }

    int GetAllowableClass(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetTemplate()->AllowableClass);
        return 1;
    }

    int GetAllowableRace(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetTemplate()->AllowableRace);
        return 1;
    }

    int GetItemLevel(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetTemplate()->ItemLevel);
        return 1;
    }

    int GetRequiredLevel(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetTemplate()->RequiredLevel);
        return 1;
    }

#ifdef WOTLK
    int GetStatsCount(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetTemplate()->StatsCount);
        return 1;
    }
#endif

    int GetRandomProperty(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetTemplate()->RandomProperty);
        return 1;
    }

#ifndef CLASSIC
    int GetRandomSuffix(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetTemplate()->RandomSuffix);
        return 1;
    }
#endif

    int GetItemSet(Eluna* E, Item* item)
    {
        Eluna::Push(E->L, item->GetTemplate()->ItemSet);
        return 1;
    }

    int GetBagSize(Eluna* E, Item* item)
    {
        if (Bag* bag = item->ToBag())
            Eluna::Push(E->L, bag->GetBagSize());
        else
            Eluna::Push(E->L, 0);
        return 1;
    }

    /* SETTERS */
    int SetOwner(Eluna* E, Item* item)
    {
        Player* player = Eluna::CHECKOBJ<Player>(E->L, 2);
#ifndef TRINITY
        item->SetOwnerGuid(player->GET_GUID());
#else
        item->SetOwnerGUID(player->GET_GUID());
#endif
        return 0;
    }

    int SetBinding(Eluna* E, Item* item)
    {
        bool soulbound = Eluna::CHECKVAL<bool>(E->L, 2);

        item->SetBinding(soulbound);
        return 0;
    }

    int SetCount(Eluna* E, Item* item)
    {
        uint32 count = Eluna::CHECKVAL<uint32>(E->L, 2);
        item->SetCount(count);
        return 0;
    }

    int SetEnchantment(Eluna* E, Item* item)
    {
        Player* owner = item->GetOwner();
        if (!owner)
        {
            Eluna::Push(E->L, false);
            return 1;
        }

        uint32 enchant = Eluna::CHECKVAL<uint32>(E->L, 2);
        if (!sSpellItemEnchantmentStore.LookupEntry(enchant))
        {
            Eluna::Push(E->L, false);
            return 1;
        }

        EnchantmentSlot slot = (EnchantmentSlot)Eluna::CHECKVAL<uint32>(E->L, 3);
        if (slot >= MAX_INSPECTED_ENCHANTMENT_SLOT)
            return luaL_argerror(E->L, 2, "valid EnchantmentSlot expected");

        owner->ApplyEnchantment(item, slot, false);
        item->SetEnchantment(slot, enchant, 0, 0);
        owner->ApplyEnchantment(item, slot, true);
        Eluna::Push(E->L, true);
        return 1;
    }

    /* OTHER */
    int ClearEnchantment(Eluna* E, Item* item)
    {
        Player* owner = item->GetOwner();
        if (!owner)
        {
            Eluna::Push(E->L, false);
            return 1;
        }

        EnchantmentSlot slot = (EnchantmentSlot)Eluna::CHECKVAL<uint32>(E->L, 2);
        if (slot >= MAX_INSPECTED_ENCHANTMENT_SLOT)
            return luaL_argerror(E->L, 2, "valid EnchantmentSlot expected");

        if (!item->GetEnchantmentId(slot))
        {
            Eluna::Push(E->L, false);
            return 1;
        }

        owner->ApplyEnchantment(item, slot, false);
        item->ClearEnchantment(slot);
        Eluna::Push(E->L, true);
        return 1;
    }

    int SaveToDB(Eluna* /*E*/, Item* item)
    {
#ifndef TRINITY
        item->SaveToDB();
#else
        SQLTransaction trans = SQLTransaction(NULL);
        item->SaveToDB(trans);
#endif
        return 0;
    }
};
#endif
