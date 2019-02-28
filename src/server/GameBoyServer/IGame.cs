using System.Collections.Generic;

namespace GameBoyServer
{
    public interface IGame
    {
        GameState State { get; set; }
        List<Player> Players { get; }
        Board Board { get; }
        void Reset();
    }
}
