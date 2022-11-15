﻿using PathfinderCombatSimulator.ConsoleUI;

namespace PathfinderCombatSimulator
{
    public class OrcNecksplitterAttack : IAttack
    {
        private readonly IDiceManager _rng;

        public OrcNecksplitterAttack(IDiceManager rng)
        {
            _rng = rng;
        }

        public int RollDamage()
        {
            return _rng.Roll(8) + 4;
        }

        public int RollToHit()
        {
            return _rng.Roll(20) + 7;
        }
    }
}