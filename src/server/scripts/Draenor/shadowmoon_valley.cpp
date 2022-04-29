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

/* Start of Things to Come. Early Development Stage */

#include "Conversation.h"
#include "CreatureTextMgr.h"
#include "GameObject.h"
#include "MapManager.h"
#include "ObjectMgr.h"
#include "PhasingHandler.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "ScriptedGossip.h"
#include "SpellScript.h"

/* Start of Finding a Foothold */

enum ShadowmoonIntroQuest
{
    QUEST_FINDING_A_FOOTHOLD                = 34582
};

enum ShadowmoonIntroNPCS
{
    NPC_VELEN                              = 79635,
    NPC_VINDICATOR_MARAAD                   = 79655,
    NPC_YREL                                = 79656,
    NPC_ARCHMAGE_KHADGAR                    = 79657,
    NPC_FINDING_A_FOOTHOLD_KILL_CREDIT      = 79697,
};

enum ShadowmoonIntroCreatureText
{
    SAY_LINE_YREL_0 = 0,
    SAY_LINE_VELEN_0 = 0,
    SAY_LINE_VELEN_1 = 1,
    SAY_LINE_MARAAD_0 = 0
};

enum ShadowmoonIntroEventIds
{
    EVENT_VELEN_MOVE_TO_POS = 1,
    EVENT_VELEN_SAY_LINE_1,
    EVENT_YREL_MOVE_TO_POS,
    EVENT_KHADGAR_ACTIONS
};

enum ShadowmoonIntroActionIds
{
    ACTION_YREL_MOVE_TO_POS = 1,
    ACTION_YREL_SAY_LINE_1,
    ACTION_KHADGAR_ITEM_SLOT_CHANGE,
    ACTION_KHADGAR_DISPLAY_ID_CHANGE,
    ACTION_KHADGAR_WAYPOINTS
};

enum ShadowmoonIntroCreaturePos
{
    POINT_VELEN_1,
    POINT_YREL_1
};

enum ShadowmoonIntroPhases
{
   QUEST_STEP_THREE_PROPHET_PHASE = 3329
};

Position const VelenPos = {298.3508, 461.69098, 8.652778, 0.74855065};  /* Spawn position for Vellen */
Position const YrelPos = {2306.0105, 457.5521, 6.9606037, 2.2839944};  /* Spawn position for Yrel */
Position const MaraadPos = {2305.837, 461.2882, 7.5034723, 3.1415927};  /* Spawn position for Maraad */
Position const KhadgarPos = {2298.337, 455.1875, 8.322916, 2.0769417};  /* Spawn position for Khadgar */

class npc_velen_shadowmoon_79206 : public CreatureScript
{
public:
    npc_velen_shadowmoon_79206() : CreatureScript("npc_velen_shadowmoon_79206") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (Quest->GetQuestId() != QUEST_FINDING_A_FOOTHOLD)
        {
            me->SummonCreature(NPC_VELEN, VelenPos, TEMPSUMMON_MANUAL_DESPAWN, 0, 0, true);
            me->SummonCreature(NPC_YREL, YrelPos, TEMPSUMMON_MANUAL_DESPAWN, 0, 0, true);
            me->SummonCreature(NPC_VINDICATOR_MARAAD, MaraadPos, TEMPSUMMON_MANUAL_DESPAWN, 0, 0, true);
            me->SummonCreature(NPC_ARCHMAGE_KHADGAR, KhadgarPos, TEMPSUMMON_MANUAL_DESPAWN, 0, 0, true);
            /* TODO REMOVE PHASE 3329 */
        }
    }
};

/* 79635 - Prophet Velen in Shadowmoon Valley Intro */
struct npc_velen_shadowmoon_start : public ScriptedAI
{
       npc_velen_shadowmoon_start(Creature* creature) : ScriptedAI(creature) { }

       Position const VelenPath01[0] =
       {

       }

       using VelenPath01Size = std::extent<decltype(VelenPath01)>;

       Position const VelenPath02[0] =
       {

       }

       using VelenPath02Size = std::extent<decltype(VelenPath02)>;

       void Reset() override
       {
           _events.Reset();
       }

       void JustSummoned(Creature* /*summoned*/) { }
       {

       }

       void SetData(uint32 type, uint32 data) override
       {
           if (Creature* yrel = me->FindNearestCreature(NPC_YREL, 20.0f))
           {
               if (Creature* maraad = me->FindNearestCreature(NPC_VINDICATOR_MARAAD, 20.0f))
               {
                   if (Creature* khadgar = me->FindNearestCreature(NPC_ARCHMAGE_KHADGAR, 20.0f))
                   {

                       me->SetActive(true);
                       me->SetFarVisible(true);
                       _yrel = yrel->GetGUID();
                       yrel->SetActive(true);
                       yrel->SetFarVisible(true);
                       _maraad = maraad->GetGUID();
                       maraad->SetActive(true);
                       maraad->SetFarVisible(true);
                       _khadgar = khadgar->GetGUID();
                       khadgar->SetActive(true);
                       khadgar->SetFarVisible(true);
                   }
               }
           }

           if (!_yrel || !_maraad)
             return;
       }

       void sQuestAccept(Player* player, Quest const* /*quest*/) override
       {
           if (quest->GetQuestId() != QUEST_FINDING_A_FOOTHOLD)
           {
                events.ScheduleEvent(EVENT_VELEN_MOVE_TO_POS, 3s);
                events.ScheduleEvent(EVENT_YREL_MOVE_TO_POS, 3s);
           }
        }

