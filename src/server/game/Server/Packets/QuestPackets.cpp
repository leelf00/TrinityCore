/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "QuestPackets.h"
#include "Util.h"

namespace WorldPackets::Quest
{
ByteBuffer& operator<<(ByteBuffer& data, QuestCompleteDisplaySpell const& questDisplaySpell)
{
    data << int32(questDisplaySpell.SpellID);
    data << int32(questDisplaySpell.PlayerConditionID);
    data << int32(questDisplaySpell.Type);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, ConditionalQuestText const& conditionalQuestText)
{
    data << int32(conditionalQuestText.PlayerConditionID);
    data << int32(conditionalQuestText.QuestGiverCreatureID);
    data.WriteBits(conditionalQuestText.Text.length(), 12);
    data.FlushBits();

    data.WriteString(conditionalQuestText.Text);

    return data;
}

void QuestGiverStatusQuery::Read()
{
    _worldPacket >> QuestGiverGUID;
}

WorldPacket const* QuestGiverStatus::Write()
{
    _worldPacket << QuestGiver.Guid;
    _worldPacket << uint64(QuestGiver.Status);

    return &_worldPacket;
}

WorldPacket const* QuestGiverStatusMultiple::Write()
{
    _worldPacket << int32(QuestGiver.size());
    for (QuestGiverInfo const& questGiver : QuestGiver)
    {
        _worldPacket << questGiver.Guid;
        _worldPacket << uint64(questGiver.Status);
    }

    return &_worldPacket;
}

void QuestGiverHello::Read()
{
    _worldPacket >> QuestGiverGUID;
}

void QueryQuestInfo::Read()
{
    _worldPacket >> QuestID;
    _worldPacket >> QuestGiver;
}

WorldPacket const* QueryQuestInfoResponse::Write()
{
    _worldPacket << uint32(QuestID);

    _worldPacket.WriteBit(Allow);
    _worldPacket.FlushBits();

    if (Allow)
    {
        _worldPacket << int32(Info.QuestID);
        _worldPacket << int32(Info.QuestType);
        _worldPacket << int32(Info.QuestPackageID);
        _worldPacket << int32(Info.ContentTuningID);
        _worldPacket << int32(Info.QuestSortID);
        _worldPacket << int32(Info.QuestInfoID);
        _worldPacket << int32(Info.SuggestedGroupNum);
        _worldPacket << int32(Info.RewardNextQuest);
        _worldPacket << int32(Info.RewardXPDifficulty);
        _worldPacket << float(Info.RewardXPMultiplier);
        _worldPacket << int32(Info.RewardMoney);
        _worldPacket << int32(Info.RewardMoneyDifficulty);
        _worldPacket << float(Info.RewardMoneyMultiplier);
        _worldPacket << int32(Info.RewardBonusMoney);
        _worldPacket << uint32(Info.RewardDisplaySpell.size());
        _worldPacket << int32(Info.RewardSpell);
        _worldPacket << int32(Info.RewardHonor);
        _worldPacket << float(Info.RewardKillHonor);
        _worldPacket << int32(Info.RewardArtifactXPDifficulty);
        _worldPacket << float(Info.RewardArtifactXPMultiplier);
        _worldPacket << int32(Info.RewardArtifactCategoryID);
        _worldPacket << int32(Info.StartItem);
        _worldPacket << uint32(Info.Flags);
        _worldPacket << uint32(Info.FlagsEx);
        _worldPacket << uint32(Info.FlagsEx2);
        _worldPacket << uint32(Info.FlagsEx3);

        for (uint32 i = 0; i < QUEST_REWARD_ITEM_COUNT; ++i)
        {
            _worldPacket << int32(Info.RewardItems[i]);
            _worldPacket << int32(Info.RewardAmount[i]);
            _worldPacket << int32(Info.ItemDrop[i]);
            _worldPacket << int32(Info.ItemDropQuantity[i]);
        }

        for (uint32 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        {
            _worldPacket << int32(Info.UnfilteredChoiceItems[i].ItemID);
            _worldPacket << int32(Info.UnfilteredChoiceItems[i].Quantity);
            _worldPacket << int32(Info.UnfilteredChoiceItems[i].DisplayID);
        }

        _worldPacket << int32(Info.POIContinent);
        _worldPacket << float(Info.POIx);
        _worldPacket << float(Info.POIy);
        _worldPacket << int32(Info.POIPriority);

        _worldPacket << int32(Info.RewardTitle);
        _worldPacket << int32(Info.RewardArenaPoints);
        _worldPacket << int32(Info.RewardSkillLineID);
        _worldPacket << int32(Info.RewardNumSkillUps);

        _worldPacket << int32(Info.PortraitGiver);
        _worldPacket << int32(Info.PortraitGiverMount);
        _worldPacket << int32(Info.PortraitGiverModelSceneID);
        _worldPacket << int32(Info.PortraitTurnIn);

        for (uint32 i = 0; i < QUEST_REWARD_REPUTATIONS_COUNT; ++i)
        {
            _worldPacket << int32(Info.RewardFactionID[i]);
            _worldPacket << int32(Info.RewardFactionValue[i]);
            _worldPacket << int32(Info.RewardFactionOverride[i]);
            _worldPacket << int32(Info.RewardFactionCapIn[i]);
        }

        _worldPacket << int32(Info.RewardFactionFlags);

        for (uint32 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        {
            _worldPacket << int32(Info.RewardCurrencyID[i]);
            _worldPacket << int32(Info.RewardCurrencyQty[i]);
        }

        _worldPacket << int32(Info.AcceptedSoundKitID);
        _worldPacket << int32(Info.CompleteSoundKitID);

        _worldPacket << int32(Info.AreaGroupID);
        _worldPacket << int64(Info.TimeAllowed);

        _worldPacket << uint32(Info.Objectives.size());
        _worldPacket << uint64(Info.AllowableRaces.RawValue);
        _worldPacket << uint32(Info.TreasurePickerID.size());
        _worldPacket << uint32(Info.TreasurePickerID2.size());
        _worldPacket << int32(Info.Expansion);
        _worldPacket << int32(Info.ManagedWorldStateID);
        _worldPacket << int32(Info.QuestSessionBonus);
        _worldPacket << int32(Info.QuestGiverCreatureID);

        _worldPacket << uint32(Info.ConditionalQuestDescription.size());
        _worldPacket << uint32(Info.ConditionalQuestCompletionLog.size());

        for (QuestCompleteDisplaySpell const& rewardDisplaySpell : Info.RewardDisplaySpell)
            _worldPacket << rewardDisplaySpell;

        if (!Info.TreasurePickerID.empty())
            _worldPacket.append(Info.TreasurePickerID.data(), Info.TreasurePickerID.size());

        if (!Info.TreasurePickerID2.empty())
            _worldPacket.append(Info.TreasurePickerID2.data(), Info.TreasurePickerID2.size());

        _worldPacket << BitsSize<9>(Info.LogTitle);
        _worldPacket << BitsSize<12>(Info.LogDescription);
        _worldPacket << BitsSize<12>(Info.QuestDescription);
        _worldPacket << BitsSize<9>(Info.AreaDescription);
        _worldPacket << BitsSize<10>(Info.PortraitGiverText);
        _worldPacket << BitsSize<8>(Info.PortraitGiverName);
        _worldPacket << BitsSize<10>(Info.PortraitTurnInText);
        _worldPacket << BitsSize<8>(Info.PortraitTurnInName);
        _worldPacket << BitsSize<11>(Info.QuestCompletionLog);
        _worldPacket << Bits<1>(Info.ResetByScheduler);
        _worldPacket << Bits<1>(Info.ReadyForTranslation);
        _worldPacket.FlushBits();

        for (QuestObjective const& questObjective : Info.Objectives)
        {
            _worldPacket << uint32(questObjective.ID);
            _worldPacket << int32(questObjective.Type);
            _worldPacket << int8(questObjective.StorageIndex);
            _worldPacket << int32(questObjective.ObjectID);
            _worldPacket << int32(questObjective.Amount);
            _worldPacket << uint32(questObjective.Flags);
            _worldPacket << uint32(questObjective.Flags2);
            _worldPacket << float(questObjective.ProgressBarWeight);

            _worldPacket << int32(questObjective.VisualEffects.size());
            for (int32 visualEffect : questObjective.VisualEffects)
                _worldPacket << int32(visualEffect);

            _worldPacket << BitsSize<8>(questObjective.Description);
            _worldPacket.FlushBits();

            _worldPacket.WriteString(questObjective.Description);
        }

        _worldPacket.WriteString(Info.LogTitle);
        _worldPacket.WriteString(Info.LogDescription);
        _worldPacket.WriteString(Info.QuestDescription);
        _worldPacket.WriteString(Info.AreaDescription);
        _worldPacket.WriteString(Info.PortraitGiverText);
        _worldPacket.WriteString(Info.PortraitGiverName);
        _worldPacket.WriteString(Info.PortraitTurnInText);
        _worldPacket.WriteString(Info.PortraitTurnInName);
        _worldPacket.WriteString(Info.QuestCompletionLog);

        for (ConditionalQuestText const& conditionalQuestText : Info.ConditionalQuestDescription)
            _worldPacket << conditionalQuestText;

        for (ConditionalQuestText const& conditionalQuestText : Info.ConditionalQuestCompletionLog)
            _worldPacket << conditionalQuestText;
    }

    return &_worldPacket;
}

WorldPacket const* QuestUpdateAddCredit::Write()
{
    _worldPacket << VictimGUID;
    _worldPacket << int32(QuestID);
    _worldPacket << int32(ObjectID);
    _worldPacket << uint16(Count);
    _worldPacket << uint16(Required);
    _worldPacket << uint8(ObjectiveType);

    return &_worldPacket;
};

WorldPacket const* QuestUpdateAddCreditSimple::Write()
{
    _worldPacket << int32(QuestID);
    _worldPacket << int32(ObjectID);
    _worldPacket << uint8(ObjectiveType);

    return &_worldPacket;
}

WorldPacket const* QuestUpdateAddPvPCredit::Write()
{
    _worldPacket << int32(QuestID);
    _worldPacket << uint16(Count);

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, QuestRewardItem const& questRewardItem)
{
    data << int32(questRewardItem.ItemID);
    data << int32(questRewardItem.ItemQty);
    data << OptionalInit(questRewardItem.ContextFlags);
    data.FlushBits();
    if (questRewardItem.ContextFlags)
        data << As<int32>(*questRewardItem.ContextFlags);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, QuestChoiceItem const& questChoiceItem)
{
    data << Bits<2>(questChoiceItem.LootItemType);
    data << OptionalInit(questChoiceItem.ContextFlags);
    data << questChoiceItem.Item;
    data << int32(questChoiceItem.Quantity);
    if (questChoiceItem.ContextFlags)
        data << As<int32>(*questChoiceItem.ContextFlags);

    return data;
}

ByteBuffer& operator>>(ByteBuffer& data, QuestChoiceItem& questChoiceItem)
{
    data.ResetBitPos();
    data >> Bits<2>(questChoiceItem.LootItemType);
    data >> OptionalInit(questChoiceItem.ContextFlags);
    data >> questChoiceItem.Item;
    data >> questChoiceItem.Quantity;
    if (questChoiceItem.ContextFlags)
        data >> As<int32>(*questChoiceItem.ContextFlags);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, QuestRewardCurrency const& questRewardCurrency)
{
    data << int32(questRewardCurrency.CurrencyID);
    data << int32(questRewardCurrency.CurrencyQty);
    data << int32(questRewardCurrency.BonusQty);
    data << OptionalInit(questRewardCurrency.ContextFlags);
    data.FlushBits();
    if (questRewardCurrency.ContextFlags)
        data << As<int32>(*questRewardCurrency.ContextFlags);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, QuestRewards const& questRewards)
{
    for (QuestRewardItem const& item : questRewards.Items)
        data << item;

    data << int32(questRewards.ChoiceItemCount);
    data << int32(questRewards.ItemCount);
    data << int32(questRewards.Money);
    data << int32(questRewards.XP);
    data << int64(questRewards.ArtifactXP);
    data << int32(questRewards.ArtifactCategoryID);
    data << int32(questRewards.Honor);
    data << int32(questRewards.Title);
    data << int32(questRewards.FactionFlags);

    for (uint32 i = 0; i < QUEST_REWARD_REPUTATIONS_COUNT; ++i)
    {
        data << int32(questRewards.FactionID[i]);
        data << int32(questRewards.FactionValue[i]);
        data << int32(questRewards.FactionOverride[i]);
        data << int32(questRewards.FactionCapIn[i]);
    }

    data.append(questRewards.SpellCompletionDisplayID.data(), questRewards.SpellCompletionDisplayID.size());

    data << int32(questRewards.SpellCompletionID);

    data << int32(questRewards.SkillLineID);
    data << int32(questRewards.NumSkillUps);
    data << uint32(questRewards.TreasurePickerID.size());
    if (!questRewards.TreasurePickerID.empty())
        data.append(questRewards.TreasurePickerID.data(), questRewards.TreasurePickerID.size());

    for (QuestRewardCurrency const& currency : questRewards.Currencies)
        data << currency;

    data << Bits<1>(questRewards.IsBoostSpell);
    data.FlushBits();

    for (QuestChoiceItem const& choiceItem : questRewards.ChoiceItems)
        data << choiceItem;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, QuestGiverOfferReward const& offer)
{
    data << offer.Rewards; // QuestRewards
    data << int32(offer.Emotes.size());
    data << offer.QuestGiverGUID;
    data.append(offer.QuestFlags);
    data << int32(offer.QuestGiverCreatureID);
    data << int32(offer.QuestID);
    data << int32(offer.SuggestedPartyMembers);
    data << int32(offer.QuestInfoID);
    for (QuestDescEmote const& emote : offer.Emotes)
    {
        data << int32(emote.Type);
        data << uint32(emote.Delay);
    }

    data << Bits<1>(offer.AutoLaunched);
    data << Bits<1>(false); // Unused
    data << Bits<1>(offer.ResetByScheduler);
    data.FlushBits();

    return data;
}

WorldPacket const* QuestGiverOfferRewardMessage::Write()
{
    _worldPacket << QuestData; // QuestGiverOfferReward
    _worldPacket << int32(QuestPackageID);
    _worldPacket << int32(PortraitGiver);
    _worldPacket << int32(PortraitGiverMount);
    _worldPacket << int32(PortraitGiverModelSceneID);
    _worldPacket << int32(PortraitTurnIn);
    _worldPacket << int32(QuestGiverCreatureID);
    _worldPacket << uint32(ConditionalRewardText.size());

    _worldPacket << BitsSize<9>(QuestTitle);
    _worldPacket << BitsSize<12>(RewardText);
    _worldPacket << BitsSize<10>(PortraitGiverText);
    _worldPacket << BitsSize<8>(PortraitGiverName);
    _worldPacket << BitsSize<10>(PortraitTurnInText);
    _worldPacket << BitsSize<8>(PortraitTurnInName);
    _worldPacket.FlushBits();

    for (ConditionalQuestText const& conditionalQuestText : ConditionalRewardText)
        _worldPacket << conditionalQuestText;

    _worldPacket.WriteString(QuestTitle);
    _worldPacket.WriteString(RewardText);
    _worldPacket.WriteString(PortraitGiverText);
    _worldPacket.WriteString(PortraitGiverName);
    _worldPacket.WriteString(PortraitTurnInText);
    _worldPacket.WriteString(PortraitTurnInName);

    return &_worldPacket;
};

void QuestGiverChooseReward::Read()
{
    _worldPacket >> QuestGiverGUID;
    _worldPacket >> QuestID;
    _worldPacket >> Choice;
}

WorldPacket const* QuestGiverQuestComplete::Write()
{
    _worldPacket << int32(QuestID);
    _worldPacket << int32(XPReward);
    _worldPacket << int64(MoneyReward);
    _worldPacket << int32(SkillLineIDReward);
    _worldPacket << int32(NumSkillUpsReward);
    _worldPacket.WriteBit(UseQuestReward);
    _worldPacket.WriteBit(LaunchGossip);
    _worldPacket.WriteBit(LaunchQuest);
    _worldPacket.WriteBit(HideChatMessage);

    _worldPacket << ItemReward;

    return &_worldPacket;
}

void QuestGiverCompleteQuest::Read()
{
    _worldPacket >> QuestGiverGUID;
    _worldPacket >> QuestID;
    FromScript = _worldPacket.ReadBit();
}

void QuestGiverCloseQuest::Read()
{
    _worldPacket >> QuestID;
}

WorldPacket const* QuestGiverQuestDetails::Write()
{
    _worldPacket << QuestGiverGUID;
    _worldPacket << InformUnit;
    _worldPacket << int32(QuestID);
    _worldPacket << int32(QuestPackageID);
    _worldPacket << int32(PortraitGiver);
    _worldPacket << int32(PortraitGiverMount);
    _worldPacket << int32(PortraitGiverModelSceneID);
    _worldPacket << int32(PortraitTurnIn);
    _worldPacket.append(QuestFlags);
    _worldPacket << int32(SuggestedPartyMembers);
    _worldPacket << uint32(LearnSpells.size());
    _worldPacket << uint32(DescEmotes.size());
    _worldPacket << uint32(Objectives.size());
    _worldPacket << int32(QuestStartItemID);
    _worldPacket << int32(QuestInfoID);
    _worldPacket << int32(QuestSessionBonus);
    _worldPacket << int32(QuestGiverCreatureID);
    _worldPacket << uint32(ConditionalDescriptionText.size());

    for (int32 spell : LearnSpells)
        _worldPacket << int32(spell);

    for (QuestDescEmote const& emote : DescEmotes)
    {
        _worldPacket << int32(emote.Type);
        _worldPacket << uint32(emote.Delay);
    }

    for (QuestObjectiveSimple const& obj : Objectives)
    {
        _worldPacket << int32(obj.ID);
        _worldPacket << int32(obj.Type);
        _worldPacket << int32(obj.ObjectID);
        _worldPacket << int32(obj.Amount);
    }

    _worldPacket << BitsSize<9>(QuestTitle);
    _worldPacket << BitsSize<12>(DescriptionText);
    _worldPacket << BitsSize<12>(LogDescription);
    _worldPacket << BitsSize<10>(PortraitGiverText);
    _worldPacket << BitsSize<8>(PortraitGiverName);
    _worldPacket << BitsSize<10>(PortraitTurnInText);
    _worldPacket << BitsSize<8>(PortraitTurnInName);
    _worldPacket << Bits<1>(AutoLaunched);
    _worldPacket << Bits<1>(FromContentPush);
    _worldPacket << Bits<1>(false);   // unused in client
    _worldPacket << Bits<1>(ResetByScheduler);
    _worldPacket << Bits<1>(StartCheat);
    _worldPacket << Bits<1>(DisplayPopup);
    _worldPacket.FlushBits();

    _worldPacket << Rewards; // QuestRewards
    _worldPacket.WriteString(QuestTitle);
    _worldPacket.WriteString(DescriptionText);
    _worldPacket.WriteString(LogDescription);
    _worldPacket.WriteString(PortraitGiverText);
    _worldPacket.WriteString(PortraitGiverName);
    _worldPacket.WriteString(PortraitTurnInText);
    _worldPacket.WriteString(PortraitTurnInName);

    for (ConditionalQuestText const& conditionalQuestText : ConditionalDescriptionText)
        _worldPacket << conditionalQuestText;

    return &_worldPacket;
}

WorldPacket const* QuestGiverRequestItems::Write()
{
    _worldPacket << int32(Collect.size());
    _worldPacket << int32(Currency.size());
    _worldPacket << QuestGiverGUID;
    _worldPacket.append(QuestFlags);
    _worldPacket << int32(StatusFlags);
    _worldPacket << int32(QuestGiverCreatureID);
    _worldPacket << int32(QuestID);
    _worldPacket << int32(CompEmoteDelay);
    _worldPacket << int32(CompEmoteType);
    _worldPacket << int32(SuggestPartyMembers);
    _worldPacket << int32(MoneyToGet);
    _worldPacket << int32(QuestInfoID);

    for (QuestObjectiveCollect const& obj : Collect)
    {
        _worldPacket << int32(obj.ObjectID);
        _worldPacket << int32(obj.Amount);
        _worldPacket << uint32(obj.Flags);
    }

    for (QuestCurrency const& cur : Currency)
    {
        _worldPacket << int32(cur.CurrencyID);
        _worldPacket << int32(cur.Amount);
    }

    _worldPacket << Bits<1>(AutoLaunched);
    _worldPacket << Bits<1>(ResetByScheduler);
    _worldPacket.FlushBits();

    _worldPacket << int32(QuestGiverCreatureID);
    _worldPacket << uint32(ConditionalCompletionText.size());

    _worldPacket << BitsSize<9>(QuestTitle);
    _worldPacket << BitsSize<12>(CompletionText);
    _worldPacket.FlushBits();

    for (ConditionalQuestText const& conditionalQuestText : ConditionalCompletionText)
        _worldPacket << conditionalQuestText;

    _worldPacket.WriteString(QuestTitle);
    _worldPacket.WriteString(CompletionText);

    return &_worldPacket;
}

void QuestGiverRequestReward::Read()
{
    _worldPacket >> QuestGiverGUID;
    _worldPacket >> QuestID;
}

void QuestGiverQueryQuest::Read()
{
    _worldPacket >> QuestGiverGUID;
    _worldPacket >> QuestID;
    RespondToGiver = _worldPacket.ReadBit();
}

void QuestGiverAcceptQuest::Read()
{
    _worldPacket >> QuestGiverGUID;
    _worldPacket >> QuestID;
    StartCheat = _worldPacket.ReadBit();
}

void QuestLogRemoveQuest::Read()
{
    _worldPacket >> Entry;
}

WorldPacket const* QuestGiverQuestListMessage::Write()
{
    _worldPacket << QuestGiverGUID;
    _worldPacket << uint32(GreetEmoteDelay);
    _worldPacket << uint32(GreetEmoteType);
    _worldPacket << uint32(QuestDataText.size());
    _worldPacket.WriteBits(Greeting.size(), 11);
    _worldPacket.FlushBits();

    for (NPC::ClientGossipText const& gossip : QuestDataText)
        _worldPacket << gossip;

    _worldPacket.WriteString(Greeting);

    return &_worldPacket;
}

WorldPacket const* QuestUpdateComplete::Write()
{
    _worldPacket << int32(QuestID);

    return &_worldPacket;
}

WorldPacket const* QuestConfirmAcceptResponse::Write()
{
    _worldPacket << uint32(QuestID);
    _worldPacket << InitiatedBy;

    _worldPacket.WriteBits(QuestTitle.size(), 10);
    _worldPacket.FlushBits();

    _worldPacket.WriteString(QuestTitle);

    return &_worldPacket;
}

void QuestConfirmAccept::Read()
{
    _worldPacket >> QuestID;
}

WorldPacket const* QuestPushResultResponse::Write()
{
    _worldPacket << SenderGUID;
    _worldPacket << uint8(Result);

    _worldPacket.WriteBits(QuestTitle.size(), 9);
    _worldPacket.FlushBits();

    _worldPacket.WriteString(QuestTitle);

    return &_worldPacket;
}

void QuestPushResult::Read()
{
    _worldPacket >> SenderGUID;
    _worldPacket >> QuestID;
    _worldPacket >> Result;
}

WorldPacket const* QuestGiverInvalidQuest::Write()
{
    _worldPacket << uint32(Reason);
    _worldPacket << int32(ContributionRewardID);

    _worldPacket.WriteBit(SendErrorMessage);
    _worldPacket.WriteBits(ReasonText.length(), 9);

    _worldPacket.FlushBits();

    _worldPacket.WriteString(ReasonText);

    return &_worldPacket;
}

WorldPacket const* QuestUpdateFailedTimer::Write()
{
    _worldPacket << uint32(QuestID);

    return &_worldPacket;
}

WorldPacket const* QuestGiverQuestFailed::Write()
{
    _worldPacket << uint32(QuestID);
    _worldPacket << uint32(Reason);

    return &_worldPacket;
}

void PushQuestToParty::Read()
{
    _worldPacket >> QuestID;
}

WorldPacket const* DailyQuestsReset::Write()
{
    _worldPacket << int32(Count);

    return &_worldPacket;
}

WorldPacket const* QuestForceRemoved::Write()
{
    _worldPacket << int32(QuestID);

    return &_worldPacket;
}

WorldPacket const* WorldQuestUpdateResponse::Write()
{
    _worldPacket << uint32(WorldQuestUpdates.size());

    for (WorldQuestUpdateInfo const& worldQuestUpdate : WorldQuestUpdates)
    {
        _worldPacket << worldQuestUpdate.LastUpdate;
        _worldPacket << uint32(worldQuestUpdate.QuestID);
        _worldPacket << uint32(worldQuestUpdate.Timer);
        _worldPacket << int32(worldQuestUpdate.VariableID);
        _worldPacket << int32(worldQuestUpdate.Value);
    }

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, PlayerChoiceResponseRewardEntry const& playerChoiceResponseRewardEntry)
{
    data << playerChoiceResponseRewardEntry.Item;
    data << int32(playerChoiceResponseRewardEntry.Quantity);
    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, PlayerChoiceResponseReward const& playerChoiceResponseReward)
{
    data << int32(playerChoiceResponseReward.TitleID);
    data << int32(playerChoiceResponseReward.PackageID);
    data << int32(playerChoiceResponseReward.SkillLineID);
    data << uint32(playerChoiceResponseReward.SkillPointCount);
    data << uint32(playerChoiceResponseReward.ArenaPointCount);
    data << uint32(playerChoiceResponseReward.HonorPointCount);
    data << uint64(playerChoiceResponseReward.Money);
    data << uint32(playerChoiceResponseReward.Xp);
    data << uint32(playerChoiceResponseReward.Items.size());
    data << uint32(playerChoiceResponseReward.Currencies.size());
    data << uint32(playerChoiceResponseReward.Factions.size());
    data << uint32(playerChoiceResponseReward.ItemChoices.size());

    for (PlayerChoiceResponseRewardEntry const& item : playerChoiceResponseReward.Items)
        data << item;

    for (PlayerChoiceResponseRewardEntry const& currency : playerChoiceResponseReward.Currencies)
        data << currency;

    for (PlayerChoiceResponseRewardEntry const& faction : playerChoiceResponseReward.Factions)
        data << faction;

    for (PlayerChoiceResponseRewardEntry const& itemChoice : playerChoiceResponseReward.ItemChoices)
        data << itemChoice;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, PlayerChoiceResponseMawPower const& playerChoiceResponseMawPower)
{
    data << int32(playerChoiceResponseMawPower.Unused901_1);
    data << int32(playerChoiceResponseMawPower.TypeArtFileID);
    data << int32(playerChoiceResponseMawPower.Unused901_2);
    data << int32(playerChoiceResponseMawPower.SpellID);
    data << int32(playerChoiceResponseMawPower.MaxStacks);
    data.WriteBit(playerChoiceResponseMawPower.Rarity.has_value());
    data.FlushBits();

    if (playerChoiceResponseMawPower.Rarity)
        data << int32(*playerChoiceResponseMawPower.Rarity);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, PlayerChoiceResponse const& playerChoiceResponse)
{
    data << int32(playerChoiceResponse.ResponseID);
    data << uint16(playerChoiceResponse.ResponseIdentifier);
    data << int32(playerChoiceResponse.ChoiceArtFileID);
    data << int32(playerChoiceResponse.Flags);
    data << uint32(playerChoiceResponse.WidgetSetID);
    data << uint32(playerChoiceResponse.UiTextureAtlasElementID);
    data << uint32(playerChoiceResponse.SoundKitID);
    data << uint8(playerChoiceResponse.GroupID);
    data << int32(playerChoiceResponse.UiTextureKitID);

    data.WriteBits(playerChoiceResponse.Answer.length(), 9);
    data.WriteBits(playerChoiceResponse.Header.length(), 9);
    data.WriteBits(playerChoiceResponse.SubHeader.length() , 7);
    data.WriteBits(playerChoiceResponse.ButtonTooltip.length(), 9);
    data.WriteBits(playerChoiceResponse.Description.length(), 11);
    data.WriteBits(playerChoiceResponse.Confirmation.length(), 7);
    data.WriteBit(playerChoiceResponse.RewardQuestID.has_value());
    data.WriteBit(playerChoiceResponse.Reward.has_value());
    data.WriteBit(playerChoiceResponse.MawPower.has_value());
    data.FlushBits();

    if (playerChoiceResponse.Reward)
        data << *playerChoiceResponse.Reward;

    data.WriteString(playerChoiceResponse.Answer);
    data.WriteString(playerChoiceResponse.Header);
    data.WriteString(playerChoiceResponse.SubHeader);
    data.WriteString(playerChoiceResponse.ButtonTooltip);
    data.WriteString(playerChoiceResponse.Description);
    data.WriteString(playerChoiceResponse.Confirmation);

    if (playerChoiceResponse.RewardQuestID)
        data << uint32(*playerChoiceResponse.RewardQuestID);

    if (playerChoiceResponse.MawPower)
        data << *playerChoiceResponse.MawPower;

    return data;
}

WorldPacket const* DisplayPlayerChoice::Write()
{
    _worldPacket << int32(ChoiceID);
    _worldPacket << uint32(Responses.size());
    _worldPacket << SenderGUID;
    _worldPacket << int32(UiTextureKitID);
    _worldPacket << uint32(SoundKitID);
    _worldPacket << uint32(CloseUISoundKitID);
    _worldPacket << uint8(NumRerolls);
    _worldPacket << Duration;
    _worldPacket.WriteBits(Question.length(), 8);
    _worldPacket.WriteBits(PendingChoiceText.length(), 8);
    _worldPacket.WriteBit(InfiniteRange);
    _worldPacket.WriteBit(HideWarboardHeader);
    _worldPacket.WriteBit(KeepOpenAfterChoice);
    _worldPacket.WriteBit(Unknown_1115_1);
    _worldPacket.WriteBit(Unknown_1115_2);
    _worldPacket.FlushBits();

    for (PlayerChoiceResponse const& response : Responses)
        _worldPacket << response;

    _worldPacket.WriteString(Question);
    _worldPacket.WriteString(PendingChoiceText);
    return &_worldPacket;
}

void ChoiceResponse::Read()
{
    _worldPacket >> ChoiceID;
    _worldPacket >> ResponseIdentifier;
    IsReroll = _worldPacket.ReadBit();
}

WorldPacket const* UiMapQuestLinesResponse::Write()
{
    _worldPacket << int32(UiMapID);
    _worldPacket << uint32(QuestLineXQuestIDs.size());
    _worldPacket << uint32(QuestIDs.size());
    _worldPacket << uint32(QuestLineIDs.size());

    for (uint32 const& questLineQuestID : QuestLineXQuestIDs)
        _worldPacket << uint32(questLineQuestID);

    for (uint32 const& questID : QuestIDs)
        _worldPacket << uint32(questID);

    for (uint32 const& questLineID : QuestLineIDs)
        _worldPacket << uint32(questLineID);

    return &_worldPacket;
}

void UiMapQuestLinesRequest::Read()
{
    _worldPacket >> UiMapID;
}

ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::Quest::SpawnTrackingRequestInfo& spawnTrackingRequestInfo)
{
    data >> spawnTrackingRequestInfo.ObjectTypeMask;
    data >> spawnTrackingRequestInfo.ObjectID;
    data >> spawnTrackingRequestInfo.SpawnTrackingID;
    return data;
}

void SpawnTrackingUpdate::Read()
{
    SpawnTrackingRequests.resize(_worldPacket.read<uint32>());
    for (SpawnTrackingRequestInfo& spawnTrackingRequestInfo : SpawnTrackingRequests)
        _worldPacket >> spawnTrackingRequestInfo;
}

ByteBuffer& operator<<(ByteBuffer& data, SpawnTrackingResponseInfo const& spawnTrackingResponseInfo)
{
    data << uint32(spawnTrackingResponseInfo.SpawnTrackingID);
    data << int32(spawnTrackingResponseInfo.ObjectID);
    data << int32(spawnTrackingResponseInfo.PhaseID);
    data << int32(spawnTrackingResponseInfo.PhaseGroupID);
    data << int32(spawnTrackingResponseInfo.PhaseUseFlags);

    data << Bits<1>(spawnTrackingResponseInfo.Visible);
    data.FlushBits();

    return data;
}

WorldPacket const* QuestPOIUpdateResponse::Write()
{
    _worldPacket << uint32(SpawnTrackingResponses.size());

    for (SpawnTrackingResponseInfo const& spawnTrackingResponseInfo : SpawnTrackingResponses)
        _worldPacket << spawnTrackingResponseInfo;

    return &_worldPacket;
}

WorldPacket const* ForceSpawnTrackingUpdate::Write()
{
    _worldPacket << int32(QuestID);

    return &_worldPacket;
}
}
