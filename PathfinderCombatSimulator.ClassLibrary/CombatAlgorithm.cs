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
        public List<AlliedCombatGroup> Groups { get; set; }
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

        public CombatResults ExecuteCombat(List<AlliedCombatGroup> combatGroups)
        {
            if (combatGroups.Count != 2) throw new NotImplementedException();

            List<(Mob mob, int initiativeRoll)> sortedInitiative = RollAndOrderByInitiative(combatGroups);

            int turnId = 1;
            while (IsCombatStillActive(combatGroups))
            {
                _ui.TurnStarts(turnId);
                foreach ((Mob mob, int initiativeRoll) item in sortedInitiative)
                {
                    Mob? targetMob = _ai.GetTargetFor(item.mob, combatGroups);

                    if (targetMob == null || _combat.IsDead(item.mob) || _combat.IsUnconcious(item.mob)) continue;
                    
                    AttackResults? results = Attack(item.mob, targetMob);

                    if (results?.TargetMob != null)
                    {
                        if (results.DamageDelivered <= 0) continue;

                        results.TargetMob.CurrentHitPoints -= results.DamageDelivered;

                        _ui.ReceiveDamage(results);
                        if (_combat.IsDead(results.TargetMob)) _ui.Die(results.TargetMob);
                        if (_combat.IsUnconcious(results.TargetMob)) _ui.KnockOut(results.TargetMob);

                    }
                    else
                    {
                        _ui.AttackMisses(item.mob, targetMob);
                    }
                }
                _ui.TurnEnds(turnId);

                turnId++;
            }
            
            return new CombatResults() { Groups = combatGroups };
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

        public List<(Mob mob, int initiativeRoll)> RollAndOrderByInitiative(List<AlliedCombatGroup> groups)
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


        public bool IsCombatStillActive(List<AlliedCombatGroup> combatGroups)
        {
            var count = 0;
            foreach(AlliedCombatGroup combatGroup in combatGroups)
            {
                if (StillLives(combatGroup)) count++;
            }
            return (count > 1);
        }

        private bool StillLives(AlliedCombatGroup acg)
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
