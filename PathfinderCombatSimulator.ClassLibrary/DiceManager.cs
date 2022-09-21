namespace PathfinderCombatSimulator
{
    public class DiceManager : IDiceManager
    {
        private Random _rnd = new Random();

        public int Roll(int dieSize) =>
            _rnd.Next(1, dieSize);
    }
}
