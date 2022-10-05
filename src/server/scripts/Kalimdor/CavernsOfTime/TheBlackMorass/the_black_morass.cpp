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

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "Log.h"
#include "Map.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "SpellInfo.h"
#include "the_black_morass.h"

enum MedivhBm
{
    SAY_ENTER                            = 0,
    SAY_WEAK75                           = 2,
    SAY_WEAK50                           = 3,
    SAY_WEAK25                           = 4,
    SAY_DEATH                            = 5,
    SAY_WIN                              = 6,
    SAY_ORCS_ENTER                       = 7,
    SAY_ORCS_ANSWER                      = 8,

    EVENT_COUNCIL_WAVE1                  = 1,
    EVENT_COUNCIL_WAVE2                  = 2,
    EVENT_COUNCIL_WAVE3                  = 3,
    EVENT_COUNCIL_WAVE4                  = 4,
    EVENT_MEDIVH_SAY_ORCS_ENTER          = 5,
    EVENT_COUNCIL_SAY_ORCS_ANSWER        = 6,

    SPELL_CHANNEL                        = 31556,
    SPELL_PORTAL_RUNE                    = 32570,    // aura(portal on ground effect)

    SPELL_BLACK_CRYSTAL                  = 32563,    // aura
    SPELL_PORTAL_CRYSTAL                 = 32564,    // summon

    SPELL_BANISH_PURPLE                  = 32566,    // aura
    SPELL_BANISH_GREEN                   = 32567,    // aura

    SPELL_CORRUPT                        = 31326,
    SPELL_CORRUPT_AEONUS                 = 37853,

    // NYI
    SPELL_SUMMON_INFINITE_WHELP          = 37606,
    SPELL_SUMMON_INFINITE_ASSASSIN       = 31318,
    SPELL_SUMMON_INFINITE_ASSASSIN_2     = 36229,
    SPELL_SUMMON_INFINITE_CHRONOMANCER   = 31421,
    SPELL_SUMMON_INFINITE_CHRONOMANCER_2 = 36231,
    SPELL_SUMMON_INFINITE_EXECUTIONER    = 33363,
    SPELL_SUMMON_INFINITE_EXECUTIONER_2  = 36232,
    SPELL_SUMMON_INFINITE_VANQUISHER     = 33364,
    SPELL_SUMMON_INFINITE_VANQUISHER_2   = 36233,

    SPELL_SUMMON_RIFT_LORD               = 31321,
    SPELL_SUMMON_RIFT_LORD_2             = 36234,
    SPELL_SUMMON_RIFT_KEEPER             = 36235,
    SPELL_SUMMON_RIFT_KEEPER_2           = 36236,

    SPELL_SUMMON_CHRONO_LORD_DEJA        = 31391,
    SPELL_SUMMON_TEMPORUS                = 31392,
    SPELL_SUMMON_AEONUS                  = 31393,
    SPELL_SUMMON_INFINITE_CHRONO_LORD    = 37177,
    SPELL_SUMMON_INFINITE_TIMEREAVER     = 37178,

    SPELL_SUMMON_TIME_RIFT_PERIODIC_90   = 31353,
    SPELL_SUMMON_TIME_RIFT_PERIODIC_120  = 31632,
    SPELL_SUMMON_TIME_RIFT_PERIODIC_45   = 31636,
    SPELL_SUMMON_TIME_RIFT_PERIODIC_75   = 31637,
    SPELL_SUMMON_TIME_RIFT_PERIODIC_30   = 34188,
    SPELL_SUMMON_TIME_RIFT_PERIODIC_5    = 35143,
    SPELL_SUMMON_TIME_RIFT_EFFECT_1      = 31354,
    SPELL_SUMMON_TIME_RIFT_EFFECT_2      = 31355,
    SPELL_SUMMON_TIME_RIFT_EFFECT_3      = 31356,
    SPELL_SUMMON_TIME_RIFT_EFFECT_4      = 31357,
    SPELL_SUMMON_TIME_RIFT               = 39041,
    SPELL_TIME_RIFT_1_READY              = 39570,
    SPELL_TIME_RIFT_2_READY              = 39571,
    SPELL_TIME_RIFT_3_READY              = 39572,
    SPELL_TIME_RIFT_4_READY              = 39573,
    SPELL_CLOSE_TIME_RIFT_TRIGGER        = 31322,
    SPELL_CLOSE_TIME_RIFT_EFFECT         = 31323,
    SPELL_TIME_RIFT_READY_PRIMER         = 31352,
    SPELL_TIME_RIFT_CHANNEL              = 31387,
    SPELL_TIME_RIFT_CHANNEL_TRIGGER      = 31388,
    SPELL_TIME_RIFT_PERIODIC             = 31320,

