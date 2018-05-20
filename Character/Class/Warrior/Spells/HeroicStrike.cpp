
#include "HeroicStrike.h"
#include "Warrior.h"
#include "WarriorSpells.h"
#include "Flurry.h"
#include "DeepWounds.h"
#include "HeroicStrikeBuff.h"
#include "MainhandAttackWarrior.h"

HeroicStrike::HeroicStrike(Engine* engine, Character* pchar, CombatRoll* roll) :
    Spell("Heroic Strike", engine, pchar, roll, 0, 15)
{
    this->pchar = dynamic_cast<Warrior*>(pchar);

    spell_ranks = {11, 21, 32, 44, 58, 80, 111, 138, 157};
    // TODO: Remove hardcoded assumption of rank 9 Heroic Strike.
    rank_spell = 8;
    additional_dmg = spell_ranks[rank_spell];

    talent_ranks = {15, 14, 13, 12};
}

int HeroicStrike::spell_effect(const int) {
    pchar->get_hs_buff()->use_charge();
    pchar->get_spells()->get_mh_attack()->complete_swing();

    AttackResult* result = roll->get_melee_ability_result(pchar->get_mh_wpn_skill());

    if (result->is_miss()) {
        increment_miss();
    }
    // TODO: Apply Overpower
    else if (result->is_dodge()) {
        increment_dodge();
    }
    else if (result->is_parry()) {
        increment_parry();
    }

    float damage_dealt = pchar->get_random_non_normalized_mh_dmg() + additional_dmg;
    damage_dealt *= pchar->get_total_phys_dmg_mod();

    if (result->is_critical()) {
        damage_dealt = round(damage_dealt * pchar->get_ability_crit_dmg_mod());
        pchar->melee_mh_yellow_critical_effect();
        add_crit_dmg(damage_dealt);
    }
    else if (result->is_hit()) {
        pchar->melee_mh_yellow_hit_effect();
        add_hit_dmg(round(damage_dealt));
    }

    // TODO: Resource cost on failed hit should not be 100% of cost.
    return resource_cost;
}

void HeroicStrike::increase_effect_via_talent() {
    ++rank_talent;
    // TODO: Assert max rank?
    resource_cost = talent_ranks[rank_talent];
}

void HeroicStrike::decrease_effect_via_talent() {
    --rank_talent;
    assert(rank_talent >= 0);
    resource_cost = talent_ranks[rank_talent];
}
