using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathfinderCombatSimulator
{
    public class CombatResults
    {
        //public List<Mob> Combatants { get; set; }
        public List<CombatTeam> Groups { get; set; }
    }

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

        public CombatResults ExecuteCombat(List<CombatTeam> combatGroups)
        {
            if (combatGroups.Count != 2) throw new NotImplementedException();

            List<(Mob mob, int initiativeRoll)> sortedInitiative = RollAndOrderByInitiative(combatGroups);

            int turnId = 1;
            while (IsCombatStillActive(combatGroups))
            {
                _ui.TurnStarts(turnId);
                foreach ((Mob mob, int initiativeRoll) item in sortedInitiative)
                {
                    if (HasAvailableActionThisTurn(item.mob)) ProcessAction(1, item.mob, combatGroups);
                    if (HasAvailableActionThisTurn(item.mob)) ProcessAction(2, item.mob, combatGroups);
                    if (HasAvailableActionThisTurn(item.mob)) ProcessAction(3, item.mob, combatGroups);
                }
                _ui.TurnEnds(turnId);

                turnId++;
            }
            
            return new CombatResults() { Groups = combatGroups };
        }

        private bool HasAvailableActionThisTurn(Mob mob)
        {
            return true;
        }

        private void ProcessAction(int actionNumber, Mob mob, List<CombatTeam> combatGroups)
        {
            Mob? targetMob = _ai.GetTargetFor(mob, combatGroups);

            if (targetMob == null || _combat.IsDead(mob) || _combat.IsUnconcious(mob)) return;

            AttackResults? results = Attack(mob, targetMob);

            if (results?.TargetMob != null)
            {
                if (results.DamageDelivered <= 0) return;

                results.TargetMob.CurrentHitPoints -= results.DamageDelivered;

                _ui.ReceiveDamage(results);
                if (_combat.IsDead(results.TargetMob)) _ui.Die(results.TargetMob);
                if (_combat.IsUnconcious(results.TargetMob)) _ui.KnockOut(results.TargetMob);

            }
            else
            {
                _ui.AttackMisses(mob, targetMob);
            }
        }

        private AttackResults? Attack(Mob mob, Mob victim)
        {
            var attackValue = mob.Attack.RollToHit();
            if (attackValue > victim.CurrentArmorClass)
            {
                var returnable = new AttackResults();
                int damageRoll = mob.Attack.RollDamage();

                returnable.AttackingMob = mob;
                returnable.TargetMob = victim;
                returnable.DamageDelivered = damageRoll;

                return returnable;
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
            actor.CurrentInitiativeModifier + _rng.Roll(10);
    }
}