    SPELL_MEDIVH_LIVES                   = 31395,
    SPELL_MEDIVH_DIES                    = 31327,
    SPELL_MEDIVH_SHIELD                  = 32613,

    SPELL_THE_BLACK_MORASS_COMPLETE      = 37215,

    C_COUNCIL_ENFORCER                   = 17023
};

std::vector<ObjectGuid> CouncilEnforcerGuid; // 6 councils per wave,total 4 waves

float CouncilEnforcerWave1Pos[6][4]=  // 05/20/2012 03:24:28.130
{
    {-2086.8477f,    7132.722f,     34.671482f,    6.126106f }, // 19517
    {-2087.1946f,    7129.969f,     34.67148f,     6.143559f }, // 19518
    {-2087.4797f,    7127.2734f,    34.671482f,    6.161012f }, // 19519
    {-2087.8013f,    7124.5103f,    34.671482f,    6.1784654f}, // 19520
    {-2088.082f,     7121.8877f,    34.67148f,     6.1959186f}, // 19521
    {-2088.3865f,    7119.379f,     34.671482f,    6.213372f }  // 19522
};

float CouncilEnforcerWave2Pos[6][4]=  // 05/20/2012 03:24:30.158
{
    {-2086.5723f,    7132.635f,     34.67148f,     6.126106f},  // 19523
    {-2087.409f,     7127.2026f,    34.671482f,    6.161012f},  // 19524
    {-2087.8833f,    7124.376f,     34.671482f,   6.1784654f},  // 19525
    {-2088.2249f,    7121.99f,      34.671474f,   6.1959186f},  // 19526
    {-2088.5989f,    7119.243f,     34.67148f,     6.213372f},  // 19527
    {-2086.9849f,    7129.968f,     34.67148f,     6.143559f}   // 19528
};

float CouncilEnforcerWave3Pos[6][4]=  // 05/20/2012 03:24:32.561
{
    {-2086.9536f,    7129.987f,     34.671482f,     6.143559f},  // 19529
    {-2086.509f,     7132.6777f,    34.67148f,      6.126106f},  // 19530
    {-2087.3103f,    7127.19f,      34.671482f,     6.161012f},  // 19531
    {-2087.6462f,    7124.358f,     34.67148f,     6.1784654f},  // 19532
    {-2088.0132f,    7121.874f,     34.67148f,     6.1959186f},  // 19533
    {-2088.501f,     7119.221f,     34.67148f,      6.213372f}   // 19534
};

float CouncilEnforcerWave4Pos[6][4]=  // 05/20/2012 03:24:34.979
{
    {-2086.9536f,    7129.987f,     34.671482f,     6.143559f},  // 19535
    {-2086.8782f,    7129.967f,     34.67148f,      6.143559f},  // 19536
    {-2087.149f,     7127.2046f,    34.67148f,      6.143559f},  // 19537
    {-2087.4875f,    7124.4136f,    34.67148f,     6.1784654f},  // 19538
    {-2087.8918f,    7121.918f,     34.67148f,     6.1959186f},  // 19539
    {-2088.3884f,    7119.204f,     34.67148f,      6.213372f}   // 19540
};

