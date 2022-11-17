using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathfinderCombatSimulator.ClassLibrary.Constants
{

    [Flags]
    public enum SavingThrow
    {
        None = 0,
        Fort = 1,
        Reflex = 2,
        Will = 4
    }
}
