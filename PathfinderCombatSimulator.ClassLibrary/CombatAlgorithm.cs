using PathfinderCombatSimulator.ClassLibrary.Constants;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathfinderCombatSimulator
{

    public class CombatAlgorithm : ICombatAlgorithm
    {
        private readonly IDiceManager _rng;
        private readonly IUserInterface _ui;
        private readonly ICombatHelper _combat;
        private readonly IMobAI _ai;

        public CombatAlgorithm(IDiceManager rng, IUserInterface ui, ICombatHelper combat, IMobAI ai)
        {
            _rng = rng;
            _ui = ui;
            _combat = combat;
            _ai = ai;
        }

        public BattleResults ProcessBattleUntilOnlyOneTeamIsConcious(List<CombatTeam> combatGroups)
        {
            if (combatGroups.Count != 2) throw new NotImplementedException();

            List<(Mob mob, int initiativeRoll)> sortedInitiative = RollAndOrderByInitiative(combatGroups);

            int roundId = 1;
            while (IsCombatStillActive(combatGroups))
            {
                _ui.RoundStarts(roundId);
                Dictionary<Mob, int> previousAttackCountForMobThisRound = new();
                foreach ((Mob mob, int initiativeRoll) item in sortedInitiative)
                {
                    if (HasAvailableActionThisTurn(item.mob)) ProcessAction(1, item.mob, combatGroups, previousAttackCountForMobThisRound);
                    if (HasAvailableActionThisTurn(item.mob)) ProcessAction(2, item.mob, combatGroups, previousAttackCountForMobThisRound);
                    if (HasAvailableActionThisTurn(item.mob)) ProcessAction(3, item.mob, combatGroups, previousAttackCountForMobThisRound);
                }
                _ui.RoundEnds(roundId);

                roundId++;
            }
            
            return new BattleResults() { Groups = combatGroups };
        }

        private bool HasAvailableActionThisTurn(Mob mob) => true;

        private void ProcessAction(int actionNumber, Mob mob, List<CombatTeam> combatGroups, Dictionary<Mob, int> dictionaryOfPreviousAttackCounts)
        {
            Mob? targetMob = _ai.GetTargetFor(mob, combatGroups);

            if (targetMob == null || _combat.IsDead(mob) || _combat.IsUnconcious(mob)) return;

            dictionaryOfPreviousAttackCounts.TryGetValue(mob, out int prevAttacks);

            AttackResults? results = Attack(mob, targetMob, prevAttacks);

            if (results?.TargetMob == null) 
                _ui.AttackMisses(mob, targetMob);
            else
            {
                if (results.DamageDelivered <= 0) return;

                results.TargetMob.CurrentHitPoints -= results.DamageDelivered;

                _ui.ReceiveDamage(results);
                if (_combat.IsDead(results.TargetMob)) _ui.Die(results.TargetMob);
                if (_combat.IsUnconcious(results.TargetMob)) _ui.KnockOut(results.TargetMob);
            }
        }

        public int RollToHit(int numberOfPreviousAttacksThisTurn, int attackModifier) =>
            _rng.Roll(20) + attackModifier - (numberOfPreviousAttacksThisTurn * 5);


        private AttackResults? Attack(Mob mob, Mob victim, int numberOfPreviousAttacksMadeThisTurn)
        {
            if (mob.DefaultAttack != null)
            {
                int attackValue = RollToHit(numberOfPreviousAttacksMadeThisTurn, mob.DefaultAttack.AttackModifier);
                if (attackValue > victim.CurrentArmorClass)
                {
                    var returnable = new AttackResults();
                    var damageRoll = RollDamage(mob.DefaultAttack.DamageEffects);

                    //todo: update this to work with more than just physical damage...
                    if (damageRoll.ContainsKey(ClassLibrary.Constants.DamageType.Physical) )
                    {
                        returnable.AttackingMob = mob;
                        returnable.TargetMob = victim;
                        returnable.DamageDelivered = damageRoll[ClassLibrary.Constants.DamageType.Physical];
                    }

                    //todo: apply saving throw feature
                    


                    return returnable;
                }
            }
            return null;
        }

        public Dictionary<DamageType, int> RollDamage(IList<DamageEffect> damageEffects)
        {
            var returnable = new Dictionary<DamageType, int>();
            foreach (DamageEffect _damageEffect in damageEffects)
            {
                if (returnable.ContainsKey(_damageEffect.DamageType))
                {
                    returnable[_damageEffect.DamageType] = returnable[_damageEffect.DamageType] + _rng.AddRolls(_damageEffect.DamageDice);
                }
                else
                {
                    returnable[_damageEffect.DamageType] = _rng.AddRolls(_damageEffect.DamageDice);
                }
            }
            return returnable;
        }

        public List<(Mob mob, int initiativeRoll)> RollAndOrderByInitiative(List<CombatTeam> groups)
        {
            List<(Mob mob, int initiativeRoll)> initiativeTable = new List<(Mob mob, int initiativeRoll)>();

            foreach(var group in groups)
            {
                foreach (Mob mob in group.Combatants)
                {
                    initiativeTable.Add((mob, RollInitiative(mob)));
                }
            }

            return (from item in initiativeTable orderby item.initiativeRoll select item).ToList();
        }


        public bool IsCombatStillActive(List<CombatTeam> combatGroups)
        {
            var count = 0;
            foreach(CombatTeam combatGroup in combatGroups)
            {
                if (StillLives(combatGroup)) count++;
            }
            return (count > 1);
        }

        private bool StillLives(CombatTeam acg)
        {
            foreach (Mob mob in acg.Combatants)
            {
                if (!_combat.IsDead(mob)) return true;
            }
            return false;
        }


        public int RollInitiative(Mob actor) =>
            actor.PerceptionSkillCheckModifier + _rng.Roll(20);
    }
}