float CouncilEnforcerWave1WPs[6][2][3]=
{
    {{-2084.1829f,    7132.281f,       34.648f},    {-2045.5182f,    7128.8394f,    24.624517f}},
    {{-2085.246f,     7129.609f,      34.8077f},    {-2045.7974f,    7126.2485f,    24.443928f}},
    {{-2086.0464f,    7126.9746f,   34.751137f},    {-2046.1133f,    7123.1763f,    24.330793f}},
    {{-2085.871f,     7124.3896f,   34.775032f},    {-2044.941f,     7120.269f,     23.878582f}},
    {{-2086.4094f,    7121.713f,    34.823685f},    {-2047.2369f,    7118.0376f,    24.475895f}},
    {{-2086.8538f,    7119.0415f,   34.669765f},    {-2047.8209f,    7115.204f,     24.66805f}}
};

float CouncilEnforcerWave2WPs[6][2][3]=
{
    {{-2084.4084f,    7132.2734f,   34.836506f},    {-2051.7446f,    7129.4126f,    27.501535f}},
    {{-2084.9575f,    7127.092f,     34.87588f},    {-2053.0063f,    7123.481f,      27.08028f}},
    {{-2084.6553f,    7124.0728f,    34.70954f},    {-2052.927f,     7120.2695f,    26.747597f}},
    {{-2085.5784f,    7121.6196f,    34.69463f},    {-2054.432f,     7117.749f,     27.217783f}},
    {{-2086.9763f,    7119.2246f,     34.6985f},    {-2053.3538f,    7114.2056f,    26.725523f}},
    {{-2084.9065f,    7129.718f,     34.64542f},    {-2052.3281f,     7125.968f,    27.119364f}}
};

float CouncilEnforcerWave3WPs[6][2][3]=
{
    {{-2085.251f,    7129.9688f,    34.759438f},    {-2065.048f,     7127.451f,     30.347391f}},
    {{-2083.7397f,   7132.198f,      34.71596f},    {-2064.9705f,    7129.719f,     30.260447f}},
    {{-2085.726f,    7126.8955f,     34.78042f},    {-2065.6416f,    7125.1006f,     30.38935f}},
    {{-2086.1528f,   7124.1562f,     34.75782f},    {-2065.6597f,    7122.454f,     30.344164f}},
    {{-2084.8438f,   7121.5435f,     34.68887f},    {-2065.1746f,    7119.713f,     30.206259f}},
    {{-2085.176f,    7118.9185f,    34.875107f},    {-2064.851f,     7116.6157f,    30.078735f}}
};

float CouncilEnforcerWave4WPs[6][2][3]=
{
    {{-2082.52f,     7132.08f,      34.577217f},    {-2082.52f,      7132.08f,      34.577217f}},
    {{-2083.65f,     7129.36f,      34.583836f},    {-2083.65f,      7129.36f,      34.583836f}},
    {{-2083.05f,     7126.4f,        34.58211f},    {-2083.05f,      7126.4f,        34.58211f}},
    {{-2082.417f,    7123.9824f,     34.75353f},    {-2081.3464f,    7123.551f,      33.83558f}},
    {{-2083.91f,     7121.34f,      34.588364f},    {-2083.91f,      7121.34f,      34.588364f}},
    {{-2083.99f,     7118.74f,       34.58981f},    {-2083.99f,      7118.74f,       34.58981f}}
};

struct npc_medivh_bm : public ScriptedAI
{
    npc_medivh_bm(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
        instance = creature->GetInstanceScript();
    }

    void Initialize()
    {
        SpellCorrupt_Timer = 0;
        Check_Timer = 0;
        Life75 = true;
        Life50 = true;
        Life25 = true;
    }

    InstanceScript* instance;

    uint32 SpellCorrupt_Timer;
    uint32 Check_Timer;

    bool Life75;
    bool Life50;
    bool Life25;

    EventMap events;

