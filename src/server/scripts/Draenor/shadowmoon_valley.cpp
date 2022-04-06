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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "CreatureTextMgr.h"
#include "GameObjectAI.h"

enum ShadowmoonQuests
{
    QUEST_FINDING_A_FOOTHOLD                = 34582,
    QUEST_FOR_THE_ALLIANCE                  = 34583,
    QUEST_ESTABLISH_YOUR_GARRISON           = 34586,
};

enum ShadowmoonNPCS
{
    NPC_VELEN_FOLLOWER_MARAAD               = 79218,
    NPC_VELEN_FOLLOWER_YREL                 = 79219,
    NPC_FINDING_A_FOOTHOLD_KILL_CREDIT      = 79697,
    NPC_FOR_THE_ALLIANCE_PORTAL_KILL_CREDIT = 79433,
    NPC_ESTABLISH_YOUR_GARRISON_KILL_CREDIT = 79757,
};

// 79206 - Prophet Velen in Shadowmoon Valley Intro
class npc_velen_shadowmoon_start : public CreatureScript
{
public:
    npc_velen_shadowmoon_begin() : CreatureScript("npc_velen_shadowmoon_start") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
  
    }

};

void AddSC_draenor_shadowmoon_valley()
{

}