using System.Drawing;

// not presently used...   I may re-visit
namespace PathfinderCombatSimulator
{
    public class FieldManager
    {
        public void PlaceTeams(List<CombatTeam> combatGroups, FlatTwoDimensionalBattlefield flatTwoDimensionalBattlefield)
        {
            List<Point> teamCenters = new List<Point>();
            if (combatGroups.Count <= 1) return;  
            if (combatGroups.Count == 2)
            {
                teamCenters.Add(new Point(-5, 0));
                teamCenters.Add(new Point(5, 0));
            }
            if (combatGroups.Count == 3)
            {
                teamCenters.Add(new Point(6, 0));
                teamCenters.Add(new Point(-3, 5));
                teamCenters.Add(new Point(-3, -5));
            }
            else
                throw new NotImplementedException("Maximum of 3 teams are supported.");
        }
    }
}