    void Reset() override
    {
        Initialize();

        if (instance->GetData(TYPE_MEDIVH) == IN_PROGRESS)
            DoCast(me, SPELL_CHANNEL, true);
        else if (me->HasAura(SPELL_CHANNEL))
            me->RemoveAura(SPELL_CHANNEL);
    }

    void MoveInLineOfSight(Unit* who) override
    {
        if (who->GetTypeId() == TYPEID_PLAYER && me->IsWithinDistInMap(who, 10.0f))
        {
            if (instance->GetData(TYPE_MEDIVH) == IN_PROGRESS || instance->GetData(TYPE_MEDIVH) == DONE)
                return;

            Talk(SAY_ENTER);
            //events.ScheduleEvent(EVENT_COUNCIL_WAVE1, 2s); //test
            instance->SetData(TYPE_MEDIVH, IN_PROGRESS);
            DoCast(me, SPELL_CHANNEL, false);
            Check_Timer = 5000;
        }
        else if (who->GetTypeId() == TYPEID_UNIT && me->IsWithinDistInMap(who, 15.0f))
        {
            if (instance->GetData(TYPE_MEDIVH) != IN_PROGRESS)
                return;

            uint32 entry = who->GetEntry();
            if (entry == NPC_INFINITE_ASSASIN || entry == NPC_INFINITE_WHELP || entry == NPC_INFINITE_CRONOMANCER || entry == NPC_INFINITE_EXECUTIONER || entry == NPC_INFINITE_VANQUISHER)
            {
                who->StopMoving();
                who->CastSpell(me, SPELL_CORRUPT, false);
            }
            else if (entry == NPC_AEONUS)
            {
                who->StopMoving();
                who->CastSpell(me, SPELL_CORRUPT_AEONUS, false);
            }
        }
    }

    void AttackStart(Unit* /*who*/) override
    {
        //if (instance->GetData(TYPE_MEDIVH) == IN_PROGRESS)
        //    return;

        //ScriptedAI::AttackStart(who);
    }

    void JustEngagedWith(Unit* /*who*/) override { }

    void SpellHit(WorldObject* /*caster*/, SpellInfo const* spellInfo) override
    {
        if (SpellCorrupt_Timer)
            return;

        if (spellInfo->Id == SPELL_CORRUPT_AEONUS)
            SpellCorrupt_Timer = 1000;

        if (spellInfo->Id == SPELL_CORRUPT)
            SpellCorrupt_Timer = 3000;
    }