       void UpdateAI(uint32 diff) override
       {
           _events.Update(diff);

           while (uint32 eventId = _events.ExecuteEvent())
           {
               switch (eventId)
               {
                   case EVENT_VELEN_MOVE_TO_POS:
                   {
                       
                       break;
                   }
                   case EVENT_YREL_MOVE_TO_POS:
                   {
                       if (Creature* creature = ObjectAccessor::GetCreature(*me, (NPC_VELEN_FOLLOWER_YREL)))
                           creature->AI()->DoAction(ACTION_YREL_MOVE_TO_POS);
                           break;
                   }
                   case EVENT_VELEN_SAY_LINE_1:
                   {
                       Talk(SAY_LINE_VELEN_0);
                       break;
                   }
                   case EVENT_KHADGAR_ACTIONS:
                   {

                   }
               }
           }
       }

       void WaypointReached(uint32 waypointId, uint32 /*path*/) override
       {
           switch (waypointId)
           {

           }
       }

       void LastWaypointReached()
       {
           me->DespawnOrUnsummon();
       }

};
/* 79656 - Yrel in Shadowmoon Valley Intro */
struct npc_yrel_shadowmoon_start : public ScriptedAI
{
    npc_yrel_shadowmoon_start(Creature* creature) : ScriptedAI(creature) { }

    void Reset()
    {

    }

    void DoAction(int32 action) override
    {
        switch (action)
        {
            ACTION_YREL_MOVE_TO_POS:
            {
                
                break;
            }
            ACTION_YREL_SAY_LINE_1:
            {
                Talk(SAY_LINE_YREL_0);
                break;
            }
        }
    }

    void WaypointReached(uint32 waypointId, uint32 /*path*/) override
    {
        switch (waypointId)
        {

        }
    }

    void LastWaypointReached()
    {
        me->DespawnOrUnsummon();
    }
}

/* 79655 - Vindicator Maraad in Shadowmoon Valley Intro */
struct npc_vindicator_maraad_start : public ScriptedAI
{
    npc_vindicator_maraad_start(Creature* creature) : ScriptedAI(creature) { }

    void Reset()
    {

    }

    void DoAction(int32 action) override
    {
        switch (action)
        {

        }

    }

    void WaypointReached(uint32 waypointId, uint32 /*path*/) override
    {
        switch (waypointId)
        {

        }
    }

    void LastWaypointReached()
    {
        me->DespawnOrUnsummon();
    }
}

/* 79657 - Archmage Khadgar in Shadowmoon Valley Intro */
struct npc_archmage_khadgar_start : public ScriptedAI
{
    npc_archmage_khadgar_start(Creature* creature) : ScriptedAI(creature) { }

    void DoAction(int32 action) override
    {
        switch (action)
        {
            case ACTION_KHADGAR_ITEM_SLOT_CHANGE:
            {

            }
            case ACTION_KHADGAR_DISPLAY_ID_CHANGE:
            {

            }
            case ACTION_KHADGAR_WAYPOINTS:
            {
                
            }
        }

    }

    void LastWaypointReached()
    {
        me->DespawnOrUnsummon();
    }
}

/* Finding a Foothold End */

/* Lunarfall Intro Pre Garrison Start */

enum LunarfallQuests
{
    QUEST_FOR_THE_ALLIANCE                  = 34583,
    QUEST_LOOKING_FOR_LUMBER                = 34584,
    QUEST_RADENOUS_RAVENS                   = 34616,
    QUEST_QUAKEFIST                         = 34585,
    QUEST_ESTABLISH_YOUR_GARRISON           = 34586,
};

enum LunarfallCreatures
{
    NPC_VINDICATOR_MARAAD_LUNARFALL         = 79470,
    NPC_FOR_THE_ALLIANCE_PORTAL_KILL_CREDIT = 79433,
    NPC_ESTABLISH_YOUR_GARRISON_KILL_CREDIT = 79757
};

enum LunarfallCreatureText
{
    SAY_NPC_VINDICATOR_MARAAD_LINE_0 = 0,
    SAY_NPC_YREL_LINE_0 = 0,
};

enum LunarfallPhases
{

};

class npc_vindicator_maraad_lunarfall : public CreatureScript
{
public:
    npc_vindicator_maraad_lunarfall() : CreatureScript("npc_vindicator_maraad_lunarfall") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (Quest->GetQuestId() != QUEST_FOR_THE_ALLIANCE)
        {
            me->Say(SAY_NPC_VINDICATOR_MARAAD_LINE_0);
            return;
        }
    }
};

class npc_yrel_lunarfall : public CreatureScript
{
public:
    npc_yrel_lunarfall() : CreatureScript("npc_yrel_lunarfall") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (Quest->GetQuestId() != QUEST_QUAKEFIST)
        {
            me->Say(SAY_NPC_YREL_LINE_0);
            return;
        }
    }
}


/* Lunarfall Intro Pre Garrison End */

void AddSC_draenor_shadowmoon_valley()
{
    RegisterCreatureAI(npc_velen_shadowmoon_start);
    RegisterCreatureAI(npc_yrel_shadowmoon_start);
    RegisterCreatureAI(npc_vindicator_maraad_start);
    RegisterCreatureAI(npc_archmage_khadgar_start);
}