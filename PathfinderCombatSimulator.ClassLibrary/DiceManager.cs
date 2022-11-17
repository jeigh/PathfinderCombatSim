namespace PathfinderCombatSimulator
{
    public class DiceManager : IDiceManager
    {
        private Random _rnd = new Random();

        public int AddRolls(IEnumerable<DieRoll> dice)
        {
            int returnable = 0;
            foreach(DieRoll d in dice)
            {
                var rollsLeft = d.DieCount;
                while (rollsLeft > 0)
                {
                    if (d.DieSize.HasValue)
                        returnable += Roll(d.DieSize.Value);

                    rollsLeft--;
                }
            }
            return returnable;  
        }

        public int Roll(int dieSize) =>
            _rnd.Next(1, dieSize);
    }
}