    void JustDied(Unit* /*killer*/) override
    {
        Talk(SAY_DEATH);
        Reset();
    }

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);
        switch (events.ExecuteEvent())
        {
              case EVENT_COUNCIL_WAVE1:
                  SummonCouncilEnforcer(1); // 05/20/2012 03:24:28.130 19s after say win
                  events.ScheduleEvent(EVENT_COUNCIL_WAVE2, 2s);
                  break;

              case EVENT_COUNCIL_WAVE2:
                  SummonCouncilEnforcer(2);
                  events.ScheduleEvent(EVENT_COUNCIL_WAVE3, 2s);
                  break;

              case EVENT_COUNCIL_WAVE3:
                  SummonCouncilEnforcer(3);
                  events.ScheduleEvent(EVENT_COUNCIL_WAVE4, 2s);
                  break;

              case EVENT_COUNCIL_WAVE4:
                  SummonCouncilEnforcer(4); // 25s after say win
                  events.ScheduleEvent(EVENT_MEDIVH_SAY_ORCS_ENTER, 8s);
                  break;

              case EVENT_MEDIVH_SAY_ORCS_ENTER: // 33s after say win
                  Talk(SAY_ORCS_ENTER); // 05/20/2012 03:24:42.841
                  events.ScheduleEvent(EVENT_COUNCIL_SAY_ORCS_ANSWER, 2s);
                  break;

              case EVENT_COUNCIL_SAY_ORCS_ANSWER:
                  if(CouncilEnforcerGuid.size()>0)
                  {
                      Creature* council=CouncilEnforcerGuid[0]
                      council->Talk(SAY_ORCS_ANSWER);
                  }
                  // events.ScheduleEvent(COUNCIL_WAVE4, 2s);
                  break;

              default:
                  break;
        }

        if (SpellCorrupt_Timer)
        {
            if (SpellCorrupt_Timer <= diff)
            {
                instance->SetData(TYPE_MEDIVH, SPECIAL);

                if (me->HasAura(SPELL_CORRUPT_AEONUS))
                    SpellCorrupt_Timer = 1000;
                else if (me->HasAura(SPELL_CORRUPT))
                    SpellCorrupt_Timer = 3000;
                else
                    SpellCorrupt_Timer = 0;
            } else SpellCorrupt_Timer -= diff;
        }

        if (Check_Timer)
        {
            if (Check_Timer <= diff)
            {
                uint32 pct = instance->GetData(DATA_SHIELD);

                Check_Timer = 5000;

                if (Life25 && pct <= 25)
                {
                    Talk(SAY_WEAK25);
                    Life25 = false;
                }
                else if (Life50 && pct <= 50)
                {
                    Talk(SAY_WEAK50);
                    Life50 = false;
                }
                else if (Life75 && pct <= 75)
                {
                    Talk(SAY_WEAK75);
                    Life75 = false;
                }

                //if we reach this it means event was running but at some point reset.
                if (instance->GetData(TYPE_MEDIVH) == NOT_STARTED)
                {
                    me->DespawnOrUnsummon();
                    me->Respawn();
                    return;
                }

                if (instance->GetData(TYPE_RIFT) == DONE)
                {
                    Talk(SAY_WIN); // 05/20/2012 03:24:09.020
                    Check_Timer = 0;

                    if (me->HasAura(SPELL_CHANNEL))
                        me->RemoveAura(SPELL_CHANNEL);

                    /// @todo start the post-event here
                    instance->SetData(TYPE_MEDIVH, DONE);
                    events.ScheduleEvent(EVENT_COUNCIL_WAVE1, 19s);
                }
            } else Check_Timer -= diff;
        }

        //if (!UpdateVictim())
        //return;

        //DoMeleeAttackIfReady();
    }

    void SummonCouncilEnforcer(uint32 councilWaveId)
    {
        switch (councilWaveId)
        {
            case 1:
                SummonCouncilEnforcerWave(CouncilEnforcerWave1Pos,CouncilEnforcerWave1WPs);
                break;

            case 2:
                SummonCouncilEnforcerWave(CouncilEnforcerWave2Pos,CouncilEnforcerWave2WPs);
                break;

            case 3:
                SummonCouncilEnforcerWave(CouncilEnforcerWave3Pos,CouncilEnforcerWave3WPs);
                break;

            case 4:
                SummonCouncilEnforcerWave(CouncilEnforcerWave4Pos,CouncilEnforcerWave4WPs);
                break;

            default:
                break;
        }

    }

    void SummonCouncilEnforcerWave(float CouncilEnforcerWavePos[6][4], float CouncilEnforcerWaveWPs[6][2][3])
    {
        for (uint8 i = 0; i < 6; ++i)
        {
            Creature* council = me->SummonCreature(C_COUNCIL_ENFORCER, CouncilEnforcerWavePos[i][0], CouncilEnforcerWavePos[i][1], CouncilEnforcerWavePos[i][2], CouncilEnforcerWavePos[i][3], TEMPSUMMON_TIMED_DESPAWN, 180s);
            CouncilEnforcerGuid.push_back(council->GetGUID());
            council->GetMotionMaster()->MovePoint(0, CouncilEnforcerWaveWPs[i][0][0], CouncilEnforcerWaveWPs[i][0][1], CouncilEnforcerWaveWPs[i][0][2]);
            council->GetMotionMaster()->MovePoint(0, CouncilEnforcerWaveWPs[i][1][0], CouncilEnforcerWaveWPs[i][1][1], CouncilEnforcerWaveWPs[i][1][2]);
        }
    }

};

