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

        private void ProcessAction(int actionNumber, Mob mob, List<CombatTeam> combatGroups, Dictionary<Mob, int> previousAttackCountForMobThisTurn)
        {
            Mob? targetMob = _ai.GetTargetFor(mob, combatGroups);

            if (targetMob == null || _combat.IsDead(mob) || _combat.IsUnconcious(mob)) return;

            int prevAttacks = 0;
            var abc = previousAttackCountForMobThisTurn.TryGetValue(mob, out prevAttacks);

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

        private AttackResults? Attack(Mob mob, Mob victim, int numberOfPreviousAttacksMadeThisTurn)
        {
            if (mob.Attack != null)
            {
                int attackValue = mob.Attack.RollToHit(numberOfPreviousAttacksMadeThisTurn);
                if (attackValue > victim.CurrentArmorClass)
                {
                    var returnable = new AttackResults();
                    var damageRoll = mob.Attack.RollDamage();

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
