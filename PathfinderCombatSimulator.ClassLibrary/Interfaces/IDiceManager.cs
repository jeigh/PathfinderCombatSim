namespace PathfinderCombatSimulator
{
    public interface IDiceManager
    {
        public int Roll(int dieSize);
        public int AddRolls(IEnumerable<DieRoll> dice);
    }
}