struct Wave
{
    uint32 PortalMob[4];                                    //spawns for portal waves (in order)
};

static Wave PortalWaves[]=
{
    { {NPC_INFINITE_ASSASIN, NPC_INFINITE_WHELP, NPC_INFINITE_CRONOMANCER, 0} },
    { {NPC_INFINITE_EXECUTIONER, NPC_INFINITE_CRONOMANCER, NPC_INFINITE_WHELP, NPC_INFINITE_ASSASIN} },
    { {NPC_INFINITE_EXECUTIONER, NPC_INFINITE_VANQUISHER, NPC_INFINITE_CRONOMANCER, NPC_INFINITE_ASSASIN} }
};

struct npc_time_rift : public ScriptedAI
{
    npc_time_rift(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
        instance = creature->GetInstanceScript();
    }

    void Initialize()
    {
        TimeRiftWave_Timer = 15000;
        mRiftWaveCount = 0;
        mWaveId = 0;
    }

    InstanceScript* instance;

    uint32 TimeRiftWave_Timer;
    uint8 mRiftWaveCount;
    uint8 mWaveId;

    void Reset() override
    {
        Initialize();

        uint8 mPortalCount = instance->GetData(DATA_PORTAL_COUNT);

        if (mPortalCount < 6)
            mWaveId = 0;
        else if (mPortalCount > 12)
            mWaveId = 2;
        else mWaveId = 1;

    }
    void JustEngagedWith(Unit* /*who*/) override { }

    void DoSummonAtRift(uint32 creature_entry)
    {
        if (!creature_entry)
            return;

        if (instance->GetData(TYPE_MEDIVH) != IN_PROGRESS)
        {
            me->InterruptNonMeleeSpells(true);
            me->RemoveAllAuras();
            return;
        }

        Position pos = me->GetRandomNearPosition(10.0f);

        //normalize Z-level if we can, if rift is not at ground level.
        pos.m_positionZ = std::max(me->GetMap()->GetHeight(pos.m_positionX, pos.m_positionY, MAX_HEIGHT), me->GetMap()->GetWaterLevel(pos.m_positionX, pos.m_positionY));

        if (Unit* Summon = DoSummon(creature_entry, pos, 30s, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT))
            if (Unit* temp = ObjectAccessor::GetUnit(*me, instance->GetGuidData(DATA_MEDIVH)))
                AddThreat(temp, 0.0f, Summon);
    }

    void DoSelectSummon()
    {
        if ((mRiftWaveCount > 2 && mWaveId < 1) || mRiftWaveCount > 3)
            mRiftWaveCount = 0;

        uint32 entry = 0;

        entry = PortalWaves[mWaveId].PortalMob[mRiftWaveCount];
        TC_LOG_DEBUG("scripts", "npc_time_rift: summoning wave Creature (Wave %u, Entry %u).", mRiftWaveCount, entry);

        ++mRiftWaveCount;

        if (entry == NPC_INFINITE_WHELP)
        {
            for (uint8 i = 0; i < 3; ++i)
                DoSummonAtRift(entry);
        } else DoSummonAtRift(entry);
    }

    void UpdateAI(uint32 diff) override
    {
        if (TimeRiftWave_Timer <= diff)
        {
            DoSelectSummon();
            TimeRiftWave_Timer = 15000;
        } else TimeRiftWave_Timer -= diff;

        if (me->IsNonMeleeSpellCast(false))
            return;

        TC_LOG_DEBUG("scripts", "npc_time_rift: not casting anylonger, i need to die.");
        me->setDeathState(JUST_DIED);

        if (instance->GetData(TYPE_RIFT) == IN_PROGRESS)
            instance->SetData(TYPE_RIFT, SPECIAL);
    }
};

void AddSC_the_black_morass()
{
    RegisterBlackMorassCreatureAI(npc_medivh_bm);
    RegisterBlackMorassCreatureAI(npc_time_rift);
}
